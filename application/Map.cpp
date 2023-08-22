#include "Map.h"
#include<MyMath.h>	
#include<Easing.h>
#include<sstream>

std::unique_ptr < Model> Map::model;

Map::~Map(){}

void Map::StaticInitialize(Model* model_) {
	if (model == nullptr)
	{
		model.reset(model_);
	}
}

void Map::Initialize() {
	//�ϐ��̏�����
	nowMax = 0;
	//�X�P�[��
	scaleEnd += 10.0f;
	scaleStart += 0.1f;
	endFrame = 100;
	//�ʒu
	posStartY = -(scaleEnd.y + 0.2f) + constStartY;
	posEndY =  -(scaleEnd.y + 0.2f);
	//��]
	rotEndZ = 0;
	rotStartZ=360*2;
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
					//�t���O�������Ă��Ȃ��ꍇ
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

			//�t���O�������Ă�����
			if (blocks[i][j].isUp)
			{
				//�X�P�[��
				Vector3 easeScale = blocks[i][j].obj->GetScale();
				easeScale = scaleStart + (scaleEnd - scaleStart) * Easing::easeOutCubic(blocks[i][j].range);
				blocks[i][j].obj->SetScale(easeScale);
				//��]
				Vector3 easeRot = blocks[i][j].obj->GetRotation();
				easeRot.z = rotStartZ + (rotEndZ - rotStartZ) * Easing::easeOutSine(blocks[i][j].range);
				blocks[i][j].obj->SetRotation(easeRot);
				//���W
				Vector3 easePos = blocks[i][j].obj->GetPosition();
				easePos.y = posStartY + (posEndY - posStartY) * Easing::easeOutCubic(blocks[i][j].range);
				blocks[i][j].obj->SetPosition(easePos);
				//1.0f�܂ŉ��Z
				blocks[i][j].range+=0.02f;
			}
			if (blocks[i][j].range >= 1.0f)
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
	//�X�e�[�W�̓ǂݍ���
	std::stringstream mapLoad;
	//�t�@�C�����J��
	const std::string filename = "Resources/CSV/" + num_ + ".csv";
	std::ifstream file;
	file.open(filename);
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	mapLoad << file.rdbuf();

	//�t�@�C�������
	file.close();

	//1�s���̕����������ϐ�
	std::string line;

	//�R�}���h���s���[�v
	std::getline(mapLoad, line, '\n');

	std::istringstream line_stream(line);
	std::string num;
	

	std::getline(line_stream, num, ',');
	high = (int8_t)std::atof(num.c_str());
	std::getline(line_stream, num, ',');
	width = (int8_t)std::atof(num.c_str());
	numW = (width / 2);
	numH = (high / 2);
	
	
	
	// �񎟌��z��̃T�C�Y��������
	blocks.resize(high);
	for (int i = 0; i < high; ++i) {
		blocks[i].resize(width);
	}

	//0,1�̃}�b�v�`�b�v�𖄂ߍ��݂i���j������
	for (size_t i = 0; i < high; i++)
	{
		std::getline(mapLoad, line, '\n');

		for (size_t j = 0; j < width; j++)
		{
			//

			Vector3 pos = { 0.0f,posStartY, 0.0f };
			pos.x = (j * scaleEnd.x) * 2.0f - (scaleEnd.x * width);
			pos.z = (i * scaleEnd.z) * 2.0f - (scaleEnd.z * high);
			//�I�u�W�F�N�g�Ƀp�����[�^���Z�b�g
			blocks[i][j].obj = std::make_unique<Object3d>();
			blocks[i][j].obj->Initialize();
			blocks[i][j].obj->SetModel(model.get());
			blocks[i][j].obj->SetScale(scaleStart); 
			blocks[i][j].obj->SetRotation({0.0f,0.0f,rotStartZ});
			blocks[i][j].obj->SetPosition(pos);
			//blocks[i][j].obj->SetColor({ 0.0f, 0.0f, 0.1f,1.0f});
			//blocks[i][j].pos = pos;
			blocks[i][j].frame = 0;
			blocks[i][j].obj->Update();
		}
	}
}


//�I��
void Map::Finalize() {
	model.release();
}