#pragma once
#include "../../enemies/enemy/enemy.h"

class C_Player;
class EnemyManager;
class GameScene;

class C_Drone:public C_Enemy
{
public:
	C_Drone(GameScene* _gameScene,const Math::Vector3& _drone_pos);
	~C_Drone()override;

	// �`�揈��
	void Draw()override;
	void Update(C_Player* _player, Math::Matrix& _player_mat,int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)override;

	Math::Matrix& GetdroneMat()
	{
		return drone_mat;
	}

	const Math::Vector3& GetEnemyPos()override
	{
		return drone_pos = drone_mat.Translation();
	}
	float GetEnemyR()override { return drone_range_R; };

	const int Getdronehp()
	{
		return m_hp;
	}

	void SetMat(Math::Matrix _drone_mat)
	{
		drone_mat = _drone_mat;
	}

	void AddPos(const Math::Vector3& _addpos)override
	{
		drone_pos += _addpos;
		drone_mat *= DirectX::XMMatrixTranslation(_addpos.x, _addpos.y, _addpos.z);

		roter_pos += _addpos;
		roter_mat *= DirectX::XMMatrixTranslation(_addpos.x, _addpos.y, _addpos.z);

		//taser_pos += _addpos;
		//taser_mat *= DirectX::XMMatrixTranslation(_addpos.x, _addpos.y, _addpos.z);
	}

private:
	GameScene* m_gameScene;

	//�h���[���{�̂̃��f��
	KdModel* drone_model;
	//�h���[���̈ʒu
	Math::Vector3 drone_pos;
	//�h���[���̍s��
	Math::Matrix drone_mat;

	//�v���y���̃��f��
	KdModel* roter_model;
	//�v���y���̈ʒu
	Math::Vector3 roter_pos;
	//�v���y���̍s��
	Math::Matrix roter_mat;

	Math::Matrix transmat;

	bool alive_flg = false;
	bool rotersound_flg = false;

	float target_ang_X = 0;
	float target_ang_Y = 0;
	float drone_ang = 0;
	float roter_ang = 0;
	float taser_ang = 0;
	float Psinang = 0;
	float alpha = 0;
	float hitDis = 0;

	const int rotersound_count = 2160;
	int nowrotersound_count = 0;

	//�h���[���̔��a
	const float drone_range_R = 2.5f;
	const float addalpha = 0.03f;
	const float maxalpha = 1.0f;
};