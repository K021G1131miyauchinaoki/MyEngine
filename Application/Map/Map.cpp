
/**
 * @file Map.cpp
 * @brief ステージ
 */

#include "Map.h"
#include<MyMath.h>	
#include<Easing.h>
#include<sstream>
#include<random>

float Map::moveLimitW;
float Map::moveLimitH;
float Map::mapScaleW;
float Map::mapScaleH;
bool Map::isStaging;
int16_t Map::width;
int16_t Map::height;

Map::Map() {}
Map::~Map(){}

void Map::Initialize(bool isStaging_,Model* model_) {
	model = model_;
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
	rotStartZ=720;
	isStaging = isStaging_;
	provisionTime = provisionTimer;
	change = false;
	flag = false;
}

void Map::Update() {
	Preparation();
	for (size_t i = 0; i < height; i++)
	{

		for (size_t j = 0; j < width; j++)
		{
			Staging(i, j);
			blocks[i][j].Update();
		}
	}
}

void Map::Draw() {
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			if ( blocks[i][j].GetDrawNum()>=1 )
			{
				blocks[ i ][ j ].Draw();
			}
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
	height = (int16_t)std::atof(num.c_str());
	std::getline(line_stream, num, ',');
	width = (int16_t)std::atof(num.c_str());
	numW = (width / 2);
	numH = (height / 2);
	
	
	
	// 二次元配列のサイズを初期化
	blocks.resize(height);
	for (int i = 0; i < height; ++i) {
		blocks[i].resize(width);
	}

	//パラメータの組み込み
	for (size_t i = 0; i < height; i++)
	{
		if ( i%2==0 )
		{
			change = false;
		}
		else
		{
			change = true;
		}
		for (size_t j = 0; j < width; j++)
		{
			//フラグで挿入するパラメータを変化
			float rotZ;
			Vector3	scale, pos,rot;
			if (isStaging)
			{
				rotZ = rotStartZ;
				scale = scaleStart;
				pos.y = posStartY;
			}
			else
			{
				rotZ = rotEndZ;
				scale = scaleEnd;
				pos.y = posEndY;
			}
			pos.z = ( i * scaleEnd.z ) * 2.0f - ( scaleEnd.z * ( height - 1 ) );
			pos.x = (j * scaleEnd.x) * 2.0f - (scaleEnd.x * (width-1 ));

			rot = { 0.0f,0.0f,rotZ };
			//オブジェクトにパラメータをセット
			blocks[ i ][ j ].Initialize(pos,rot,scale,model);
			if ( change )
			{
				blocks[ i ][ j ].SetColor({0.7f,0.7f, 0.7f, 1.0f});
			}
			change ^= 1;

		}
	}
	for ( int32_t i = height-1; i >= 0; i-- )
	{
		std::getline(mapLoad,line,'\n');
		line_stream.clear();
		line_stream.str(line);
		for ( int32_t j = 0; j < width; j++ )
		{
			
			std::getline(line_stream,num,',');
			int32_t mapChip;
			mapChip = static_cast< int32_t >( std::atof(num.c_str()) );

			blocks[ i ][ j ].SetDrawNum(mapChip);
		}
	}



	//移動制限の数値設定
	moveLimitW = static_cast<float>(width) * scaleEnd.x;
	moveLimitH = static_cast<float>(height) * scaleEnd.z;
	mapScaleW = scaleEnd.x;
	mapScaleH = scaleEnd.z;
}

void Map::RandomCreate()
{
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());


	//std::uniform_real_distribution<float> rotDist(-shift, shift);
	std::uniform_int_distribution<int16_t> Dist(7, 17);
	//ブロックのサイズを設定

	height = Dist(engine);
	width = Dist(engine);
	numW = (width / 2);
	numH = (height / 2);

	// 二次元配列のサイズを初期化
	blocks.resize(height);
	for (int i = 0; i < height; ++i) {
		blocks[i].resize(width);
	}

	//パラメータの組み込み
	for (size_t i = 0; i < height; i++)
	{
		if (i % 2 == 0)
		{
			change = false;
		}
		else
		{
			change = true;
		}
		for (size_t j = 0; j < width; j++)
		{
			//パラメータをセット
			Vector3	scale, pos, rot;
			if (isStaging)

			scale = scaleStart;
			pos.y = posStartY;
			
			pos.z = (i * scaleEnd.z) * 2.0f - (scaleEnd.z * (height - 1));
			pos.x = (j * scaleEnd.x) * 2.0f - (scaleEnd.x * (width - 1));

			rot = { 0.0f,0.0f,rotStartZ };
			//オブジェクトにパラメータをセット
			blocks[i][j].Initialize(pos, rot, scale, model);
			if (change)
			{
				blocks[i][j].SetColor({ 0.7f,0.7f, 0.7f, 1.0f });
			}
			change ^= 1;
			//最初なので全部1
			blocks[i][j].SetDrawNum(1);
		}
	}

	//移動制限の数値設定
	moveLimitW = static_cast<float>(width) * scaleEnd.x;
	moveLimitH = static_cast<float>(height) * scaleEnd.z;
	mapScaleW = scaleEnd.x;
	mapScaleH = scaleEnd.z;
}

void Map::Preparation() {
	if (isStaging&&!flag)
	{
		provisionTime--;
		if (provisionTime < 0)
		{
			provisionTime = provisionTimer;
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
						if (!blocks[h][w].GetIsStaging())
						{
							blocks[h][w].SetIsStaging(true);
						}

					}
				}
				nowMax++;
			}
		}
	}
}

void Map::Staging(size_t y_, size_t x_) {
	float time = blocks[ y_ ][ x_ ].GetTime();
	//フラグが立っていたら
	if (blocks[y_][x_].GetIsStaging() &&isStaging&&!flag)
	{
		//スケール
		Vector3 easeScale = blocks[y_][x_].GetScale();
		easeScale = scaleStart + (scaleEnd - scaleStart) * Easing::easeOutCubic(time/ stagingTimer);
		//回転
		Vector3 easeRot = blocks[y_][x_].GetRot();
		easeRot.z = rotStartZ + (rotEndZ - rotStartZ) * Easing::easeOutSine(time/ stagingTimer);
		//座標
		Vector3 easePos = blocks[y_][x_].GetPos();
		easePos.y = posStartY + (posEndY - posStartY) * Easing::easeOutCubic(time/ stagingTimer);

		blocks[ y_ ][ x_ ].SetParameter(easePos,easeRot,easeScale);
		//1.0fまで加算
		if ( time < stagingTimer )
		{
			time++;
			blocks[ y_ ][ x_ ].SetTime(time);
		}
		else
		{
			blocks[y_][x_].SetIsStaging(false);
			if ( x_==0||x_==width-1 )
			{
				if ( y_ == 0 || y_ == height - 1 )
				{
					count++;
					if ( count>=4 )
					{
						flag = true;
					}
				}
			}
		}
	}
	

}