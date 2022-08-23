#pragma once

class explosionClass
{
public:
	explosionClass(const Math::Vector3& _enemy_pos);
	~explosionClass();

	bool Update();

	void Draw();

private:
	//�����|��
	KdBuffer explosion_vb;
	//�����e�N�X�`��
	KdTexture* explosion_tex;

	//�����A�j���[�V�����̐i�s�x
	int explosion_anime;

	//�����̍s��
	Math::Matrix explosion_mat;

	//�����̈ʒu
	Math::Vector3 explosion_pos;

	Math::Matrix transMat;
	Math::Matrix rotMat;
	Math::Matrix scalMat;
	
};