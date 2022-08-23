#pragma once
namespace function
{
	float TurnToAng(Math::Matrix& _mat, const float _targetAng, const float _plusAng);
	float GetVecAngY(Math::Vector3 _vec);
	//ModelIntersects(���f���A���f���̍s��A���C�̔��ˈʒu�A���C�̔��˕����A���C�����������ʒu�܂ł̋���)
	bool ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _wallnormal);
}