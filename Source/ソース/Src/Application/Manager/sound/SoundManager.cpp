#include "SoundManager.h"

C_SoundManager::C_SoundManager()
{
}

C_SoundManager::~C_SoundManager()
{
	//���X�g���̂��̂��폜
	m_soundList.clear();
	m_InstanceList.clear();
}

void C_SoundManager::PlayBGM(const std::string _fileName, float _volume)
{
	//�C���X�^���X�f�[�^�̎擾
	auto tmpInstance = GetInstance(_fileName);
	//�Đ����łȂ���΍Đ�
	if (tmpInstance->IsPlay() == false)
	{
		tmpInstance->SetVolume(_volume);
		tmpInstance->Play(true);
	}
}

void C_SoundManager::StopBGM(const std::string _fileName)
{
	//�C���X�^���X�f�[�^�̎擾
	auto tmpInstance = GetInstance(_fileName);
	//�Đ����Œ�~
	if (tmpInstance->IsPlay())
	{
		tmpInstance->Stop();
	}
}

void C_SoundManager::SetVolume(const std::string _fileName, const float _volume)
{
	//�C���X�^���X�f�[�^�̎擾
	auto tmpInstance = GetInstance(_fileName);

	tmpInstance->SetVolume(_volume);
}

void C_SoundManager::PauseBGM(const std::string _fileName, bool _res)
{
	//�C���X�^���X�f�[�^�̎擾
	auto tmpInstance = GetInstance(_fileName);
	if (_res)
	{
		//�ꎞ��~
		if (tmpInstance->IsPlay())
		{
			tmpInstance->Pause();
		}
	}
	else
	{
		//�ĊJ
		if (tmpInstance->IsPlay() == false)
		{
			tmpInstance->Resume();
		}
	}
}


void C_SoundManager::Play3D(const std::string _fileName, const Math::Vector3& _pos,const float _volume)
{
	//�C���X�^���X�̍쐬���Đ�
	auto instance = GetSound(_fileName)->CreateInstance(true);
	instance->SetVolume(_volume);
	instance->SetCurveDistanceScaler(2.0f);
	instance->Play3D(_pos,false);
}

void C_SoundManager::Play2D(const std::string _fileName,float _volume)
{
	//�C���X�^���X�̍쐬���Đ�
	auto instance = GetSound(_fileName)->CreateInstance(true);
	instance->SetVolume(_volume);
	instance->Play(false);
}

void C_SoundManager::Update(const Math::Vector3& _pos, const Math::Vector3& _frontVec, const Math::Vector3& _head)
{
	//���X�i�[�̈ʒu��ݒ�
	AUDIO.GetListener().SetPosition(_pos);
	//���X�i�[�̌����i�O-��j��ݒ�
	AUDIO.GetListener().SetOrientation(
		_frontVec,
		_head
	);
}

std::shared_ptr<KdSoundEffect> C_SoundManager::GetSound(const std::string _fileName)
{
	//�����̃p�X�����[�h���A���̃A�h���X��Ԃ�
	std::shared_ptr<KdSoundEffect> tmpSound;
	if (m_soundList.find(_fileName) == m_soundList.end())
	{
		//map�N���X�̒���T�������A�Y���̃t�@�C�����͌�����Ȃ�����
		tmpSound = std::make_shared<KdSoundEffect>();
		tmpSound->Load(_fileName);//�Y���̃t�@�C�������[�h
		m_soundList[_fileName] = tmpSound;//�t�@�C�������L�[�Ƃ���
	}
	else
	{
		tmpSound = m_soundList[_fileName];
	}
	return tmpSound;
}

std::shared_ptr<KdSoundInstance> C_SoundManager::GetInstance(const std::string _fileName)
{
	//�����̃p�X�����[�h���A���̃A�h���X��Ԃ�
	std::shared_ptr<KdSoundInstance> tmpInstance;
	if (m_InstanceList.find(_fileName) == m_InstanceList.end())
	{
		tmpInstance = GetSound(_fileName)->CreateInstance(false);
		m_InstanceList[_fileName] = tmpInstance;//�t�@�C�������L�[�Ƃ���
	}
	else
	{
		tmpInstance = m_InstanceList[_fileName];
	}
	return tmpInstance;
}
