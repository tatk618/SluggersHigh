#include "../../Manager/Enemy/EnemyManager.h"
#include "../../Scene/Game/GameScene.h"
#include "../../States/list/Statelist.h"
#include "../../enemies/drone/drone.h"
#include "../../Manager/sound/SoundManager.h"
#include "../../player/Player.h"
#include <stdio.h>

EnemyManager* EnemyManager::instance = nullptr;

EnemyManager::~EnemyManager()
{
	for (UINT en = 0;en < enemy.size();en++)
	{
		delete enemy[en];
	}

	enemy.clear();

	for (UINT b = 0;b < boss.size();b++)
	{
		delete boss[b];
	}

	boss.clear();
}

void EnemyManager::Spawn(GameScene* _gameScene,const Math::Vector3& _drone_pos)
{
	//敵の配列を一個増やす
	enemy.push_back(new C_Drone(_gameScene,_drone_pos));

}

void EnemyManager::BossSpawn(C_Player* _player)
{
	if (bossspawnlimitter_flg == false)
	{
		//ボスの配列を一個増やす
		boss.push_back(new C_Boss(_player));
		if (boss.size() >= 1)
		{
			bossspawnlimitter_flg = true;
		}
	}
}

void EnemyManager::EnemyHitCheck(Math::Matrix& _player_mat, Math::Vector3& _arm_pos,float _playerR,bool _attackflg, hitmarkManager* _hitmarkmanager)
{
	if (_attackflg == true)
	{
		for (UINT en = 0;en < enemy.size();en++)
		{
			Math::Vector3 vLen;
			float len;

			//敵の位置の宣言
			Math::Vector3 enemy_pos;
			//ゲッターで求めた敵の位置を代入
			enemy_pos = enemy[en]->GetEnemyPos();

			//敵の位置からプレイヤーの腕の位置までのベクトルを作成
			vLen = enemy_pos - _arm_pos;
			//ベクトルの距離を求める
			len = vLen.Length();

			//ベクトルの距離が攻撃範囲（プレイヤーと敵の半径を足したもの）より小さいとき
			if (len < (enemy[en]->GetEnemyR() + _playerR))
			{
				Math::Vector3 player_vec;
				//プレイヤーの方向ベクトルを取得する
				player_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), _player_mat);

				//敵の無敵時間が0以下のとき

				if (enemy[en]->GetInviTime() <= 0)
				{
					SOUND.Play3D("Data/Sound/SoundEffect/dageki.wav", enemy_pos, 1.0f);
					_hitmarkmanager->Spawn(enemy[en]->GetEnemyPos());

					//敵のHPを求めて、50分減らす
					int tmpHP;
					tmpHP = enemy[en]->GetHP();
					tmpHP -= instantdeath_damage;

					//敵のHPを設定する
					enemy[en]->SetHP(tmpHP);

					//敵を吹っ飛ばすステートの状態にする
					enemy[en]->SetNextState(new BlowState(player_vec));

					//SOUND.Play3D("Data/Sound/SoundEffect/jakudageki.wav", enemy_pos, 1.0f);
					//_hitmarkmanager->Spawn(enemy[en]->GetEnemyPos());
					////敵のHPを求めて、50分減らす
					//int tmpHP;
					//tmpHP = enemy[en]->GetHP();
					//tmpHP -= normaldamage;

					////敵のHPを設定する
					//enemy[en]->SetHP(tmpHP);

					////敵を吹っ飛ばすステートの状態にする
					//enemy[en]->SetNextState(new hitBlowState(player_vec));

					////敵の無敵時間を設定する
					//enemy[en]->SetInviTime((30));
				}
			}
		}
	}
}

void EnemyManager::Draw()
{
	for (UINT en = 0;en < enemy.size();en++)
	{
		enemy[en]->Draw();
	}
}

void EnemyManager::BossDraw()
{
	for (UINT b = 0;b < boss.size();b++)
	{
		boss[b]->Draw();
	}
}

void EnemyManager::Update(C_Player* _player, Math::Matrix& _player_mat, ExplosionManager* _explosionManager, int _playerhp, RedlightManager* _redlightManager, hitmarkManager* _hitmarkmanager, const KdModel& _mapmodel, const Math::Matrix& _mapmat)
{
	invincible_time--;
	if (invincible_time <= 0)
	{
		invincible_time = 0;
	}

	if (enemy.size() <= 0)
	{
		count++;
		spawnlimitter_flg = false;
	}

	for (UINT nowupdate = 0;nowupdate < enemy.size();nowupdate++)
	{
		enemy[nowupdate]->Update(_player,_player_mat,_playerhp,_redlightManager,_hitmarkmanager);
		//HPが０以下のとき
		if (enemy[nowupdate]->GetHP() <= 0)
		{
			//敵の死亡モーション

			dessapearcount++;

			if (dessapearcount >= 60)
			{
				//敵の撃墜数のカウントを一個増やす
				enemykillcount += 1;

				//爆発の発生
				_explosionManager->Spawn(enemy[nowupdate]->GetEnemyPos());

				//敵の配列を一個減らす
				delete enemy[nowupdate];
				enemy.erase(enemy.begin() + nowupdate);
				nowupdate--;

				dessapearcount -= 60;
			}
		}
	}

	//当たり判定
	for (UINT en = 0;en < enemy.size();en++)
	{
		EnemiesHitCheck(en);
	}
}

void EnemyManager::BossUpdate(C_Player* _player, Math::Matrix& _player_mat, ExplosionManager* _explosionManager, int _playerhp)
{
}

bool EnemyManager::EnemiesHitCheck(const UINT _en)
{
	for (UINT en = 0;en < enemy.size();en++)
	{
		//同じ番号が被ったら当たり判定を飛ばす
		if (en == _en)
		{
			continue;
		}

		Math::Vector3 vLen;
		float len;

		vLen = enemy[en]->GetEnemyPos() - enemy[_en]->GetEnemyPos();

		vLen.y = 0;

		len = vLen.Length();
		float CheckLen;
		CheckLen = enemy[en]->GetEnemyR() + enemy[_en]->GetEnemyR();

		if (len < CheckLen)
		{
			CheckLen -= len;
			vLen.Normalize();
			vLen *= CheckLen * 0.5f;
			enemy[en]->AddPos(vLen);
			enemy[_en]->AddPos(-vLen);
			return true;
		}
	}

	return false;
}

bool EnemyManager::EnemiesHitCheck(hitmarkManager* _hitmarkmanager)
{
	for (UINT en = 0;en < enemy.size();en++)
	{
		//同じ番号が被ったら当たり判定を飛ばす
		if (en == nowupdate)
		{
			continue;
		}

		Math::Vector3 vLen;
		float len;

		//敵ともう一つの敵の距離を引き、それをベクトルとする
		vLen = enemy[en]->GetEnemyPos() - enemy[nowupdate]->GetEnemyPos();

		//ベクトルのｙ方向をつぶす
		vLen.y = 0;

		//ベクトルの長さを求める
		len = vLen.Length();

		//当たり判定とする長さを求める
		float CheckLen;
		CheckLen = enemy[en]->GetEnemyR() + enemy[nowupdate]->GetEnemyR();

		//ベクトルの長さが当たり判定より小さい時
		if (len < CheckLen)
		{
			//モデル同士のめり込んでいる分の長さを求める
			CheckLen -= len;
			 
			//ベクトルを正規化する
			vLen.Normalize();

			//押し出すベクトルの長さを作成する
			vLen *= CheckLen * 3.0f;

			_hitmarkmanager->Spawn(enemy[nowupdate]->GetEnemyPos());

			SOUND.Play3D("Data/Sound/SoundEffect/damaged.wav", enemy[nowupdate]->GetEnemyPos(), 0.3f);

			//HPの計算
			int tmpHP;
			tmpHP = enemy[en]->GetHP();
			tmpHP -= 20;
			enemy[en]->SetHP(tmpHP);

			tmpHP = enemy[nowupdate]->GetHP();
			tmpHP -= 20;
			enemy[nowupdate]->SetHP(tmpHP);

			//当たった敵を吹き飛ばすステートの状態にする
			enemy[en]->SetNextState(new BlowState(vLen));

			return true;
		}
	}

	return false;
}

bool EnemyManager::EnemyToPlayerHitCheck(Math::Matrix& _player_mat, float _playerR, int _playerhp, C_Player* _player, hitmarkManager* _hitmarkmanager)
{
	//_redlightManager->Spawn(enemy[nowupdate]->GetEnemyPos());

	for (UINT en = 0;en < enemy.size();en++)
	{
		Math::Vector3 vLen;
		float len;

		//敵とプレイヤーの距離を引き、それをベクトルとする
		vLen = enemy[en]->GetEnemyPos() - _player_mat.Translation();

		//ベクトルのｙ方向をつぶす
		vLen.y = 0;

		//ベクトルの長さを求める
		len = vLen.Length();

		//当たり判定とする長さを求める
		float CheckLen;
		CheckLen = enemy[en]->GetEnemyR() + _playerR;

		//ベクトルの長さが当たり判定より小さい時
		if (len < CheckLen)
		{
			float pushlen;
			pushlen = (enemy[en]->GetEnemyR() + _playerR) - len;

			vLen.Normalize();
			vLen *= pushlen;

			Math::Vector3 vLen2;
			vLen2 = _player_mat.Translation() - vLen;
			_player->SetPos(vLen2);

			//無敵時間が0のときHP減少
			if (invincible_time <= 0)
			{
				_hitmarkmanager->Spawn(_player_mat.Translation());
				SOUND.Play2D("Data/Sound/SoundEffect/damaged.wav", 0.6f);

				////HPの計算
				int tmpHP;
				tmpHP = _player->GetPlayerHP();
				tmpHP -= 10;
				_player->SetPlayerHP(tmpHP);

				//無敵時間を60×2追加
				invincible_time += 180;

			}
		}
	}

	return false;
}
