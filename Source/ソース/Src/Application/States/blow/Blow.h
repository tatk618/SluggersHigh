#pragma once
#include "../../States/State.h"
#include "../../namespace/namespace.h"
#include "../../player/Player.h"

class BlowState :public Stateclass
{
public:
	BlowState(const Math::Vector3& _targetVec);
	Stateclass* Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)override;

	const Math::Vector3& GetTargetVec() const
	{
		return target_vec;
	}

	//�ǐՃX�e�[�g�͓��ɒǉ��̕ϐ����K�v�Ƃ��Ȃ��̂�
	//�R���X�g���N�^�������o�ϐ����s�v�ł���
private:
	Math::Vector3 target_vec;

	int death_time = 0;
	int ang = 0;

	float y = 0;

	//�d�͂̑傫��
	const float gravityscale = -0.05f;
};