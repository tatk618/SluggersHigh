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
	// ゲーム更新処理
	Update(_enemymanager);

	// バックバッファクリア
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.0f, 0.0f, 1.0f, 1));//RGBAを0-1の範囲で
	// Zバッファクリア
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//カメラ・視界のセット
	SetCamera();

	// ゲーム描画処理
	Draw();

}