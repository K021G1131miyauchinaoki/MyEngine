#include "Map.h"

const int8_t Map::heith;
const int8_t Map::width;

void Map::Initialize(Model*model_) {
	scale += 10.0f;
	scale.y = 1.0f;
	for (size_t i = 0; i < heith; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			Vector3 pos = {0.0f,-(scale.y+0.5f), 0.0f};
			pos.x = (j * scale.x)*2 + num;
			pos.z = (i * scale.z)*2 + num;
			block[i][j].obj = std::make_unique<Object3d>();
			block[i][j].obj->Initialize();
			block[i][j].obj->SetModel(model_);
			block[i][j].obj->SetPosition(pos);
			block[i][j].obj->SetScale(scale);
			block[i][j].obj->SetColor({ 0.0f, 0.0f, 0.1f,1.0f});
			//block[i][j].pos = pos;

		}
	}
}

void Map::Updata() {
	for (size_t i = 0; i < heith; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			block[i][j].obj->Update();
		}
	}
}

void Map::Draw() {
	for (size_t i = 0; i < heith; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			block[i][j].obj->Draw();
		}
	}
}

Map::Map() {}

Map::~Map(){}