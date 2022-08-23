#include "SceneManager.h"

void SceneManager::DoScene()
{

	//���̏�ʂƂ��Č��݂Ƃ͈Ⴄ��ʂ��ݒ肳��Ă�����
	if (nextScene != nowScene)
	{
		//���݂̏�ʂ��폜����
		if (nowScene != nullptr)
		{
			delete nowScene;
		}

		//���Ɏ��s��������ʂ����݂̏�ʂƂ���
		nowScene = nextScene;
	}


	if (nowScene != nullptr)
	{
		//���݂̏�ʂ����s����
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
	//���ɍs��������ʂ�\�񂷂�
	nextScene = _next;
}