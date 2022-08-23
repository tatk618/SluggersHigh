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

	//�����N�ꗗ
	rank[0] = 'S';
	rank[1] = 'A';
	rank[2] = 'B';
	rank[3] = 'C';
	rank[4] = 'D';
	rank[5] = 'E';


	//�ŏ��͍Œ჉���N�́uE�v����J�n
	nowrank = 5;
	
}

ClearScene::~ClearScene()
{

}

void ClearScene::Update(EnemyManager* _enemymanager)
{
	//BGM�Đ��t���O
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
		//�Ó]�I�t
		if (black_Alpha <= 0.0f)
		{
			destroycount_flg = true;
			black_Alpha = 0.0f;

			//���Đ��J�E���g�t���O�I��
			if (destroycount_flg == true)
			{
				destroycount += 1;
				SOUND.PlayBGM("Data/Sound/SoundEffect/countup.wav", 0.2f);
				if (destroycount >= ENEMY.GetEnemyKillCount())
				{
					SOUND.StopBGM("Data/Sound/SoundEffect/countup.wav");
					destroycount = ENEMY.GetEnemyKillCount();
					drone_destroy_score = destroycount * 1000;

					//�v���C���[�̎c��HP���v�Z
					left_playerlife += 1;
					SOUND.PlayBGM("Data/Sound/SoundEffect/countup.wav", 0.2f);
					if (left_playerlife >= lefthp)
					{
						SOUND.StopBGM("Data/Sound/SoundEffect/countup.wav");
						//�v���C���[�̎c��HP���c���Ă��镪�ɓ��B
						left_playerlife = lefthp;
						left_playerlife_score = left_playerlife * 100;

						//���X�R�A�Ƀh���[�����ăX�R�A�ƃv���C���[�̎c��HP�X�R�A�𑫂������̂���
						tmpscore = drone_destroy_score + left_playerlife_score;
						scoreflg = true;
					}

				}

				if (scoreflg == true)
				{
					//�ŏI�X�R�A��ǉ�
					endscore += 1000;
					SOUND.PlayBGM("Data/Sound/SoundEffect/countup.wav", 0.2f);

					//�ŏI�X�R�A�����X�R�A�ȏ�
					if (endscore >= tmpscore)
					{
						SOUND.StopBGM("Data/Sound/SoundEffect/countup.wav");
						//�ŏI�X�R�A�����X�R�A
						endscore = tmpscore;
					}
				}
			}

			//�X�R�A�Ń����N�𔻕�
			switch (endscore)
			{
			//�X�R�A
			case 32000:
				//���̎��̃����N
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
	sprintf_s(text, sizeof(text), "���Đ�=%d\n�c�胉�C�t=%d\n�X�R�A=%d\n�����N=%c",
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
