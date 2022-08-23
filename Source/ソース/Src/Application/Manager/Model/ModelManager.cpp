#include "ModelManager.h"

KdModel* ModelManager::GetModel(std::string _fileName)
{

	KdModel* tmpModel;

	if (m_modelList.find(_fileName) == m_modelList.end())
	{
		//map�N���X��T�������A�Y���̃t�@�C������������Ȃ������ꍇ

		tmpModel = new KdModel(); //���f���P���̃��������m�ۂ���
		tmpModel->Load(_fileName); //�Y���̃t�@�C�������[�h����

		//�t�@�C�������L�[�Ƃ��ăA�h���X��map�ɕۑ�����
		m_modelList[_fileName] = tmpModel;

	}
	else {
		tmpModel = m_modelList[_fileName];
	}

	return tmpModel;
}

ModelManager::~ModelManager()
{

	//map���ɕۑ�����Ă���S���ڂ�����Release����
	for (auto itr = m_modelList.begin(); itr != m_modelList.end();++itr)
	{
		//�e���ڂ̍���(�t�@�C����) == itr->first
		//�e���ڂ̉E��(�A�h���X) == itr->second
		(itr->second)->Release();

		delete (itr->second);
	}

	//���X�g���̂��̂��폜����
	m_modelList.clear();

}