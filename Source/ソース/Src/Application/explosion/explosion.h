#pragma once

class explosionClass
{
public:
	explosionClass(const Math::Vector3& _enemy_pos);
	~explosionClass();

	bool Update();

	void Draw();

private:
	//爆発板ポリ
	KdBuffer explosion_vb;
	//爆発テクスチャ
	KdTexture* explosion_tex;

	//爆発アニメーションの進行度
	int explosion_anime;

	//爆発の行列
	Math::Matrix explosion_mat;

	//爆発の位置
	Math::Vector3 explosion_pos;

	Math::Matrix transMat;
	Math::Matrix rotMat;
	Math::Matrix scalMat;
	
};