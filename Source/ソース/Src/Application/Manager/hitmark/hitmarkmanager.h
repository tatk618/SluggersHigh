#pragma once
#include "../../Hitmark/hitmark.h"

class hitmarkManager
{
public:
	~hitmarkManager();
	//�����𐶂ݏo��
	void Spawn(const Math::Vector3& _enemy_pos);
	//������\������
	void Draw();
	void Update();

private:
	//������
	std::vector<hitmarkClass*>hitmark;
};