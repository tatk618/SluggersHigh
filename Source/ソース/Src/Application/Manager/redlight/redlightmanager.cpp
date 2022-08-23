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
	//�����̔z�������₷
	redlight.push_back(new redlightClass(_enemy_pos));
}

void RedlightManager::Draw()
{
	//�����̐������\�����鏈�����J��Ԃ�
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
