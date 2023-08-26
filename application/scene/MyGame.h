#pragma once
#include<vector>
#include"Framework.h"
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include<ImguiManager.h>
#include<BaseScene.h>

class MyGame:public Framework
{
public://�����o�֐�
	//������
	void Initialize()override;
	//�I��
	void Finalize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw()override;

private:
	//imgui
	std::unique_ptr<ImguiManager> ImgM;
};

