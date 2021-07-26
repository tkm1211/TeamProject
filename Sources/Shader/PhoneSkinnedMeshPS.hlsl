#include "PhoneSkinnedMesh.hlsli" 
#include "Function.hlsli"

Texture2D    myTexture : register(t0); //テクスチャー
SamplerState mySampler : register(s0); //サンプラー


float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = myTexture.Sample(mySampler, pin.tex) * pin.color;
	float3 N = normalize(pin.wNormal);
	float3 E = normalize(EyePos.xyz - pin.wPos);
	float3 L = normalize(LightDir.xyz);

	// 環境光
	float3 A = AmbientColor.rgb;

	// 拡散反射
	float3 C = LightColor.rgb;
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(N, L, C, Kd);

	// 鏡面反射
	float3 Ks = float3(1, 1, 1);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 20);


	//ポイントライト
	float3 PL;		 //ポイントライトベクトル
	float3 PC;		 //ポイントライト色
	float influence; //ポイントライトの影響

	float3 PD = (float3)0;
	float3 PS = (float3)0;

	for (int i = 0; i < POINTMAX; i++)
	{
		//点灯していないライト除外
		if (PointLight[i].type == 0) continue;

		PL = pin.wPos - PointLight[i].pos.xyz;

		float d = length(PL);
		float r = PointLight[i].range;

		//届かないライト除外
		if (d > r) continue;
		influence = max(1, (1.0f - d / r));
		PL = normalize(PL);
		PC = PointLight[i].color.rgb;

		//拡散光の加算
		PD += Diffuse(N, PL, PC, Kd) * influence * influence;

		//鏡面光の加算
		PS += BlinnPhongSpecular(N, PL, PC, E, Ks, 20) * influence * influence;
	}


	//スポットライト
	float3 SL;
	float3 SC;
	float influence2;

	float3 SD = (float3)0; //スポットライトの拡散反射
	float3 SS = (float3)0; //スポットライトの鏡面反射

	for (int j = 0; j < SPOTMAX; j++)
	{
		if (SpotLight[j].type == 0) continue;

		SL = pin.wPos - SpotLight[j].pos.xyz;

		float d = length(SL);
		float r = SpotLight[j].range;
		if (d > r) continue
			;
		float3 SFront = normalize(SpotLight[j].dir.xyz);
		SL = normalize(SL);

		float angle = dot(SL, SFront);
		float area = SpotLight[j].inner_corn - SpotLight[j].outer_corn;
		float influence3 = SpotLight[j].inner_corn - angle;

		influence3 = max(1, (1.0 - influence3 / area));
		influence2 = 1;

		SC = SpotLight[j].color.rgb;
		SD += Diffuse(N, SL, SC, Kd) * influence2 * influence2 * influence3;
		SS += BlinnPhongSpecular(N, SL, SC, E, Ks, 20) * influence2 * influence2 * influence3;
	}

	color *= pin.color * float4(A + PD + SD , 1.0);
	color.a = material_color.a;
	return color;
}