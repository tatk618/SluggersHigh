#pragma once

class hitmarkClass
{
public:
	hitmarkClass(const Math::Vector3& _enemy_pos);
	~hitmarkClass();

	bool Update();

	void Draw();

private:
	//ヒットマーク板ポリ
	KdBuffer hitmark_vb;
	//ヒットマークテクスチャ
	KdTexture* hitmark_tex;

	float hitmark_alpha = 0;

	//ヒットマークの行列
	Math::Matrix hitmark_mat;

	//ヒットマークの位置
	Math::Vector3 hitmark_pos;

	Math::Matrix transMat;
	Math::Matrix rotMat;
	Math::Matrix scalMat;

	float plus = 0;

	int count = 0;

	bool hitmark_flg = false;
	bool alphazero_flg = false;

};