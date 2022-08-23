#include "../drone/drone.h"
#include "../../Manager/Model/ModelManager.h"
#include "../../States/list/Statelist.h"
#include "../../Scene/Game/GameScene.h"
#include "../../Manager/sound/SoundManager.h"
#include "../../namespace/namespace.h"

C_Drone::C_Drone(GameScene* _gameScene,const Math::Vector3& _drone_pos)
{
	m_gameScene = _gameScene;

	drone_model = MODEL.GetModel("Data/Model/enemies/drone/droneb.gltf");
	target_ang_X = 0;
	target_ang_Y = 0;

	roter_model = MODEL.GetModel("data/Model/enemies/drone/dronep.gltf");
	roter_ang = 0;

	//taser_model = MODEL.GetModel("data/Model/enemies/drone/dronet.gltf");
	//taser_ang = 0;

	drone_pos = { _drone_pos.x + ((float)(rand() % 30) - 10), 100 + ((float)(rand() % 30) - 10), _drone_pos.z - ((float)(rand() % 20) - 10) };

	nowState = new FallingState();
	drone_mat.Translation(drone_pos);

	alive_flg = false;
	rotersound_flg = false;

	m_hp = 50;
	nowrotersound_count = 0;


	alpha = 0;
}

C_Drone::~C_Drone()
{

}

void C_Drone::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(drone_mat);
	SHADER.m_standardShader.DrawModel(drone_model,alpha);

	SHADER.m_standardShader.SetWorldMatrix(roter_mat);
	SHADER.m_standardShader.DrawModel(roter_model,alpha);

	//SHADER.m_standardShader.SetWorldMatrix(taser_mat);
	//SHADER.m_standardShader.DrawModel(taser_model);
};

void C_Drone::Update(C_Player* _player, Math::Matrix& _player_mat, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	alpha += addalpha;

	if (alpha > maxalpha)
	{
		alpha = maxalpha;
	}

	float dis;//���C�������������܂ł̋���(���C�����������Ƃ��̂ݏ�񂪏㏑�������)

	//���݂̍s���p�^�[��(�X�e�[�g)���ݒ肳��Ă����
	if (nowState != nullptr)
	{
		//�ʂ̍s���p�^�[���Ɉڍs����ꍇ
		if (nextState != nullptr)
		{
			delete nowState;	//���݂̍s���p�^�[�����폜

			//���̍s���p�^�[�������݂̂��̂Ƃ���
			nowState = nextState;
			nextState = nullptr;
		}

		//���݂̍s���p�^�[�������s
		nextState = nowState->Action(drone_mat,_player,_player_mat,drone_range_R,_playerhp,_redlightManager,_hitmarkmanager);
	}

	Math::Matrix rotmatP = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(roter_ang));

	if (alive_flg == true)
	{
		roter_ang += 20;
	}
	else {

	}

	roter_mat = rotmatP * drone_mat;

	//taser_mat = drone_mat;

	if (m_hp >= 1)
	{
		alive_flg = true;
	}

	if (m_hp <= 0)
	{
		alive_flg = false;
	}

	//���G���Ԃ��ւ炷
	invincible_time--;

	if (invincible_time < 0)
	{
		invincible_time = 0;
	}
}
