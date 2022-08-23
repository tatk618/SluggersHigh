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
	//�����|��
	KdBuffer redlight_vb;
	//�����e�N�X�`��
	KdTexture* redlight_tex;

	//�����̍s��
	Math::Matrix redlight_mat[30];

	//�����̈ʒu
	Math::Vector3 redlight_pos;

	Math::Matrix transMat;
	Math::Matrix rotMat;
	Math::Matrix scalMat;

	int count = 0;

	bool redlight_flg = false;

	ID3D11BlendState* blendadd;
	ID3D11BlendState* blendnormal;


};