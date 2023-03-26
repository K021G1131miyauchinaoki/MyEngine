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
		output.Append(elemdent);
	}


	////����
	//elemdent.normal = input[0].normal;
	//elemdent.uv = input[0].uv;
	////1�_��
	//elemdent.svpos = input[0].svpos;
	//output.Append(elemdent);
	////2�_��
	//elemdent.svpos = input[0].svpos+float4(10.0f,10.0f,0,0);
	//output.Append(elemdent);
	////3�_��
	//elemdent.svpos = input[0].svpos + float4(10.0f, 0, 0, 0);
	//output.Append(elemdent);
	//for (uint i = 0; i < 3; i++)
	//{
	//}
	//���݂̃X�g���b�v���I��
	//output.RestartStrip();

	/*for (uint i = 0; i < 3; i++)
	{
		GSOutput elemdent;
		elemdent.svpos = input[i].svpos+float4(20.0f,0.0f,0.0f,0.0f);
		elemdent.normal = input[i].normal;
		elemdent.uv = input[i].uv * 5.0f;
		output.Append(elemdent);
	}*/

}