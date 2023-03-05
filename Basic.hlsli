cbuffer	ConstBufferDataMaterial:register(b0)
{
	float4	color;//RGBA
}

cbuffer	ConstBufferDataTransform : register(b1)
{
	matrix	mat;//３D変換行列
}

//頂点シェーダーの出力構造体
//(頂点シェーダーからピクセルシェーダーへのやり取りに使用)
struct VSOutput
{
	//システム用頂点座標
	float4	svpos	:SV_POSITION;
	//法線ベクトル
	float3	normal	:NORMAL;
	//uv座標
	float2	uv		:TEXCOORD;
};