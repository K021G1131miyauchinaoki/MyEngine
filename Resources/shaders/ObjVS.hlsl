#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
	
	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mul(viewproj, world),pos);
	//�����ˌ�
    float3 ambient = color.rgb *m_ambient;
	//�g�U���ˌ�
    float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;
	//����x
    const float shininess = 1000.0f;
	//���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - wpos.xyz);
	//���ˌ��x�N�g��
    float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);
	//���ʔ��ˌ�
    float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	
	//output.normal = normal;
    output.color.rbg = (ambient + diffuse + specular) * lightcolor;
    output.color.a = color.a*m_alpha;
	output.uv = uv;
	return output;
}