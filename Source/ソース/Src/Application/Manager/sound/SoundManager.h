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
	/// BGMを再生する
	/// </summary>
	/// <param name="_fileName">音データのパス</param>
	/// <param name="_volume">音量</param>
	void PlayBGM(const std::string _fileName,float _volume = 0.3f);

	/// <summary>
	/// BGMを止める
	/// </summary>
	/// <param name="_fileName">音データのパス</param>
	void StopBGM(const std::string _fileName);

	/// <summary>
	/// ボリュームを設定する
	/// </summary>
	/// <param name="_fileName">音データのパス</param>
	/// <param name="_volume">音量</param>
	void SetVolume(const std::string _fileName,const float _volume);
 
	/// <summary>
	/// 一時停止＆再開
	/// </summary>
	/// <param name="_fileName">音データのパス</param>
	/// <param name="_res">一時停止はtrue、再開はfalse</param>
	void PauseBGM(const std::string _fileName,bool _res = true);

	/// <summary>
	/// 3D効果音を発生させる
	/// </summary>
	/// <param name="_fileName">音データのパス</param>
	/// <param name="_pos">音源座標</param>
	void Play3D(const std::string _fileName, const Math::Vector3& _pos,const float _volume = 1.0f);

	/// <summary>
	/// 2D効果音を発生させる
	/// </summary>
	/// <param name="_fileName">音データのパス</param>
	/// <param name="_volume">音量(引数無しでも可)</param>
	void Play2D(const std::string _fileName,float _volume = 0.3f);

	/// <summary>
	/// リスナーの位置を設定
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_frontVec">前方方向</param>
	/// <param name="_head">頭方向</param>
	void Update(const Math::Vector3& _pos, const Math::Vector3& _frontVec, const Math::Vector3& _head);
private:
	C_SoundManager();

	//パスをキーとしたインスタンスのリスト
	std::map<std::string, std::shared_ptr<KdSoundInstance>>m_InstanceList;
	//パスをキーとした音データのリスト
	std::map<std::string, std::shared_ptr<KdSoundEffect>>m_soundList;

	//音データのロードをする関数
	std::shared_ptr<KdSoundEffect> GetSound(const std::string _fileName);
	//インスタンスデータのロードをする関数
	std::shared_ptr<KdSoundInstance> GetInstance(const std::string _fileName);
};
#define SOUND C_SoundManager::GetInstance()