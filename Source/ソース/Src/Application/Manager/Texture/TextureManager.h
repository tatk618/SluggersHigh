#pragma once

class TextureManager
{
public:
	KdTexture* GetTexture(std::string _fileName);
	
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

	~TextureManager();

private:
	//「ファイル名(string型)」・・・「データのアドレス(KdTexture*型)」という一覧表を作る
	std::map<std::string, KdTexture*>m_textureList;

	TextureManager()
	{

	}

};
#define TEXTURE TextureManager::GetInstance()