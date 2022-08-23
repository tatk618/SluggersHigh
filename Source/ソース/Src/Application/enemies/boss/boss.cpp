#include "../boss/boss.h"
#include "../../Manager/Model/ModelManager.h"
#include "../../States/list/Statelist.h"
#include "../../Scene/Game/GameScene.h"

C_Boss::C_Boss(C_Player* _player)
{
	boss_model = MODEL.GetModel("Data/Model/enemies/boss/bossdroneb.gltf");
	boss_ang = 0;

	roter_model = MODEL.GetModel("Data/Model/enemies/boss/bossdronep.gltf");
	roter_ang = 0;

	taser_model = MODEL.GetModel("Data/Model/enemies/boss/bossdronet.gltf");
	taser_ang = 0;

	boss_pos = { -700,100,600 };

	nowState = new FallingState();
	boss_mat.Translation(boss_pos);

	m_hp = 300;
}

C_Boss::~C_Boss()
{

}

void C_Boss::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(boss_mat);
	SHADER.m_standardShader.DrawModel(boss_model);

	SHADER.m_standardShader.SetWorldMatrix(roter_mat);
	SHADER.m_standardShader.DrawModel(roter_model);

	SHADER.m_standardShader.SetWorldMatrix(taser_mat);
	SHADER.m_standardShader.DrawModel(taser_model);
};

void C_Boss::Update(C_Player* _player, Math::Matrix& _player_mat, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{

	//現在の行動パターン(ステート)が設定されていれば
	if (nowState != nullptr)
	{
		//別の行動パターンに移行する場合
		if (nextState != nullptr)
		{
			delete nowState;	//現在の行動パターンを削除

			//次の行動パターンを現在のものとする
			nowState = nextState;
			nextState = nullptr;
		}

		//現在の行動パターンを実行
		nextState = nowState->Action(boss_mat, _player, _player_mat, boss_range_R, _playerhp,_redlightManager,_hitmarkmanager);
	}

	Math::Matrix rotmatP = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(roter_ang));

	roter_ang += 20;
	roter_mat = rotmatP * boss_mat;

	taser_mat = boss_mat;

	//無敵時間をへらす
	invincible_time--;

	if (invincible_time < 0)
	{
		invincible_time = 0;
	}
}
