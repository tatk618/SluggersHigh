#pragma once

class C_Sky
{
public:
	C_Sky();
	~C_Sky();

	void Update();
	void Draw();

private:
	KdModel* sky_mod;
	float sky_ang = 0;
	Math::Matrix sky_mat;
};