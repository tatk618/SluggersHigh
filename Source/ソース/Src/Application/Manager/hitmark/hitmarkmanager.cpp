#include "../../Manager/hitmark/hitmarkmanager.h"
#include "../../Scene/Game/GameScene.h"

hitmarkManager::~hitmarkManager()
{
	for (auto&& i : hitmark)
	{
		delete i;
		i = nullptr;
	}

	hitmark.clear();
}

void hitmarkManager::Spawn(const Math::Vector3& _enemy_pos)
{
	//爆発の配列を一個増やす
	hitmark.push_back(new hitmarkClass(_enemy_pos));
}

void hitmarkManager::Draw()
{
	//爆発の数だけ表示する処理を繰り返す
	for (UINT hm = 0;hm < hitmark.size();hm++)
	{
		hitmark[hm]->Draw();
	}
}

void hitmarkManager::Update()
{
	for (UINT hm = 0;hm < hitmark.size();hm++)
	{
		if (hitmark[hm]->Update() == false)
		{
			delete hitmark[hm];
			hitmark.erase(hitmark.begin() + hm);
			hm--;
		}
	}
}
