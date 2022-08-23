#include "../../States/list/Statelist.h"
#include "../../Scene/Game/GameScene.h"
#include "../../States/attack/Attack.h"
#include "../../Manager/Enemy/EnemyManager.h"

float function::GetVecAngY(Math::Vector3 _vec);
float function::TurnToAng(Math::Matrix& _mat, const float _targetAng, const float _plusAng);

AttackState::AttackState()
{
	chargespeed = 1.5f;
	attackinterval = 0;

	enemyattackflg = false;
}

Stateclass* AttackState::Action(Math::Matrix& _mat,C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager)
{
	if (attackinterval >= 0)
	{
		attackinterval += 1.0f;

		Math::Matrix tmpmat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(ang));
		_mat = tmpmat * _mat;
	}

	if (attackinterval >= 60)
	{
		Math::Vector3 ene_ply_vec;
		ene_ply_vec = _player->GetPos() - _mat.Translation();
		ene_ply_len = ene_ply_vec.Length();

		Math::Matrix tmpMat;
		tmpMat = DirectX::XMMatrixTranslation(0, 0, chargespeed);
		_mat = tmpMat * _mat;

		enemyattackflg = true;
	}

	if (enemyattackflg == true)
	{
		ENEMY.EnemyToPlayerHitCheck(_player_mat, _playerR, _playerhp, _player,_hitmarkmanager);
	}

	if (ene_ply_len >= 30)
	{
		enemyattackflg = false;
		return new ApproachState();
	}

	return nullptr;
}
