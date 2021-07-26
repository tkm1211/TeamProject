#include "NormalMapSkinnedMesh.hlsli" 
#include "Function.hlsli"


Texture2D    diffuseTexture : register(t0);
Texture2D    normalTexture : register(t1);

SamplerState pointSampler		: register(s0);
SamplerState linerSampler		: register(s1);
SamplerState anisotropicSampler : register(s2);


float4 main(VS_OUT pin) : SV_TARGET
{
#if 1
	float3 E = normalize(EyePos.xyz - pin.wPos);
	float3 L = normalize(LightDir.xyz);

	// ����
	float3 A = AmbientColor.rgb;
	float3 C = LightColor.rgb;

	// �ڋ�Ԏ�
	float3 vx = normalize(pin.vT);
	float3 vy = normalize(pin.vB);
	float3 vz = normalize(pin.vN);

	float2 Tex = pin.tex;

	/*
	// �����ڋ�ԕϊ�
	float3x3 inv_mat = { vx,vy,vz };
	float3 inv_E = normalize(mul(inv_mat, E));

	// �����v�Z
	float H = HeightTexture.Sample(HeightSampler, Tex).r;
	H = H * 2.0f - 1.0;
	Tex.x -= inv_E.x * H * viewOffset;
	Tex.y -= inv_E.y * H * viewOffset;
	*/

	// �e�N�X�`���F�擾
	float4 color = diffuseTexture.Sample(anisotropicSampler, pin.tex);
	float3 N = normalTexture.Sample(pointSampler, Tex).xyz;
	N = N * 2.0f - 1.0f;

	// �@�����[���h�ϊ�
	float3x3 mat = { vx,vy,vz };
	N = normalize(mul(N, mat));

	// �g�U����
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(N, L, C, Kd);

	// ���ʔ���
	float3 Ks = float3(1, 1, 1);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 200);

	color *= pin.color * float4(A + D + S, 1.0);
	return color;
#else
	return myTexture.Sample(diffuseSampler, pin.tex) * pin.color;
#endif
}