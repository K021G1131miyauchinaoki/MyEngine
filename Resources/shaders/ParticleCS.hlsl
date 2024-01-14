AppendStructuredBuffer<uint>freeList:register(u0);

cbuffer emitData : register(b0)
{
	//���W
    float3 pos;
	//����
    float lifeTime;
	//�����x
    float3 accel;
	//�X�P�[��
    float scale;
	//�F
    float4 startColor;
	//�I���F
    float4 endColor;
	//�o�ߎ���
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