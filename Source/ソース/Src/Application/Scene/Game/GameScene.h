#pragma once
#include "../../Scene/Scene.h"
#include "../../Stage/colossus.h"
#include "../../sky/Sky.h"
#include "../../player/Player.h"
#include "../../manager/redlight/redlightmanager.h"
#include "../../enemies/enemy/enemy.h"
#include "../../enemies/drone/drone.h"
#include "../../explosion/explosion.h"
#include "../../Manager/Explosion/ExplosionManager.h"
#include "../../Manager/hitmark/hitmarkmanager.h"
#include "../../Manager/sound/SoundManager.h"
#include "../../Lifegauge/Lifegauge.h"
#include "../../States/list/Statelist.h"

class GameScene:public C_Scene
{
public:
	GameScene() { Init(); }
	~GameScene() { Release(); }

	// ������
	void Init();

	void Frame();

	// �X�V����
	void Update();
	// �`�揈��
	void Draw()override;
	// �I������
	void Release();

	//�`�F�b�N�|�C���g�ʉ߃t���O
	bool Getreach() 
	{ 
		return reach_flg[5]; 
	}

	const Math::Vector3& getchkptpos()
	{
		return chkpt_pos[5];
	}

	const bool Geteliminatedflg()
	{
		return enemyeliminated_flg;
	}

	const C_Colossus* GetWall()
	{
		return colossus;
	}

private:
	C_Colossus* colossus = nullptr;
	C_Sky* sky = nullptr;
	C_Player* player = nullptr;
	//C_Drone* drone = nullptr;
	GameScene* gamescene = nullptr;
	LifeGauge* lifegauge = nullptr;
	ExplosionManager* explosionManager = nullptr;
	RedlightManager* redlightManager = nullptr;
	hitmarkManager* hitmarkmanager = nullptr;
	
	KdTexture* blackscreen_tex = nullptr;

	DirectX::SpriteFont* spriteFont;

	bool target_flg = true;
	bool explosion_flg;
	bool player_flg = true;
	bool spawn_flg = false;
	bool respawn_flg = false;
	bool stopallsound = false;

	bool blackchangeFlg = false;
	bool resultflg = false;
	bool clearflg = false;
	bool crosshairFlg = false;
	bool time_overflg = false;

	//�`�F�b�N�|�C���g��ʉ߂��������ʂ���t���O
	static const int rflg = 5;
	bool reach_flg[rflg];

	bool spawnlimitter_flg = false;
	bool enemymaxed_flg = false;
	bool enemyeliminated_flg = false;
	bool bgmturnoff_flg = false;

	//�J�����A���O��X
	float camera_ang_X = 0;
	//�J�����A���O��Y
	float camera_ang_Y = 0;
	float pan_atai = 0;

	//���l�n
	float _ang;
	float _R;
	float fov;
	float player_hp = 0;
	float gamebgmvol = 0;

	//�v���C���[�ƃ`�F�b�N�|�C���g�̋���
	float chkpt_ply_len = 0;

	//�Ó]��ʂ̓����x
	float black_Alpha = 0;

	//�c�莞��
	int left_time = 0;
	int nowenemy_num = 0;
	int enemy_num = 0;	
	int enemymaxnum = 0;

	//���W�n
	Math::Vector3 _pos;
	Math::Vector3 _tpos;
	Math::Vector3 rock_len;
	Math::Vector3 _Dpos;
	Math::Vector3 mapmat;
	
	//�`�F�b�N�|�C���g�̈ʒu
	static const int chkpt = 5;
	Math::Vector3 chkpt_pos[chkpt];

	//�J�����ʒu�A�J�����p�x�A�J������
	Math::Vector3 camera_Pos, camera_Look, camera_Head;

	KdTexture* mission_tex = nullptr;
	KdTexture* complete_tex = nullptr;

	Math::Matrix mission_mat;
	Math::Matrix complete_mat;

	Math::Vector3 mission_pos;
	Math::Vector3 complete_pos;

	bool missiontoujou_flg = false;
	bool completetoujou_flg = false;

	float malpha = 0;
	float calpha = 0;
	
	int count = 0;
	int nowchkpt = 0;
	int enemynum = 0;

	KdTexture* crosshair_tex = nullptr;
	Math::Vector3 m_crosshair_pos;
	Math::Matrix m_matrix;

	ID3D11BlendState* m_BlendAdd;
	ID3D11BlendState* m_BlendAlpha;
};