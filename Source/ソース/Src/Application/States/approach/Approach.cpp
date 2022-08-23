#include "../../States/list/Statelist.h"
#include "../../Scene/Game/GameScene.h"
#include "../../player/Player.h"

Stateclass* ApproachState::Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	Math::Vector3 player_pos;
	player_pos = _player->GetPos();

	Math::Vector3 enemy_pos;
	enemy_pos = _mat.Translation();

	//��l���ɐڋ߂���
	Math::Vector3 targetVec;
	targetVec = player_pos - enemy_pos;
	//�G���猩����l�������ւ̃x�N�g��

	//�ڕW�����̊p�x���擾
	float targetAng;
	targetAng = function::GetVecAngY(targetVec);

	//�ڕW������3.0f�x����]������
	if (function::TurnToAng(_mat, targetAng, 6.0f) == 0)
	{
		//�ڕW�����ɐU������I����Ă�����O�i����
		Math::Matrix tmpMat;
		tmpMat = DirectX::XMMatrixTranslation(0,0,1.5f);
		_mat = tmpMat * _mat;
	}

	//��l������30�ȏ㗣�ꂽ��p�j�X�e�[�g�Ɉڍs����
	float len;
	len = targetVec.Length();

	//if (len >= 100)
	//{
	//	return new WalkAroundState();
	//}

	//�p�j�X�e�[�g����
	//��l����30�ȉ��܂Őڋ߂�����U���X�e�[�g�Ɉڍs����
	if (len <= 30)
	{
		return new AttackState();
	}

	return nullptr;
}
