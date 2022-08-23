#pragma once
#include "../../main.h"
#include "../../Scene/Scene.h"

class ResultScene:public C_Scene
{
public:
	ResultScene();
	~ResultScene()override;

	void Update(EnemyManager* _enemymanager);
	void Draw()override;
private:
	KdTexture* resultTex = nullptr;
	KdTexture* blackTex = nullptr;
	DirectX::SpriteFont* spriteFont;

	float black_Alpha = 0;
	bool resultchangeFlg = false;
	bool continueFlg = false;
	bool titlebackFlg = false;
};