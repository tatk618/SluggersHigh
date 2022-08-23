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
	//「ファイル名(string型)」・・・「データのアドレス(KdTexture*型)」という一覧表を作る
	std::map<std::string, KdModel*>m_modelList;

	ModelManager()
	{

	}

};
#define MODEL ModelManager::GetInstance()