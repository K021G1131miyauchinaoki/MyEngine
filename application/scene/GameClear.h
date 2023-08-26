#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<Player.h>
#include<ImguiManager.h>
#include<Enemy.h>
#include<Map.h>
#include<BaseScene.h>

class GameClear :public BaseScene
{
public://�����o�֐�

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void ObjDraw()override;
	void SpriteDraw()override;

	//�I��
	void Finalize()override;

	GameClear();
	~GameClear()override;

private:
	std::unique_ptr<Sprite> clearSprite;
	std::unique_ptr <Input>input;

};