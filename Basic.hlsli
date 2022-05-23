cbuffer	ConstBufferDataMaterial:register(b0)
{
	float4	color;//RGBA
}

//頂点シェーダーの出力構造体
//(頂点シェーダーからピクセルシェーダーへのやり取りに使用)
struct VSOutput
{
	//システム用頂点座標
	float4	svpos	:SV_POSITION;
	//uv座標
	float2	uv		:TEXCOORD;
};