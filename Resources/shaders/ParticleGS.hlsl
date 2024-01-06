#include"Particle.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0,0),//����
	float4(-0.5f,+0.5f,0,0),//����
	float4(+0.5f,-0.5f,0,0),//�E��
	float4(+0.5f,+0.5f,0,0),//�E��
};

//���オ0,0�@�E����1,1
static const float2 uv_array[vnum] =
{
	float2(0,1),//����
	float2(0,0),//����
	float2(1,1),//�E��
	float2(1,0),//�E��
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput elemdent;
	//4�_���܂킷
	for (uint i = 0; i < vnum; i++) {
		//���S����̃I�t�Z�b�g���X�P�[�����O
		float4	offset = offset_array[i] * input[0].scale;
		//���S����̃I�t�Z�b�g���r���{�[�h��]�i���f�����W�j
		offset = mul(matBillboard, offset);
		//�I�t�Z�b�g�����炷�i���[���h���W�j
		elemdent.svpos = input[0].pos + offset;
		//�r���[�A�ˉe�ϊ�
		elemdent.svpos = mul(mat, elemdent.svpos);
		//elemdent.uv = float2(0.5f, 0.5f);
		elemdent.uv = uv_array[i];
		//�F
        elemdent.color = input[0].color;
		output.Append(elemdent);
	}

}