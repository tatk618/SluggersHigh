#pragma once

class hitmarkClass
{
public:
	hitmarkClass(const Math::Vector3& _enemy_pos);
	~hitmarkClass();

	bool Update();

	void Draw();

private:
	//�q�b�g�}�[�N�|��
	KdBuffer hitmark_vb;
	//�q�b�g�}�[�N�e�N�X�`��
	KdTexture* hitmark_tex;

	float hitmark_alpha = 0;

	//�q�b�g�}�[�N�̍s��
	Math::Matrix hitmark_mat;

	//�q�b�g�}�[�N�̈ʒu
	Math::Vector3 hitmark_pos;

	Math::Matrix transMat;
	Math::Matrix rotMat;
	Math::Matrix scalMat;

	float plus = 0;

	int count = 0;

	bool hitmark_flg = false;
	bool alphazero_flg = false;

};