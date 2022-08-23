#pragma once
#include "../States/State.h"

class GameScene;
class EnemyManager;
class C_Enemy;
class C_Drone;

class C_Player
{
public:
	C_Player(GameScene* _gameScene);

	void Update(const KdModel& _mapmodel,const Math::Matrix& _mapmat);
	void Draw();

	Math::Vector3 GetLookVec()const;

	Math::Vector3 GetFrontVec()const;
	Math::Vector3 GetBackVec()const;
	Math::Vector3 GetLeftVec()const;
	Math::Vector3 GetRightVec()const;

	Math::Matrix GetMat() { return player_mat; };

	float GetR() { return player_R; };

	const Math::Vector3 GetPos()const { return player_pos; };

	Math::Vector3 GetArmPos();

	float GetcameraX()
	{
		return camera_ang_X;
	}

	float GetcameraY()
	{
		return camera_ang_Y;
	}

	float GetAng() {return player_ang;}
	 
	float GetArmAngY() { return arm_angY; }


	void SetPos(Math::Vector3 _player_pos)
	{
		player_pos = _player_pos;
	}

	void AddPos(const Math::Vector3& _Paddpos)
	{
		player_pos += _Paddpos;
		player_mat *= DirectX::XMMatrixTranslation(_Paddpos.x, _Paddpos.y, _Paddpos.z);
	}

	void SetMat(Math::Matrix _mat)
	{
		player_mat = _mat;
	}

	void SetR(float _playerR)
	{
		player_R = _playerR;
	}

	void SetAng(float _ang)
	{
		player_ang = _ang;
	}
	
	bool GetPlayerAliveFlg()
	{
		return player_alive_flg;
	}

	bool GetMeleeAttackFlg()
	{
		return meleeattackFlg;
	}

	bool GetLB_flg()
	{
		return LB_flg;
	}

	int GetPlayerHP() const
	{
		return player_hp;
	}

	//HPの計算
	void SetPlayerHP(int _playerhp)
	{
		player_hp = _playerhp;
	}

private:
	GameScene* m_gameScene;

	//モデル
	KdModel* playerhead_mod;
	KdModel* torsoupper_mod;
	KdModel* torsolower_mod;
	KdModel* leftleg_mod;
	KdModel* rightleg_mod;
	KdModel* arm_mod;
	KdModel* bat_mod;

	//角度系
	float player_ang = 0;
	float player_angX = 0;
	float upper_ang = 0;
	float playerhead_ang = 0;
	float player_R = 5.0f;
	float arm_angY = 0;
	float arm_angZ = 0;
	float leftleg_ang = 0;
	float rightleg_ang = 0;
	float kani_sinAng = 0;
	float fb_sinAng = 0;
	float BsinAng = 0;
	float hitDis = 0;

	POINT base_point;

	//カメラアングルX
	float camera_ang_X = 0;
	//カメラアングルY
	float camera_ang_Y = 0;

	//位置系
	Math::Vector3 player_pos;
	Math::Vector3 head_pos;
	Math::Vector3 upper_pos;
	Math::Vector3 arm_pos;
	Math::Vector3 leftleg_pos;
	Math::Vector3 rightleg_pos;

	//行列系
	Math::Matrix player_mat;
	Math::Matrix playerhead_mat;
	Math::Matrix upper_mat;
	Math::Matrix arm_mat;
	Math::Matrix leftleg_mat;
	Math::Matrix rightleg_mat;

	//回転行列系
	Math::Matrix head_trans;
	Math::Matrix head_rot;
	Math::Matrix arm_rot;
	Math::Matrix leftleg_rot;
	Math::Matrix rightleg_rot;

	//移動行列系
	Math::Matrix arm_trans;
	Math::Matrix leftleg_trans;
	Math::Matrix rightleg_trans;

	//ジャンプ速度
	float jumpspeed = 0;
	//落下速度
	float fallspeed = 0;
	//素振り固定時間
	float bat_stopcount = 0;

	int player_hp = 0;
	float swayspeed = 0;
	int batstopcount = 0;
	int regainintervalcount = 0;
	int lowhp_count = 0;
	int down_count = 0;

	bool player_down_flg = false;

	//プレイヤージャンプ
	bool playerJumpFlg = false;

	//プレイヤー攻撃フラグ
	bool meleeattackFlg = false;

	//移動フラグ
	bool move_flg = false;

	//構えフラグ
	bool aim_flg = false;

	//左クリックフラグ
	bool LB_flg = false;

	//攻撃開始フラグ
	bool attackstart_flg = false;

	//頭回転フラグ
	bool headrot_flg = false;

	//プレイヤー生存フラグ
	bool player_alive_flg = false;

	bool regainflg = false;

	bool walksound_flg = false;
	bool dashsound_flg = false;

	//操作可能フラグ
	bool player_able_command = false;
};