#include "SceneManager.h"

void SceneManager::DoScene()
{

	//次の場面として現在とは違う場面が設定されていたら
	if (nextScene != nowScene)
	{
		//現在の場面を削除する
		if (nowScene != nullptr)
		{
			delete nowScene;
		}

		//次に実行したい場面を現在の場面とする
		nowScene = nextScene;
	}


	if (nowScene != nullptr)
	{
		//現在の場面を実行する
		nowScene->Frame();
	}
}

SceneManager::~SceneManager()
{
	if (nowScene != nullptr)
	{
		delete nowScene;
	}
}

void SceneManager::ChangeScene(C_Scene* _next)
{
	//次に行きたい場面を予約する
	nextScene = _next;
}