
/**
 * @file Map.cpp
 * @brief ステージ
 */

#include "Map.h"
#include<MyMath.h>	
#include<Easing.h>
#include<sstream>
#include<random>
#include<GamePlayScene.h>

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
	totalCount = 0;
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
	isPreparation = true;
}

void Map::Update() {
	Preparation();
	count = 0;
	for (size_t i = 0; i < height; i++)
	{

		for (size_t j = 0; j < width; j++)
		{
			StartStaging(i,j);
			OutStaging(i, j);
			if ((posEndY-plus<=blocks[ i ][ j ].GetPos().y&& !blocks[i][j].GetIsStaging()&&isPreparation)||//スタート演出
				( posStartY+plus >= blocks[ i ][ j ].GetPos().y && GamePlayScene::isOut && GamePlayScene::outCount == GamePlayScene::FallMap ) )//落ちる演出
			{
				count++;
				if (count >= totalCount-1&& GamePlayScene::outCount == GamePlayScene::FallMap )
				{
					GamePlayScene::outCount++;
				}
				if ( count >= totalCount )
				{
					if ( GamePlayScene::outCount == GamePlayScene::Up )
					{
						GamePlayScene::outCount++;
					}
					isPreparation = false;
					Reset();
				}
			}

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

	//ブロックの総数を数える
	totalCount = 0;
	for ( const auto& innerVec : blocks )
	{
		totalCount += innerVec.size();
	}
}

void Map::RandomCreate()
{
	for (auto& innerVec : blocks )
	{
		innerVec.clear();
	}
	blocks.clear();
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());


	//std::uniform_real_distribution<float> rotDist(-shift, shift);
	std::uniform_int_distribution<int16_t> Dist(6,10);
	//ブロックのサイズを設定

	height = Dist(engine);
	width = Dist(engine);

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

	//ブロックの総数を数える
	totalCount = 0;
	for ( const auto& innerVec : blocks )
	{
		totalCount += innerVec.size();
	}
}

void Map::CenterMapChip(const Vector3& playerPos_)
{
	//プレイヤーがマップ上のどの位置にいるかを調べる
	float diameterW = mapScaleW * 2.0f;
	float diameterH = mapScaleH * 2.0f;
	
	centerH = static_cast< int16_t >( ( playerPos_.z + moveLimitH ) / diameterH );
	centerW = static_cast< int16_t >( ( playerPos_.x + moveLimitW ) / diameterW );

	if ( GamePlayScene::isOut )
	{
		Vector3 pos = blocks[ centerH ][ centerW ].GetPos();
		pos.y = posEndY;
		blocks[ centerH ][ centerW ].SetTime(stagingTimer);
		blocks[ centerH ][ centerW ].SetPos(pos);
		blocks[ centerH ][ centerW ].SetRot({rotEndZ,rotEndZ, rotEndZ});
		blocks[ centerH ][ centerW ].SetScale(scaleEnd);
		blocks[ centerH ][ centerW ].Update();
	}
	//フラグをオンに
	if ( GamePlayScene::outCount==GamePlayScene::Create )
	{
		isStaging = true;
		isPreparation = true;
	}
	nowMax = 0;
}

void Map::Preparation() {
	if ((isStaging&&isPreparation)||
		(GamePlayScene::isOut&&GamePlayScene::outCount==GamePlayScene::FallMap))
	{
		provisionTime--;
		if (provisionTime < 0)
		{
			provisionTime = provisionTimer;
			if (nowMax <width || nowMax <height)
			{
				setPoint = -nowMax;

				for (int16_t i = setPoint; i <= nowMax; i++)
				{
					for (int16_t j = setPoint; j <= nowMax; j++)
					{
						int16_t h = centerH + i;
						int16_t w = centerW + j;

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

void Map::StartStaging(size_t y_, size_t x_) {
	float time = blocks[ y_ ][ x_ ].GetTime();
	//フラグが立っていたら
	if (blocks[y_][x_].GetIsStaging() &&isStaging&&isPreparation)
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
		if ( time <= stagingTimer )
		{
			time++;
			blocks[ y_ ][ x_ ].SetTime(time);
		}
		else
		{
			//フラグの変更
			blocks[y_][x_].SetIsStaging(false);
		}
	}
}

void Map::OutStaging(size_t y_,size_t x_) {
	float time = blocks[ y_ ][ x_ ].GetTime();
	bool flag = centerH == y_ && centerW == x_;
	//フラグが立っていたら
	if ( blocks[ y_ ][ x_ ].GetIsStaging() &&
		GamePlayScene::isOut &&
		GamePlayScene::outCount == GamePlayScene::FallMap &&
		 !flag)
	{
		//スケール
		Vector3 easeScale = blocks[ y_ ][ x_ ].GetScale();
		easeScale = scaleEnd + ( scaleStart - scaleEnd  ) * Easing::easeInQuint(time / stagingTimer);
		//回転
		Vector3 easeRot = blocks[ y_ ][ x_ ].GetRot();
		easeRot.z = rotEndZ + ( rotStartZ - rotEndZ ) * Easing::easeInSine(time / stagingTimer);
		//座標
		Vector3 easePos = blocks[ y_ ][ x_ ].GetPos();
		easePos.y = posEndY + ( posStartY - posEndY ) * Easing::easeInSine(time / stagingTimer);

		blocks[ y_ ][ x_ ].SetParameter(easePos,easeRot,easeScale);
		//1.0fまで加算
		if ( time <= stagingTimer )
		{
			time++;
			blocks[ y_ ][ x_ ].SetTime(time);
		}
		else
		{
			blocks[ y_ ][ x_ ].SetIsStaging(false);
		}
	}
}

void Map::Reset()
{
	//変数
	nowMax = 0;

	for ( size_t i = 0; i < height; i++ )
	{
		for ( size_t j = 0; j < width; j++ )
		{
			blocks[i][j].SetTime(0);//タイマー	
		}
	}
}
