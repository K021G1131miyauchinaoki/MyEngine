#include"Basic.hlsli"

Texture2D<float4>		tex:register(t0);
SamplerState	smp : register(s0);

float4 main(VSOutput	input) : SV_TARGET
{
	//�e�N�X�`��
	float4 texcolor = float4(tex.Sample(smp,input.uv));

	float3 light = normalize(float3(1,-1,1));//�E�����@�����̃��C�g
	//����
	float	diffuse = saturate(dot(-light, input.normal));//diffuse��[0�C1]�͈̔͂�Clamp����
	float	brightness = diffuse + 0.3f;
	//�g�U���ˌ�
	//float	brightness = dot(-light, input.normal);//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	
	return	float4(texcolor.rgb*brightness, texcolor.a)*color;//�P�x���e�N�X�`���ɑ��
	//return	float4(brightness, brightness, brightness, 1);//�P�x��RGB�ɑ��
	//return float4(input.normal,1);//RGB�����ꂼ��@����XYZA��1�ŏo��
	//return float4(tex.Sample(smp,input.uv))*color;
	//return	float4(1,1,1,1);
}