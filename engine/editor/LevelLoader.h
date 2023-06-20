#pragma once
#include<fstream>
#include <vector>
#include <DirectXMath.h>
#include<nlohmann/json.hpp>

// レベルデータ
struct LevelData {

	struct ObjectData {
		// ファイル名
		std::string fileName;
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;

	std::vector < LevelData> *chidren;
};

class LevelLoader
{
public:
	static LevelData* LoadJson(const std::string& fileName);

private:
	static LevelData* ImportValue(nlohmann::json& json, LevelData* levelData );
};

