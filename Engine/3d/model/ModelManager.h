#pragma once
#include<string>
#include<unordered_map>
#include<memory>
#include<Model.h>
using namespace MyEngin;
enum ModelData
{
	skydome,
	cube,
	body,
	had,
	map,
	enemy,
	parachute,
	fixedgun,
	wall,
	bullet,
	bom,
	smoke,
	Max,
};
class ModelManager
{
public://静的メンバ関数
	static ModelManager* GetInstance();
public://メンバ関数
	//モデル読み込み
	void LoadModel();
	//モデルの取得
	Model* GetModel(const ModelData&key_);

private:
	std::unordered_map<ModelData,std::unique_ptr<Model>>models;

private:
	//コンストラクタ
	ModelManager();
	//デストラクタ
	~ModelManager();
public:
	//コピーコンストラクタを無効
	ModelManager(const ModelManager& obj) = delete;
	//代入演算子を無効
	ModelManager& operator=(const ModelManager& obj) = delete;
};

