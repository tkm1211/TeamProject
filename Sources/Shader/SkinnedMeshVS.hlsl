#include "SkinnedMesh.hlsli"
#include "Function.hlsli"

VS_OUT main(float4 position : POSITION, float4 normal : NORMAL, float2 tex : TEXCOORD, float4 bone_weights : WEIGHTS, uint4 bone_indices : BONES)
{
	VS_OUT vout;

	float3 p = { 0,0,0 };
	float3 n = { 0,0,0 };
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		p += (bone_weights[i] * mul(position, bone_transforms[bone_indices[i]])).xyz;
		n += (bone_weights[i] * mul(float4(normal.xyz, 0), bone_transforms[bone_indices[i]])).xyz;
	}
	position = float4(p, 1.0f);
	normal = float4(n, 0.0f);

	vout.position = mul(position, world_view_projection);

	normal.w = 0;
	float4 N = normalize(mul(normal, world));
	float4 L = normalize(-light_direction);

	vout.color = material_color * max(1, dot(L, N));
	vout.color.a = material_color.a;

	vout.tex = tex;

	return vout;
}