#include "../../main.h"
#include "../../define.h"
#include "../../Scene/list/SceneList.h"
#include "../../Scene/Game/GameScene.h"
#include "../../Manager/Enemy/EnemyManager.h"
#include "../../Manager/Texture/TextureManager.h"
#include "../../namespace/namespace.h"
#include "../../States/blow/Blow.h"

void GameScene::Update()
{
	//�eupdate����
	if (player != nullptr)
	{
		if (player_flg == true)
		{
			player->Update(colossus->GetMapModel(),colossus->GetMapMat());
		}
	}

	if (sky != nullptr)
	{
		sky->Update();
	}

	if (lifegauge != nullptr)
	{
		lifegauge->Update(player);
	}

	ENEMY.Update(player,player->GetMat(),explosionManager,player->GetPlayerHP(),redlightManager,hitmarkmanager, colossus->GetMapModel(), colossus->GetMapMat());

	Math::Vector3 fvec, hvec;

	fvec = SHADER.m_cb7_Camera.Work().mV.Invert().Backward();
	hvec = SHADER.m_cb7_Camera.Work().mV.Up();

	SOUND.Update(player->GetPos(), fvec, hvec);

	if (explosionManager != nullptr)
	{
		explosionManager->Update();
	}

	if (redlightManager != nullptr)
	{
		redlightManager->Update();
	}

	if (hitmarkmanager != nullptr)
	{
		hitmarkmanager->Update();
	}

	Math::Vector3 playerFrontvec;
	Math::Vector3 tmpvec;
	tmpvec = { 0,0,1 };
	playerFrontvec = player->GetMat().Translation() + tmpvec;

	SOUND.PlayBGM("Data/Sound/BGM/allornothing.wav", gamebgmvol);

	//�V�[���ύX�t���O���I�t�̂Ƃ�
	if (blackchangeFlg == false)
	{
		black_Alpha -= 0.01f;
		if (black_Alpha <= 0.0f)
		{
			black_Alpha = 0.0f;
		}
	}

	//�ŏI�`�F�b�N�|�C���g��ʉ߂��ĂȂ����c�G����0�̏ꍇ
	if(nowchkpt==5&& ENEMY.GetEnemyLeft() <= 0)
	//if(GetAsyncKeyState(VK_SPACE)&0x8000)
	{
		bgmturnoff_flg = true;
		missiontoujou_flg = true;
		completetoujou_flg = true;
	}

	if (bgmturnoff_flg == true)
	{
		gamebgmvol -= 0.01f;
		if (gamebgmvol <= 0)
		{
			gamebgmvol = 0;
			SOUND.StopBGM("Data/Sound/BGM/allornothing.wav");
		}
	}

	if (missiontoujou_flg == true)
	{
		malpha += 0.01f;
		if (malpha >= 1.0f)
		{
			malpha = 1.0f;
		}
	}

	if (completetoujou_flg == true)
	{
		calpha += 0.01f;
		if (calpha >= 1.0f)
		{
			calpha = 1.0f;
		}
	}
	
	if (malpha >= 1.0f && calpha >= 1.0f)
	{
		count++;
		if (count >= 180)
		{
			count -= 180;
			blackchangeFlg = true;
			clearflg = true;
		}
	}

	//�v���C���[�����t���O���I�t�̏ꍇ
	if (player->GetPlayerAliveFlg() == false)
	{
		bgmturnoff_flg = true;
		blackchangeFlg = true;
		resultflg = true;
	}

	//���U���g��ʕύX�t���O�ƃV�[���ύX�t���O���I���̂Ƃ�
	if (blackchangeFlg == true)
	{
		black_Alpha += 0.005f;
		if (black_Alpha >= 1.0f&& resultflg == true)
		{
			black_Alpha = 1.0f;
			SCENE.ChangeScene(new ResultScene());
		}
	}

	//�N���A��ʕύX�t���O�ƃV�[���ύX�t���O���I���̂Ƃ�
	if (blackchangeFlg == true)
	{
		black_Alpha += 0.005f;
		if (black_Alpha >= 1.0f&&clearflg == true)
		{
			black_Alpha = 1.0f;
			SCENE.ChangeScene(new ClearScene(player));
		}
	}

	//�v���C���[�̋ߐڍU���t���O���I���̂Ƃ�
	if (player->GetMeleeAttackFlg() == true)
	{
		ENEMY.EnemyHitCheck(player->GetMat(), player->GetArmPos(), player->GetR(), player->GetMeleeAttackFlg(), hitmarkmanager);
	}

	enemymaxnum = ENEMY.GetEnemyKillCount();

	if (nowchkpt < chkpt)
	{
		//�v���C���[�ƃ`�F�b�N�|�C���g�̃x�N�g��
		Math::Vector3 chkpt_ply_vec;
		chkpt_ply_vec = chkpt_pos[nowchkpt] - player->GetMat().Translation();
		chkpt_ply_len = chkpt_ply_vec.Length();
		if (chkpt_ply_len <= 50)
		{
			for (int e = 0;e < enemynum;e++)
			{
				ENEMY.Spawn(this, chkpt_pos[nowchkpt]);
			}

			//�G�̓����o�����������₷
			enemynum += 2;
			//�`�F�b�N�|�C���g���X�V
			nowchkpt++;
			//���B�t���O���I���ɂ���
			reach_flg[rflg] = true;
		}
	}
}

void GameScene::Draw()
{
	//�e�`�ʏ���
	SHADER.m_standardShader.SetToDevice();

	if (player != nullptr)
	{
		if (player_flg == true)
		{
			player->Draw();
		}
	}

	if (colossus != nullptr)
	{
		colossus->Draw();
	}

	//���C�e�B���O(���ɑ΂���A�e�̌v�Z�j��OFF�ɂ���
	SHADER.m_standardShader.SetLightEnable(false);

	if (sky != nullptr)
	{
		sky->Draw();
	}

	//���C�e�B���O��ON�ɖ߂�
	SHADER.m_standardShader.SetLightEnable(true);

	ENEMY.Draw();

	//Z�o�b�t�@���������݂��Ȃ��悤�ɂ���
	D3D.GetDevContext()->OMSetDepthStencilState(D3D.CreateDepthStencilState(true, false), 0);
	//���Z����
	D3D.GetDevContext()->OMSetBlendState(m_BlendAdd, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	if (hitmarkmanager != nullptr)
	{
		hitmarkmanager->Draw();
	}

	//�ȉ��G�t�F�N�g�n�����̕`��
	if (explosionManager != nullptr)
	{
		explosionManager->Draw();
	}

	//if (redlightManager != nullptr)
	//{
	//	redlightManager->Draw();
	//}

	//Z�o�b�t�@���������݂��Ȃ��悤�ɂ���
	D3D.GetDevContext()->OMSetDepthStencilState(D3D.CreateDepthStencilState(true, true), 0);
	//���Z����
	D3D.GetDevContext()->OMSetBlendState(m_BlendAlpha, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	SHADER.m_spriteShader.Begin();

	if (lifegauge != nullptr)
	{
		lifegauge->Draw();
	}
	
	if (mission_tex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(mission_mat);
		SHADER.m_spriteShader.DrawTex(mission_tex, Math::Rectangle(0, 0, 500, 200), malpha);
	}

	if (complete_tex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(complete_mat);
		SHADER.m_spriteShader.DrawTex(complete_tex, Math::Rectangle(0, 0, 600, 200), calpha);
	}

	char text[200];
	sprintf_s(text, sizeof(text), "X=%f\nY=%f\nZ=%f\nSCORE=%d\n�c�G=%d\nNOWCHKPT=%d", 
		player->GetMat().Translation().x, player->GetMat().Translation().y, player->GetMat().Translation().z,
		ENEMY.GetScore(),ENEMY.GetEnemyLeft(), nowchkpt);
	SHADER.m_spriteShader.DrawString(spriteFont, -600, 300, text, Math::Vector4(1, 1, 1, 1));

	SHADER.m_spriteShader.End();

	if (blackscreen_tex != nullptr)
	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawTex(blackscreen_tex, Math::Rectangle(0, 0, 1280, 720), black_Alpha);
	}

}

void GameScene::Frame()
{
	// �Q�[���X�V����
	Update();

	// �o�b�N�o�b�t�@�N���A
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.0f, 0.0f, 1.0f, 1));//RGBA��0-1�͈̔͂�
	// Z�o�b�t�@�N���A
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//�J����
	Math::Matrix tmpRotY, tmpRotX;
	tmpRotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(player->GetcameraY()));
	tmpRotX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(player->GetcameraX()));

	tmpRotX = tmpRotX * tmpRotY;

	camera_Look = DirectX::XMVector3TransformCoord(Math::Vector3(5, 4, -2), player->GetMat());
	camera_Pos = DirectX::XMVector3TransformCoord(Math::Vector3(0, 5, -10), tmpRotX);
	camera_Pos += camera_Look;

	camera_Head = { 0,1,0 };

	SHADER.m_cb7_Camera.Work().mV = DirectX::XMMatrixLookAtLH
	(
		camera_Pos,
		camera_Look,
		camera_Head
	);

	SHADER.m_cb7_Camera.Work().CamPos = SHADER.m_cb7_Camera.Work().mV.Invert().Translation();

	//�ˉe�s��
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), (float)ScrW / (float)ScrH, 0.01f, 3500.0f);

	//�r���[�s��E�ˉe�s���GPU�ɓ]��
	SHADER.m_cb7_Camera.Write();

	// �Q�[���`�揈��
	Draw();
}

void GameScene::Init()
{
	//���Z�����p�X�e�[�g
	m_BlendAdd = D3D.CreateBlendState(KdBlendMode::Add);
	//�ʏ피�����p�X�e�[�g
	m_BlendAlpha = D3D.CreateBlendState(KdBlendMode::Alpha);

	//��p�̑傫���i�W���l60�j
	fov = 60;

	// ���s���C�g�ݒ�
	SHADER.m_cb8_Light.Work().DL_Dir = { 1,-1,0 };
	SHADER.m_cb8_Light.Work().DL_Dir.Normalize();
	SHADER.m_cb8_Light.Work().DL_Color = { 3,3,3 };

	//����
	SHADER.m_cb8_Light.Work().AmbientLight = { 0.5f,0.5f,0.5f };

	// ���C�g�̃f�[�^����������
	SHADER.m_cb8_Light.Write();

	//�C���X�^���X���Ə�����
	player = new C_Player(this);
	//drone = new C_Drone(this);
	sky = new C_Sky();
	explosionManager = new ExplosionManager();
	redlightManager = new RedlightManager();
	hitmarkmanager = new hitmarkManager();
	colossus = new C_Colossus();
	lifegauge = new LifeGauge();

	//�Ó]�p��ʓǂݍ���
	blackscreen_tex = TEXTURE.GetTexture("Data/Texture/black.png");

	mission_tex = TEXTURE.GetTexture("Data/Texture/mission.png");
	complete_tex = TEXTURE.GetTexture("Data/Texture/complete.png");

	player = new C_Player(this);

	//�Ó]�p��ʂ̏����A���t�@�l
	black_Alpha = 1.0f;

	spriteFont = new DirectX::SpriteFont(D3D.GetDev(), L"Data/Font/SpriteFont.dat");

	spawnlimitter_flg = false;
	spawn_flg = false;

	//�c�莞��(�b�~60)
	left_time = 60 * 60;
	//�ŏ��̓G�̓����o����
	enemynum = 1;

	//�e�`�F�b�N�|�C���g�̈ʒu
	chkpt_pos[0] = { 680,15,-500 };
	chkpt_pos[1] = { 640,15,-180 };
	chkpt_pos[2] = { 320,15,-20 };
	chkpt_pos[3] = { -200,15,-260 };
	chkpt_pos[4] = { -480,15,430 };

	mission_pos = { 0,180,0 };
	complete_pos = { 0,90,0 };

	malpha = 0.0f;
	calpha = 0.0f;

	Math::Matrix mission_tmat = DirectX::XMMatrixTranslation(mission_pos.x, mission_pos.y, mission_pos.z);
	Math::Matrix complete_tmat = DirectX::XMMatrixTranslation(complete_pos.x, complete_pos.y, complete_pos.z);

	mission_mat = mission_tmat * mission_mat;
	complete_mat = complete_tmat * complete_mat;

	gamebgmvol = 0.1f;
}

void GameScene::Release()
{
	m_BlendAdd->Release();
	m_BlendAlpha->Release();

	/*�v���C���[�����[�X*/
	if (player != nullptr)
	{
		delete player;
	}

	/*���������[�X*/
	if (explosionManager != nullptr)
	{
		delete explosionManager;
	}

	//if (redlightManager != nullptr)
	//{
	//	delete redlightManager;
	//}

	if (hitmarkmanager != nullptr)
	{
		delete hitmarkmanager;
	}

	/*�󃊃��[�X*/
	if (sky != nullptr)
	{
		delete sky;
	}

	/*�X�e�[�W�̃����[�X*/
	if (colossus != nullptr)
	{
		delete colossus;
	}
}

