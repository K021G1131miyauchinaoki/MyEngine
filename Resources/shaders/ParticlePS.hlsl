#include "Particle.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

//float4 main(VSOutput input) : SV_TARGET
float4 main(GSOutput input) : SV_TARGET
{

	float4 texcolor= tex.Sample(smp,input.uv);
    float4 color= {input.color.rgb * texcolor.rgb,input.color.a};
    return color;
}