#pragma once
#include "../Manager/Enemy/EnemyManager.h"
#include "../Manager/sound/SoundManager.h"

class C_Scene
{
public:
	virtual ~C_Scene();

	virtual void Update(EnemyManager* _enemymanager);
	virtual void Draw();
	virtual void SetCamera();
	virtual void Frame();

	EnemyManager* _enemymanager;
	C_SoundManager* soundmanager = nullptr;
};