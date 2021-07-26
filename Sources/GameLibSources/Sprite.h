#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>


struct TexData
{
	DirectX::XMFLOAT2 pos;    // 描画座標
	DirectX::XMFLOAT2 size;   // 描画サイズ
	DirectX::XMFLOAT2 texPos; // テクスチャ切り取り座標
	DirectX::XMFLOAT2 texSize;   // テクスチャ切り取りサイズ
	int animCnt;			  // アニメーション用カウント
	int animFrame;			  // アニメーション用フレーム
};

class Sprite
{
private:
	struct vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		  vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		  pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		  inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			  vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		  samplerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	  rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>   depthStencilState;
	
	D3D11_TEXTURE2D_DESC texture2dDesc;

public:
	Sprite();
	Sprite( const wchar_t *fileName );
	~Sprite() {}

public:
	void Draw
	(
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a
	) const;
	void Draw
	(
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 size,
		DirectX::XMFLOAT2 texPos,
		DirectX::XMFLOAT2 texSize,
		float angle,
		DirectX::XMFLOAT4 color
	)
	{
		Draw( pos.x, pos.y, size.x, size.y, texPos.x, texPos.y, texSize.x, texSize.y, angle, color.x, color.y, color.z, color.w );
	}
	void Draw2
	(
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a
	) const;
	void Draw2
	(
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 size,
		DirectX::XMFLOAT2 texPos,
		DirectX::XMFLOAT2 texSize,
		float angle,
		DirectX::XMFLOAT4 color
	)
	{
		Draw2(pos.x, pos.y, size.x, size.y, texPos.x, texPos.y, texSize.x, texSize.y, angle, color.x, color.y, color.z, color.w);
	}
	void Textout( std::string s, float x, float y, float w, float h, float r = 1, float g = 1, float b = 1, float a = 1 ) const;
	void SetShaderResourceView(ID3D11ShaderResourceView* _shaderResourceView)
	{
		shaderResourceView = _shaderResourceView;
	}
private:
	void CreateStates( ID3D11Device* device );
};


class SpriteBatch
{
private:
	struct vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texcoord;
	};
	struct instance
	{
		DirectX::XMFLOAT4X4 ndcTransform;
		DirectX::XMFLOAT4 texcoordTransform;
		DirectX::XMFLOAT4 color;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		  vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		  pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		  inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			  buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		  samplerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	  rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>   depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> instanceBuffer;

	D3D11_TEXTURE2D_DESC texture2dDesc;

	size_t MAX_INSTANCES;

	D3D11_VIEWPORT viewport;

	UINT countInstance = 0;
	instance* instances = nullptr;

public:
	SpriteBatch( const wchar_t *fileName, size_t maxInstance = 1 );
	~SpriteBatch()
	{
		/*if (instances)
		{
			instances = nullptr;
			delete[] instances;
		}*/
	}

public:
	void Begin();
	void Draw
	(
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 size,
		DirectX::XMFLOAT2 texPos,
		DirectX::XMFLOAT2 texSize,
		float angle,
		DirectX::XMFLOAT4 color
	);

	void Draw(TexData texData, float angle, DirectX::XMFLOAT4 color);

	void Draw2(float dx, float dy, float dw, float dh, float angle/*degree*/, float r, float g, float b, float a, DirectX::XMFLOAT4 inf);

	/*void Draw
	(
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 size,
		DirectX::XMFLOAT2 texPos,
		DirectX::XMFLOAT2 texSize,
		float angle,
		DirectX::XMFLOAT4 color,
		int animFrame
		)
	{
		Draw(pos, size, texPos, texSize, angle, color);
	}*/
	void End();

private:
	void CreateStates( ID3D11Device* device );
};