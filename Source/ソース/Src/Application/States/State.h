#pragma once
#include "../player/Player.h"
#include "../Manager/redlight/redlightmanager.h"
#include "../Manager/hitmark/hitmarkmanager.h"

class C_Player;

class Stateclass
{
public:
	virtual Stateclass* Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager) = 0;

};