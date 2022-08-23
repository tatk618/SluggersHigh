#include "../../Scene/Clear/ClearScene.h"
#include "../../Scene/list/SceneList.h"
#include "../../Scene/Game/GameScene.h"
#include "../../Manager/Texture/TextureManager.h"
#include "../../player/Player.h"
#include "../../main.h"

ClearScene::ClearScene(C_Player* _player)
{

	resultTex = TEXTURE.GetTexture("Data/Texture/result.jpg");
	blackTex = TEXTURE.GetTexture("Data/Texture/black.png");

	black_Alpha = 1.0f;
	spriteFont = new DirectX::SpriteFont(D3D.GetDev(), L"Data/Font/SpriteFont.dat");

	lefthp = _player->GetPlayerHP();

	count = 0;
	left_playerlife = 0;
	destroycount = 0;
	left_playerlife_score = 0;
	tmpscore = 0;
	endscore = 0;

	destroycount_flg = false;
	playerlefthp_flg = false;
	bgmturnon_flg = true;
	enterFlg = false;

	//ランク一覧
	rank[0] = 'S';
	rank[1] = 'A';
	rank[2] = 'B';
	rank[3] = 'C';
	rank[4] = 'D';
	rank[5] = 'E';


	//最初は最低ランクの「E」から開始
	nowrank = 5;
	
}

ClearScene::~ClearScene()
{

}

void ClearScene::Update(EnemyManager* _enemymanager)
{
	//BGM再生フラグ
	if (bgmturnon_flg == true)
	{
		SOUND.PlayBGM("Data/Sound/BGM/result.wav", 0.15f);
	}
	else {
		SOUND.StopBGM("Data/Sound/BGM/result.wav");
	}

	if (resultchangeFlg == false)
	{
		black_Alpha -= 0.01f;
		bgmturnon_flg = true;
		//暗転オフ
		if (black_Alpha <= 0.0f)
		{
			destroycount_flg = true;
			black_Alpha = 0.0f;

			//撃墜数カウントフラグオン
			if (destroycount_flg == true)
			{
				destroycount += 1;
				SOUND.PlayBGM("Data/Sound/SoundEffect/countup.wav", 0.2f);
				if (destroycount >= ENEMY.GetEnemyKillCount())
				{
					SOUND.StopBGM("Data/Sound/SoundEffect/countup.wav");
					destroycount = ENEMY.GetEnemyKillCount();
					drone_destroy_score = destroycount * 1000;

					//プレイヤーの残りHPを計算
					left_playerlife += 1;
					SOUND.PlayBGM("Data/Sound/SoundEffect/countup.wav", 0.2f);
					if (left_playerlife >= lefthp)
					{
						SOUND.StopBGM("Data/Sound/SoundEffect/countup.wav");
						//プレイヤーの残りHPが残っている分に到達
						left_playerlife = lefthp;
						left_playerlife_score = left_playerlife * 100;

						//仮スコアにドローン撃墜スコアとプレイヤーの残りHPスコアを足したものを代入
						tmpscore = drone_destroy_score + left_playerlife_score;
						scoreflg = true;
					}

				}

				if (scoreflg == true)
				{
					//最終スコアを追加
					endscore += 1000;
					SOUND.PlayBGM("Data/Sound/SoundEffect/countup.wav", 0.2f);

					//最終スコア＝仮スコア以上
					if (endscore >= tmpscore)
					{
						SOUND.StopBGM("Data/Sound/SoundEffect/countup.wav");
						//最終スコア＝仮スコア
						endscore = tmpscore;
					}
				}
			}

			//スコアでランクを判別
			switch (endscore)
			{
			//スコア
			case 32000:
				//その時のランク
				nowrank = 0;
				break;
			case 16000:
				nowrank = 1;
				break;
			case 8000:
				nowrank = 2;
				break;
			case 4000:
				nowrank = 3;
				break;
			case 2000:
				nowrank = 4;
				break;
			case 0:
				nowrank = 5;
				break;
			}
		}
	}

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		enterFlg = true;
	}

	if (enterFlg == true)
	{
		if (resultchangeFlg == false)
		{
			resultchangeFlg = true;
			SOUND.Play2D("Data/Sound/SoundEffect/enter18.wav", 0.3f);
		}
	}

	if (resultchangeFlg == true)
	{
		black_Alpha += 0.01f;
		bgmturnon_flg = false;
		if (black_Alpha >= 1.0f)
		{
			black_Alpha = 1.0f;
			enterFlg = false;
			SCENE.ChangeScene(new TitleScene());
		}
	}
}

void ClearScene::Draw()
{
	if (resultTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawTex(resultTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);
	}

	SHADER.m_spriteShader.Begin();

	char text[200];
	sprintf_s(text, sizeof(text), "撃墜数=%d\n残りライフ=%d\nスコア=%d\nランク=%c",
		destroycount,left_playerlife, endscore,rank[nowrank]);
	SHADER.m_spriteShader.DrawString(spriteFont, 0, 0, text, Math::Vector4(1, 1, 1, 1));

	SHADER.m_spriteShader.End();

	if (blackTex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawTex(blackTex, Math::Rectangle(0, 0, 1280, 720), black_Alpha);
	}

	SHADER.m_standardShader.SetToDevice();
}
