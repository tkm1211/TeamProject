#include "Mesh.h"
#include "ResourceManager.h"


void Mesh::Init( ID3D11Device *device, BOOL handedCoordinateSystem )
{

	HRESULT hr = S_OK;


// Create Sampler State **********************************************************

	//Wireframe
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = handedCoordinateSystem;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;

	hr = device->CreateRasterizerState( &rasterizerDesc, wireframeRasterizerState.GetAddressOf() );
	assert( !hr && "CreateRasterizerState	Error" );


	//Solid
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = handedCoordinateSystem;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;

	hr = device->CreateRasterizerState( &rasterizerDesc, solidRasterizerState.GetAddressOf() );
	assert( !hr && "CreateRasterizerState	Error" );

// *******************************************************************************

// Create Depth Stencil State ****************************************************

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = device->CreateDepthStencilState( &depthStencilDesc, depthStencilState.GetAddressOf() );
	assert( !hr && "CreateDepthStencilState	Error" );

// *******************************************************************************

// Create Sampler ****************************************************************

	D3D11_SAMPLER_DESC smpDesc = {};
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	/*smpDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	smpDesc.MipLODBias = 0;
	smpDesc.MaxAnisotropy = 16;
	smpDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	smpDesc.MinLOD = 0;
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;*/

	hr = device->CreateSamplerState( &smpDesc, samplerState.GetAddressOf() );
	assert( !hr && "CreateSamplerState	Error" );

// *******************************************************************************
}