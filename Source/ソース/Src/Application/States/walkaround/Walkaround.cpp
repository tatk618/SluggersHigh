#include "../../States/list/Statelist.h"

WalkAroundState::WalkAroundState()
{
	//徘徊ステートが始まる瞬間に準備しておく変数を初期化する
	 
	target_ang = rand() % 360;	//適当な角度を目標方向とする
	last_time = (rand() % 5 + 1) * 60;	//1秒～5秒間（60回~300回回る）

	Psinang = 0;

}

Stateclass* WalkAroundState::Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp,RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{

	//目標方向に3度ずつ振り向く
	if (function::TurnToAng(_mat, target_ang, 3.0f) == 0)
	{
		//目標方向に振り向き終わっていたら前進する
		Math::Matrix tmpMat;
		tmpMat = DirectX::XMMatrixTranslation(0,0,0.5f);

		//前進した分の行列を合成する
		_mat = tmpMat * _mat;
	}

	last_time--;	//残り行動時間を減らす
	if (last_time <= 0)
	{
		//再び徘徊ステートを開始し直す
		return new WalkAroundState();
	}
	
	//徘徊ステート中に
	//主人公と30以下まで接近したら追跡ステートに移行
	Math::Vector3 targetVec;
	targetVec = _player->GetPos() - _mat.Translation();

	float len;
	len = targetVec.Length();

	if (len <= 100)
	{
		return new ApproachState();
	}

	if (len <= 30)
	{
		return new AttackState();
	}

	return nullptr;	//現在の行動を次回も継続する
}

