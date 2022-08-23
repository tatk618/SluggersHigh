#include "../../Scene/Result/ResultScene.h"
#include "../../Scene/list/SceneList.h"
#include "../../Scene/Game/GameScene.h"
#include "../../Manager/Texture/TextureManager.h"
#include "../../main.h"

ResultScene::ResultScene()
{
	resultTex = TEXTURE.GetTexture("Data/Texture/result.png");
	blackTex = TEXTURE.GetTexture("Data/Texture/black.png");

	black_Alpha = 1.0f;

	spriteFont = new DirectX::SpriteFont(D3D.GetDev(), L"Data/Font/SpriteFont.dat");
}

ResultScene::~ResultScene()
{

}

void ResultScene::Update(EnemyManager* _enemymanager)
{
	if (resultchangeFlg == false)
	{
		black_Alpha -= 0.01f;
		if (black_Alpha <= 0.0f)
		{
			black_Alpha = 0.0f;
		}
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		continueFlg = true;
	}

	if (continueFlg == true)
	{
		black_Alpha += 0.01f;
		if (black_Alpha >= 1.0f)
		{
			black_Alpha = 1.0f;
			SCENE.ChangeScene(new GameScene());
		}
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		titlebackFlg = true;
	}

	if (titlebackFlg == true)
	{
		black_Alpha += 0.01f;
		if (black_Alpha >= 1.0f)
		{
			black_Alpha = 1.0f;
			SCENE.ChangeScene(new TitleScene());
		}
	}
}

void ResultScene::Draw()
{
	if (resultTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawTex(resultTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);
	}

	SHADER.m_spriteShader.Begin();

	char text[200];
	sprintf_s(text, sizeof(text), "”C–±Ž¸”s");
	SHADER.m_spriteShader.DrawString(spriteFont, 0, 0, text, Math::Vector4(1, 1, 1, 1));

	SHADER.m_spriteShader.End();

	if (blackTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawTex(blackTex, Math::Rectangle(0, 0, 1280, 720), black_Alpha);
	}

	SHADER.m_standardShader.SetToDevice();
}
