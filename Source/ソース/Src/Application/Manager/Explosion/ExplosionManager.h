#pragma once
#include "../../explosion/explosion.h"

class ExplosionManager
{
public:
	~ExplosionManager();
	//爆発を生み出す
	void Spawn(const Math::Vector3& _enemy_pos);
	//爆発を表示する
	void Draw();
	void Update();

private:
	//爆発の
	std::vector<explosionClass*>explosion;
};