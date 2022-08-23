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
	//�G�̔z�������₷
	enemy.push_back(new C_Drone(_gameScene,_drone_pos));

}

void EnemyManager::BossSpawn(C_Player* _player)
{
	if (bossspawnlimitter_flg == false)
	{
		//�{�X�̔z�������₷
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

			//�G�̈ʒu�̐錾
			Math::Vector3 enemy_pos;
			//�Q�b�^�[�ŋ��߂��G�̈ʒu����
			enemy_pos = enemy[en]->GetEnemyPos();

			//�G�̈ʒu����v���C���[�̘r�̈ʒu�܂ł̃x�N�g�����쐬
			vLen = enemy_pos - _arm_pos;
			//�x�N�g���̋��������߂�
			len = vLen.Length();

			//�x�N�g���̋������U���͈́i�v���C���[�ƓG�̔��a�𑫂������́j��菬�����Ƃ�
			if (len < (enemy[en]->GetEnemyR() + _playerR))
			{
				Math::Vector3 player_vec;
				//�v���C���[�̕����x�N�g�����擾����
				player_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), _player_mat);

				//�G�̖��G���Ԃ�0�ȉ��̂Ƃ�

				if (enemy[en]->GetInviTime() <= 0)
				{
					SOUND.Play3D("Data/Sound/SoundEffect/dageki.wav", enemy_pos, 1.0f);
					_hitmarkmanager->Spawn(enemy[en]->GetEnemyPos());

					//�G��HP�����߂āA50�����炷
					int tmpHP;
					tmpHP = enemy[en]->GetHP();
					tmpHP -= instantdeath_damage;

					//�G��HP��ݒ肷��
					enemy[en]->SetHP(tmpHP);

					//�G�𐁂���΂��X�e�[�g�̏�Ԃɂ���
					enemy[en]->SetNextState(new BlowState(player_vec));

					//SOUND.Play3D("Data/Sound/SoundEffect/jakudageki.wav", enemy_pos, 1.0f);
					//_hitmarkmanager->Spawn(enemy[en]->GetEnemyPos());
					////�G��HP�����߂āA50�����炷
					//int tmpHP;
					//tmpHP = enemy[en]->GetHP();
					//tmpHP -= normaldamage;

					////�G��HP��ݒ肷��
					//enemy[en]->SetHP(tmpHP);

					////�G�𐁂���΂��X�e�[�g�̏�Ԃɂ���
					//enemy[en]->SetNextState(new hitBlowState(player_vec));

					////�G�̖��G���Ԃ�ݒ肷��
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
		//HP���O�ȉ��̂Ƃ�
		if (enemy[nowupdate]->GetHP() <= 0)
		{
			//�G�̎��S���[�V����

			dessapearcount++;

			if (dessapearcount >= 60)
			{
				//�G�̌��Đ��̃J�E���g������₷
				enemykillcount += 1;

				//�����̔���
				_explosionManager->Spawn(enemy[nowupdate]->GetEnemyPos());

				//�G�̔z�������炷
				delete enemy[nowupdate];
				enemy.erase(enemy.begin() + nowupdate);
				nowupdate--;

				dessapearcount -= 60;
			}
		}
	}

	//�����蔻��
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
		//�����ԍ���������瓖���蔻����΂�
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
		//�����ԍ���������瓖���蔻����΂�
		if (en == nowupdate)
		{
			continue;
		}

		Math::Vector3 vLen;
		float len;

		//�G�Ƃ�����̓G�̋����������A������x�N�g���Ƃ���
		vLen = enemy[en]->GetEnemyPos() - enemy[nowupdate]->GetEnemyPos();

		//�x�N�g���̂��������Ԃ�
		vLen.y = 0;

		//�x�N�g���̒��������߂�
		len = vLen.Length();

		//�����蔻��Ƃ��钷�������߂�
		float CheckLen;
		CheckLen = enemy[en]->GetEnemyR() + enemy[nowupdate]->GetEnemyR();

		//�x�N�g���̒����������蔻���菬������
		if (len < CheckLen)
		{
			//���f�����m�̂߂荞��ł��镪�̒��������߂�
			CheckLen -= len;
			 
			//�x�N�g���𐳋K������
			vLen.Normalize();

			//�����o���x�N�g���̒������쐬����
			vLen *= CheckLen * 3.0f;

			_hitmarkmanager->Spawn(enemy[nowupdate]->GetEnemyPos());

			SOUND.Play3D("Data/Sound/SoundEffect/damaged.wav", enemy[nowupdate]->GetEnemyPos(), 0.3f);

			//HP�̌v�Z
			int tmpHP;
			tmpHP = enemy[en]->GetHP();
			tmpHP -= 20;
			enemy[en]->SetHP(tmpHP);

			tmpHP = enemy[nowupdate]->GetHP();
			tmpHP -= 20;
			enemy[nowupdate]->SetHP(tmpHP);

			//���������G�𐁂���΂��X�e�[�g�̏�Ԃɂ���
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

		//�G�ƃv���C���[�̋����������A������x�N�g���Ƃ���
		vLen = enemy[en]->GetEnemyPos() - _player_mat.Translation();

		//�x�N�g���̂��������Ԃ�
		vLen.y = 0;

		//�x�N�g���̒��������߂�
		len = vLen.Length();

		//�����蔻��Ƃ��钷�������߂�
		float CheckLen;
		CheckLen = enemy[en]->GetEnemyR() + _playerR;

		//�x�N�g���̒����������蔻���菬������
		if (len < CheckLen)
		{
			float pushlen;
			pushlen = (enemy[en]->GetEnemyR() + _playerR) - len;

			vLen.Normalize();
			vLen *= pushlen;

			Math::Vector3 vLen2;
			vLen2 = _player_mat.Translation() - vLen;
			_player->SetPos(vLen2);

			//���G���Ԃ�0�̂Ƃ�HP����
			if (invincible_time <= 0)
			{
				_hitmarkmanager->Spawn(_player_mat.Translation());
				SOUND.Play2D("Data/Sound/SoundEffect/damaged.wav", 0.6f);

				////HP�̌v�Z
				int tmpHP;
				tmpHP = _player->GetPlayerHP();
				tmpHP -= 10;
				_player->SetPlayerHP(tmpHP);

				//���G���Ԃ�60�~2�ǉ�
				invincible_time += 180;

			}
		}
	}

	return false;
}
