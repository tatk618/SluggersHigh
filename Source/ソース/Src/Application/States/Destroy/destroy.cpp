#include "../../States/Destroy/destroy.h"
#include "../../States/list/Statelist.h"
#include "../../Scene/Game/GameScene.h"
#include "../../player/Player.h"

Stateclass* DestroyState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	//落下速度
	fall_speed -= 0.1f;

	ang = 0;

	ang += 0.25f;

	if (ang >= 45)
	{
		ang = 45;
	}

	Math::Matrix rotmat;
	rotmat = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(ang));
	_mat = rotmat * _mat;

	Math::Vector3 player_vec;
	//プレイヤーの方向ベクトルを取得する
	player_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 0.5f), _player->GetMat());

	//落下関係の行列
	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(0, fall_speed, 0);
	_mat *= tmpMat;

	//Yの値が10以下のとき
	if (_mat._42 <= 10)
	{
		//Ｙの値を20に固定
		_mat._42 = 10;
	}

	return nullptr;
}