#pragma once
class Framework
{
public://�����o�֐�
	virtual ~Framework() = default;
	//������
	virtual void Initialize();
	//�I��
	virtual	void Finalize();
	//�X�V
	virtual	void Update();
	//�`��
	virtual	void Draw() = 0;
	//���s
	void Run();
	//�I���`�F�b�N
	virtual	bool IsEndRequst() { return isEndRequst; }
private:
	//�I���t���O
	bool isEndRequst = false;
};

