#include "NormalMapSkinnedMesh.hlsli"
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
	float3 N = normalize(mul(normal.xyz, (float3x3)world));

	//Ú‹óŠÔ
	float3 vN;
	float3 vB;
	float3 vT;
	if (EyePos.w == 0.0f)
	{
		vN = N;
		vB = float3(0, 1, -0.001f);
		vB = normalize(vB);
		vT = normalize(cross(vB, vN));
		vB = normalize(cross(vN, vT));
	}
	else
	{
		vN = N;
		vB = float3(0, -0.001f, 1);
		vB = normalize(vB);
		vT = normalize(cross(vN, vB));
		vB = normalize(cross(vT, vN));
	}

	//vout.color = material_color * max(1, dot(L, N));
	//vout.color.a = material_color.a;

	vout.color = material_color;

	vout.tex = tex;

	vout.wPos = mul(world, position).xyz;
	vout.wNormal = N;

	//Ú‹óŠÔŽ²
	vout.vT = vT;
	vout.vB = vB;
	vout.vN = vN;

	return vout;
}