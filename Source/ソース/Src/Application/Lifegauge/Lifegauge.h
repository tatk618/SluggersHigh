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

	//HPの値
	int hp = 0;

	//HPバーの大きさ
	int hpbarsize;

	//透明値
	float alpha = 0;

	//透明化フラグ
	bool alphaFlg = true;

	Math::Matrix gauge_mat;
	Math::Matrix hpbar_mat;

	Math::Vector3 gauge_pos;
	Math::Vector3 hpbar_pos;
};