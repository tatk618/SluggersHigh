#pragma once
#include "../../States/State.h"
#include "../../namespace/namespace.h"
#include "../../player/Player.h"
class C_Player;

class DestroyState :public Stateclass
{
public:
	Stateclass* Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)override;

private:
	//—Ž‰º‘¬“x
	float fall_speed = 0;

	float ang = 0;

};