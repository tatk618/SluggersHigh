#include "Sky.h"
#include "../Manager/Model/ModelManager.h"
#include "../Scene/Game/GameScene.h"

C_Sky::C_Sky()
{
	sky_ang = 0;

	//スカイスフィアの表示
	sky_mod = MODEL.GetModel("Data/Model/sky/Sky.gltf");
}

C_Sky::~C_Sky()
{
}

void C_Sky::Update()
{
	Math::Matrix scalmat;
	scalmat = DirectX::XMMatrixScaling(100.0f, 100.0f, 100.0f);

	sky_ang += 0.05f;
	Math::Matrix rotmat;
	rotmat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(sky_ang));

	sky_mat = rotmat * scalmat;
}

void C_Sky::Draw()
{
	//スカイスフィアの表示
	SHADER.m_standardShader.SetToDevice();
	SHADER.m_standardShader.SetWorldMatrix(sky_mat);
	SHADER.m_standardShader.DrawModel(sky_mod);
}
