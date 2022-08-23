#pragma once

class C_Ground
{
public:
	C_Ground();

	~C_Ground();

	void Update();
	void Draw();

private:
	KdBuffer ground_vb;
	KdTexture* ground_tex;

	Math::Matrix ground_mat;
};