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
	//�u�t�@�C����(string�^)�v�E�E�E�u�f�[�^�̃A�h���X(KdTexture*�^)�v�Ƃ����ꗗ�\�����
	std::map<std::string, KdTexture*>m_textureList;

	TextureManager()
	{

	}

};
#define TEXTURE TextureManager::GetInstance()