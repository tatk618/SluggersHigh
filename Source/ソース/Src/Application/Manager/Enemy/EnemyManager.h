#pragma once
#include "../../enemies/enemy/enemy.h"
#include "../../enemies/boss/boss.h"
#include "../../Manager/Explosion/ExplosionManager.h"
#include "../../Manager/hitmark/hitmarkmanager.h"
#include "../../namespace/namespace.h"
#include "../../main.h"
#include "../../States/attack/Attack.h"

class GameClass;

class EnemyManager
{
public:
	~EnemyManager();

	//敵を生み出す
	void Spawn(GameScene* _gameScene, const Math::Vector3& _drone_pos);
	void BossSpawn(C_Player* _player);
	void EnemyHitCheck(Math::Matrix& _player_mat,Math::Vector3& _arm_pos, float _playerR, bool _attackflg,hitmarkManager* _hitmarkmanager);
	//敵を表示する
	void Draw();
	void BossDraw();
	void Update(C_Player* _player, Math::Matrix& _player_mat, ExplosionManager* _explosionManager, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager, const KdModel& _mapmodel, const Math::Matrix& _mapmat);
	void BossUpdate(C_Player* _player, Math::Matrix& _player_mat, ExplosionManager* _explosionManager, int _playerhp);

	bool EnemiesHitCheck(const UINT _en);
	bool EnemiesHitCheck(hitmarkManager* _hitmarkmanager);
	bool EnemyToPlayerHitCheck(Math::Matrix& _player_mat, float _playerR, int _playerhp, C_Player* _player, hitmarkManager* _hitmarkmanager);

	int GetEnemyKillCount() { return enemykillcount; }
	int GetEnemyLeft() { return enemy.size(); }
	int GetScore() { return score; }

	static EnemyManager& GetInstance()
	{
		if (instance == nullptr)	//まだインスタンスされていなければ
		{
			instance = new EnemyManager();	//インスタンス化する
		}
		return (*instance);
	}

private:
	static EnemyManager* instance;

	Math::Vector3 target_vec;

	//敵の配列
	std::vector<C_Enemy*>enemy;
	std::vector<C_Boss*>boss;

	bool target_flg = false;

	bool bossspawnlimitter_flg = false;
	bool spawnlimitter_flg = false;

	int enemykillcount = 0;

	int count = 0;

	int score = 0;

	int dessapearcount = 0;

	const int normaldamage = 25;

	const int instantdeath_damage = 60;

	int nowupdate = 0;

	int invincible_time = 0;
};

#define ENEMY EnemyManager::GetInstance()