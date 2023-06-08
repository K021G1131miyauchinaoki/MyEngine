cbuffer CounstbufferData:register(b0)
{
	float4 color;//色
	matrix mat;//3D変換行列
}

//頂点シェーダーの出力構造体
//頂点シェーダーからピクセルシェーダーへのやり取りに使用する
struct VSOutput
{
	float4	svpos : SV_POSITION;//システム用頂点座標
	//float3 normal:Normal;//法線ベクトル
	float4 color : COLOR;//色
	float2	uv	:TEXCOORD;//uv値
};