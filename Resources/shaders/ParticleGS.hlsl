#include"Particle.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0,0),//左下
	float4(-0.5f,+0.5f,0,0),//左上
	float4(+0.5f,-0.5f,0,0),//右下
	float4(+0.5f,+0.5f,0,0),//右上
};

//左上が0,0　右下が1,1
static const float2 uv_array[vnum] =
{
	float2(0,1),//左下
	float2(0,0),//左上
	float2(1,1),//右下
	float2(1,0),//右上
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput elemdent;
	//4点分まわす
	for (uint i = 0; i < vnum; i++) {
		//中心からのオフセットをスケーリング
		float4	offset = offset_array[i] * input[0].scale;
		//中心からのオフセットをビルボード回転（モデル座標）
		offset = mul(matBillboard, offset);
		//オフセット分ずらす（ワールド座標）
		elemdent.svpos = input[0].pos + offset;
		//ビュー、射影変換
		elemdent.svpos = mul(mat, elemdent.svpos);
		//elemdent.uv = float2(0.5f, 0.5f);
		elemdent.uv = uv_array[i];
		//色
        elemdent.color = input[0].color;
		output.Append(elemdent);
	}

}