#pragma once
#include "../../explosion/explosion.h"

class ExplosionManager
{
public:
	~ExplosionManager();
	//”š”­‚ğ¶‚İo‚·
	void Spawn(const Math::Vector3& _enemy_pos);
	//”š”­‚ğ•\¦‚·‚é
	void Draw();
	void Update();

private:
	//”š”­‚Ì
	std::vector<explosionClass*>explosion;
};