#include"../Scene/Scene.h"
#include "../Manager/Enemy/EnemyManager.h"

C_Scene::~C_Scene()
{

}

void C_Scene::Update(EnemyManager* _enemymanager)
{

}

void C_Scene::Draw()
{

}

void C_Scene::SetCamera()
{

}

void C_Scene::Frame()
{
	// �Q�[���X�V����
	Update(_enemymanager);

	// �o�b�N�o�b�t�@�N���A
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.0f, 0.0f, 1.0f, 1));//RGBA��0-1�͈̔͂�
	// Z�o�b�t�@�N���A
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//�J�����E���E�̃Z�b�g
	SetCamera();

	// �Q�[���`�揈��
	Draw();

}