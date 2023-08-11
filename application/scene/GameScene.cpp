#include "GameScene.h"
#include<MyMath.h>
#include<EnemyBullet.h>
#include<Bullet.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void GameScene::Initialize() {
	Framework::Initialize();
	/*�ϐ�*/
	mapStratY = -50;
	//�J����
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	Object3d::SetCamera(camera.get());
	

	// ���f���ǂݍ���
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
	modelMap.reset(Model::LoadFromOBJ("map"));

	//���f���̃Z�b�g
	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());
	Map::StaticInitialize(modelMap.get());


	//�X�J�C�h�[��
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });

	//���N�e�B��
	aim = std::make_unique<Aimposition>();
	aim->Initialeze(cube.get(), input.get());
	//�v���C���[
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input.get(), aim.get());
	//�G�l�~�[
	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(tank.get(), player.get());

	
	//pe = new PostEffect;
	//pe->Initialize();
	ImgM = std::make_unique<ImguiManager>();
	ImgM->Initialize(winApp.get(), dxCommon.get());
	//�}�b�v
	map = std::make_unique<Map>(mapStratY);
	map->Initialize();
	map->LoadCSV("1");

}

void GameScene::Update(){
	Framework::Update();
	
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
	//imgui�֘A
	ImgM->Begin();
	//�������璆�g�������Ă���
	ImGui::Begin("a");
	ImGui::SliderFloat2("mousePos", vec, -100.0f, static_cast<float>(WinApp::width));
	ImGui::End();
}

void GameScene::Draw(){
	//PostEffect::PostDrawScene(dxCommon->GetCommandList());
	//PostEffect::PreDrawScene(dxCommon->GetCommandList());

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

void GameScene::Finalize(){
	Bullet::Finalize();
	EnemyBullet::Finalize();
	Map::Finalize();
	Framework::Finalize();
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