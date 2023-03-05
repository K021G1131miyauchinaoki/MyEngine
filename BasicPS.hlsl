#include"Basic.hlsli"

Texture2D<float4>		tex:register(t0);
SamplerState	smp : register(s0);

float4 main(VSOutput	input) : SV_TARGET
{
	//テクスチャ
	float4 texcolor = float4(tex.Sample(smp,input.uv));

	float3 light = normalize(float3(1,-1,1));//右下奥　向きのライト
	//環境光
	float	diffuse = saturate(dot(-light, input.normal));//diffuseを[0，1]の範囲にClampする
	float	brightness = diffuse + 0.3f;
	//拡散反射光
	//float	brightness = dot(-light, input.normal);//光源へのベクトルと法線ベクトルの内積
	
	return	float4(texcolor.rgb*brightness, texcolor.a)*color;//輝度をテクスチャに代入
	//return	float4(brightness, brightness, brightness, 1);//輝度をRGBに代入
	//return float4(input.normal,1);//RGBをそれぞれ法線のXYZAを1で出力
	//return float4(tex.Sample(smp,input.uv))*color;
	//return	float4(1,1,1,1);
}