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
	//”š”­‚Ì”z—ñ‚ğˆêŒÂ‘‚â‚·
	hitmark.push_back(new hitmarkClass(_enemy_pos));
}

void hitmarkManager::Draw()
{
	//”š”­‚Ì”‚¾‚¯•\¦‚·‚éˆ—‚ğŒJ‚è•Ô‚·
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
