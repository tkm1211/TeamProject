cbuffer CBPerFrame : register(b1)
{
	float bright;
	float contrast;
	float saturate;
	float dummy;
	float4 screenColor;
};

float4 ScreenFilter(float4 color, float bright, float contrast, float saturate, float4 screenColor)
{
	float4 inputColor = color;

	// ���x����
	inputColor.rgb += bright;

	// �R���g���X�g����
	inputColor.rgb = ((color.rgb - 0.5f) * contrast) + 0.5f;

	// �ʓx����
	float avr = (inputColor.r + inputColor.g + inputColor.b) / 3;
	inputColor.rgb = (inputColor.rgb - avr) * saturate + avr;

	// �J���[�o�����X����
	inputColor.rgb += screenColor.rgb;

	return inputColor;
}


//--------------------------------------------
// �g�U���ˊ֐��i�����o�[�g�V�F�[�f�B���O�j
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�(0�`1.0)
float3 Diffuse(float3 N, float3 L, float3 C, float3 K)
{
	float D = dot(N, -L);
	D = max(0, D);
	return K * C * D;
}

//--------------------------------------------
// ���ʔ��ˊ֐�(�u�����E�t�H��)
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// E:���_�����x�N�g��(���K���ς�)
// K:���˗�(0�`1.0)
// Power:�n�C���C�g�̋���(�P���x)
float3 BlinnPhongSpecular(float3 N, float3 L, float3 C, float3 E, float3 K, float Power)
{
	float3 H = normalize(-L + E);

	float3 S = dot(H, N);
	S = max(0, S);
	S = pow(S, Power);
	S = S * K * C;
	return S;
}