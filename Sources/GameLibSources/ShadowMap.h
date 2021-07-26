#pragma once

#include <memory>
#include <vector>
#include "Sprite.h"


class ShadowMap
{
private:
	std::unique_ptr<Sprite> pTex;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState[2];
	DirectX::XMFLOAT4X4 lightViewProjection;

public:
	//DepthStencilState
	enum { DS_FALSE, DS_TRUE };

public:
	void Init();
	void Update();
	void Draw();
};