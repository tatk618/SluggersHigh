#include "../../States/list/Statelist.h"
#include "../../Manager/Enemy/EnemyManager.h"

BlowState::BlowState(const Math::Vector3& _targetVec)
{
	target_vec = _targetVec;

	//y�������Ԃ�
	//target_vec.y;

	//������ԕ����𐳋K��
	target_vec.Normalize();

	//������ԑ��x
	target_vec *= 1.0f;

	//�������ł�����ł��鎞��
	death_time = 1 * 60;

	//�ŏ���y���W�ւ̈ړ����x
	y = 3;

	//�����̊p�x
	ang = 5;
}

Stateclass* BlowState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	//�p�x��ǉ�
	ang += 1.0f;
	//�p�x��360�𒴂�����|360�ɕύX
	if (ang > 360)
	{
		ang = -360;
	}

	Math::Matrix rotmat;
	rotmat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(ang));

	y += gravityscale;

	Math::Vector3 player_vec;
	//�v���C���[�̕����x�N�g�����擾����
	player_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, y, 2), _player->GetMat());

	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(target_vec.x, player_vec.y, player_vec.z);

	_mat = rotmat * _mat * tmpMat;

	death_time--;
	
	//ENEMY.EnemiesHitCheck(_hitmarkmanager);

	//�c�莞�Ԃ��O�ɂȂ�����
	if (death_time <= 0)
	{
		//return new ApproachState();
	}

	return nullptr;
}
