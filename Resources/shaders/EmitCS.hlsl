#include"Math.hlsli"

cbuffer EmitData : register(b1)
{
    //���W
    float3 pos;
		//����
    float lifeTime;
    float3 velocity;
		//�����x
    float3 accel;
		//�X�P�[��
    float scale = 1.0f;
		//�F
    float4 startColor = { 1, 1, 1, 1 };
		//�I���F
    float4 endColor = { 1, 1, 1, 1 };
		//�o�ߎ���
    float deltaTime;

    int maxParticle;
}

struct ParticleData
{
    float4 color; // �F (RGBA)
    float3 position; // xyz���W
    float age;
    float3 vec;
    float isAlive;
    float3 accel;
    float size; //�X�P�[��
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