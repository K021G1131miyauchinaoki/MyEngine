#include "Particle.hlsli"

//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
VSOutput main(float4 pos : POSITION, float scale : TEXCOORD,float4 color:COLOR)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = pos;
	output.scale = scale;
    output.color = color;
	return output;
	//output.svpos = mul(mat, pos);
   //output.normal = normal;
   //output.uv = uv;
}