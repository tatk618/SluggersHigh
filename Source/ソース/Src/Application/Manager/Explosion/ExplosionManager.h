#pragma once
#include "../../explosion/explosion.h"

class ExplosionManager
{
public:
	~ExplosionManager();
	//�����𐶂ݏo��
	void Spawn(const Math::Vector3& _enemy_pos);
	//������\������
	void Draw();
	void Update();

private:
	//������
	std::vector<explosionClass*>explosion;
};