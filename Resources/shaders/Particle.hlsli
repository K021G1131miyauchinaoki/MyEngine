cbuffer cbuff0 : register(b0)
{
	//float4 color; // �F(RGBA)
	matrix mat; // �R�c�ϊ��s��
	matrix	matBillboard;//�r���{�[�h�s��
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 pos : SV_POSITION; // ���_���W
	float	scale : TEXCOORD;//�X�P�[��
	//float3 normal :NORMAL; // �@���x�N�g��
	//float2 uv  :TEXCOORD; // uv�l
};

//�W�I���g���V�F�[�_
struct GSOutput
{
	float4 svpos : SV_POSITION;//�V�X�e���p���_���W
	//float3	normal : NORMAL;//�@���x�N�g��
	float2 uv:TEXCOORD;//uv�l

};