AppendStructuredBuffer<uint>freeList:register(u0);

cbuffer emitData : register(b0)
{
	//座標
    float3 pos;
	//寿命
    float lifeTime;
	//加速度
    float3 accel;
	//スケール
    float scale;
	//色
    float4 startColor;
	//終了色
    float4 endColor;
	//経過時間
    float deltaTime;

    uint maxParticle;
}

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x > maxParticle)
    {
        return;
    }
    freeList.Append(DTid.x);
}