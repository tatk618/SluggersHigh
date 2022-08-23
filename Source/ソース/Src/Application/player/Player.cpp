#include "../player/Player.h"
#include "../define.h"
#include "../Scene/Game/GameScene.h"
#include "../Manager/Model/ModelManager.h"
#include "../Manager/Enemy/EnemyManager.h"
#include "../States/list/Statelist.h"
#include "../namespace/namespace.h"
#include "../main.h"
#include "../Manager/sound/SoundManager.h"

//�R���X�g���N�^
C_Player::C_Player(GameScene* _gameScene)
{
	m_gameScene = _gameScene;

	playerhead_mod = MODEL.GetModel("Data/Model/player/head.gltf");

	torsoupper_mod = MODEL.GetModel("Data/Model/player/upper.gltf");
	torsolower_mod = MODEL.GetModel("Data/Model/player/lower.gltf");

	leftleg_mod = MODEL.GetModel("Data/Model/player/leftLeg.gltf");
	rightleg_mod = MODEL.GetModel("Data/Model/player/rightLeg.gltf");

	arm_mod = MODEL.GetModel("Data/Model/player/batarms.gltf");

	//�Q�[����ʂ̒������W�i�N���C�A���g���W�j��������
	base_point = { ScrW / 2,ScrH / 2 };

	//�N���C�A���g���W���X�N���[�����W�Ɏ���
	ClientToScreen(APP.m_window.GetWndHandle(), &base_point);

	//�}�E�X���Q�[����ʒ����Ɉړ�
	SetCursorPos(base_point.x, base_point.y);

	//�}�E�X�J�[�\�����폜
	ShowCursor(FALSE);

	//�����J�����p�x
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

	//�v���C���[�̏����p�x
	player_ang = -90;

	//�v���C���[�̏���hp
	player_hp = 100;

	//�o�b�g���~�߂钷��
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

		//�J�����̊��x�ɔ��f���I������}�E�X�ʒu������W�ɖ߂�
		SetCursorPos(base_point.x, base_point.y);
	}

	//�v���C���[��HP���O�ɂȂ����Ƃ�
	if (player_hp <= 0)
	{
		//�v���C���[��HP���O�ɌŒ肷��
		player_hp = 0;

		//�v���C���[�̐����t���O���I�t�ɂ���
		player_alive_flg = false;
	}

	//�v���C���[�̐����t���O���I�t�̂Ƃ�
	if (player_alive_flg == false)
	{
		//�v���C���[���|��郂�[�V����
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

		//�v���C���[�̑���\�t���O���I�t�ɂ���
		player_able_command = false;
	}
	
	//�v���C���[�̑���\�t���O���I���̂Ƃ�
	if (player_able_command == true)
	{
		//�ړ��E�O�i
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

		//�ړ��E���
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

		//�ړ��E�����s�ړ�
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

		//�ړ��E�E���s�ړ�
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

		float dis;//���C�������������܂ł̋���(���C�����������Ƃ��̂ݏ�񂪏㏑�������)

		Math::Vector3 wallnormal;
		if (function::ModelIntersects(_mapmodel, _mapmat, player_pos, Math::Vector3(0, -1, 0), dis,wallnormal))
		{
			//dis�E�E�E�n�ʂ����ɔ�яo���Ă����l���̑̂̒���
			player_pos.y += (3 - dis);
		}

		//�}�E�X���{�^���ŋߐڍU��
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

		//�ߐڍU���t���O���I���̂Ƃ��Ƀo�b�g��U��
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

		//�o�b�g��U���Ă���30�o�߂���ƋߐڍU���t���O���I�t�ɂ���
		if (batstopcount >= 30)
		{
			meleeattackFlg = false;
			batstopcount -= 30;
		}

		//�ߐڍU���t���O���I�t�̂Ƃ��͍ŏ��̏�Ԃ܂Ŗ߂�
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

		//�\�����Ƃ��̏���
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

		//�ړ��t���O���I���̂Ƃ���movevec�������ړ�����
		if (move_flg == true)
		{
			//movevec�̐��K��
			moveVec.Normalize();
			//�ړ����x
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

	//�s��
	Math::Matrix rotmatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(player_angX));
	Math::Matrix rotmatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(player_ang));
	Math::Matrix transmat = DirectX::XMMatrixTranslation(player_pos.x, player_pos.y, player_pos.z);
	player_mat = rotmatY * rotmatX * transmat;

	//�㔼�g�̍s�񁁃v���C���[�̍s��
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

//�\��
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