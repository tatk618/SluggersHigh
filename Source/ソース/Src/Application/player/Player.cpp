#include "../player/Player.h"
#include "../define.h"
#include "../Scene/Game/GameScene.h"
#include "../Manager/Model/ModelManager.h"
#include "../Manager/Enemy/EnemyManager.h"
#include "../States/list/Statelist.h"
#include "../namespace/namespace.h"
#include "../main.h"
#include "../Manager/sound/SoundManager.h"

//コンストラクタ
C_Player::C_Player(GameScene* _gameScene)
{
	m_gameScene = _gameScene;

	playerhead_mod = MODEL.GetModel("Data/Model/player/head.gltf");

	torsoupper_mod = MODEL.GetModel("Data/Model/player/upper.gltf");
	torsolower_mod = MODEL.GetModel("Data/Model/player/lower.gltf");

	leftleg_mod = MODEL.GetModel("Data/Model/player/leftLeg.gltf");
	rightleg_mod = MODEL.GetModel("Data/Model/player/rightLeg.gltf");

	arm_mod = MODEL.GetModel("Data/Model/player/batarms.gltf");

	//ゲーム画面の中央座標（クライアント座標）を代入する
	base_point = { ScrW / 2,ScrH / 2 };

	//クライアント座標をスクリーン座標に治す
	ClientToScreen(APP.m_window.GetWndHandle(), &base_point);

	//マウスをゲーム画面中央に移動
	SetCursorPos(base_point.x, base_point.y);

	//マウスカーソルを削除
	ShowCursor(FALSE);

	//初期カメラ角度
	camera_ang_X = -30;
	camera_ang_Y = -30;

	player_pos = { 745,15,-670 };
	head_pos = { 0,0,0 };
	
	arm_pos = {0,0,-2};

	leftleg_pos = { 0,1,0 };
	rightleg_pos = { 0,1,0 };

	leftleg_ang = 0;
	rightleg_ang = 0;
	kani_sinAng = 0;
	fb_sinAng = 0;

	//プレイヤーの初期角度
	player_ang = -90;

	//プレイヤーの初期hp
	player_hp = 100;

	//バットを止める長さ
	batstopcount = 0;

	swayspeed = 0;

	playerJumpFlg = false;

	meleeattackFlg = false;

	player_able_command = true;

	player_alive_flg = true;

	regainflg = false;

	walksound_flg = false;

	lowhp_count = 0;

	static const int bat_stopcount = 3 * 60;

	Math::Matrix rotMat;
	Math::Matrix transMat;
	transMat = DirectX::XMMatrixTranslation(10, 2, 30);
	rotMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(45));
}

void C_Player::Update(const KdModel& _mapmodel, const Math::Matrix& _mapmat)
{
	Math::Matrix camera_mat;
	Math::Vector3 moveVec = {};

	move_flg = false;
	
	bool kani_flg = false;

	if (camera_ang_X < -60)
	{
		camera_ang_X = -60;
	}

	if (APP.m_window.GetWndHandle() == GetActiveWindow())
	{
		POINT pt;
		GetCursorPos(&pt);

		camera_ang_X += (pt.y - base_point.y) / 6.0f;
		camera_ang_Y += (pt.x - base_point.x) / 6.0f;

		if (camera_ang_X > 40)
		{
			camera_ang_X = 40;
		}

		if (camera_ang_X < -80)
		{
			camera_ang_X = -80;
		}

		//カメラの感度に反映し終えたらマウス位置を基準座標に戻す
		SetCursorPos(base_point.x, base_point.y);
	}

	//プレイヤーのHPが０になったとき
	if (player_hp <= 0)
	{
		//プレイヤーのHPを０に固定する
		player_hp = 0;

		//プレイヤーの生存フラグをオフにする
		player_alive_flg = false;
	}

	//プレイヤーの生存フラグがオフのとき
	if (player_alive_flg == false)
	{
		//プレイヤーが倒れるモーション
		player_angX += 0.5f;
		if (player_angX >= 90)
		{
			player_angX = 90;
		}

		player_pos.y -= 0.1f;
		if (player_pos.y <= 8.5f)
		{
			player_pos.y = 8.5f;
		}

		arm_angY -= 0.5f;
		if (arm_angY <= -90)
		{
			arm_angY = -90;
		}

		arm_angZ -= 0.5f;
		if (arm_angZ <= -20)
		{
			arm_angZ = -20;
		}

		//プレイヤーの操作可能フラグをオフにする
		player_able_command = false;
	}
	
	//プレイヤーの操作可能フラグがオンのとき
	if (player_able_command == true)
	{
		//移動・前進
		if (GetAsyncKeyState('W') & 0x8000)
		{
			camera_mat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(camera_ang_Y));
			moveVec += DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), camera_mat);
			move_flg = true;

			Math::Vector3 vec;
			vec = GetFrontVec();

			bool hitflg;
			Math::Vector3 returnVec;
			hitflg = m_gameScene->GetWall()->Checkhit(player_pos, vec, hitDis, returnVec);

			if (hitflg)
			{
				player_pos += returnVec;
			}

			kani_flg = true;

			kani_sinAng += 5.0f;
		}

		//移動・後退
		if (GetAsyncKeyState('S') & 0x8000)
		{
			camera_mat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(camera_ang_Y));
			moveVec += DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, -1), camera_mat);
			move_flg = true;

			Math::Vector3 vec;
			vec = GetBackVec();

			bool hitflg;
			Math::Vector3 returnVec;
			hitflg = m_gameScene->GetWall()->Checkhit(player_pos, vec, hitDis, returnVec);

			if (hitflg)
			{
				player_pos += returnVec;
			}

			kani_flg = true;

			kani_sinAng += 5.0f;
		}

		kani_sinAng = fmod(kani_sinAng, 180);

		if (kani_flg == false)
		{
			kani_sinAng -= 5.0f / 2;

			if (kani_sinAng <= 0)
			{
				kani_sinAng += 5.0f / 2;
			}
		}

		bool fb_flg = false;

		//移動・左平行移動
		if (GetAsyncKeyState('A') & 0x8000)
		{
			camera_mat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(camera_ang_Y));
			moveVec += DirectX::XMVector3TransformNormal(Math::Vector3(-1, 0, 0), camera_mat);
			move_flg = true;

			Math::Vector3 vec;
			vec = GetLeftVec();

			bool hitflg;
			Math::Vector3 returnVec;
			hitflg = m_gameScene->GetWall()->Checkhit(player_pos, vec, hitDis, returnVec);

			if (hitflg)
			{
				player_pos += returnVec;
			}

			fb_flg = true;

			fb_sinAng += 5.0f;

			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				fb_sinAng += 7.5f;
			}
		}

		//移動・右平行移動
		if (GetAsyncKeyState('D') & 0x8000)
		{
			camera_mat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(camera_ang_Y));
			moveVec += DirectX::XMVector3TransformNormal(Math::Vector3(1, 0, 0), camera_mat);
			move_flg = true;

			Math::Vector3 vec;
			vec = GetRightVec();

			bool hitflg;
			Math::Vector3 returnVec;
			hitflg = m_gameScene->GetWall()->Checkhit(player_pos, vec, hitDis, returnVec);

			if (hitflg)
			{
				player_pos += returnVec;
			}

			fb_flg = true;

			fb_sinAng += 5.0f;
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				fb_sinAng += 7.5f;
			}
		}

		float dis;//レイが当たった所までの距離(レイが当たったときのみ情報が上書きされる)

		Math::Vector3 wallnormal;
		if (function::ModelIntersects(_mapmodel, _mapmat, player_pos, Math::Vector3(0, -1, 0), dis,wallnormal))
		{
			//dis・・・地面から上に飛び出している主人公の体の長さ
			player_pos.y += (3 - dis);
		}

		//マウス左ボタンで近接攻撃
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (LB_flg == false)
			{
				LB_flg = true;

				if (attackstart_flg == false)
				{
					SOUND.Play2D("Data/Sound/SoundEffect/suburi.wav", 0.3f);

					meleeattackFlg = true;
					attackstart_flg = true;
				}
			}
		}
		else
		{
			LB_flg = false;
		}

		//近接攻撃フラグがオンのときにバットを振る
		if (meleeattackFlg == true)
		{
			aim_flg = true;
			arm_angY -= 35;
			if (arm_angY <= -250)
			{
				arm_angY = -250;
				batstopcount += 1;
			}
			upper_ang -= 10;
			if (upper_ang <= -90)
			{
				upper_ang = -90;
				batstopcount += 1;
			}
		}

		//バットを振ってから30経過すると近接攻撃フラグをオフにする
		if (batstopcount >= 30)
		{
			meleeattackFlg = false;
			batstopcount -= 30;
		}

		//近接攻撃フラグがオフのときは最初の状態まで戻る
		if (meleeattackFlg == false)
		{
			aim_flg = false;
			arm_angY += 25;
			if (arm_angY >= 0)
			{
				attackstart_flg = false;
				arm_angY = 0;
			}
			upper_ang += 20;
			if (upper_ang >= 0)
			{
				upper_ang = 0;
			}
		}

		//構えたときの処理
		if (aim_flg == true)
		{
			arm_angZ += 3;
			if (arm_angZ >= 15)
			{
				arm_angZ = 15;
			}
		}
		else {
			arm_angZ = -5;
			if (arm_angZ <= -15)
			{
				arm_angZ = -15;
			};
		}

		if (walksound_flg == true)
		{
			SOUND.PlayBGM("Data/Sound/SoundEffect/walk.wav", 0.15f);
		}
		else {
			SOUND.StopBGM("Data/Sound/SoundEffect/walk.wav");
		}

		//移動フラグがオンのときにmovevec分だけ移動する
		if (move_flg == true)
		{
			//movevecの正規化
			moveVec.Normalize();
			//移動速度
			moveVec *= 1.0f;

			walksound_flg = true;
			player_pos += moveVec;
		}
		else {
			walksound_flg = false;
		}

		player_ang = camera_ang_Y;
	}
	else 
	{
		SOUND.StopBGM("Data/Sound/SoundEffect/walk.wav");
	}

	//行列
	Math::Matrix rotmatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(player_angX));
	Math::Matrix rotmatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(player_ang));
	Math::Matrix transmat = DirectX::XMMatrixTranslation(player_pos.x, player_pos.y, player_pos.z);
	player_mat = rotmatY * rotmatX * transmat;

	//上半身の行列＝プレイヤーの行列
	upper_mat = player_mat;

	head_trans = DirectX::XMMatrixTranslation(head_pos.x, head_pos.y, head_pos.z);
	head_rot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(playerhead_ang));
	playerhead_mat = head_rot * head_trans * player_mat;

	Math::Matrix rotarmY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(arm_angY));

	Math::Matrix rotarmZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(arm_angZ));

	Math::Matrix transarm = DirectX::XMMatrixTranslation(arm_pos.x,arm_pos.y, arm_pos.z);

	arm_mat = transarm * rotarmZ * rotarmY * player_mat;

	Math::Matrix UpperRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(upper_ang));
	upper_mat = UpperRot * player_mat;

	float zplus = sin(DirectX::XMConvertToRadians(kani_sinAng));
	float xplus = sin(DirectX::XMConvertToRadians(fb_sinAng));

	leftleg_trans = DirectX::XMMatrixTranslation(leftleg_pos.x+xplus, leftleg_pos.y, leftleg_pos.z+zplus);
	leftleg_rot = DirectX::XMMatrixIdentity();
	leftleg_mat = leftleg_rot * leftleg_trans * player_mat;

	rightleg_trans = DirectX::XMMatrixTranslation(rightleg_pos.x-xplus, rightleg_pos.y, rightleg_pos.z-zplus);
	rightleg_rot = DirectX::XMMatrixIdentity();
	rightleg_mat = rightleg_rot * rightleg_trans * player_mat;
}

//表示
void C_Player::Draw()
{
	SHADER.m_standardShader.SetToDevice();

	SHADER.m_standardShader.SetWorldMatrix(upper_mat);
	SHADER.m_standardShader.DrawModel(torsoupper_mod);

	SHADER.m_standardShader.SetWorldMatrix(player_mat);
	SHADER.m_standardShader.DrawModel(torsolower_mod);

	SHADER.m_standardShader.SetWorldMatrix(playerhead_mat);
	SHADER.m_standardShader.DrawModel(playerhead_mod);
	
	SHADER.m_standardShader.SetWorldMatrix(arm_mat);
	SHADER.m_standardShader.DrawModel(arm_mod);

	SHADER.m_standardShader.SetWorldMatrix(leftleg_mat);
	SHADER.m_standardShader.DrawModel(leftleg_mod);

	SHADER.m_standardShader.SetWorldMatrix(rightleg_mat);
	SHADER.m_standardShader.DrawModel(rightleg_mod);

}

Math::Vector3 C_Player::GetLookVec() const
{
	Math::Matrix rotmat;
	rotmat = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(camera_ang_X), DirectX::XMConvertToRadians(camera_ang_Y),0);

	Math::Vector3 lookVec;
	lookVec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), rotmat);

	return lookVec;
}

Math::Vector3 C_Player::GetFrontVec() const
{
	Math::Matrix rotmatY;
	rotmatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(camera_ang_Y));

	Math::Vector3 frontvec;
	frontvec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), rotmatY);

	return frontvec;
}

Math::Vector3 C_Player::GetBackVec() const
{
	Math::Matrix rotmatY;
	rotmatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(camera_ang_Y));

	Math::Vector3 backvec;
	backvec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, -1), rotmatY);

	return backvec;
}

Math::Vector3 C_Player::GetLeftVec() const
{
	Math::Matrix rotmatY;
	rotmatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(camera_ang_Y));

	Math::Vector3 leftvec;
	leftvec = DirectX::XMVector3TransformNormal(Math::Vector3(-1, 0, 0), rotmatY);

	return leftvec;
}

Math::Vector3 C_Player::GetRightVec() const
{
	Math::Matrix rotmatY;
	rotmatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(camera_ang_Y));

	Math::Vector3 rightvec;
	rightvec = DirectX::XMVector3TransformNormal(Math::Vector3(1, 0, 0), rotmatY);

	return rightvec;
}

Math::Vector3 C_Player::GetArmPos()
{
	Math::Matrix tmpRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(arm_angY));

	Math::Vector3 tmpVec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, -3),tmpRot);

	return arm_pos + tmpVec + player_mat.Translation();
}