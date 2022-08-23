#pragma once
#include "../../States/State.h"
#include "../../namespace/namespace.h"
#include "../../player/Player.h"
class C_Player;

//徘徊ステート
class WalkAroundState :public Stateclass
{
public:
	WalkAroundState();
	Stateclass* Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)override;

private:
	int last_time = 0;	//残り行動時間
	float target_ang = 0;	//目標方向

	float Psinang = 0;

	Math::Vector3 drone_pos;
};