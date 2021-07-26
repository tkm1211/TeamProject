#include "StaticMesh.hlsli"

Texture2D    myTexture : register( t0 );
SamplerState mySampler : register( s0 );

float4 main( VS_OUT pin ) : SV_TARGET
{
#if 0
	return pin.color;
#else 
	return myTexture.Sample( mySampler, pin.tex )/* * pin.color*/;
#endif
}