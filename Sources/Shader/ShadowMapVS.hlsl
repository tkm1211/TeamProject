#include "ShadowMap.hlsli"


PSInputShadow main(float3 Position : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD, float4 Color : COLOR)
{
	PSInputShadow output = (PSInputShadow)0;
	float4 P = float4(Position, 1.0);

	// WVP 変換.
	output.Position = mul(world_view_projection, P);

	// 出力値設定.
	output.Depth = output.Position;
	return output;
}