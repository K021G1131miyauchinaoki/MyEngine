float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos+float4(1,0,0,1);
}