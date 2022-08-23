#pragma once
#include "../../main.h"
#include "../../Scene/Scene.h"

class TitleScene:public C_Scene
{
public:
	TitleScene();
	~TitleScene()override;

	void Update(EnemyManager* _enemymanager)override;
	void Draw()override;
private:
	KdTexture* titleTex = nullptr;
	KdTexture* blackTex = nullptr;
	KdTexture* hitenterTex = nullptr;

	//std::shared_ptr<KdSoundEffect> m_soundbgm;
	//std::shared_ptr<KdSoundInstance>m_soundbgminst;

	KdModel robomod;

	Math::Matrix entermat;

	float ang;
	float title_Alpha = 0;
	float hitenter_Alpha = 0;

	float bgmvol = 0;
	bool bgmplay_flg;
	bool enterFlg = false;

	bool scenechangeFlg = false;
};