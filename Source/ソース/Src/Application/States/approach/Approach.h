#pragma once
#include "../../States/State.h"
#include "../../namespace/namespace.h"
#include "../../player/Player.h"
class C_Player;

class ApproachState :public Stateclass
{
public:
	Stateclass* Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)override;

	//�ǐՃX�e�[�g�͓��ɒǉ��̕ϐ����K�v�Ƃ��Ȃ��̂�
	//�R���X�g���N�^�������o�ϐ����s�v�ł���

};