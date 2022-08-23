#pragma once
#include "../../States/State.h"

class playerNormalState :public Stateclass
{
public:
	playerNormalState(const Math::Vector3& _targetVec);
	Stateclass* Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp)override;

	//追跡ステートは特に追加の変数も必要としないので
	//コンストラクタもメンバ変数も不要である
private:
	Math::Vector3 target_vec;

	int death_time = 0;
};