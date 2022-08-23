#include "System/KdSystem.h"

#include "KdAudio.h"


//==================================================================
//
// KdSoundInstance
//
//==================================================================
void KdSoundInstance::Play(bool loop, bool manage)
{
	if(m_instance == nullptr)return;

	// �ݒ�
	m_instance->Stop();
	//m_Inst->SetVolume(1);

	// �Đ�
	m_instance->Play(loop);

	// �Đ����X�g�֒ǉ�
	if (manage)
	{
		AUDIO.AddPlayList(shared_from_this());
	}
}

// 3D Sound Ver
void KdSoundInstance::Play3D(const Math::Vector3& pos, bool loop, bool manage)
{
	if (m_instance == nullptr)return;

	// �ݒ�
	m_instance->Stop();
	m_instance->SetVolume(1);

	// �Đ�
	m_instance->Play(loop);

	// ���W�ݒ�
	m_emitter.SetPosition(pos);
//	m_emitter.CurveDistanceScaler = FLT_MAX;

	Apply3D();


	// �Đ����X�g�֒ǉ�
	if (manage)
	{
		AUDIO.AddPlayList(shared_from_this());
	}
}

void KdSoundInstance::Apply3D()
{
	if (m_instance == nullptr)return;

	m_instance->Apply3D(AUDIO.GetListener(), m_emitter, false);
}

void KdSoundInstance::SetVolume(float vol)
{
	if (m_instance == nullptr)return;

	m_instance->SetVolume(vol);
}

void KdSoundInstance::SetPos(const Math::Vector3& pos)
{
	if (m_instance == nullptr)return;

	// ���W�ݒ�
	m_emitter.SetPosition(pos);

	m_instance->Apply3D(AUDIO.GetListener(), m_emitter, false);
}

void KdSoundInstance::SetCurveDistanceScaler(float val)
{
	if (m_instance == nullptr)return;

	m_emitter.CurveDistanceScaler = val;

	m_instance->Apply3D(AUDIO.GetListener(), m_emitter, false);
}

bool KdSoundInstance::IsPlay()
{
	if (m_instance == nullptr)return false;

	if (m_instance->GetState() == DirectX::SoundState::PLAYING)return true;

	return false;
}

bool KdSoundInstance::IsPause()
{
	if (m_instance == nullptr)return false;

	if (m_instance->GetState() == DirectX::SoundState::PAUSED)return true;

	return false;
}

KdSoundInstance::~KdSoundInstance()
{
	Stop();
	m_instance = nullptr;
	m_srcData = nullptr;
}

//==================================================================
//
// KdSoundEffect
//
//==================================================================
bool KdSoundEffect::Load(const std::string& fileName)
{
	if(!AUDIO.IsInit())return false;


	try 
	{
		// wstring�ɕϊ�
		std::wstring wFilename = sjis_to_wide(fileName);

		// �ǂݍ���
		m_soundEffect = std::make_unique<DirectX::SoundEffect>(AUDIO.GetAudioEngine().get(), wFilename.c_str());
	}
	catch (...) 
	{
		return false;
	}
	
	// �t�@�C�����L��
	m_fileName = fileName;

	return true;
}

std::shared_ptr<KdSoundInstance> KdSoundEffect::CreateInstance(bool b3D)
{
	if(AUDIO.IsInit() == false)return nullptr;
	if (m_soundEffect == nullptr)return nullptr;

	// SoundInstance��private�R���X�g���N�^�Ȃ̂ŁAmake_shared���g��Ȃ����@�ō쐬����
	std::shared_ptr<KdSoundInstance> inst = std::shared_ptr<KdSoundInstance>(new KdSoundInstance());

	// �t���O
	DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default;
	if (b3D) {
		flags = flags | DirectX::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters;
	}

	// �C���X�^���X�ݒ�
	inst->Init(m_soundEffect->CreateInstance(flags), shared_from_this(), b3D);

	return inst;
}


//==================================================================
//
//
// KdAudioManager
//
//
//==================================================================

bool KdAudioManager::Init()
{
	Release();

	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_ReverbUseFilters;
	#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
	#endif
	
	m_audioEng = std::make_unique<DirectX::AudioEngine>( eflags );
	m_audioEng->SetReverb(DirectX::Reverb_Default);

	m_listener.OrientFront = { 0, 0, 1 };

	return true;
}


void KdAudioManager::Release()
{
	m_playList.clear();

	m_audioEng = nullptr;

}

void KdAudioManager::StopAll()
{
	if(m_audioEng == nullptr)return;

	// �Đ����X�g
	{
		auto it = m_playList.begin();
		while(it != m_playList.end()){
			(*it).second->Stop();
			++it;
		}
	}

}

void KdAudioManager::Update()
{
	if (m_audioEng == nullptr)return;

	if (!m_audioEng->Update()) {
		// No audio device is active
		if (m_audioEng->IsCriticalError())
		{

		}
	}

	// �Đ����X�g����
	{
		auto it = m_playList.begin();
		while(it != m_playList.end())
		{
			// �Đ����łȂ��Ƃ��́A���X�g����폜(�ǂ�������Q�Ƃ���Ă��Ȃ���Ή�������)
			if(!(*it).second->IsPlay())
			{
				it = m_playList.erase(it);
				continue;
			}

			++it;
		}
	}

}
