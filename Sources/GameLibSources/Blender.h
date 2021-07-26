#ifndef _BLEND_H_
#define _BLEND_H_

#include "FrameWork.h"

class Blender
{
public:
	enum BlenderState { BS_NONE, BS_ALPHA, BS_ADD, BS_SUBTRACT, BS_REPLACE, BS_MULTIPLY, BS_LIGHTEN, BS_DARKEN, BS_SCREEN, BS_END };

	Microsoft::WRL::ComPtr<ID3D11BlendState> states[BS_END];

	Blender( ID3D11Device *device )
	{
		HRESULT hr = S_OK;


		D3D11_BLEND_DESC blendDesc = {};

		// BS_NONE
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= FALSE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_NONE].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_NONE]	Error" );


		// BS_ALPHA
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= TRUE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_ALPHA].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_NONE]	Error" );


		// BS_ADD
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= TRUE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_ONE
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_ADD].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_ADD]	Error" );


		// BS_SUBTRACT
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_ZERO
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ONE; //D3D11_BLEND_INV_SRC_COLOR
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_REV_SUBTRACT; //D3D11_BLEND_OP_ADD
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_SUBTRACT].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_SUBTRACT]	Error" );


		// BS_REPLACE
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_REPLACE].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_REPLACE]	Error" );


		// BS_MULTIPLY
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_ZERO; //D3D11_BLEND_DEST_COLOR
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_SRC_COLOR; //D3D11_BLEND_SRC_COLOR
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_DEST_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_MULTIPLY].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_MULTIPLY]	Error" );


		// BS_LIGHTEN
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_LIGHTEN].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_LIGHTEN]	Error" );


		// BS_DARKEN
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_MIN;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_MIN;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_DARKEN].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_DARKEN]		Error" );


		// BS_SCREEN
		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA; //D3DBLEND_INVDESTCOLOR
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_COLOR; //D3DBLEND_ONE
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState( &blendDesc, states[BS_SCREEN].GetAddressOf() );
		assert( !hr && "CreateBlendState	[BS_SCREEN]		Error" );

	}
};

//***********************
//	BlenderModeíËêî
//***********************

#define BM_NONE		(0)
#define BM_ALPHA	(1)
#define BM_ADD		(2)
#define BM_SUBTRACT (3)
#define BM_REPLACE	(4)
#define BM_MULTIPLY (5)
#define BM_LIGHTEN	(6)
#define BM_DARKEN	(7)
#define BM_SCREEN	(8)

extern void SetBlenderMode(int mode);

#endif // !_BLEND_H_
