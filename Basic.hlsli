cbuffer	ConstBufferDataMaterial:register(b0)
{
	float4	color;//RGBA
}

cbuffer	ConstBufferDataTransform : register(b1)
{
	matrix	mat;//�RD�ϊ��s��
}

//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p)
struct VSOutput
{
	//�V�X�e���p���_���W
	float4	svpos	:SV_POSITION;
	//�@���x�N�g��
	float3	normal	:NORMAL;
	//uv���W
	float2	uv		:TEXCOORD;
};