#include "../../Scene/Title/TitleScene.h"
#include "../../Scene/list/SceneList.h"
#include "../../Manager/Texture/TextureManager.h"
#include "../../Manager/sound/SoundManager.h"
#include "../../main.h"

TitleScene::TitleScene()
{
	titleTex = TEXTURE.GetTexture("Data/Texture/title.png");
	blackTex = TEXTURE.GetTexture("Data/Texture/black.png");
	hitenterTex = TEXTURE.GetTexture("Data/Texture/hitenter.png");

	title_Alpha = 0.0f;
	hitenter_Alpha = 0.0f;

	entermat = DirectX::XMMatrixTranslation(0, -100, 0);

	bgmplay_flg = true;
	enterFlg = false;

	bgmvol = 0.1f;
}

TitleScene::~TitleScene()
{

}

void TitleScene::Update(EnemyManager* _enemymanager)
{
	if (bgmplay_flg == true)
	{
		SOUND.PlayBGM("Data/Sound/BGM/samesamewanisame.wav", bgmvol);
	}
	else {
		SOUND.StopBGM("Data/Sound/BGM/samesamewanisame.wav");
	}

	if (scenechangeFlg == false)
	{
		title_Alpha += 0.01f;
		if (title_Alpha >= 1.0f)
		{
			bgmplay_flg = true;
			title_Alpha = 1.0f;
			hitenter_Alpha += 0.01f;
			if (hitenter_Alpha >= 1.0f)
			{
				hitenter_Alpha = 1.0f;
			}
		}
	}

	//キーでシーン変更フラグをオンにする
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		enterFlg = true;
	}

	if (enterFlg == true)
	{
		if (scenechangeFlg == false)
		{
			scenechangeFlg = true;
			SOUND.Play2D("Data/Sound/SoundEffect/enter18.wav", 0.3f);
		}
	}

	//シーン変更処理
	if (scenechangeFlg == true)
	{
		hitenter_Alpha -= 0.01f;
		if (hitenter_Alpha <= 0.0f)
		{
			hitenter_Alpha = 0.0f;
			title_Alpha -= 0.01f;

			if (title_Alpha <= 0.0f)
			{
				bgmplay_flg = false;
				title_Alpha = 0.0f;
				enterFlg = false;
				SCENE.ChangeScene(new GameScene());
			}

			bgmvol -= 0.01f;
			if (bgmvol <= 0)
			{
				bgmvol = 0;
				SOUND.StopBGM("Data/Sound/BGM/samesamewanisame.wav");
			}
		}
	}
}

void TitleScene::Draw()
{
	if (blackTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawTex(blackTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);
	}

	if (titleTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawTex(titleTex, Math::Rectangle(0, 0, 1280, 720), title_Alpha);
	}

	if (hitenterTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(entermat);
		SHADER.m_spriteShader.DrawTex(hitenterTex, Math::Rectangle(0, 0, 600, 400), hitenter_Alpha);
	}

	SHADER.m_standardShader.SetToDevice();
}
