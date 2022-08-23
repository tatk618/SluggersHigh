//===============================================================
//
// ���֌W�̃N���X���܂Ƃ߂ď����Ă��܂��B
//
//	KdSoundInstance�N���X	�c ���ۍĐ������T�E���h�C���X�^���X�BKdSoundEffect�N���X���琶�������B
//	KdSoundEffect�N���X	�c �T�E���h�̓ǂݍ��݂�A���ۂɍĐ�����C���X�^���X�̍Đ��ȂǁB
//	KdAudioManager�N���X	�c XAudio�̏������E�����Đ����̃T�E���h�Ǘ��ȂǁB
//
//===============================================================
#pragma once

class KdSoundInstance;
class KdSoundEffect;

//=======================================================
// �Đ��p�T�E���h�C���X�^���X�N���X
//
//  ���ۂɍĐ�����T�E���h�I�u�W�F�N�g
//  KdSoundEffect�N���X��CreateInstance()�ō쐬�\
//
//=======================================================
class KdSoundInstance : public std::enable_shared_from_this<KdSoundInstance>{
	friend class KdSoundEffect;
public:

	//==================================================================
	// �擾
	//==================================================================
	DirectX::AudioEmitter&			GetEmitter() { return m_emitter; }
	std::shared_ptr<KdSoundEffect>	GetSoundEffect() { return m_srcData; }

	//==================================================================
	// �ݒ�
	//==================================================================
	// �����ݒ�
	void Init(std::unique_ptr<DirectX::SoundEffectInstance> inst, const std::shared_ptr<KdSoundEffect>& soundEffect, bool enable3D)
	{
		m_instance = std::move(inst);
		m_srcData = soundEffect;
		m_is3D = enable3D;
	}

	//==================================================================
	// �Đ�
	//==================================================================

	// 2D�Đ�
	// �Eloop		�c ���[�v�Đ�����H
	// �Emanage		�c AudioManager���Đ��Ǘ�������H
	void Play(bool loop = false, bool manage = true);

	// 3D�Đ�
	// �Epos		�c ���W
	// �Eloop		�c ���[�v�Đ����邩�H
	// �Emanage		�c AudioManager���Đ��Ǘ�������H
	void Play3D(const Math::Vector3& pos, bool loop = false, bool manage = true);

	// m_Emitter�̏���K�p����
	void Apply3D();

	//==================================================================
	// ��~
	//==================================================================
	// ��~
	void Stop() {
		if(m_instance)m_instance->Stop();
	}

	// �ꎞ��~
	void Pause() {
		if (m_instance)m_instance->Pause();
	}

	// �ĊJ
	void Resume() {
		if (m_instance)m_instance->Resume();
	}

	//==================================================================
	// �ݒ�
	//==================================================================
	// �{�����[���ݒ�
	// �Evol	�c �{�����[���ݒ�(1.0��100%)
	void SetVolume(float vol);
	
	// 3D�T�E���h���W�ݒ�
	// �Epos		�c ���W
	void SetPos(const Math::Vector3& pos);

	// �����{���ݒ� 1:�ʏ� FLT_MIN�`FLT_MAX
	void SetCurveDistanceScaler(float val);

	//==================================================================
	// �擾
	//==================================================================
	// �Đ����H
	bool IsPlay();

	// �|�[�Y���H
	bool IsPause();

	//==================================================================
	~KdSoundInstance();


private:

	// 
	KdSoundInstance(){}

private:

	// �T�E���h�̍Đ��C���X�^���X
	std::unique_ptr<DirectX::SoundEffectInstance>	m_instance;

	// �Đ��T�E���h�̌��f�[�^
	std::shared_ptr<KdSoundEffect>					m_srcData;

	// �G�~�b�^�[ ���3D�T�E���h�\�[�X�̒�`
	DirectX::AudioEmitter							m_emitter;

	// 3D�T�E���h��
	bool											m_is3D = false;

private:
	// �R�s�[�֎~�p
	KdSoundInstance(const KdSoundInstance& src) = delete;
	void operator=(const KdSoundInstance& src) = delete;
};

//=======================================================
//
// �T�E���h�f�[�^�N���X
//
//�@�T�E���h�̌��f�[�^�ƂȂ�N���X�B���ꎩ�͍̂Đ��ł��܂���B
//�@CreateInstance()�Ŏ��ۂɍĐ�����T�E���hKdSoundInstance�N���X�̃C���X�^���X�𐶐��B
//
//=======================================================
class KdSoundEffect : public std::enable_shared_from_this<KdSoundEffect> {
public:

	// WAVE�T�E���h�ǂݍ���
	// �Efilename	�c �t�@�C����
	// �߂�l		�c �����Ftrue
	bool Load(const std::string& fileName);

	// �Đ��p�̃T�E���h�C���X�^���X�쐬
	// �Eb3D			�c 3D�T�E���h�ɂ��邩�H
	// �E�߂�l		�c �V���ɍ쐬�����Đ��p�T�E���h�C���X�^���X
	std::shared_ptr<KdSoundInstance> CreateInstance(bool b3D);

	~KdSoundEffect() {
		m_soundEffect = nullptr;
		m_fileName = "";
	}

	// 
	KdSoundEffect() {}

private:
	// �T�E���h�G�t�F�N�g
	std::unique_ptr<DirectX::SoundEffect>	m_soundEffect;

	// �t�@�C����
	std::string								m_fileName;

private:
	// �R�s�[�֎~�p
	KdSoundEffect(const KdSoundEffect& src) = delete;
	void operator=(const KdSoundEffect& src) = delete;
};

//================================================
//
// �T�E���h�Ǘ� �V���O���g���N���X
//
// 
//================================================
class KdAudioManager{
	friend class KdSoundInstance;
public:
	//==================================================================
	// �擾�n
	//==================================================================
	//  AudioEngine�擾
	std::unique_ptr<DirectX::AudioEngine>& GetAudioEngine() { return m_audioEng; }

	// 3D���X�i�[(������)�擾
	DirectX::AudioListener&	GetListener() { return m_listener; }

	// ����������Ă��邩�H
	bool	IsInit() { return (m_audioEng != nullptr); }

	// ���ݍĐ����̃T�E���h���擾
	int		GetNumPlaying() { return (int)m_playList.size(); }


	//==================================================================
	// �������E���
	//==================================================================

	// ������
	bool Init();

	// ���
	void Release();

	//==================================================================
	// �Đ��T�E���h����
	//==================================================================

	// ���ׂĒ�~
	void StopAll();

	//==================================================================
	// ����
	//==================================================================
	// �����@����I�Ɏ��s���Ă�������
	// �Đ����̃T�E���h�̊Ǘ��⃊�X�i�[�̐ݒ�ɉh�Ȃǂ��s���Ă��܂�
	void Update();

	//==================================================================
	~KdAudioManager() {
		Release();
	}

private:

	// AudioEngine
	std::unique_ptr<DirectX::AudioEngine>				m_audioEng;

	// 3D�T�E���h�p���X�i�[
	DirectX::AudioListener								m_listener;

	// ���ݍĐ����̃T�E���h�Ǘ����X�g
	std::map<UINT, std::shared_ptr<KdSoundInstance>>		m_playList;

private:

	// �Đ����X�g�ǉ��B���ۂɍĐ�����Ă�����̂ɂȂ�B
	void AddPlayList(const std::shared_ptr<KdSoundInstance>& snd){
		if(snd == nullptr)return;
		m_playList[(UINT)snd.get()] = snd;
	}

//===================================================================
// �V���O���g��
//===================================================================
private:
	KdAudioManager() {}

public:
	static KdAudioManager& GetInstance()
	{
		static KdAudioManager	instance;
		return instance;
	}
};

#define AUDIO KdAudioManager::GetInstance()

