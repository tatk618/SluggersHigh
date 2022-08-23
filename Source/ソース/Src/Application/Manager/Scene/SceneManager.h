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

	void DoScene();	//���݂̏�ʂ����s����
	void ChangeScene(C_Scene* _next);	//���̏�ʂ�\�񂷂�

	C_Scene* GetNowScene()
	{
		return nowScene;
	}

private:
	C_Scene* nowScene = nullptr; //���ݎ��s���̉��
	C_Scene* nextScene = nullptr; //���Ɏ��s��������
	SceneManager(){}
};

#define SCENE SceneManager::GetInstance()