#include "../Lifegauge/Lifegauge.h"
#include "../Manager/Texture/TextureManager.h"
#include "../player/Player.h"

LifeGauge::LifeGauge()
{
	//HP�o�[�̘g�̓ǂݍ���
	gaugeTex = TEXTURE.GetTexture("Data/Texture/hpbar/lifegauge2.png");
	//HP�o�[�̓ǂݍ���
	hpbarTex = TEXTURE.GetTexture("Data/Texture/hpbar/lifebar2.png");

	//HP�o�[�̘g�̈ʒu
	gauge_pos = { -475,-300,0 };
	//HP�o�[�̈ʒu
	hpbar_pos = { -625,-350,0 };

	Math::Matrix tmpTmat = DirectX::XMMatrixTranslation(gauge_pos.x, gauge_pos.y, gauge_pos.z);
	Math::Matrix tmpSmat = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);

	//hp�Q�[�W�̍s��
	gauge_mat = tmpSmat * tmpTmat * gauge_mat;

	//hp�o�[�̏��������l
	alpha = 1.0f;

	Math::Matrix tmpTmatH = DirectX::XMMatrixTranslation(hpbar_pos.x, hpbar_pos.y, hpbar_pos.z);

	hpbar_mat = tmpSmat * tmpTmatH * hpbar_mat;

}

LifeGauge::~LifeGauge()
{

}

void LifeGauge::Draw()
{
	//hp�Q�[�W�̓ǂݍ���
	if (hpbarTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(hpbar_mat);
		SHADER.m_spriteShader.DrawTex(hpbarTex, Math::Rectangle(0, 0, hpbarsize, 189), alpha, {0,0});
	}

	//hp�o�[�̓ǂݍ���
	if (gaugeTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(gauge_mat);
		SHADER.m_spriteShader.DrawTex(gaugeTex, Math::Rectangle(0, 0, 600, 189), 1.0f);
	}

}

void LifeGauge::Update(C_Player* _player)
{
	//�v���C���[����HP�̒l�������Ă���
	hp = _player->GetPlayerHP();

	//HP�o�[�̑傫�����v���C���[��HP�~6
	hpbarsize = hp * 6;
}
