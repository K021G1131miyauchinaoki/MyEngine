#include "Particle.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

//float4 main(VSOutput input) : SV_TARGET
float4 main(GSOutput input) : SV_TARGET
{

	return tex.Sample(smp,input.uv);
//float3 light = normalize(float3(1,-1,1)); // �E�����@�����̃��C�g
//float diffuse = saturate(dot(-light, input.normal));
//float brightness = diffuse + 0.3f;
//float4 texcolor = tex.Sample(smp, input.uv) * color;
//return float4(texcolor.rgb * brightness, texcolor.a);
//return float4(1, 1, 1, 1);
}