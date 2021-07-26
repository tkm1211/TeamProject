#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

#include "ResourceManager.h"
#include "WICTextureLoader.h"
#include "Texture.h"


class Billboard
{
private:
	struct CBuffer
	{
		DirectX::XMFLOAT4X4	wvp;
		DirectX::XMFLOAT4 color;
	};
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
	};
	struct Subset
	{
		std::string usemtl;
		unsigned int indexStart = 0;
		unsigned int indexCount = 0;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	wireframeRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	solidRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	std::vector<Subset> subsets;
	D3D11_TEXTURE2D_DESC texture2dDesc;

	wchar_t mapFileName[256];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

	int numIndex = 0;

public:
	Billboard(ID3D11Device* device, const wchar_t* fileName)
	{

		// Create Vertex Shader and Input Layout *****************************************

		D3D11_INPUT_ELEMENT_DESC ieDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		ResourceManager::CreateVertexShaderAndInputLayout(device, "Data/Shader/BillboardVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), ieDesc, ARRAYSIZE(ieDesc));

		// *******************************************************************************

		// Create Pixel Shader ***********************************************************

		ResourceManager::CreatePixelShader(device, "Data/Shader/BillboardPS.cso", pixelShader.GetAddressOf());

		// *******************************************************************************

		Init(device);

		for (int i = 0; i < 256; i++)
		{
			mapFileName[i] = fileName[i];
		}

		CreateShaderResourceView(device, fileName);

	}
	~Billboard() {}

	void Init(ID3D11Device* device);
	void Begin(ID3D11DeviceContext* deviceContext);
	void Render
	(
		ID3D11DeviceContext* deviceContext,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT3& position,
		float tx,
		float ty,
		float tw,
		float th,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT2& scale,
		const DirectX::XMFLOAT4& color
	);
	void End();

private:
	//void CreateBoard( ID3D11Device* device );
	void CreateBuffers(ID3D11Device* device, Vertex* vertices, int numVertices, u_int* indices, int numIndices);
	void CreateShaderResourceView(ID3D11Device* device, const wchar_t* fileName)
	{
		Texture::LoadTextureFromFile(device, fileName, shaderResourceView.GetAddressOf(), &texture2dDesc);
	}
};

extern Billboard billboard;
