#pragma once
#include "../../States/State.h"
#include "../../namespace/namespace.h"
#include "../../player/Player.h"
#include "../../Manager/redlight/redlightmanager.h"

class C_Player;

class AttackState :public Stateclass
{
public:
	AttackState();
	Stateclass* Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp,RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)override;

	const bool Getattackflg()
	{
		return enemyattackflg;
	}

	int jumpFlg;

	bool enemyattackflg = false;

	float fallSpeed = 0;
	float angX = 0;
	float ene_ply_len = 0;
	float chargespeed = 0;
	float attackinterval = 0;
	float ang = 0;

	//�U���X�e�[�g�͓��ɒǉ��̕ϐ����K�v�Ƃ��Ȃ��̂�
	//�R���X�g���N�^�������o�ϐ����s�v�ł���

};