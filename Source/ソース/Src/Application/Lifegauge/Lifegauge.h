#pragma once
#include "../main.h"

class C_Player;

class LifeGauge
{
public:
	LifeGauge();
	~LifeGauge();

	void Draw();
	void Update(C_Player* _player);
private:
	KdTexture* gaugeTex = nullptr;
	KdTexture* hpbarTex = nullptr;

	//HP�̒l
	int hp = 0;

	//HP�o�[�̑傫��
	int hpbarsize;

	//�����l
	float alpha = 0;

	//�������t���O
	bool alphaFlg = true;

	Math::Matrix gauge_mat;
	Math::Matrix hpbar_mat;

	Math::Vector3 gauge_pos;
	Math::Vector3 hpbar_pos;
};