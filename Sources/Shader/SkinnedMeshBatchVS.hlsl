#include "SkinnedMeshBatch.hlsli"
#include "Function.hlsli"

VS_OUT main
(
	float4 position				: POSITION,
	float4 normal				: NORMAL,
	float2 tex					: TEXCOORD,
	float4 boneWeights			: WEIGHTS,
	uint4  boneIndices			: BONES,
	row_major float4x4 wvp		: WVP,					// インスタンスごとに設定される行列
	row_major float4x4 world	: WORLD,				// インスタンスごとに設定される行列
	uint frame					: FRAME,				// インスタンスごとに設定される行列
	uint instanceID				: SV_InstanceID			// インスタンスＩＤ
)
{
	VS_OUT vout;

	float4 p = { 0,0,0,0 };
	float4 n = { 0,0,0,0 };
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		row_major float4x4 _boneTransforms = frame < MAX_FRAME ? boneTransforms1[frame][boneIndices[i]] : boneTransforms2[frame][boneIndices[i]];
		_boneTransforms = frame < MAX_FRAME * 2 ? _boneTransforms : boneTransforms3[frame][boneIndices[i]];
		p += (boneWeights[i] * mul(position, _boneTransforms));
		n += (boneWeights[i] * mul(float4(normal.xyz, 0), _boneTransforms));
	}
	p.w = 1.0f;
	n.w = 0.0f;
	position = p;
	normal = n;

	vout.position = mul(position, wvp);

	normal.w = 0;
	float4 N = normalize(mul(normal, world));
	float4 L = normalize(-lightDirection);

	vout.color = materialColor * max(1, dot(L, N));
	vout.color.a = materialColor.a;

	vout.tex = tex;

	return vout;
}