#include "ModelManager.h"

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::LoadModel(const std::string& key_,bool smoothing_)
{
	if ( models.contains(key_) )
	{
		//読み込み済みなら早期return
		return;
	}
	else
	{
		models.emplace(key_,Model::LoadFromOBJ(key_,smoothing_));
	}
}

Model* ModelManager::GetModel(const std::string& key_)
{
	auto it = models.find(key_);
	if ( it != models.end() )
	{
		return it->second.get(); // イテレーターの second メンバが Model オブジェクトへの参照を保持しているので、そのポインタを返す
	}
	else
	{
		models.emplace(key_,Model::LoadFromOBJ(key_));
		return models.at(key_).get(); // キーが見つからなかった場合は nullptr を返す
	}
}

ModelManager::ModelManager() {}
ModelManager::~ModelManager(){}