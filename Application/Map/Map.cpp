/**
 * @file Map.cpp
 * @brief ステージ
 */
#include "Map.h"
#include<MyMath.h>	
#include<Easing.h>
#include<sstream>

std::unique_ptr < Model> Map::model;
float Map::moveLimitW;
float Map::moveLimitH;
float Map::mapScaleW;
float Map::mapScaleH;
Map::~Map(){}

void Map::StaticInitialize(Model* model_) {
	if (model == nullptr)
	{
		model.reset(model_);
	}
}

void Map::Initialize(bool isStaging_) {
	//変数の初期化
	nowMax = 0;
	//スケール
	scaleEnd += 10.0f;
	scaleStart += 0.1f;
	endFrame = 100;
	//位置
	posStartY = -(scaleEnd.y ) + constStartY;
	posEndY =  -(scaleEnd.y );
	//回転
	rotEndZ = 0;
	rotStartZ=360*2;
	isStaging = isStaging_;
}

void Map::Update() {
	Preparation();
	for (size_t i = 0; i < height; i++)
	{

		for (size_t j = 0; j < width; j++)
		{
			Staging(i, j);
			blocks[i][j].obj->Update();
		}
	}

}

void Map::Draw() {
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			blocks[i][j].obj->Draw();
		}
	}
}

void Map::LoadCSV(const std::string& num_) {
	//ステージの読み込み
	std::stringstream mapLoad;
	//ファイルを開く
	const std::string filename = "Resources/CSV/" + num_ + ".csv";
	std::ifstream file;
	file.open(filename);
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	mapLoad << file.rdbuf();

	//ファイルを閉じる
	file.close();

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	std::getline(mapLoad, line, '\n');

	std::istringstream line_stream(line);
	std::string num;
	

	std::getline(line_stream, num, ',');
	height = (int8_t)std::atof(num.c_str());
	std::getline(line_stream, num, ',');
	width = (int8_t)std::atof(num.c_str());
	numW = (width / 2);
	numH = (height / 2);
	
	
	
	// 二次元配列のサイズを初期化
	blocks.resize(height);
	for (int i = 0; i < height; ++i) {
		blocks[i].resize(width);
	}

	//0,1のマップチップを埋め込みつつ（未）初期化
	for (size_t i = 0; i < height; i++)
	{
		std::getline(mapLoad, line, '\n');

		for (size_t j = 0; j < width; j++)
		{
			//フラグで挿入するパラメータを変化
			float rotZ;
			Vector3	scale, pos;
			if (isStaging)
			{
				rotZ = rotStartZ;
				scale = scaleStart;
				pos.y = posStartY;
				pos.z = (i * scaleEnd.z) * 2.0f - (scaleEnd.z * height);
			}
			else
			{
				rotZ = rotEndZ;
				scale = scaleEnd;
				pos.y = posEndY;
				pos.z = (i * scaleEnd.z) * 2.0f - ((scaleEnd.z * height)/4.0f);
			}
			pos.z = ( i * scaleEnd.z ) * 2.0f - ( scaleEnd.z * height );

			pos.x = (j * scaleEnd.x) * 2.0f - (scaleEnd.x * width);
			//オブジェクトにパラメータをセット
			blocks[i][j].obj = std::make_unique<Object3d>();
			blocks[i][j].obj->Initialize();
			blocks[i][j].obj->SetModel(model.get());
			blocks[i][j].obj->SetScale(scale); 
			blocks[i][j].obj->SetRotation({0.0f,0.0f,rotZ});
			blocks[i][j].obj->SetPosition(pos);
			//blocks[i][j].obj->SetColor({ 0.0f, 0.0f, 0.1f,1.0f});
			//blocks[i][j].pos = pos;
			blocks[i][j].frame = 0;
			blocks[i][j].obj->Update();
		}
	}

	//移動制限の数値設定
	moveLimitW = static_cast<float>(width) * scaleEnd.x;
	moveLimitH = static_cast<float>(height) * scaleEnd.z;
	mapScaleW = scaleEnd.x;
	mapScaleH = scaleEnd.z;
}

void Map::Preparation() {
	if (isStaging)
	{
		timer--;
		if (timer < 0)
		{
			timer = time;
			if (nowMax <= numW || nowMax <= numH)
			{
				setPoint = -nowMax;

				for (int16_t i = setPoint; i <= nowMax; i++)
				{
					for (int16_t j = setPoint; j <= nowMax; j++)
					{
						int16_t h = numH + i;
						int16_t w = numW + j;

						if (h < 0)	h = 0;
						if (h >= height)h = height - 1;
						if (w < 0)	w = 0;
						if (w >= width)w = width - 1;
						//フラグが立っていない場合
						if (!blocks[h][w].isUp)
						{
							blocks[h][w].isUp = true;
						}

					}
				}
				nowMax++;
			}
		}
	}
}

void Map::Staging(size_t y_, size_t x_) {
	//フラグが立っていたら
	if (blocks[y_][x_].isUp&&isStaging)
	{
		//スケール
		Vector3 easeScale = blocks[y_][x_].obj->GetScale();
		easeScale = scaleStart + (scaleEnd - scaleStart) * Easing::easeOutCubic(blocks[y_][x_].range);
		blocks[y_][x_].obj->SetScale(easeScale);
		//回転
		Vector3 easeRot = blocks[y_][x_].obj->GetRotation();
		easeRot.z = rotStartZ + (rotEndZ - rotStartZ) * Easing::easeOutSine(blocks[y_][x_].range);
		blocks[y_][x_].obj->SetRotation(easeRot);
		//座標
		Vector3 easePos = blocks[y_][x_].obj->GetPosition();
		easePos.y = posStartY + (posEndY - posStartY) * Easing::easeOutCubic(blocks[y_][x_].range);
		blocks[y_][x_].obj->SetPosition(easePos);
		//1.0fまで加算
		blocks[y_][x_].range += 0.02f;
		if (blocks[y_][x_].range >= 1.0f)
		{
			blocks[y_][x_].isUp = false;
		}
	}
}

//終了
void Map::Finalize() {
	model.release();
}