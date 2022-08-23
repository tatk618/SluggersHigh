#pragma once
#include "../../Hitmark/hitmark.h"

class hitmarkManager
{
public:
	~hitmarkManager();
	//”š”­‚ğ¶‚İo‚·
	void Spawn(const Math::Vector3& _enemy_pos);
	//”š”­‚ğ•\¦‚·‚é
	void Draw();
	void Update();

private:
	//”š”­‚Ì
	std::vector<hitmarkClass*>hitmark;
};