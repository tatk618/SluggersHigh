#include "SoundManager.h"

C_SoundManager::C_SoundManager()
{
}

C_SoundManager::~C_SoundManager()
{
	//リストそのものを削除
	m_soundList.clear();
	m_InstanceList.clear();
}

void C_SoundManager::PlayBGM(const std::string _fileName, float _volume)
{
	//インスタンスデータの取得
	auto tmpInstance = GetInstance(_fileName);
	//再生中でなければ再生
	if (tmpInstance->IsPlay() == false)
	{
		tmpInstance->SetVolume(_volume);
		tmpInstance->Play(true);
	}
}

void C_SoundManager::StopBGM(const std::string _fileName)
{
	//インスタンスデータの取得
	auto tmpInstance = GetInstance(_fileName);
	//再生中で停止
	if (tmpInstance->IsPlay())
	{
		tmpInstance->Stop();
	}
}

void C_SoundManager::SetVolume(const std::string _fileName, const float _volume)
{
	//インスタンスデータの取得
	auto tmpInstance = GetInstance(_fileName);

	tmpInstance->SetVolume(_volume);
}

void C_SoundManager::PauseBGM(const std::string _fileName, bool _res)
{
	//インスタンスデータの取得
	auto tmpInstance = GetInstance(_fileName);
	if (_res)
	{
		//一時停止
		if (tmpInstance->IsPlay())
		{
			tmpInstance->Pause();
		}
	}
	else
	{
		//再開
		if (tmpInstance->IsPlay() == false)
		{
			tmpInstance->Resume();
		}
	}
}


void C_SoundManager::Play3D(const std::string _fileName, const Math::Vector3& _pos,const float _volume)
{
	//インスタンスの作成＆再生
	auto instance = GetSound(_fileName)->CreateInstance(true);
	instance->SetVolume(_volume);
	instance->SetCurveDistanceScaler(2.0f);
	instance->Play3D(_pos,false);
}

void C_SoundManager::Play2D(const std::string _fileName,float _volume)
{
	//インスタンスの作成＆再生
	auto instance = GetSound(_fileName)->CreateInstance(true);
	instance->SetVolume(_volume);
	instance->Play(false);
}

void C_SoundManager::Update(const Math::Vector3& _pos, const Math::Vector3& _frontVec, const Math::Vector3& _head)
{
	//リスナーの位置を設定
	AUDIO.GetListener().SetPosition(_pos);
	//リスナーの向き（前-上）を設定
	AUDIO.GetListener().SetOrientation(
		_frontVec,
		_head
	);
}

std::shared_ptr<KdSoundEffect> C_SoundManager::GetSound(const std::string _fileName)
{
	//引数のパスをロードし、そのアドレスを返す
	std::shared_ptr<KdSoundEffect> tmpSound;
	if (m_soundList.find(_fileName) == m_soundList.end())
	{
		//mapクラスの中を探したが、該当のファイル名は見つからなかった
		tmpSound = std::make_shared<KdSoundEffect>();
		tmpSound->Load(_fileName);//該当のファイルをロード
		m_soundList[_fileName] = tmpSound;//ファイル名をキーとして
	}
	else
	{
		tmpSound = m_soundList[_fileName];
	}
	return tmpSound;
}

std::shared_ptr<KdSoundInstance> C_SoundManager::GetInstance(const std::string _fileName)
{
	//引数のパスをロードし、そのアドレスを返す
	std::shared_ptr<KdSoundInstance> tmpInstance;
	if (m_InstanceList.find(_fileName) == m_InstanceList.end())
	{
		tmpInstance = GetSound(_fileName)->CreateInstance(false);
		m_InstanceList[_fileName] = tmpInstance;//ファイル名をキーとして
	}
	else
	{
		tmpInstance = m_InstanceList[_fileName];
	}
	return tmpInstance;
}
