#include"PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{    
    float4 texcolor0 = tex0.Sample(smp, input.uv);
     
    float shiftWidth = 0.005;
    
    float4 texcolor1 = { 0, 0, 0, 0 };
    texcolor1 += tex1.Sample(smp, input.uv + (-shiftWidth, -shiftWidth));
    texcolor1 += tex1.Sample(smp, input.uv + (0, -shiftWidth));
    texcolor1 += tex1.Sample(smp, input.uv + (+shiftWidth, -shiftWidth));
    
    texcolor1 += tex1.Sample(smp, input.uv + (-shiftWidth, 0));
    texcolor1 += tex1.Sample(smp, input.uv + (0, 0));
    texcolor1 += tex1.Sample(smp, input.uv + (+shiftWidth, 0));
    
    texcolor1 += tex1.Sample(smp, input.uv + (-shiftWidth, +shiftWidth));
    texcolor1 += tex1.Sample(smp, input.uv + (0, +shiftWidth));
    texcolor1 += tex1.Sample(smp, input.uv + (+shiftWidth, +shiftWidth));
    
    texcolor1.rgb = texcolor1.rgb / 9;
    texcolor1.a = 1;
    
    float4 color = texcolor1;
    
    if (fmod(input.uv.y, 0.1f) < 0.05f)
    {
        color = float4(1 - texcolor0.rgb, 1);
    }
    float grayScale = texcolor0.r * 0.299 + texcolor0.g * 0.587 + texcolor0.b * 0.114;
    float sepia = 0.2f;
    //color = float4(grayScale+sepia, grayScale, grayScale-sepia, 1);
    
    return color;
}