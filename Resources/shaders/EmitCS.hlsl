#include"Math.hlsli"

cbuffer EmitData : register(b1)
{
    //座標
    float3 pos;
		//寿命
    float lifeTime;
    float3 velocity;
		//加速度
    float3 accel;
		//スケール
    float scale = 1.0f;
		//色
    float4 startColor = { 1, 1, 1, 1 };
		//終了色
    float4 endColor = { 1, 1, 1, 1 };
		//経過時間
    float deltaTime;

    int maxParticle;
}

struct ParticleData
{
    float4 color; // 色 (RGBA)
    float3 position; // xyz座標
    float age;
    float3 vec;
    float isAlive;
    float3 accel;
    float size; //スケール
    float lifeTimer;
    float3 pad;
};

RWStructuredBuffer<ParticleData> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> freeList : register(u1);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= maxParticle)
    {
        return;
    }
    uint emitIndex = freeList.Consume();
    ParticleData particleData = ParticlePool.Load(emitIndex);
    
    float3 rnd = RandVec3((int) emitIndex, 1, 0);
    
    particleData.position = (rnd * particleData.position);
    particleData.vec = (rnd * particleData.vec);
    particleData.color = particleData.color;
    particleData.accel = particleData.accel;
    particleData.age = 0.0f;
    particleData.size = 0.5f;
    particleData.isAlive = 1.0f;
    particleData.lifeTimer = particleData.lifeTimer;
    
    ParticlePool[emitIndex] = particleData;
}