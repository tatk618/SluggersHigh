#pragma once

#include "../../Scene/Scene.h"
#include "../../Manager/Enemy/EnemyManager.h"

class SceneManager
{
public:
	~SceneManager();

	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}

	void DoScene();	//Œ»İ‚Ìê–Ê‚ğÀs‚·‚é
	void ChangeScene(C_Scene* _next);	//Ÿ‚Ìê–Ê‚ğ—\–ñ‚·‚é

	C_Scene* GetNowScene()
	{
		return nowScene;
	}

private:
	C_Scene* nowScene = nullptr; //Œ»İÀs’†‚Ì‰æ–Ê
	C_Scene* nextScene = nullptr; //Ÿ‚ÉÀs‚³‚¹‚é‰æ–Ê
	SceneManager(){}
};

#define SCENE SceneManager::GetInstance()