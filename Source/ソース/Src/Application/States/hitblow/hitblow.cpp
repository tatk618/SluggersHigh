#include "../../States/list/Statelist.h"
#include "../../Manager/Enemy/EnemyManager.h"

hitBlowState::hitBlowState(const Math::Vector3& _targetVec)
{
	target_vec = _targetVec;

	//y�������Ԃ�
	//target_vec.y;

	//������ԕ����𐳋K��
	target_vec.Normalize();

	//������ԑ��x
	target_vec *= 2.5f;

	//�������ł�����ł��鎞��
	death_time = 1 * 60;
}

Stateclass* hitBlowState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	Math::Vector3 player_vec;
	//�v���C���[�̕����x�N�g�����擾����
	player_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), _player->GetMat());

	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(player_vec.x, player_vec.y, player_vec.z);

	_mat *= tmpMat;

	death_time--;

	//ENEMY.EnemiesHitCheck(_hitmarkmanager);

	//�c�莞�Ԃ��O�ɂȂ�����
	if (death_time <= 0)
	{
		return new AttackState();
	}

	return nullptr;
}