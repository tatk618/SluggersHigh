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
	//各update処理
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

	//シーン変更フラグがオフのとき
	if (blackchangeFlg == false)
	{
		black_Alpha -= 0.01f;
		if (black_Alpha <= 0.0f)
		{
			black_Alpha = 0.0f;
		}
	}

	//最終チェックポイントを通過してなおかつ残敵数が0の場合
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

	//プレイヤー生存フラグがオフの場合
	if (player->GetPlayerAliveFlg() == false)
	{
		bgmturnoff_flg = true;
		blackchangeFlg = true;
		resultflg = true;
	}

	//リザルト画面変更フラグとシーン変更フラグがオンのとき
	if (blackchangeFlg == true)
	{
		black_Alpha += 0.005f;
		if (black_Alpha >= 1.0f&& resultflg == true)
		{
			black_Alpha = 1.0f;
			SCENE.ChangeScene(new ResultScene());
		}
	}

	//クリア画面変更フラグとシーン変更フラグがオンのとき
	if (blackchangeFlg == true)
	{
		black_Alpha += 0.005f;
		if (black_Alpha >= 1.0f&&clearflg == true)
		{
			black_Alpha = 1.0f;
			SCENE.ChangeScene(new ClearScene(player));
		}
	}

	//プレイヤーの近接攻撃フラグがオンのとき
	if (player->GetMeleeAttackFlg() == true)
	{
		ENEMY.EnemyHitCheck(player->GetMat(), player->GetArmPos(), player->GetR(), player->GetMeleeAttackFlg(), hitmarkmanager);
	}

	enemymaxnum = ENEMY.GetEnemyKillCount();

	if (nowchkpt < chkpt)
	{
		//プレイヤーとチェックポイントのベクトル
		Math::Vector3 chkpt_ply_vec;
		chkpt_ply_vec = chkpt_pos[nowchkpt] - player->GetMat().Translation();
		chkpt_ply_len = chkpt_ply_vec.Length();
		if (chkpt_ply_len <= 50)
		{
			for (int e = 0;e < enemynum;e++)
			{
				ENEMY.Spawn(this, chkpt_pos[nowchkpt]);
			}

			//敵の同時出現数を二個ずつ増やす
			enemynum += 2;
			//チェックポイントを更新
			nowchkpt++;
			//到達フラグをオンにする
			reach_flg[rflg] = true;
		}
	}
}

void GameScene::Draw()
{
	//各描写処理
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

	//ライティング(光に対する陰影の計算）をOFFにする
	SHADER.m_standardShader.SetLightEnable(false);

	if (sky != nullptr)
	{
		sky->Draw();
	}

	//ライティングをONに戻す
	SHADER.m_standardShader.SetLightEnable(true);

	ENEMY.Draw();

	//Zバッファを書き込みしないようにする
	D3D.GetDevContext()->OMSetDepthStencilState(D3D.CreateDepthStencilState(true, false), 0);
	//加算合成
	D3D.GetDevContext()->OMSetBlendState(m_BlendAdd, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	if (hitmarkmanager != nullptr)
	{
		hitmarkmanager->Draw();
	}

	//以下エフェクト系処理の描写
	if (explosionManager != nullptr)
	{
		explosionManager->Draw();
	}

	//if (redlightManager != nullptr)
	//{
	//	redlightManager->Draw();
	//}

	//Zバッファを書き込みしないようにする
	D3D.GetDevContext()->OMSetDepthStencilState(D3D.CreateDepthStencilState(true, true), 0);
	//加算合成
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
	sprintf_s(text, sizeof(text), "X=%f\nY=%f\nZ=%f\nSCORE=%d\n残敵=%d\nNOWCHKPT=%d", 
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
	// ゲーム更新処理
	Update();

	// バックバッファクリア
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.0f, 0.0f, 1.0f, 1));//RGBAを0-1の範囲で
	// Zバッファクリア
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//カメラ
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

	//射影行列
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), (float)ScrW / (float)ScrH, 0.01f, 3500.0f);

	//ビュー行列・射影行列をGPUに転送
	SHADER.m_cb7_Camera.Write();

	// ゲーム描画処理
	Draw();
}

void GameScene::Init()
{
	//加算合成用ステート
	m_BlendAdd = D3D.CreateBlendState(KdBlendMode::Add);
	//通常半透明用ステート
	m_BlendAlpha = D3D.CreateBlendState(KdBlendMode::Alpha);

	//画角の大きさ（標準値60）
	fov = 60;

	// 平行ライト設定
	SHADER.m_cb8_Light.Work().DL_Dir = { 1,-1,0 };
	SHADER.m_cb8_Light.Work().DL_Dir.Normalize();
	SHADER.m_cb8_Light.Work().DL_Color = { 3,3,3 };

	//環境光
	SHADER.m_cb8_Light.Work().AmbientLight = { 0.5f,0.5f,0.5f };

	// ライトのデータを書き込む
	SHADER.m_cb8_Light.Write();

	//インスタンス化と初期化
	player = new C_Player(this);
	//drone = new C_Drone(this);
	sky = new C_Sky();
	explosionManager = new ExplosionManager();
	redlightManager = new RedlightManager();
	hitmarkmanager = new hitmarkManager();
	colossus = new C_Colossus();
	lifegauge = new LifeGauge();

	//暗転用画面読み込み
	blackscreen_tex = TEXTURE.GetTexture("Data/Texture/black.png");

	mission_tex = TEXTURE.GetTexture("Data/Texture/mission.png");
	complete_tex = TEXTURE.GetTexture("Data/Texture/complete.png");

	player = new C_Player(this);

	//暗転用画面の初期アルファ値
	black_Alpha = 1.0f;

	spriteFont = new DirectX::SpriteFont(D3D.GetDev(), L"Data/Font/SpriteFont.dat");

	spawnlimitter_flg = false;
	spawn_flg = false;

	//残り時間(秒×60)
	left_time = 60 * 60;
	//最初の敵の同時出現数
	enemynum = 1;

	//各チェックポイントの位置
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

	/*プレイヤーリリース*/
	if (player != nullptr)
	{
		delete player;
	}

	/*爆発リリース*/
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

	/*空リリース*/
	if (sky != nullptr)
	{
		delete sky;
	}

	/*ステージのリリース*/
	if (colossus != nullptr)
	{
		delete colossus;
	}
}

