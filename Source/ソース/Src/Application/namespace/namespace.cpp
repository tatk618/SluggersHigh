#include "namespace.h"

float function::TurnToAng(Math::Matrix& _mat, const float _targetAng, const float _plusAng)
{
	//���̃L�����N�^�[�̌����𒲂ׂ�
	Math::Vector3 frontVec;
	frontVec = _mat.Backward();	//�E����W�ł̌���i������W�̑O���ɂ�����j
	//frontVec = DirectX::XMVector3TransformNormal(Math::Vector3(0,0,1),_mat);

	float nowAng;
	nowAng = GetVecAngY(frontVec);

	//���Ɖ��x��]���ׂ���
	float lastAng;
	lastAng = _targetAng - nowAng;
	//��:�ڕW��+40�x�A���̌�����-20�x�Ȃ�A�c���60�x�ƂȂ�

	//�p�x��+180�`-180�͈̔͂Ɏ��܂�悤�ɂ���
	while (lastAng > 180)
	{
		lastAng -= 360;
	}
	while (lastAng < -180)
	{
		lastAng += 360;
	}

	//�c��̊p�x��_plusAng�ȏ�Ɏc���Ă���Ȃ�A
	//_plusAng��������]�s������

	Math::Matrix tmpMat;

	if (lastAng > _plusAng)
	{
		//�E��]����ꍇ
		tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(_plusAng));
		lastAng -= _plusAng;

	}
	else if (lastAng < -_plusAng)
	{
		//����]����ꍇ
		tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(-_plusAng));
		lastAng += _plusAng;
	}
	else {
		tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(lastAng));
		lastAng = 0;
	}

	//�L�����N�^�[����]������
	_mat = tmpMat * _mat;

	//�c���Ă���p�x��Ԃ�
	return lastAng;

}

float function::GetVecAngY(Math::Vector3 _vec)
{
	//�x�N�g���̏㉺�̌������Ԃ�
	_vec.y = 0;

	_vec.Normalize();

	Math::Vector3 dot;
	dot = DirectX::XMVector3Dot(Math::Vector3(0, 0, 1), _vec);

	float ang;
	ang = DirectX::XMConvertToDegrees(acos(dot.x));
	if (_vec.x < 0)
	{
		ang *= -1;
	}

	return ang;
}

bool function::ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis,Math::Vector3& _wallnormal)
{
	Math::Vector3 hitVPos[3];	//���C�����������|���S���̒��_���W
	Math::Matrix hitMat;	//���C�����������|���S���̍s��

	float nearestDist = FLT_MAX;	//FLT_MAX==float�^�̍ő�l
	for (auto&& node : _model.GetAllNodes())
	{
		//���b�V�����܂܂�Ă��Ȃ��m�[�h�Ȃ��΂�
		if (node.Mesh == nullptr)continue;

		//���̃m�[�h�̃��[���h�s����擾
		Math::Matrix worldMat = node.LocalTransform * _mat;

		//�t�s��
		Math::Matrix inv = worldMat.Invert();

		//�t�s����g���ă��[�J�����W�ɕϊ�
		Math::Vector3 localPos = DirectX::XMVector3Transform(_rayPos, inv);
		Math::Vector3 localDir = DirectX::XMVector3TransformNormal(_rayVec, inv);

		//�g��s����g���Ă��郁�b�V���p�Ƀ��C�̒������v���Ă���
		float rayLen = localDir.Length();

		//�����͕K�������P�ɂ���
		localDir.Normalize();

		//���b�V�����̑S�|���S������
		for (auto&& face : node.Mesh->GetFaces())
		{
			float dist;

			Math::Vector3 vPos[3];
			vPos[0] = node.Mesh->GetVertexPositions()[face.Idx[0]];
			vPos[1] = node.Mesh->GetVertexPositions()[face.Idx[1]];
			vPos[2] = node.Mesh->GetVertexPositions()[face.Idx[2]];

			bool hit = DirectX::TriangleTests::Intersects(
				localPos,
				localDir,
				node.Mesh->GetVertexPositions()[face.Idx[0]],//�P���_�ڂ̍��W
				node.Mesh->GetVertexPositions()[face.Idx[1]],//�Q���_�ڂ̍��W
				node.Mesh->GetVertexPositions()[face.Idx[2]],//�R���_�ڂ̍��W
				dist	//���C�����������Ƃ���܂ł̋���(�Q�ƌ^)
			);

			if (hit == false)continue;

			//�g�啪�𔽉f
			dist /= rayLen;

			//�����Ƃ��߂��������c��
			if (dist < nearestDist)
			{
				nearestDist = dist;

				hitVPos[0] = vPos[0];
				hitVPos[1] = vPos[1];
				hitVPos[2] = vPos[2];

				//���C�������������f���̍s���ۑ�
				hitMat = worldMat;
			}
		}
	}
	if (nearestDist == FLT_MAX)
	{
		return false;
	}
	else {
		_dis = nearestDist;

		//�|���S���̂R�̕Ӂi�t�`�j�̂����̂Q��\���x�N�g�������߂�
		Math::Vector3 vec1, vec2;
		vec1 = hitVPos[0] - hitVPos[2];
		vec2 = hitVPos[1] - hitVPos[2];

		//�|���S���̂Q�ӂƂ��ɐ����ȃx�N�g�����O�ςŋ��߂�
		//�Q�ӂƂ��ɐ���==�|���S���ʂɐ����Ȃ̂Ŗ@���ƂȂ�
		_wallnormal = DirectX::XMVector3Cross(vec1, vec2);

		//���[�J�����W��ł̖@�������ۂ�3D��Ԃ̌����ɕϊ�����
		_wallnormal = DirectX::XMVector3TransformNormal(_wallnormal, hitMat);

		//�@���͒���1�ɂ���K�v������
		_wallnormal.Normalize();

		return true;
	}
}
