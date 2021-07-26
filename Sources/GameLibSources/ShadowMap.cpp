#include "ShadowMap.h"
#include "FrameWork.h"
#include "Texture.h"
#include "Blender.h"
#include "Light.h"
#include "CameraSystem.h"


void ShadowMap::Init()
{
	Microsoft::WRL::ComPtr<ID3D11Device> device = FrameWork::GetInstance().GetDevice();

	pTex = std::make_unique<Sprite>();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;
	Texture::CreateTexture(device.Get(), 1280, 720, DXGI_FORMAT_R16_FLOAT, shaderResourceView.GetAddressOf(), Texture2D.GetAddressOf());

	pTex->SetShaderResourceView(shaderResourceView.Get());


	//	レンダーターゲットビュー作成
	HRESULT hr = S_OK;
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(rtvd));
	rtvd.Format = DXGI_FORMAT_R16_FLOAT;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(Texture2D.Get(), &rtvd, renderTargetView.GetAddressOf());
	assert( !hr );


	//	サンプラステート作成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//ボーダーカラー
	sd.BorderColor[0] = 1.0f;
	sd.BorderColor[1] = 1.0f;
	sd.BorderColor[2] = 1.0f;
	sd.BorderColor[3] = 1.0f;

	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sd, sampler.GetAddressOf());
	assert( !hr );


	//デプスステンシルステート
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
	depth_stencil_desc.DepthEnable = FALSE;
	hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[DS_FALSE].GetAddressOf());
	assert( !hr );

	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
	depth_stencil_desc.DepthEnable = TRUE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[DS_TRUE].GetAddressOf());
	assert( !hr );
}

void ShadowMap::Update()
{

}

void ShadowMap::Draw()
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView = FrameWork::GetInstance().GetDepthStencilView();

	immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView.Get());

	float color[4] = { 1,1,1,1 };
	immediateContext->ClearRenderTargetView(renderTargetView.Get(), color);
	immediateContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	immediateContext->OMSetDepthStencilState(depthStencilState[DS_TRUE].Get(), 1);

	SetBlenderMode(BM_ALPHA);

	DirectX::XMFLOAT3 t(0, 0, 0);
	DirectX::XMFLOAT3 p;

	p.x = t.x - Light::GetInstance()->lightDir.x * 30.0f;
	p.y = t.y - Light::GetInstance()->lightDir.y * 30.0f;
	p.z = t.z - Light::GetInstance()->lightDir.z * 30.0f;

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)1280;
	vp.Height = (FLOAT)720;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	immediateContext->RSSetViewports(1, &vp);

	CameraSystem::GetInstance()->mainView.Set(p, DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	CameraSystem::GetInstance()->mainView.SetOrthographicMatrix(10.0f, 10.0f, 1.0f, 100.0f);

	DirectX::XMStoreFloat4x4(&lightViewProjection, CameraSystem::GetInstance()->mainView.GetViewMatrix() * CameraSystem::GetInstance()->mainView.GetProjectionMatrix());


}