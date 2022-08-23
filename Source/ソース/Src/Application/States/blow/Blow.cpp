#include "../../States/list/Statelist.h"
#include "../../Manager/Enemy/EnemyManager.h"

BlowState::BlowState(const Math::Vector3& _targetVec)
{
	target_vec = _targetVec;

	//y方向をつぶす
	//target_vec.y;

	//吹っ飛ぶ方向を正規化
	target_vec.Normalize();

	//吹っ飛ぶ速度
	target_vec *= 1.0f;

	//吹っ飛んでから消滅する時間
	death_time = 1 * 60;

	//最初のy座標への移動速度
	y = 3;

	//初期の角度
	ang = 5;
}

Stateclass* BlowState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	//角度を追加
	ang += 1.0f;
	//角度が360を超えたら－360に変更
	if (ang > 360)
	{
		ang = -360;
	}

	Math::Matrix rotmat;
	rotmat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(ang));

	y += gravityscale;

	Math::Vector3 player_vec;
	//プレイヤーの方向ベクトルを取得する
	player_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, y, 2), _player->GetMat());

	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(target_vec.x, player_vec.y, player_vec.z);

	_mat = rotmat * _mat * tmpMat;

	death_time--;
	
	//ENEMY.EnemiesHitCheck(_hitmarkmanager);

	//残り時間が０になったら
	if (death_time <= 0)
	{
		//return new ApproachState();
	}

	return nullptr;
}
