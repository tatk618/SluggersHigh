#include "../../States/list/Statelist.h"
#include "../../Manager/Enemy/EnemyManager.h"

hitBlowState::hitBlowState(const Math::Vector3& _targetVec)
{
	target_vec = _targetVec;

	//y方向をつぶす
	//target_vec.y;

	//吹っ飛ぶ方向を正規化
	target_vec.Normalize();

	//吹っ飛ぶ速度
	target_vec *= 2.5f;

	//吹っ飛んでから消滅する時間
	death_time = 1 * 60;
}

Stateclass* hitBlowState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	Math::Vector3 player_vec;
	//プレイヤーの方向ベクトルを取得する
	player_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), _player->GetMat());

	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(player_vec.x, player_vec.y, player_vec.z);

	_mat *= tmpMat;

	death_time--;

	//ENEMY.EnemiesHitCheck(_hitmarkmanager);

	//残り時間が０になったら
	if (death_time <= 0)
	{
		return new AttackState();
	}

	return nullptr;
}