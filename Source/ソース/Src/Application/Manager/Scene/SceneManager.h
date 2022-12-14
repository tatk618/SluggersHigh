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

	void DoScene();	//現在の場面を実行する
	void ChangeScene(C_Scene* _next);	//次の場面を予約する

	C_Scene* GetNowScene()
	{
		return nowScene;
	}

private:
	C_Scene* nowScene = nullptr; //現在実行中の画面
	C_Scene* nextScene = nullptr; //次に実行させる画面
	SceneManager(){}
};

#define SCENE SceneManager::GetInstance()