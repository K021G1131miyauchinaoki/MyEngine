#include "Map.h"

const int8_t Map::heith;
const int8_t Map::width;

void Map::Initialize(Model*model_) {
	scale += 3.0f;
	for (size_t i = 0; i < heith; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			block[i][j].obj = std::make_unique<Object3d>();
			block[i][j].obj->SetModel(model_);
			Vector3 pos = {0.0f,0.0f, 0.0f};
			pos.x = (j * scale.x);

		}
	}
}

void Map::Updata() {

}

void Map::Draw() {

}

Map::Map() {}

Map::~Map(){}