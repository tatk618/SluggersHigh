#include "TextureManager.h"

KdTexture* TextureManager::GetTexture(std::string _fileName)
{
	KdTexture* tmpTex;

	if (m_textureList.find(_fileName) == m_textureList.end())
	{
		//map�N���X��T�������A�Y���̃t�@�C������������Ȃ������ꍇ

		tmpTex = new KdTexture(); //�摜�P�����̃��������m�ۂ���
		tmpTex->Load(_fileName); //�Y���̃t�@�C�������[�h����

		//�t�@�C�������L�[�Ƃ��ăA�h���X��map�ɕۑ�����
		m_textureList[_fileName] = tmpTex;

	}
	else {
		tmpTex = m_textureList[_fileName];
	}

	return tmpTex;
}

TextureManager::~TextureManager()
{

	//map���ɕۑ�����Ă���S���ڂ�����Release����
	for (auto itr = m_textureList.begin(); itr != m_textureList.end();++itr)
	{
		//�e���ڂ̍���(�t�@�C����) == itr->first
		//�e���ڂ̉E��(�A�h���X) == itr->second
		(itr->second)->Release();

		delete (itr->second);
	}

	//���X�g���̂��̂��폜����
	m_textureList.clear();

}