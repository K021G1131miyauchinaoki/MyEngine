/**
 * @file LevelLoader.h
 * @brief jsonファイルの読み込み
 */

#pragma once
#include<fstream>
#include <vector>
#include <DirectXMath.h>
#include<nlohmann/json.hpp>

#pragma warning( disable : 4324 )

// レベルデータ
struct LevelData {

	struct ObjectData  
	{
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
		// ファイル名
		std::string fileName;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;

	std::vector < LevelData> *chidren;
};

class LevelLoader
{
public:
	//jsonの読み込み
	static LevelData* LoadJson(const std::string& fileName_);

private:
	//値を取得
	static LevelData* ImportValue(nlohmann::json& json_, LevelData* levelData_ );
};

