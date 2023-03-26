cbuffer cbuff0 : register(b0)
{
	//float4 color; // 色(RGBA)
	matrix mat; // ３Ｄ変換行列
	matrix	matBillboard;//ビルボード行列
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : SV_POSITION; // 頂点座標
	float	scale : TEXCOORD;//スケール
	//float3 normal :NORMAL; // 法線ベクトル
	//float2 uv  :TEXCOORD; // uv値
};

//ジオメトリシェーダ
struct GSOutput
{
	float4 svpos : SV_POSITION;//システム用長点座標
	//float3	normal : NORMAL;//法線ベクトル
	float2 uv:TEXCOORD;//uv値

};