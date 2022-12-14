#pragma once
#include "../../Hitmark/hitmark.h"

class hitmarkManager
{
public:
	~hitmarkManager();
	//爆発を生み出す
	void Spawn(const Math::Vector3& _enemy_pos);
	//爆発を表示する
	void Draw();
	void Update();

private:
	//爆発の
	std::vector<hitmarkClass*>hitmark;
};