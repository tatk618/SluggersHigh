#pragma once
#include "../../States/State.h"
#include "../../namespace/namespace.h"
#include "../../player/Player.h"

class playerBlowState :public Stateclass
{
public:
	playerBlowState(const Math::Vector3& _targetVec);
	Stateclass* Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp)override;

	//�ǐՃX�e�[�g�͓��ɒǉ��̕ϐ����K�v�Ƃ��Ȃ��̂�
	//�R���X�g���N�^�������o�ϐ����s�v�ł���
private:
	Math::Vector3 target_vec;

	int death_time = 0;
};