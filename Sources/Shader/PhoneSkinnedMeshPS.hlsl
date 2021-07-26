#include "PhoneSkinnedMesh.hlsli" 
#include "Function.hlsli"

Texture2D    myTexture : register(t0); //�e�N�X�`���[
SamplerState mySampler : register(s0); //�T���v���[


float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = myTexture.Sample(mySampler, pin.tex) * pin.color;
	float3 N = normalize(pin.wNormal);
	float3 E = normalize(EyePos.xyz - pin.wPos);
	float3 L = normalize(LightDir.xyz);

	// ����
	float3 A = AmbientColor.rgb;

	// �g�U����
	float3 C = LightColor.rgb;
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(N, L, C, Kd);

	// ���ʔ���
	float3 Ks = float3(1, 1, 1);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 20);


	//�|�C���g���C�g
	float3 PL;		 //�|�C���g���C�g�x�N�g��
	float3 PC;		 //�|�C���g���C�g�F
	float influence; //�|�C���g���C�g�̉e��

	float3 PD = (float3)0;
	float3 PS = (float3)0;

	for (int i = 0; i < POINTMAX; i++)
	{
		//�_�����Ă��Ȃ����C�g���O
		if (PointLight[i].type == 0) continue;

		PL = pin.wPos - PointLight[i].pos.xyz;

		float d = length(PL);
		float r = PointLight[i].range;

		//�͂��Ȃ����C�g���O
		if (d > r) continue;
		influence = max(1, (1.0f - d / r));
		PL = normalize(PL);
		PC = PointLight[i].color.rgb;

		//�g�U���̉��Z
		PD += Diffuse(N, PL, PC, Kd) * influence * influence;

		//���ʌ��̉��Z
		PS += BlinnPhongSpecular(N, PL, PC, E, Ks, 20) * influence * influence;
	}


	//�X�|�b�g���C�g
	float3 SL;
	float3 SC;
	float influence2;

	float3 SD = (float3)0; //�X�|�b�g���C�g�̊g�U����
	float3 SS = (float3)0; //�X�|�b�g���C�g�̋��ʔ���

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