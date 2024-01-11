/**
 * @file BaseScene.cpp
 * @brief シーン基盤
 */

#include "BaseScene.h"
#include<SoundManager.h>
using namespace MyEngin;

void BaseScene::Initialize(){}

void BaseScene::Finalize(){}

void BaseScene::Update(){}

void BaseScene::ObjDraw(){}
void BaseScene::SpriteDraw() {}
void BaseScene::GeometryDraw(){}
void BaseScene::DecisionSound() {
	SoundManager::GetInstance()->PlayWave("SE/decision.wav");
}
