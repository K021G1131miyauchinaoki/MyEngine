cbuffer CounstbufferData:register(b0)
{
	float4 color;//�F
	matrix mat;//3D�ϊ��s��
}

//���_�V�F�[�_�[�̏o�͍\����
//���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����
struct VSOutput
{
	float4	svpos : SV_POSITION;//�V�X�e���p���_���W
	//float3 normal:Normal;//�@���x�N�g��
	float4 color : COLOR;//�F
	float2	uv	:TEXCOORD;//uv�l
};