#pragma once

class redlightClass
{
public:
	redlightClass(const Math::Vector3& _enemy_pos);
	~redlightClass();

	bool Update();

	void Draw();

	void dronematset(Math::Matrix& _drone_mat);

private:
	//爆発板ポリ
	KdBuffer redlight_vb;
	//爆発テクスチャ
	KdTexture* redlight_tex;

	//爆発の行列
	Math::Matrix redlight_mat[30];

	//爆発の位置
	Math::Vector3 redlight_pos;

	Math::Matrix transMat;
	Math::Matrix rotMat;
	Math::Matrix scalMat;

	int count = 0;

	bool redlight_flg = false;

	ID3D11BlendState* blendadd;
	ID3D11BlendState* blendnormal;


};