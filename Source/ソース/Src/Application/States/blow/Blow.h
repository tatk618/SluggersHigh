#pragma once
#include "../../States/State.h"
#include "../../namespace/namespace.h"
#include "../../player/Player.h"

class BlowState :public Stateclass
{
public:
	BlowState(const Math::Vector3& _targetVec);
	Stateclass* Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)override;

	const Math::Vector3& GetTargetVec() const
	{
		return target_vec;
	}

	//追跡ステートは特に追加の変数も必要としないので
	//コンストラクタもメンバ変数も不要である
private:
	Math::Vector3 target_vec;

	int death_time = 0;
	int ang = 0;

	float y = 0;

	//重力の大きさ
	const float gravityscale = -0.05f;
};