#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<Player.h>
#include<Aimposition.h>
#include<ImguiManager.h>
#include<Enemy.h>
#include<Map.h>
#include<BaseScene.h>

class TitleScene:public BaseScene
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

private:
	std::unique_ptr<Sprite> titleSprite;
	std::unique_ptr <Input>input;

};

