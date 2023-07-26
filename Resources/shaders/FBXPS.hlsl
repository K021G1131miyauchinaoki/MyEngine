#include"FBX.hlsli"

struct PSOutput {
    float4 target0:SV_TARGET0;
    float4 target1:SV_TARGET1;
};

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);
//�G���g���[�|�C���g
PSOutput main(VSOutput input) : SV_TARGET
{
    PSOutput output;
	//�e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);
	//Lambert����
    float3 light = normalize(float3(1, -1, 1));//�E�����������C�g
    float diffuse = saturate(dot(-light, input.normal));
    float brightness = diffuse + 0.3f;
    float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

    output.target0 = shadecolor * texcolor;
    output.target1 = float4(1 - (shadecolor * texcolor).rgb, 1);
    //��e�ƃe�N�X�`���̐F������
	return output;
}