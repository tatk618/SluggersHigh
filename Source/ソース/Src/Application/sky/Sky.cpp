#include "Sky.h"
#include "../Manager/Model/ModelManager.h"
#include "../Scene/Game/GameScene.h"

C_Sky::C_Sky()
{
	sky_ang = 0;

	//�X�J�C�X�t�B�A�̕\��
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
	//�X�J�C�X�t�B�A�̕\��
	SHADER.m_standardShader.SetToDevice();
	SHADER.m_standardShader.SetWorldMatrix(sky_mat);
	SHADER.m_standardShader.DrawModel(sky_mod);
}
