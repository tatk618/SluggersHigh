#pragma once
#include "../../enemies/enemy/enemy.h"
class C_Player;
class EnemyManager;

class C_Boss:public C_Enemy
{
public:
	C_Boss(C_Player* _player);
	~C_Boss()override;

	// 描画処理
	void Draw()override;
	void Update(C_Player* _player, Math::Matrix& _player_mat, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)override;

	const Math::Matrix& GetbossMat()const
	{
		return boss_mat;
	}

	const Math::Vector3& GetEnemyPos()override
	{
		return boss_pos = boss_mat.Translation();
	}
	float GetEnemyR()override { return boss_range_R; };

	void SetMat(Math::Matrix _boss_mat)
	{
		boss_mat = _boss_mat;
	}

	void AddPos(const Math::Vector3& _addpos)override
	{
		boss_pos += _addpos;
		boss_mat *= DirectX::XMMatrixTranslation(_addpos.x, _addpos.y, _addpos.z);

		roter_pos += _addpos;
		roter_mat *= DirectX::XMMatrixTranslation(_addpos.x, _addpos.y, _addpos.z);

		taser_pos += _addpos;
		taser_mat *= DirectX::XMMatrixTranslation(_addpos.x, _addpos.y, _addpos.z);
	}

private:
	//ドローン本体のモデル
	KdModel* boss_model;
	//ドローンの位置
	Math::Vector3 boss_pos;
	//ドローンの行列
	Math::Matrix boss_mat;

	//プロペラのモデル
	KdModel* roter_model;
	//プロペラの位置
	Math::Vector3 roter_pos;
	//プロペラの行列
	Math::Matrix roter_mat;

	//スタンガンのモデル
	KdModel* taser_model;
	//スタンガンの行列
	Math::Vector3 taser_pos;
	//スタンガンの行列
	Math::Matrix taser_mat;

	Math::Matrix transmat;

	//出現位置
	Math::Vector3 spawn_pos;

	float target_ang;
	float boss_ang = 0;
	float roter_ang = 0;
	float taser_ang = 0;
	float Psinang = 0;

	//ドローンの半径
	const float boss_range_R = 3;
};