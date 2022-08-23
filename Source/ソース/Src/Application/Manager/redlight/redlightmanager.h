#pragma once
#include "../../redlight/Redlight.h"

class RedlightManager
{
public:
	~RedlightManager();
	//”š”­‚ğ¶‚İo‚·
	void Spawn(const Math::Vector3& _enemy_pos);
	//”š”­‚ğ•\¦‚·‚é
	void Draw();
	void Update();

private:
	//”š”­‚Ì
	std::vector<redlightClass*>redlight;
};