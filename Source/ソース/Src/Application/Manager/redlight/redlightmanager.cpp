#include "../../Manager/redlight/redlightmanager.h"
#include "../../Scene/Game/GameScene.h"

RedlightManager::~RedlightManager()
{
	for (auto&& i : redlight)
	{
		delete i;
		i = nullptr;
	}

	redlight.clear();
}

void RedlightManager::Spawn(const Math::Vector3& _enemy_pos)
{
	//爆発の配列を一個増やす
	redlight.push_back(new redlightClass(_enemy_pos));
}

void RedlightManager::Draw()
{
	//爆発の数だけ表示する処理を繰り返す
	for (UINT r = 0;r < redlight.size();r++)
	{
		redlight[r]->Draw();
	}
}

void RedlightManager::Update()
{
	for (UINT r = 0;r < redlight.size();r++)
	{
		if (redlight[r]->Update() == false)
		{
			delete redlight[r];
			redlight.erase(redlight.begin() + r);
			r--;
		}
	}
}
