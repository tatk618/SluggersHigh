#include "../../States/list/Statelist.h"
#include "../../player/Player.h"

playerBlowState::playerBlowState(const Math::Vector3& _targetVec)
{
	target_vec = _targetVec;

	//y�������Ԃ�
	target_vec.y = 0;

	//������ԕ����𐳋K��
	target_vec.Normalize();

	//������ԑ��x
	target_vec *= 1.0f;

	//�������ł�����ł��鎞��
	death_time = 1 * 60;
}

Stateclass* playerBlowState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp)
{
	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(target_vec.x, target_vec.y, target_vec.z);

	_mat *= tmpMat;

	death_time--;

	//�c�莞�Ԃ��O�ɂȂ�����
	if (death_time <= 0)
	{
		return nullptr;
	}

}
