#include "ModelManager.h"

KdModel* ModelManager::GetModel(std::string _fileName)
{

	KdModel* tmpModel;

	if (m_modelList.find(_fileName) == m_modelList.end())
	{
		//mapクラスを探したが、該当のファイル名が見つからなかった場合

		tmpModel = new KdModel(); //モデル１個分のメモリを確保する
		tmpModel->Load(_fileName); //該当のファイルをロードする

		//ファイル名をキーとしてアドレスをmapに保存する
		m_modelList[_fileName] = tmpModel;

	}
	else {
		tmpModel = m_modelList[_fileName];
	}

	return tmpModel;
}

ModelManager::~ModelManager()
{

	//map内に保存されている全項目を順にReleaseする
	for (auto itr = m_modelList.begin(); itr != m_modelList.end();++itr)
	{
		//各項目の左側(ファイル名) == itr->first
		//各項目の右側(アドレス) == itr->second
		(itr->second)->Release();

		delete (itr->second);
	}

	//リストそのものを削除する
	m_modelList.clear();

}