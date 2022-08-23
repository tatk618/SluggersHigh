#pragma once
 
class C_SoundManager
{
public:
	static  C_SoundManager& GetInstance()
	{
		static C_SoundManager sound;
		return sound;
	}
	~C_SoundManager();

	/// <summary>
	/// BGM���Đ�����
	/// </summary>
	/// <param name="_fileName">���f�[�^�̃p�X</param>
	/// <param name="_volume">����</param>
	void PlayBGM(const std::string _fileName,float _volume = 0.3f);

	/// <summary>
	/// BGM���~�߂�
	/// </summary>
	/// <param name="_fileName">���f�[�^�̃p�X</param>
	void StopBGM(const std::string _fileName);

	/// <summary>
	/// �{�����[����ݒ肷��
	/// </summary>
	/// <param name="_fileName">���f�[�^�̃p�X</param>
	/// <param name="_volume">����</param>
	void SetVolume(const std::string _fileName,const float _volume);
 
	/// <summary>
	/// �ꎞ��~���ĊJ
	/// </summary>
	/// <param name="_fileName">���f�[�^�̃p�X</param>
	/// <param name="_res">�ꎞ��~��true�A�ĊJ��false</param>
	void PauseBGM(const std::string _fileName,bool _res = true);

	/// <summary>
	/// 3D���ʉ��𔭐�������
	/// </summary>
	/// <param name="_fileName">���f�[�^�̃p�X</param>
	/// <param name="_pos">�������W</param>
	void Play3D(const std::string _fileName, const Math::Vector3& _pos,const float _volume = 1.0f);

	/// <summary>
	/// 2D���ʉ��𔭐�������
	/// </summary>
	/// <param name="_fileName">���f�[�^�̃p�X</param>
	/// <param name="_volume">����(���������ł���)</param>
	void Play2D(const std::string _fileName,float _volume = 0.3f);

	/// <summary>
	/// ���X�i�[�̈ʒu��ݒ�
	/// </summary>
	/// <param name="_pos">���W</param>
	/// <param name="_frontVec">�O������</param>
	/// <param name="_head">������</param>
	void Update(const Math::Vector3& _pos, const Math::Vector3& _frontVec, const Math::Vector3& _head);
private:
	C_SoundManager();

	//�p�X���L�[�Ƃ����C���X�^���X�̃��X�g
	std::map<std::string, std::shared_ptr<KdSoundInstance>>m_InstanceList;
	//�p�X���L�[�Ƃ������f�[�^�̃��X�g
	std::map<std::string, std::shared_ptr<KdSoundEffect>>m_soundList;

	//���f�[�^�̃��[�h������֐�
	std::shared_ptr<KdSoundEffect> GetSound(const std::string _fileName);
	//�C���X�^���X�f�[�^�̃��[�h������֐�
	std::shared_ptr<KdSoundInstance> GetInstance(const std::string _fileName);
};
#define SOUND C_SoundManager::GetInstance()