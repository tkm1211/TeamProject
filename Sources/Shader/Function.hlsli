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

	// 明度調整
	inputColor.rgb += bright;

	// コントラスト調整
	inputColor.rgb = ((color.rgb - 0.5f) * contrast) + 0.5f;

	// 彩度調整
	float avr = (inputColor.r + inputColor.g + inputColor.b) / 3;
	inputColor.rgb = (inputColor.rgb - avr) * saturate + avr;

	// カラーバランス調整
	inputColor.rgb += screenColor.rgb;

	return inputColor;
}


//--------------------------------------------
// 拡散反射関数（ランバートシェーディング）
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// K:反射率(0～1.0)
float3 Diffuse(float3 N, float3 L, float3 C, float3 K)
{
	float D = dot(N, -L);
	D = max(0, D);
	return K * C * D;
}

//--------------------------------------------
// 鏡面反射関数(ブリン・フォン)
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// E:視点方向ベクトル(正規化済み)
// K:反射率(0～1.0)
// Power:ハイライトの強さ(輝き度)
float3 BlinnPhongSpecular(float3 N, float3 L, float3 C, float3 E, float3 K, float Power)
{
	float3 H = normalize(-L + E);

	float3 S = dot(H, N);
	S = max(0, S);
	S = pow(S, Power);
	S = S * K * C;
	return S;
}