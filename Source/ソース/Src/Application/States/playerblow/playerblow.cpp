#include "../../States/list/Statelist.h"
#include "../../player/Player.h"

playerBlowState::playerBlowState(const Math::Vector3& _targetVec)
{
	target_vec = _targetVec;

	//y方向をつぶす
	target_vec.y = 0;

	//吹っ飛ぶ方向を正規化
	target_vec.Normalize();

	//吹っ飛ぶ速度
	target_vec *= 1.0f;

	//吹っ飛んでから消滅する時間
	death_time = 1 * 60;
}

Stateclass* playerBlowState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp)
{
	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(target_vec.x, target_vec.y, target_vec.z);

	_mat *= tmpMat;

	death_time--;

	//残り時間が０になったら
	if (death_time <= 0)
	{
		return nullptr;
	}

}
