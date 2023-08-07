#include "Map.h"
#include<MyMath.h>	
#include<Easing.h>


const int8_t Map::heith;
const int8_t Map::width;

Map::~Map(){}

void Map::Initialize(Model*model_) {
	//変数の初期化
	num = -0;
	//演出用
	nowMax = 0;
	numW = width / 2;
	numH = heith / 2;
	scale += 10.0f;
	endFrame = 1000;
	
	startY = -(scale.y + 0.2f) + constStartY;
	endY =  -(scale.y + 0.2f);
	//scale.y = 1.0f;
	for (size_t i = 0; i < heith; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			//
			
			Vector3 pos = {0.0f,startY, 0.0f};
			pos.x = (j * scale.x) * 2.0f - (scale.x * width);
			pos.z = (i * scale.z) * 2.0f - (scale.z * heith);
			//オブジェクトにパラメータをセット
			block[i][j].obj = std::make_unique<Object3d>();
			block[i][j].obj->Initialize();
			block[i][j].obj->SetModel(model_);
			block[i][j].obj->SetPosition(pos);
			block[i][j].obj->SetScale(scale);
			//block[i][j].obj->SetColor({ 0.0f, 0.0f, 0.1f,1.0f});
			//block[i][j].pos = pos;
			block[i][j].frame = 0;
			block[i][j].obj->Update();
		}
	}
}

void Map::Updata() {
	timer--;
	if (timer < 0)
	{
		timer = time;
		if (nowMax <= numW || nowMax <= numH)
		{
			setPoint = -nowMax;
			nowMax++;
			
			for (size_t i = setPoint; i < nowMax; i++)
			{

				for (size_t j = setPoint; j < nowMax; j++)
				{

					//要素が0以上、決めた値より小さい場合
					if ((numH + setPoint) >= 0 && (numH + setPoint) < heith && (numW + setPoint) >= 0 && (numW + setPoint) <= width)
					{
						//フラグが立っていない場合
						if (!block[numH + setPoint][numW + setPoint].isUp)
						{
							block[numH + setPoint][numW + setPoint].isUp = true;
						}
					}

				}
			}
		}
	}
	for (size_t i = 0; i < heith; i++)
	{

		for (size_t j = 0; j < width; j++)
		{

			//フラグが立っていたら
			if (block[i][j].isUp)
			{
				Vector3 easeVec = block[i][j].obj->GetPosition();
				float a = Easing::easeOutSine(block[i][j].frame / endFrame);
				easeVec.y = startY + (endY - startY) * Easing::easeOutCubic(block[i][j].y);
				block[i][j].y+=0.02f;
				block[i][j].obj->SetPosition(easeVec);
			}
			if (block[i][j].y >= 1.0f)
			{
				block[i][j].isUp = false;
			}
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
