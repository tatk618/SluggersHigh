#include "../../States/list/Statelist.h"

WalkAroundState::WalkAroundState()
{
	//�p�j�X�e�[�g���n�܂�u�Ԃɏ������Ă����ϐ�������������
	 
	target_ang = rand() % 360;	//�K���Ȋp�x��ڕW�����Ƃ���
	last_time = (rand() % 5 + 1) * 60;	//1�b�`5�b�ԁi60��~300����j

	Psinang = 0;

}

Stateclass* WalkAroundState::Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp,RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{

	//�ڕW������3�x���U�����
	if (function::TurnToAng(_mat, target_ang, 3.0f) == 0)
	{
		//�ڕW�����ɐU������I����Ă�����O�i����
		Math::Matrix tmpMat;
		tmpMat = DirectX::XMMatrixTranslation(0,0,0.5f);

		//�O�i�������̍s�����������
		_mat = tmpMat * _mat;
	}

	last_time--;	//�c��s�����Ԃ����炷
	if (last_time <= 0)
	{
		//�Ăќp�j�X�e�[�g���J�n������
		return new WalkAroundState();
	}
	
	//�p�j�X�e�[�g����
	//��l����30�ȉ��܂Őڋ߂�����ǐՃX�e�[�g�Ɉڍs
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

	return nullptr;	//���݂̍s����������p������
}

