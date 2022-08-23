#pragma once

class ModelManager
{
public:
	KdModel* GetModel(std::string _fileName);

	static ModelManager& GetInstance()
	{
		static ModelManager instance;
		return instance;
	}

	~ModelManager();

private:
	//�u�t�@�C����(string�^)�v�E�E�E�u�f�[�^�̃A�h���X(KdTexture*�^)�v�Ƃ����ꗗ�\�����
	std::map<std::string, KdModel*>m_modelList;

	ModelManager()
	{

	}

};
#define MODEL ModelManager::GetInstance()