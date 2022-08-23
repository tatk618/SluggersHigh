#include "../Lifegauge/Lifegauge.h"
#include "../Manager/Texture/TextureManager.h"
#include "../player/Player.h"

LifeGauge::LifeGauge()
{
	//HPバーの枠の読み込み
	gaugeTex = TEXTURE.GetTexture("Data/Texture/hpbar/lifegauge2.png");
	//HPバーの読み込み
	hpbarTex = TEXTURE.GetTexture("Data/Texture/hpbar/lifebar2.png");

	//HPバーの枠の位置
	gauge_pos = { -475,-300,0 };
	//HPバーの位置
	hpbar_pos = { -625,-350,0 };

	Math::Matrix tmpTmat = DirectX::XMMatrixTranslation(gauge_pos.x, gauge_pos.y, gauge_pos.z);
	Math::Matrix tmpSmat = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);

	//hpゲージの行列
	gauge_mat = tmpSmat * tmpTmat * gauge_mat;

	//hpバーの初期透明値
	alpha = 1.0f;

	Math::Matrix tmpTmatH = DirectX::XMMatrixTranslation(hpbar_pos.x, hpbar_pos.y, hpbar_pos.z);

	hpbar_mat = tmpSmat * tmpTmatH * hpbar_mat;

}

LifeGauge::~LifeGauge()
{

}

void LifeGauge::Draw()
{
	//hpゲージの読み込み
	if (hpbarTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(hpbar_mat);
		SHADER.m_spriteShader.DrawTex(hpbarTex, Math::Rectangle(0, 0, hpbarsize, 189), alpha, {0,0});
	}

	//hpバーの読み込み
	if (gaugeTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(gauge_mat);
		SHADER.m_spriteShader.DrawTex(gaugeTex, Math::Rectangle(0, 0, 600, 189), 1.0f);
	}

}

void LifeGauge::Update(C_Player* _player)
{
	//プレイヤーからHPの値を持ってくる
	hp = _player->GetPlayerHP();

	//HPバーの大きさ＝プレイヤーのHP×6
	hpbarsize = hp * 6;
}
