#include "../../States/falling/Falling.h"
#include "../../States/list/Statelist.h"
#include "../../Scene/Game/GameScene.h"
#include "../../player/Player.h"

Stateclass* FallingState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	//落下速度
	fall_speed -= 0.01f;

	float rot = 0;
	if (rot >= 0)
	{
		rot += 3;
		if (rot >= 180)
		{
			rot = 180;
		}
	}

	Math::Matrix rotmat;
	rotmat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rot));
	_mat = rotmat * _mat;

	//落下関係の行列
	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(0, fall_speed, 0);
	_mat = tmpMat * _mat;

	//Yの値が20以下のとき
	if (_mat._42 <= 15)
	{
		//Ｙの値を20に固定
		_mat._42 = 15;

		return new ApproachState();
	}

	return nullptr;
}
