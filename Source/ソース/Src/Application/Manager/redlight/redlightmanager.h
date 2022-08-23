#pragma once
#include "../../redlight/Redlight.h"

class RedlightManager
{
public:
	~RedlightManager();
	//�����𐶂ݏo��
	void Spawn(const Math::Vector3& _enemy_pos);
	//������\������
	void Draw();
	void Update();

private:
	//������
	std::vector<redlightClass*>redlight;
};