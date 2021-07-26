#include <map>
#include <DirectXMath.h>
#include "Billboard.h"


void Billboard::Init(ID3D11Device* device)
{

	HRESULT hr = S_OK;

	Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, +0.5f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
	{ DirectX::XMFLOAT3(+0.5f, +0.5f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
	{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(+0.5f, -0.5f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;

	hr = device->CreateBuffer(&bufferDesc, &subresourceData, vertexBuffer.GetAddressOf());
	assert(!hr && "CreateBuffer		Error");


	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(CBuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	assert(!hr && "CreateBuffer	Error");


	//ラスタライザーステートオブジェクト作成************************************
	//ワイヤーフレーム
	D3D11_RASTERIZER_DESC rasterizer_desc = {};
	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.AntialiasedLineEnable = TRUE;
	hr = device->CreateRasterizerState(&rasterizer_desc, wireframeRasterizerState.GetAddressOf());
	assert(!hr && "CreateRasterizerState	Error");

	//ソリッド
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthClipEnable = TRUE;
	hr = device->CreateRasterizerState(&rasterizer_desc, solidRasterizerState.GetAddressOf());
	assert(!hr && "CreateRasterizerState	Error");
	//**************************************************************************

	//深度ステンシル ステート オブジェクト**************************************
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
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
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
	assert(!hr && "CreateDepthStencilState		Error");
	//**************************************************************************

	// Create Sampler **********************************************************

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

	// *************************************************************************

}

//void Billboard::CreateBoard( ID3D11Device* device )
//{
//
//	Vertex vertices[4] = {};
//	unsigned int indices[3 * 2] = {};
//
//
//	vertices[0].pos = DirectX::XMFLOAT3( -0.5f, +0.5f, 0.0f );
//	vertices[1].pos = DirectX::XMFLOAT3( +0.5f, +0.5f, 0.0f );
//	vertices[2].pos = DirectX::XMFLOAT3( -0.5f, -0.5f, 0.0f );
//	vertices[3].pos = DirectX::XMFLOAT3( +0.5f, -0.5f, 0.0f );
//
//	vertices[0].normal =
//	vertices[1].normal =
//	vertices[2].normal =
//	vertices[3].normal = DirectX::XMFLOAT3( +0.0f, +0.0f, -1.0f );
//
//	vertices[0].tex = DirectX::XMFLOAT2( 0.0f, 1.0f );
//	vertices[1].tex = DirectX::XMFLOAT2( 1.0f, 1.0f );
//	vertices[2].tex = DirectX::XMFLOAT2( 0.0f, 0.0f );
//	vertices[3].tex = DirectX::XMFLOAT2( 1.0f, 0.0f );
//
//	indices[0] = 0;	indices[1] = 1;	indices[2] = 2;
//	indices[3] = 1;	indices[4] = 3;	indices[5] = 2;
//
//	CreateBuffers( device, vertices, ARRAYSIZE( vertices ), indices, ARRAYSIZE( indices ) );
//
//}

void Billboard::Begin(ID3D11DeviceContext* deviceContext)
{
	//頂点バッファのバインド
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	//インデックスバッファのバインド
	//	deviceContext->IASetIndexBuffer( indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0 );

	//プリミティブモードの設定
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//入力レイアウトのバインド
	deviceContext->IASetInputLayout(inputLayout.Get());

	//	ラスタライザーの設定
	deviceContext->RSSetState( solidRasterizerState.Get() );

	//シェーダー(2種)の設定
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	//深度テストの設定
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
}

void Billboard::Render
(ID3D11DeviceContext* deviceContext,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT3& position,
	float tx,
	float ty,
	float tw,
	float th,
	const DirectX::XMFLOAT3& angle,
	const DirectX::XMFLOAT2& scale,
	const DirectX::XMFLOAT4& color)
{
	HRESULT hr = S_OK;

	D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	hr = deviceContext->Map(vertexBuffer.Get(), 0, map, 0, &mappedBuffer);
	assert(!hr);

	Vertex* vertices = static_cast<Vertex*>(mappedBuffer.pData);

	vertices[0].pos = DirectX::XMFLOAT3(-0.5f, +0.5f, 0.0f);
	vertices[1].pos = DirectX::XMFLOAT3(+0.5f, +0.5f, 0.0f);
	vertices[2].pos = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertices[3].pos = DirectX::XMFLOAT3(+0.5f, -0.5f, 0.0f);

	vertices[0].tex.x = (tx) / texture2dDesc.Width;
	vertices[0].tex.y = (ty) / texture2dDesc.Height;
	vertices[1].tex.x = (tx + tw) / texture2dDesc.Width;
	vertices[1].tex.y = (ty) / texture2dDesc.Height;
	vertices[2].tex.x = (tx) / texture2dDesc.Width;
	vertices[2].tex.y = (ty + th) / texture2dDesc.Height;
	vertices[3].tex.x = (tx + tw) / texture2dDesc.Width;
	vertices[3].tex.y = (ty + th) / texture2dDesc.Height;

	deviceContext->Unmap(vertexBuffer.Get(), 0);

	//定数バッファのバインド
	//定数バッファの作成
	CBuffer cb;
#if 0
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, T;

	// Extract camera's position and direction from view matrix.
	DirectX::XMVECTOR P;	// camera_position
	DirectX::XMMATRIX M;

	DirectX::XMMATRIX V = view;
	DirectX::XMMATRIX C = XMMatrixTranspose(V);
	P = DirectX::XMVectorSetW(DirectX::XMVector3Transform(DirectX::XMVectorNegate(V.r[3]), C), 1);

	bool fixed_up_vector = true;
	DirectX::XMVECTOR Y = fixed_up_vector ? DirectX::XMVectorSet(0, 1, 0, 0) : DirectX::XMVector3Normalize(C.r[1]);
	DirectX::XMVECTOR Z = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(P, XMLoadFloat3(&position)));
	DirectX::XMVECTOR X = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Y, Z));
	Y = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Z, X));

	M = DirectX::XMMatrixIdentity();
	M.r[0] = DirectX::XMVectorSetW(X, 0);
	M.r[1] = DirectX::XMVectorSetW(Y, 0);
	M.r[2] = DirectX::XMVectorSetW(Z, 0);
	M.r[3] = DirectX::XMVectorSetW(DirectX::XMLoadFloat3(&position), 1);
	//	初期化
	world = DirectX::XMMatrixIdentity();

	//	拡大・縮小
	float aspect_ratio = static_cast<float>(texture2dDesc.Width) / texture2dDesc.Height;
	S = DirectX::XMMatrixScaling(scale.x * aspect_ratio, scale.y, 1);

	//	回転
	R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//	平行移動
	T = M;

	//	ワールド変換行列
	world = S * R * T;

	DirectX::XMStoreFloat4x4(&cb.wvp, world * view * projection);
	cb.color = color;

#else
	DirectX::XMMATRIX W, S, R, T;

	float aspect_ratio = static_cast<float>(texture2dDesc.Width) / texture2dDesc.Height;
	S = DirectX::XMMatrixScaling(scale.x * aspect_ratio, scale.y, 1);

	R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	DirectX::XMMATRIX C = XMMatrixTranspose(view);
	T = DirectX::XMMatrixIdentity();
	T.r[0] = DirectX::XMVectorSetW(DirectX::XMVectorNegate(C.r[0]), 0);
	T.r[1] = DirectX::XMVectorSetW(C.r[1], 0);
	T.r[2] = DirectX::XMVectorSetW(DirectX::XMVectorNegate(C.r[2]), 0);
	T.r[3] = DirectX::XMVectorSetW(DirectX::XMLoadFloat3(&position), 1);

	//	ワールド変換行列
	W = S * R * T;

	DirectX::XMStoreFloat4x4(&cb.wvp, W * view * projection);
	cb.color = color;

#endif

	//
	//DirectX::XMFLOAT4X4 v;
	//DirectX::XMStoreFloat4x4(&v, view);
	//DirectX::XMMATRIX billboard = DirectX::XMMatrixSet(
	//	v._11, v._21, v._31, 0.0f,
	//	v._12, v._22, v._32, 0.0f,
	//	v._13, v._23, v._33, 0.0f,
	//	position.x, position.y, position.z, 1.0f
	//	);

	//C.r[0] = DirectX::XMVectorNegate(C.r[0]);
	//T.r[0] = DirectX::XMVectorSetW(C.r[0], 0);
	//T.r[1] = DirectX::XMVectorSetW(C.r[1], 0);
	//C.r[2] = DirectX::XMVectorNegate(C.r[2]);
	//T.r[2] = DirectX::XMVectorSetW(C.r[2], 0);
	//T.r[3] = DirectX::XMVectorSetW(DirectX::XMLoadFloat3(&position), 1);

	deviceContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
	deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	//プリミティブの描画
	deviceContext->Draw(4, 0);
}

void Billboard::End()
{

}

void Billboard::CreateBuffers(ID3D11Device *device, Vertex *vertices, int numVertices, u_int *indices, int numIndices)
{

	HRESULT hr;


	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(Vertex) * numVertices;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;
	subresourceData.SysMemPitch = 0; //Not use for vertex buffers.mm 
	subresourceData.SysMemSlicePitch = 0; //Not use for vertex buffers.

	hr = device->CreateBuffer(&bufferDesc, &subresourceData, vertexBuffer.ReleaseAndGetAddressOf());
	assert(!hr && "CreateBuffer	Error");


	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(u_int) * numIndices;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	ZeroMemory(&subresourceData, sizeof(subresourceData));
	subresourceData.pSysMem = indices;
	subresourceData.SysMemPitch = 0; //Not use for index buffers.
	subresourceData.SysMemSlicePitch = 0; //Not use for index buffers.

	numIndex = numIndices;

	hr = device->CreateBuffer(&bufferDesc, &subresourceData, indexBuffer.ReleaseAndGetAddressOf());
	assert(!hr && "CreateBuffer	Error");


	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(CBuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	assert(!hr && "CreateBuffer	Error");

}