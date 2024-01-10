/**
 * @file LevelLoader.cpp
 * @brief jsonファイルの読み込み
 */

#include "LevelLoader.h"
#include<assert.h>
using namespace MyEngin;

LevelData* LevelLoader::LoadJson(const std::string& filename_) {
	
	//拡張子
	const std::string extension=".json";
	//ディレクトリ
	const std::string directoryPath = "Resources/";
	//フルパス
	const std::string fullPath = directoryPath + filename_ + extension;
	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullPath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}
	//json文字列から解答したデータ
	nlohmann::json deserialized;

	//解答
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	ImportValue(deserialized,levelData);
	
	return  levelData;
}

LevelData* LevelLoader::ImportValue(nlohmann::json& json, LevelData* levelData){
	for (nlohmann::json& object : json["objects"])
	{
		assert(object.contains("type"));
		//種類を取得
		std::string type = object["type"].get<std::string>();
		//MESH
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}
			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			//回転角
			objectData.rotation.x = -( float ) transform[ "rotation" ][ 0 ];
			objectData.rotation.y = -( float ) transform[ "rotation" ][ 2 ];
			objectData.rotation.z = (float)transform["rotation"][1];
			//スケーリング
			objectData.scaling.x = ( float ) transform[ "scaling" ][ 1 ];
			objectData.scaling.y = ( float ) transform[ "scaling" ][ 2 ];
			objectData.scaling.z = (float)transform["scaling"][0];
			//子がいたら走査
			if (object.contains("children"))
			{
				
			}
		}

	}

	return levelData;
}