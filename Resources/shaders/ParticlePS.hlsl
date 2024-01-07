#include "Particle.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

//float4 main(VSOutput input) : SV_TARGET
float4 main(GSOutput input) : SV_TARGET
{

	float4 texcolor= tex.Sample(smp,input.uv);
    return input.color*texcolor;
}