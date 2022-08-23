#include "../../States/list/Statelist.h"
#include "../../Scene/Game/GameScene.h"
#include "../../player/Player.h"

Stateclass* ApproachState::Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	Math::Vector3 player_pos;
	player_pos = _player->GetPos();

	Math::Vector3 enemy_pos;
	enemy_pos = _mat.Translation();

	//主人公に接近する
	Math::Vector3 targetVec;
	targetVec = player_pos - enemy_pos;
	//敵から見た主人公方向へのベクトル

	//目標方向の角度を取得
	float targetAng;
	targetAng = function::GetVecAngY(targetVec);

	//目標方向に3.0f度ずつ回転させる
	if (function::TurnToAng(_mat, targetAng, 6.0f) == 0)
	{
		//目標方向に振り向き終わっていたら前進する
		Math::Matrix tmpMat;
		tmpMat = DirectX::XMMatrixTranslation(0,0,1.5f);
		_mat = tmpMat * _mat;
	}

	//主人公から30以上離れたら徘徊ステートに移行する
	float len;
	len = targetVec.Length();

	//if (len >= 100)
	//{
	//	return new WalkAroundState();
	//}

	//徘徊ステート中に
	//主人公と30以下まで接近したら攻撃ステートに移行する
	if (len <= 30)
	{
		return new AttackState();
	}

	return nullptr;
}
