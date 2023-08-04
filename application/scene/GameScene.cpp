#include "GameScene.h"
#include<MyMath.h>
#include<EnemyBullet.h>
#include<Bullet.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void GameScene::Initialize() {
	winApp = new	WinApp;
	winApp->Initialize();

	dxCommon = new DirectXCommon;
	dxCommon->Initialize(winApp);

	input = new	Input;
	input->Initialize(winApp);
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	Model::SetDevice(dxCommon->GetDevice());
	//�J����
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,80,-20 });
	camera->Update();
	//�f�o�C�X���Z�b�g
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::width, WinApp::height, camera.get());
	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	// �J�������Z�b�g
	FbxObject3d::SetCamera(camera.get());
	//�O���t�B�b�N�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
	
	//---------------------------2D----------------------------------


	//---------------------------3D----------------------------------


	/*---------------- - FBX------------------------*/
	////3D�I�u�W�F�N�g�����ƃ��f���Z�b�g
	//fbxM = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	//fbxObj = new FbxObject3d;
	//fbxObj->Initialize();
	//fbxObj->SetModel(fbxM);
	//fbxObj->PlayAnimation();

	// ���f���ǂݍ���
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
	modelMap.reset(Model::LoadFromOBJ("map"));

	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());

	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });


	aim = std::make_unique<Aimposition>();
	aim->Initialeze(cube.get(), input);
	
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input, aim.get());

	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(tank.get(), player.get());

	//�|�X�g�G�t�F�N�g
	PostEffect::StaticInitialize(dxCommon);
	//pe = new PostEffect;
	//pe->Initialize();
	ImgM = std::make_unique<ImguiManager>();
	ImgM->Initialize(winApp, dxCommon);
	//�}�b�v
	map = std::make_unique<Map>();
	map->Initialize(modelMap.get());

}

void GameScene::Update(){
#pragma region ���b�Z�[�W
	if (winApp->ProcessMessage()|| input->PushKey(DIK_ESCAPE)) {
		isEndRequst = true;
	}
#pragma endregion
	input->Update();
	//imgui
	// �f��
	//ImGui::ShowDemoWindow();
	
	//------------------------------
	CheckAllCollision();

	camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
	camera->SetEye({ player->GetPos().x, 100, player->GetPos().z - 30 });
	camera->Update();
	player->Updata();
	enemy->Updata();
	aim->Updata();
	map->Updata();
	objSkydome->Update();
	float vec[2] = { input->GetPos().x,input->GetPos().y};
	float posA[2] = { aim->GetPosition().x,aim->GetPosition().z };
	//imgui�֘A
	ImgM->Begin();
	//�������璆�g�������Ă���
	ImGui::Begin("a");
	ImGui::SliderFloat2("mousePos", vec, 0.0f, static_cast<float>(WinApp::width));
	ImGui::End();
	//ImGui::SliderFloat2("pos",posA,0.0f, WinApp::width);
	//fbxObj->Update();

	//for (auto object : objects) {
	//	object->Update();
	//}
}

void GameScene::Draw(){
	//Direct���t���[�������@��������
	dxCommon->PreDraw();
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon->GetCommandList());
	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>
	enemy->Draw();
	objSkydome->Draw();
	player->Draw();
	aim->Draw();
	map->Draw();

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	//imgui
	ImgM->End();
	ImgM->Draw();
	//pe->Draw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

bool GameScene::IsEndRequst(){
	return isEndRequst;
}

void GameScene::Finalize(){
	//�������
	winApp->Finalize();
	
	FbxLoader::GetInstance()->Finalize();
	//safe_delete(fbxObj);
	//safe_delete(fbxM);
	Bullet::Finalize();
	EnemyBullet::Finalize();

	delete	input;
	delete winApp;
	delete	dxCommon;
}

void GameScene::CheckAllCollision() {
	//����Ώ�A��B�̍��W
	Vector3 posA, posB;

	//���e���X�g���擾
	const std::list<std::unique_ptr<Bullet>>& playerBullets = player->GetBullets();
	//�G�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
#pragma	region	���L�����ƓG�e�̓����蔻��
	//���L�����̍��W
	posA = player->GetPos();
	//���L�����ƓG�e�S�Ă̓����蔻��
	for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
		//�G�e�̍��W
		posB = e_bullet->GetBulletPosition();
		//A,B�̋���
		Vector3 vecPos = MyMath::lens(posA, posB);
		float dis = MyMath::length(vecPos);
		//
		float	radius = player->GetRadius() + e_bullet->GetRadius();
		//����
		if (dis <= radius) {
			//���L�����̃R�[���o�b�N���Ăяo��
			player->OnCollision();
			//�G�e�̃R�[���o�b�N���Ăяo��
			e_bullet->OnCollision();
		}
	}
#pragma	endregion

#pragma region ���e�ƓG�L�����̓����蔻��
	//�G�e�̍��W
	posA = enemy->GetPos();
	//�G�L�����Ǝ��e�S�Ă̓����蔻��
	for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
		//���e�̍��W
		posB = p_bullet->GetBulletPosition();
		// A,B�̋���
		Vector3 vecPos = MyMath::lens(posA, posB);
		float dis = MyMath::length(vecPos);
		//
		float radius = enemy->GetRadius() + p_bullet->GetRadius();
		//����
		if (dis <= radius) {
			//�G�L�����̃R�[���o�b�N���Ăяo��
			enemy->OnCollision();
			//���e�̃R�[���o�b�N���Ăяo��
			p_bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region ���e�ƓG�e�̓����蔻��
	//���e�̍��W
	for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
		posA = p_bullet->GetBulletPosition();
		//���e�ƓG�e�S�Ă̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
			//�G�e�̍��W
			posB = e_bullet->GetBulletPosition();
			// A,B�̋���
			Vector3 vecPos = MyMath::lens(posA, posB);
			float dis = MyMath::length(vecPos);
			//
			float radius = e_bullet->GetRadius() + p_bullet->GetRadius();
			//����
			if (dis <= radius) {
				//���e�̃R�[���o�b�N���Ăяo��
				p_bullet->OnCollision();
				//�G�e�̃R�[���o�b�N���Ăяo��
				e_bullet->OnCollision();
			}
		}
	}
#pragma endregion
}