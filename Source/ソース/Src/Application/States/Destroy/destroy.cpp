#include "../../States/Destroy/destroy.h"
#include "../../States/list/Statelist.h"
#include "../../Scene/Game/GameScene.h"
#include "../../player/Player.h"

Stateclass* DestroyState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	//�������x
	fall_speed -= 0.1f;

	ang = 0;

	ang += 0.25f;

	if (ang >= 45)
	{
		ang = 45;
	}

	Math::Matrix rotmat;
	rotmat = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(ang));
	_mat = rotmat * _mat;

	Math::Vector3 player_vec;
	//�v���C���[�̕����x�N�g�����擾����
	player_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 0.5f), _player->GetMat());

	//�����֌W�̍s��
	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(0, fall_speed, 0);
	_mat *= tmpMat;

	//Y�̒l��10�ȉ��̂Ƃ�
	if (_mat._42 <= 10)
	{
		//�x�̒l��20�ɌŒ�
		_mat._42 = 10;
	}

	return nullptr;
}