#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>


class Light
{
public:
	static const int POINTMAX = 96;
	static const int SPOTMAX = 32;

	struct POINTLIGHT
	{
		float index;
		float range;
		float type;
		float dumy;
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 color;
	};
	struct SPOTLIGHT {
		float index;
		float range;//光の届く範囲
		float type; //有効か無効か
		float inner_corn;
		float outer_corn;
		float dumy0;
		float dumy1;
		float dumy2;
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 dir;
	};

	struct CBufferLight01
	{
		DirectX::XMFLOAT4 lightColor;	//ライトの色
		DirectX::XMFLOAT4 lightDir;		//ライトの方向
		DirectX::XMFLOAT4 ambientColor; //環境光
		DirectX::XMFLOAT4 eyePos;		//カメラ座標
	};
	struct CBufferLight02
	{
		POINTLIGHT pointLight[POINTMAX];
		SPOTLIGHT spotLight[SPOTMAX];
	};

public:
	 Light() {}
	~Light() {}

	static Light* GetInstance()
	{
		static Light instance;
		return &instance;
	}

public:
	Microsoft::WRL::ComPtr <ID3D11Buffer>	constantBufferLight1;
	Microsoft::WRL::ComPtr <ID3D11Buffer>	constantBufferLight2;

	float lightAngle = 4.16f;
	float posY = -0.36f;

	DirectX::XMFLOAT4 lightDir = DirectX::XMFLOAT4(1, 1, 1, 1);
	DirectX::XMFLOAT4 lightColor = DirectX::XMFLOAT4(1, 1, 1, 1);
	DirectX::XMFLOAT4 ambient = DirectX::XMFLOAT4(1, 1, 1, 1);
	POINTLIGHT pointLight[POINTMAX];
	SPOTLIGHT spotLight[SPOTMAX];

	int pointLightNo;

	void Init();
	void Update();
	void ImGui();

	void SetDirLight(DirectX::XMFLOAT4 dir, DirectX::XMFLOAT3 color);
	void SetAmbient(DirectX::XMFLOAT3 amb);
	void SetPointLight(int index, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, float range);
	void ReSetPointLight(int index);
	void SetPointLight(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, float range);
	void SetSpotLight(int index, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 dir, float range, float near_area, float far_area);
};