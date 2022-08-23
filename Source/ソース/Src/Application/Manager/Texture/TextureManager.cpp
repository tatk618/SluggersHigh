#include "TextureManager.h"

KdTexture* TextureManager::GetTexture(std::string _fileName)
{
	KdTexture* tmpTex;

	if (m_textureList.find(_fileName) == m_textureList.end())
	{
		//mapクラスを探したが、該当のファイル名が見つからなかった場合

		tmpTex = new KdTexture(); //画像１枚分のメモリを確保する
		tmpTex->Load(_fileName); //該当のファイルをロードする

		//ファイル名をキーとしてアドレスをmapに保存する
		m_textureList[_fileName] = tmpTex;

	}
	else {
		tmpTex = m_textureList[_fileName];
	}

	return tmpTex;
}

TextureManager::~TextureManager()
{

	//map内に保存されている全項目を順にReleaseする
	for (auto itr = m_textureList.begin(); itr != m_textureList.end();++itr)
	{
		//各項目の左側(ファイル名) == itr->first
		//各項目の右側(アドレス) == itr->second
		(itr->second)->Release();

		delete (itr->second);
	}

	//リストそのものを削除する
	m_textureList.clear();

}