#include "ModelManager.h"

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::LoadModel()
{
	models.emplace(skydome,Model::LoadFromOBJ("skydome",true));
	models.emplace(cube,Model::LoadFromOBJ("cube"));
	models.emplace(body,Model::LoadFromOBJ("TankBody"));
	models.emplace(had,Model::LoadFromOBJ("TankHad"));
	models.emplace(map,Model::LoadFromOBJ("map"));
	models.emplace(enemy,Model::LoadFromOBJ("tank"));
	models.emplace(parachute,Model::LoadFromOBJ("parachute"));
	models.emplace(fixedgun,Model::LoadFromOBJ("fixedgun"));
	models.emplace(wall,Model::LoadFromOBJ("wall"));
	models.emplace(bullet,Model::LoadFromOBJ("bullet"));

}

Model* ModelManager::GetModel(const ModelData& key_)
{
	auto it = models.find(key_);
	if ( it != models.end() )
	{
		return it->second.get(); // イテレーターの second メンバが Model オブジェクトへの参照を保持しているので、そのポインタを返す
	}
	else
	{
		return nullptr; // キーが見つからなかった場合は nullptr を返す
	}
}

ModelManager::ModelManager() {}
ModelManager::~ModelManager(){}