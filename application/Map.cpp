#include "Map.h"
#include<MyMath.h>	
#include<Easing.h>
#include<sstream>

std::unique_ptr < Model> Map::model;

Map::~Map(){}

void Map::StaticInitialize(Model* model_) {
	model.reset(model_);
}

void Map::Initialize() {
	//変数の初期化
	nowMax = 0;
	
	scale += 10.0f;
	endFrame = 100;
	
	startY = -(scale.y + 0.2f) + constStartY;
	endY =  -(scale.y + 0.2f);
}

void Map::Update() {
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
					
					if (h < 0 )	h = 0;
					if (h >= high)h = high - 1;
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
	for (size_t i = 0; i < high; i++)
	{

		for (size_t j = 0; j < width; j++)
		{

			//フラグが立っていたら
			if (blocks[i][j].isUp)
			{
				Vector3 easeVec = blocks[i][j].obj->GetPosition();
				easeVec.y = startY + (endY - startY) * Easing::easeOutBack(blocks[i][j].y);
				blocks[i][j].y+=0.02f;
				blocks[i][j].obj->SetPosition(easeVec);
			}
			if (blocks[i][j].y >= 1.0f)
			{
				blocks[i][j].isUp = false;
			}
			blocks[i][j].obj->Update();
			

		}
	}

}

void Map::Draw() {
	for (size_t i = 0; i < high; i++)
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

	high = (int8_t)std::atof(num.c_str());
	std::getline(line_stream, num, ',');
	width = (int8_t)std::atof(num.c_str());
	numW = (width / 2);
	numH = (high / 2);
	
	
	
	// 二次元配列のサイズを初期化
	blocks.resize(high);
	for (int i = 0; i < high; ++i) {
		blocks[i].resize(width);
	}


	for (size_t i = 0; i < high; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			//

			Vector3 pos = { 0.0f,startY, 0.0f };
			pos.x = (j * scale.x) * 2.0f - (scale.x * width);
			pos.z = (i * scale.z) * 2.0f - (scale.z * high);
			//オブジェクトにパラメータをセット
			blocks[i][j].obj = std::make_unique<Object3d>();
			blocks[i][j].obj->Initialize();
			blocks[i][j].obj->SetModel(model.get());
			blocks[i][j].obj->SetPosition(pos);
			blocks[i][j].obj->SetScale(scale);
			//blocks[i][j].obj->SetColor({ 0.0f, 0.0f, 0.1f,1.0f});
			//blocks[i][j].pos = pos;
			blocks[i][j].frame = 0;
			blocks[i][j].obj->Update();
		}
		std::getline(mapLoad, line, '\n');
	}
}


//終了
void Map::Finalize() {
	model.release();
}