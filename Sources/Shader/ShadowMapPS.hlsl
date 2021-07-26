#include "ShadowMap.hlsli"

Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);


float4 main(PSInputShadow input) : SV_TARGET0
{
	float4 color = input.Depth.z / input.Depth.w;
	color.a = 1.0f;

	return color;
}