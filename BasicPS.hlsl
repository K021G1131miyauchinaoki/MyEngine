cbuffer	ConstBufferDataMaterial:register(b0)
{
	float4	color;//RGBA
}

float4 main() : SV_TARGET
{
	return color;
}