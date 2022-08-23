#pragma once
#include "../../main.h"
#include "../../Scene/Scene.h"

class C_Player;

class ClearScene :public C_Scene
{
public:
	ClearScene(C_Player* _player);
	~ClearScene()override;

	void Update(EnemyManager* _enemymanager);
	void Draw()override;
private:
	KdTexture* resultTex = nullptr;
	KdTexture* blackTex = nullptr;
	DirectX::SpriteFont* spriteFont;

	float black_Alpha = 0;
	float rank_alpha = 0;

	int count = 0;
	int lefthp = 0;
	int endscore = 0;
	int destroycount = 0;
	int drone_destroy_score = 0;
	int left_playerlife_score = 0;
	int tmpscore = 0;
	int nowrank = 0;
	int left_playerlife = 0;

	static const int r = 6;
	char rank[r];

	bool resultchangeFlg = false;
	bool destroycount_flg = false;
	bool playerlefthp_flg = false;
	bool scorechangeflg = false;
	bool scoreflg = false;
	bool enterFlg = false;
	bool soundflg = false;
	bool bgmturnon_flg = false;
};