#include "sprite.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "FrameWork.h"


Sprite::Sprite()
{

	HRESULT hr = S_OK;

	// Get Device ********************************************************************

	Microsoft::WRL::ComPtr<ID3D11Device> device = FrameWork::GetInstance().GetDevice();

	// *******************************************************************************

	// Create Vertex Buffer **********************************************************

	vertex vertices[] = {
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT2(0, 0) },
	};


	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(vertex) * 4;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;

	hr = device->CreateBuffer(&bufferDesc, &subresourceData, vertexBuffer.GetAddressOf());
	assert(!hr && "CreateBuffer		Error");

	// *******************************************************************************

	// Create Vertex Shader and Input Layout *****************************************

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ResourceManager::CreateVertexShaderAndInputLayout(device.Get(), "./Data/Shader/SpriteVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));

	// *******************************************************************************

	// Create Pixel Shader ***********************************************************

	ResourceManager::CreatePixelShader(device.Get(), "./Data/Shader/SpritePS.cso", pixelShader.GetAddressOf());

	// *******************************************************************************

	// Create States *****************************************************************

	CreateStates(device.Get());

	// ******************************************************************************
}

Sprite::Sprite( const wchar_t *fileName )
{

	HRESULT hr = S_OK;

// Get Device ********************************************************************

	Microsoft::WRL::ComPtr<ID3D11Device> device = FrameWork::GetInstance().GetDevice();

// *******************************************************************************

// Create Vertex Buffer **********************************************************

	vertex vertices[] = {
		{ DirectX::XMFLOAT3( 0, 0, 0 ), DirectX::XMFLOAT4( 1, 1, 1, 1 ), DirectX::XMFLOAT2( 0, 0 ) },
		{ DirectX::XMFLOAT3( 0, 0, 0 ), DirectX::XMFLOAT4( 1, 1, 1, 1 ), DirectX::XMFLOAT2( 0, 0 ) },
		{ DirectX::XMFLOAT3( 0, 0, 0 ), DirectX::XMFLOAT4( 1, 1, 1, 1 ), DirectX::XMFLOAT2( 0, 0 ) },
		{ DirectX::XMFLOAT3( 0, 0, 0 ), DirectX::XMFLOAT4( 1, 1, 1, 1 ), DirectX::XMFLOAT2( 0, 0 ) },
	};


	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth		= sizeof( vertex ) * 4;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;

	hr = device->CreateBuffer( &bufferDesc, &subresourceData, vertexBuffer.GetAddressOf() );
	assert( !hr && "CreateBuffer		Error" );

// *******************************************************************************

// Create Vertex Shader and Input Layout *****************************************

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ResourceManager::CreateVertexShaderAndInputLayout( device.Get(), "./Data/Shader/SpriteVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, ARRAYSIZE( inputElementDesc ) );

// *******************************************************************************

// Create Pixel Shader ***********************************************************

	ResourceManager::CreatePixelShader( device.Get(), "./Data/Shader/SpritePS.cso", pixelShader.GetAddressOf() );

// *******************************************************************************

// Load Texture ******************************************************************

	if (fileName[0] != NULL) Texture::LoadTextureFromFile( device.Get(), fileName, shaderResourceView.GetAddressOf(), &texture2dDesc );

// *******************************************************************************

// Create States *****************************************************************

	CreateStates( device.Get() );

// ******************************************************************************
}

void Sprite::CreateStates( ID3D11Device* device )
{

	HRESULT hr = S_OK;


// Create Sampler State **********************************************************

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//samplerDesc.Filter			= D3D11_FILTER_ANISOTROPIC;
	//samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_BORDER;
	//samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_BORDER;
	//samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias		= 0;
	samplerDesc.MaxAnisotropy	= 16;
	//samplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD			= 0;
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState( &samplerDesc, samplerState.GetAddressOf() );
	assert( !hr && "CreateSamplerState	Error" );

// *******************************************************************************

// Create Rasterizer State *******************************************************

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode			= D3D11_FILL_SOLID;
	rasterizerDesc.CullMode			= D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable	= FALSE;

	hr = device->CreateRasterizerState( &rasterizerDesc, rasterizerState.GetAddressOf() );
	assert( !hr && "CreateRasterizerState	Error" );

// *******************************************************************************

// Create Depth Stencil State ****************************************************

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable	= FALSE;
	depthStencilDesc.StencilEnable	= FALSE;

	hr = device->CreateDepthStencilState( &depthStencilDesc, depthStencilState.GetAddressOf() );
	assert( !hr && "CreateDepthStencilState		Error" );

// *******************************************************************************

}

void Sprite::Draw( float dx, float dy, float dw, float dh, float sx, float sy, float sw, float sh, float angle, float r, float g, float b, float a ) const
{

// Get Context *******************************************************************

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

// *******************************************************************************

// Get Screen Size for ViewPort **************************************************

	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;

	immediateContext->RSGetViewports( &numViewports, &viewport );

	float screenWidth  = viewport.Width;
	float screenHeight = viewport.Height;

// *******************************************************************************

// Convert Screen Coordinates to NDC *********************************************

	//left, top
	float x0 = dx;
	float y0 = dy;

	//right, top
	float x1 = dx + dw;
	float y1 = dy;

	//left, bottom
	float x2 = dx;
	float y2 = dy + dh;

	//right, bottom
	float x3 = dx + dw;
	float y3 = dy + dh;

	//Get Center Position
	float mx = dx + dw * 0.5f;
	float my = dy + dy * 0.5f;


	//Convert to Coordinates Centered at 0
	x0 -= mx;
	y0 -= my;
	x1 -= mx;
	y1 -= my;
	x2 -= mx;
	y2 -= my;
	x3 -= mx;
	y3 -= my;


	//Rotate Each Vertex Coordinate by Angle
	float rx, ry;

	float cos = cosf( angle * 0.01745f );
	float sin = sinf( angle * 0.01745f );

	//left, top
	rx = x0;
	ry = y0;

	x0 = cos * rx + -sin * ry;
	y0 = sin * rx + cos * ry;

	//right, top
	rx = x1;
	ry = y1;

	x1 = cos * rx + -sin * ry;
	y1 = sin * rx + cos * ry;

	//left, bottom
	rx = x2;
	ry = y2;

	x2 = cos * rx + -sin * ry;
	y2 = sin * rx + cos * ry;

	//right, bottom
	rx = x3;
	ry = y3;
	x3 = cos * rx + -sin * ry;
	y3 = sin * rx + cos * ry;


	//Convert to Original Coordinates
	x0 += mx;
	y0 += my;
	x1 += mx;
	y1 += my;
	x2 += mx;
	y2 += my;
	x3 += mx;
	y3 += my;


	//Normalize and Convert to NDC
	x0 = 2.0f*x0 / screenWidth - 1.0f;
	y0 = 1.0f - 2.0f*y0 / screenHeight;

	x1 = 2.0f*x1 / screenWidth - 1.0f;
	y1 = 1.0f - 2.0f*y1 / screenHeight;

	x2 = 2.0f*x2 / screenWidth - 1.0f;
	y2 = 1.0f - 2.0f*y2 / screenHeight;

	x3 = 2.0f*x3 / screenWidth - 1.0f;
	y3 = 1.0f - 2.0f*y3 / screenHeight;

// *******************************************************************************

// Update Vertex Buffer with Calculation Result **********************************

	HRESULT hr = S_OK;
	
	D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;

	hr = immediateContext->Map( vertexBuffer.Get(), 0, map, 0, &mappedBuffer );
	assert( !hr && "Map	Error" );

	// Assign Calculation Result to Vertex Buffer
	vertex *vertices = static_cast<vertex *>( mappedBuffer.pData );
	vertices[0].pos.x = x0;
	vertices[0].pos.y = y0;
	vertices[1].pos.x = x1;
	vertices[1].pos.y = y1;
	vertices[2].pos.x = x2;
	vertices[2].pos.y = y2;
	vertices[3].pos.x = x3;
	vertices[3].pos.y = y3;
	vertices[0].pos.z = vertices[1].pos.z = vertices[2].pos.z = vertices[3].pos.z = 0.0f;

	//Assign ( r, g, b, a )
	DirectX::XMFLOAT4 color( r, g, b, a );
	vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

	//texcoord
	vertices[0].texcoord.x = static_cast<FLOAT>( sx ) / texture2dDesc.Width;
	vertices[0].texcoord.y = static_cast<FLOAT>( sy ) / texture2dDesc.Height;
	vertices[1].texcoord.x = static_cast<FLOAT>( sx + sw ) / texture2dDesc.Width;
	vertices[1].texcoord.y = static_cast<FLOAT>( sy ) / texture2dDesc.Height;
	vertices[2].texcoord.x = static_cast<FLOAT>( sx ) / texture2dDesc.Width;
	vertices[2].texcoord.y = static_cast<FLOAT>( sy + sh ) / texture2dDesc.Height;
	vertices[3].texcoord.x = static_cast<FLOAT>( sx + sw ) / texture2dDesc.Width;
	vertices[3].texcoord.y = static_cast<FLOAT>( sy + sh ) / texture2dDesc.Height;


	immediateContext->Unmap( vertexBuffer.Get(), 0 );

// *******************************************************************************

// Set Object and Draw ***********************************************************

	UINT stride = sizeof( vertex );
	UINT offset = 0;

	immediateContext->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddressOf(), &stride, &offset );
	immediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	immediateContext->IASetInputLayout( inputLayout.Get() );

	immediateContext->RSSetState( rasterizerState.Get() );

	immediateContext->VSSetShader( vertexShader.Get(), nullptr, 0 );
	immediateContext->PSSetShader( pixelShader.Get(), nullptr, 0 );

	immediateContext->PSSetShaderResources( 0, 1, shaderResourceView.GetAddressOf() );
	immediateContext->PSSetSamplers( 0, 1, samplerState.GetAddressOf() );

	immediateContext->OMSetDepthStencilState( depthStencilState.Get(), 1 );

	immediateContext->Draw( 4, 0 );

// *******************************************************************************

}

void Sprite::Draw2(float dx, float dy, float dw, float dh, float sx, float sy, float sw, float sh, float angle, float r, float g, float b, float a) const
{

	// Get Context *******************************************************************

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

	// *******************************************************************************

	// Get Screen Size for ViewPort **************************************************

	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;

	immediateContext->RSGetViewports(&numViewports, &viewport);

	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	// *******************************************************************************

	// Convert Screen Coordinates to NDC *********************************************

		//left, top
	float x0 = dx - dw / 2.0f;
	float y0 = dy - dh / 2.0f;

	//right, top
	float x1 = dx + dw / 2.0f;
	float y1 = dy - dh / 2.0f;

	//left, bottom
	float x2 = dx - dw / 2.0f;
	float y2 = dy + dh / 2.0f;

	//right, bottom
	float x3 = dx + dw / 2.0f;
	float y3 = dy + dh / 2.0f;

	//Get Center Position
	float mx = dx;
	float my = dy;


	//Convert to Coordinates Centered at 0
	x0 -= mx;
	y0 -= my;
	x1 -= mx;
	y1 -= my;
	x2 -= mx;
	y2 -= my;
	x3 -= mx;
	y3 -= my;


	//Rotate Each Vertex Coordinate by Angle
	float rx, ry;

	float cos = cosf(angle * 0.01745f);
	float sin = sinf(angle * 0.01745f);

	//left, top
	rx = x0;
	ry = y0;

	x0 = cos * rx + -sin * ry;
	y0 = sin * rx + cos * ry;

	//right, top
	rx = x1;
	ry = y1;

	x1 = cos * rx + -sin * ry;
	y1 = sin * rx + cos * ry;

	//left, bottom
	rx = x2;
	ry = y2;

	x2 = cos * rx + -sin * ry;
	y2 = sin * rx + cos * ry;

	//right, bottom
	rx = x3;
	ry = y3;
	x3 = cos * rx + -sin * ry;
	y3 = sin * rx + cos * ry;


	//Convert to Original Coordinates
	x0 += mx;
	y0 += my;
	x1 += mx;
	y1 += my;
	x2 += mx;
	y2 += my;
	x3 += mx;
	y3 += my;


	//Normalize and Convert to NDC
	x0 = 2.0f * x0 / screenWidth - 1.0f;
	y0 = 1.0f - 2.0f * y0 / screenHeight;

	x1 = 2.0f * x1 / screenWidth - 1.0f;
	y1 = 1.0f - 2.0f * y1 / screenHeight;

	x2 = 2.0f * x2 / screenWidth - 1.0f;
	y2 = 1.0f - 2.0f * y2 / screenHeight;

	x3 = 2.0f * x3 / screenWidth - 1.0f;
	y3 = 1.0f - 2.0f * y3 / screenHeight;

	// *******************************************************************************

	// Update Vertex Buffer with Calculation Result **********************************

	HRESULT hr = S_OK;

	D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;

	hr = immediateContext->Map(vertexBuffer.Get(), 0, map, 0, &mappedBuffer);
	assert(!hr && "Map	Error");

	// Assign Calculation Result to Vertex Buffer
	vertex* vertices = static_cast<vertex*>(mappedBuffer.pData);
	vertices[0].pos.x = x0;
	vertices[0].pos.y = y0;
	vertices[1].pos.x = x1;
	vertices[1].pos.y = y1;
	vertices[2].pos.x = x2;
	vertices[2].pos.y = y2;
	vertices[3].pos.x = x3;
	vertices[3].pos.y = y3;
	vertices[0].pos.z = vertices[1].pos.z = vertices[2].pos.z = vertices[3].pos.z = 0.0f;

	//Assign ( r, g, b, a )
	DirectX::XMFLOAT4 color(r, g, b, a);
	vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

	//texcoord
	vertices[0].texcoord.x = static_cast<FLOAT>(sx) / texture2dDesc.Width;
	vertices[0].texcoord.y = static_cast<FLOAT>(sy) / texture2dDesc.Height;
	vertices[1].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2dDesc.Width;
	vertices[1].texcoord.y = static_cast<FLOAT>(sy) / texture2dDesc.Height;
	vertices[2].texcoord.x = static_cast<FLOAT>(sx) / texture2dDesc.Width;
	vertices[2].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2dDesc.Height;
	vertices[3].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2dDesc.Width;
	vertices[3].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2dDesc.Height;


	immediateContext->Unmap(vertexBuffer.Get(), 0);

	// *******************************************************************************

	// Set Object and Draw ***********************************************************

	UINT stride = sizeof(vertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	immediateContext->IASetInputLayout(inputLayout.Get());

	immediateContext->RSSetState(rasterizerState.Get());

	immediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	immediateContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	immediateContext->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
	immediateContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	immediateContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

	immediateContext->Draw(4, 0);

	// *******************************************************************************

}

void Sprite::Textout( std::string s, float x, float y, float w, float h, float r, float g, float b, float a ) const
{

	float sw = static_cast<float>( texture2dDesc.Width / 16 );
	float sh = static_cast<float>( texture2dDesc.Height / 16 );
	
	float cursor = 0;
	
	for ( auto c : s )
	{
		LONG sx = c % 0x0F;
	
		Draw( x + cursor, y, w, h, sw*(c & 0x0F), sh*(c >> 4), sw, sh, 0, r, g, b, a );
		
		cursor += w;
	}

}

SpriteBatch::SpriteBatch( const wchar_t *fileName, size_t maxInstance )
{

	HRESULT hr = S_OK;

// Set MaxInstance ***************************************************************

	MAX_INSTANCES = maxInstance;

// *******************************************************************************

// Get Device ********************************************************************

	Microsoft::WRL::ComPtr<ID3D11Device> device = FrameWork::GetInstance().GetDevice();

// *******************************************************************************

// Create Vertex Buffer **********************************************************

	vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT2(1, 0) },
		{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT2(0, 1) },
		{ DirectX::XMFLOAT3(1, 1, 0), DirectX::XMFLOAT2(1, 1) },
	};

	D3D11_BUFFER_DESC bufferDesc = {};

	bufferDesc.Usage		  = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth	  = sizeof( vertices );
	bufferDesc.BindFlags	  = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;

	hr = device->CreateBuffer( &bufferDesc, &subresourceData, buffer.GetAddressOf() );
	assert( !hr && "CreateBuffer		Error" );

// *******************************************************************************

// Create Vertex Shader and Input Layout *****************************************

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "TEXCOORD",			0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NDC_TRANSFORM",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "NDC_TRANSFORM",		1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "NDC_TRANSFORM",		2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "NDC_TRANSFORM",		3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD_TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR",				0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	ResourceManager::CreateVertexShaderAndInputLayout( device.Get(), "Data/Shader/SpriteBatchVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, ARRAYSIZE( inputElementDesc ) );

// *******************************************************************************

// Create Pixel Shader ***********************************************************

	ResourceManager::CreatePixelShader( device.Get(), "Data/Shader/SpriteBatchPS.cso", pixelShader.GetAddressOf() );

// *******************************************************************************


// Create Instance Buffer **********************************************************

	instance* _instances = new instance[MAX_INSTANCES];
	//ZeroMemory( instances, sizeof( instance ) * MAX_INSTANCES );

	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.ByteWidth		= sizeof( instance ) * MAX_INSTANCES;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;

	ZeroMemory( &subresourceData, sizeof( subresourceData ) );
	subresourceData.pSysMem = _instances;

	hr = device->CreateBuffer( &bufferDesc, &subresourceData, &instanceBuffer );
	assert( !hr && "CreateBuffer		Error" );

	delete[] _instances;

// *******************************************************************************

// Load Texture ******************************************************************

	Texture::LoadTextureFromFile( device.Get(), fileName, shaderResourceView.GetAddressOf(), &texture2dDesc );

// *******************************************************************************

// Create States *****************************************************************

	CreateStates( device.Get() );

// ******************************************************************************

}

void SpriteBatch::CreateStates( ID3D11Device* device )
{

	HRESULT hr = S_OK;


// Create Sampler State **********************************************************

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
	assert(!hr && "CreateSamplerState	Error");

// *******************************************************************************

// Create Rasterizer State *******************************************************

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable = FALSE;

	hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
	assert(!hr && "CreateRasterizerState	Error");

// *******************************************************************************

// Create Depth Stencil State ****************************************************

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.StencilEnable = FALSE;

	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
	assert(!hr && "CreateDepthStencilState		Error");

// *******************************************************************************

}

void SpriteBatch::Begin()
{

	HRESULT hr = S_OK;

// Get Context *******************************************************************

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

// *******************************************************************************

// Preparation Draw **************************************************************

	UINT strides[2] = { sizeof( vertex ), sizeof( instance ) };
	UINT offsets[2] = { 0, 0 };
	ID3D11Buffer *vbs[2] = { buffer.Get(), instanceBuffer.Get() };

	immediateContext->IASetVertexBuffers( 0, 2, vbs, strides, offsets );
	immediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	immediateContext->IASetInputLayout( inputLayout.Get() );
	immediateContext->OMSetDepthStencilState( depthStencilState.Get(), 1 );
	immediateContext->RSSetState( rasterizerState.Get() );
	immediateContext->VSSetShader( vertexShader.Get(), nullptr, 0 );
	immediateContext->PSSetShader( pixelShader.Get(), nullptr, 0 );
	immediateContext->PSSetShaderResources( 0, 1, shaderResourceView.GetAddressOf() );
	immediateContext->PSSetSamplers( 0, 1, samplerState.GetAddressOf() );

	UINT numViewports = 1;
	immediateContext->RSGetViewports( &numViewports, &viewport );

	D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;

	hr = immediateContext->Map( instanceBuffer.Get(), 0, map, 0, &mappedBuffer );
	assert( !hr && "Map		Error" );

	instances = static_cast<instance *>( mappedBuffer.pData );

	countInstance = 0;

// *******************************************************************************

}

void SpriteBatch::Draw( DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 texPos, DirectX::XMFLOAT2 texSize, float angle, DirectX::XMFLOAT4 color )
{

	assert( countInstance < MAX_INSTANCES && "MAX_INSTANCES <= countInstance" );


	float cx = pos.x * 0.5f, cy = pos.y * 0.5f; /*Center of Rotation*/

	FLOAT c = cosf( angle * 0.01745f );
	FLOAT s = sinf( angle * 0.01745f );
	FLOAT w = 2.0f / viewport.Width;
	FLOAT h = -2.0f / viewport.Height;

	instances[countInstance].ndcTransform._11 = w * size.x * c;
	instances[countInstance].ndcTransform._21 = h * size.x * s;
	instances[countInstance].ndcTransform._31 = 0.0f;
	instances[countInstance].ndcTransform._41 = 0.0f;
	instances[countInstance].ndcTransform._12 = w * size.y * -s;
	instances[countInstance].ndcTransform._22 = h * size.y * c;
	instances[countInstance].ndcTransform._32 = 0.0f;
	instances[countInstance].ndcTransform._42 = 0.0f;
	instances[countInstance].ndcTransform._13 = 0.0f;
	instances[countInstance].ndcTransform._23 = 0.0f;
	instances[countInstance].ndcTransform._33 = 1.0f;
	instances[countInstance].ndcTransform._43 = 0.0f;
	instances[countInstance].ndcTransform._14 = w * ( -cx * c + -cy * -s + cx + pos.x ) - 1.0f;
	instances[countInstance].ndcTransform._24 = h * ( -cx * s + -cy * c + cy + pos.y ) + 1.0f;
	instances[countInstance].ndcTransform._34 = 0.0f;
	instances[countInstance].ndcTransform._44 = 1.0f;

	float tw = texSize.x;
	float th = texSize.y;
	if ( texSize.x <= 0.0f || texSize.y <= 0.0f )
	{
		tw = static_cast<float>( texture2dDesc.Width );
		th = static_cast<float>( texture2dDesc.Height );
	}

	float texWidth = static_cast<float>( texture2dDesc.Width );
	float texHeight = static_cast<float>( texture2dDesc.Height );

	instances[countInstance].texcoordTransform = DirectX::XMFLOAT4( texPos.x / texWidth, texPos.y / texHeight, tw / texWidth, th / texHeight );
	instances[countInstance].color = color;

	countInstance++;

}

void SpriteBatch::Draw(TexData texData, float angle, DirectX::XMFLOAT4 color)
{
	Draw(texData.pos, texData.size, texData.texPos, texData.texSize, angle, color);
}

void SpriteBatch::Draw2(float dx, float dy, float dw, float dh, float angle/*degree*/, float r, float g, float b, float a, DirectX::XMFLOAT4 inf)
{

	assert(countInstance < MAX_INSTANCES && "MAX_INSTANCES <= countInstance");

	float cx = dw * 0.5f, cy = dh * 0.5f; /*Center of Rotation*/
#if 0
#else//À•W•ÏŠ·
	FLOAT c = cosf(angle * 0.01745f);
	FLOAT s = sinf(angle * 0.01745f);
	FLOAT w = 2.0f / viewport.Width;
	FLOAT h = -2.0f / viewport.Height;
	instances[countInstance].ndcTransform._11 = w * dw * c;
	instances[countInstance].ndcTransform._21 = h * dw * s;
	instances[countInstance].ndcTransform._31 = 0.0f;
	instances[countInstance].ndcTransform._41 = 0.0f;
	instances[countInstance].ndcTransform._12 = w * dh * -s;
	instances[countInstance].ndcTransform._22 = h * dh * c;
	instances[countInstance].ndcTransform._32 = 0.0f;
	instances[countInstance].ndcTransform._42 = 0.0f;
	instances[countInstance].ndcTransform._13 = 0.0f;
	instances[countInstance].ndcTransform._23 = 0.0f;
	instances[countInstance].ndcTransform._33 = 1.0f;
	instances[countInstance].ndcTransform._43 = 0.0f;
	instances[countInstance].ndcTransform._14 = w * (-cx * c + -cy * -s + cx + dx) - 1.0f;
	instances[countInstance].ndcTransform._24 = h * (-cx * s + -cy * c + cy + dy) + 1.0f;
	instances[countInstance].ndcTransform._34 = 0.0f;
	instances[countInstance].ndcTransform._44 = 1.0f;
#endif
	//texcood
	float tw = static_cast<float>(texture2dDesc.Width);
	float th = static_cast<float>(texture2dDesc.Height);
	instances[countInstance].texcoordTransform = DirectX::XMFLOAT4(inf.x / tw, inf.y / th, inf.z / tw, inf.w / th);
	instances[countInstance].color = DirectX::XMFLOAT4(r, g, b, a);

	countInstance++;

}


void SpriteBatch::End()
{

// Get Context *******************************************************************

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

// *******************************************************************************

// Draw *************************************************************************

	immediateContext->Unmap(instanceBuffer.Get(), 0);

	immediateContext->DrawInstanced(4, countInstance, 0, 0);

	//count_instance = 0;

// *******************************************************************************

}