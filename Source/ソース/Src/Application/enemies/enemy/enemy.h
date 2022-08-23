#pragma once
#include "../../States/State.h"
#include "../../Manager/redlight/redlightmanager.h"
#include "../../Manager/hitmark/hitmarkmanager.h"

class GameScene;
class C_Player;
class EnemyManager;

class C_Enemy
{
public:
			 C_Enemy();
	virtual ~C_Enemy();

	virtual void Update(C_Player* _player, Math::Matrix& _player_mat, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager);
	virtual void Draw();

	virtual const Math::Vector3& GetEnemyPos() = 0;
	virtual float GetEnemyR() = 0;

	void SetNextState(Stateclass* _nextstate)
	{
		nextState = _nextstate;
	}

	virtual void AddPos(const Math::Vector3& _addpos) = 0;

	int GetHP() const
	{
		return m_hp;
	}

	//HP‚ÌŒvZ
	void SetHP(int _hp) 
	{
		m_hp = _hp;
	}

	int GetInviTime()
	{
		return invincible_time;
	}

	//–³“GŠÔ‚ğİ’è
	void SetInviTime(int _time)
	{
		invincible_time = _time;
	}

protected:
	GameScene* m_gameScene;
	Stateclass* nowState = nullptr;

	Stateclass* nextState = nullptr;

	int m_hp = 0;

	int invincible_time = 0;
};