#include "SceneManager.h"

void SceneManager::DoScene()
{

	//Ÿ‚Ìê–Ê‚Æ‚µ‚ÄŒ»İ‚Æ‚Íˆá‚¤ê–Ê‚ªİ’è‚³‚ê‚Ä‚¢‚½‚ç
	if (nextScene != nowScene)
	{
		//Œ»İ‚Ìê–Ê‚ğíœ‚·‚é
		if (nowScene != nullptr)
		{
			delete nowScene;
		}

		//Ÿ‚ÉÀs‚µ‚½‚¢ê–Ê‚ğŒ»İ‚Ìê–Ê‚Æ‚·‚é
		nowScene = nextScene;
	}


	if (nowScene != nullptr)
	{
		//Œ»İ‚Ìê–Ê‚ğÀs‚·‚é
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
	//Ÿ‚És‚«‚½‚¢ê–Ê‚ğ—\–ñ‚·‚é
	nextScene = _next;
}