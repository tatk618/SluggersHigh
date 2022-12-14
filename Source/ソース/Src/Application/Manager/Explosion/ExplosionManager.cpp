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
	//爆発の配列を一個増やす
	explosion.push_back(new explosionClass(_enemy_pos));

}

void ExplosionManager::Draw()
{
	//爆発の数だけ表示する処理を繰り返す
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
