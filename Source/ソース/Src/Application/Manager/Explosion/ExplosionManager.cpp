#include "../../Manager/Explosion/ExplosionManager.h"
#include "../../Manager/sound/SoundManager.h"
#include "../../Scene/Game/GameScene.h"

ExplosionManager::~ExplosionManager()
{
	for (auto&& i : explosion)
	{
		delete i;
		i = nullptr;
	}

	explosion.clear();
}

void ExplosionManager::Spawn(const Math::Vector3& _enemy_pos)
{
	//”š”­‚Ì”z—ñ‚ğˆêŒÂ‘‚â‚·
	explosion.push_back(new explosionClass(_enemy_pos));

}

void ExplosionManager::Draw()
{
	//”š”­‚Ì”‚¾‚¯•\¦‚·‚éˆ—‚ğŒJ‚è•Ô‚·
	for (UINT e = 0;e < explosion.size();e++)
	{
		explosion[e]->Draw();
	}
}

void ExplosionManager::Update()
{
	for (UINT e = 0;e < explosion.size();e++)
	{
		if (explosion[e]->Update() == false) 
		{
			delete explosion[e];
			explosion.erase(explosion.begin() + e);
			e--;
		}
	}
}
