#include <vector>

#include "framework.h"
#include "Blender.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "Camera.h"
#include "InputDevice.h"


bool FrameWork::Initialize( bool enableMsaa, int subSamples )
{

	HRESULT hr = S_OK;


	// window size
	RECT rc;
	GetClientRect( hwnd, &rc );

	UINT width  = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;


// Create Device *****************************************************************

	UINT flags = 0;

#ifdef USE_DEBUG

	flags |= D3D11_CREATE_DEVICE_DEBUG;

#endif


	// Create DXGIFactor 
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;

	hr = CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast<void**>( dxgiFactory.GetAddressOf() ) );
	assert( !hr && "CreateDXGIFactory	Error" );


	// Adapter
	IDXGIAdapter *dxgiAdapter;
	std::vector<IDXGIAdapter *> dxgiAdapters;

	for ( size_t i = 0; dxgiFactory->EnumAdapters( static_cast<UINT>(i), &dxgiAdapter ) != DXGI_ERROR_NOT_FOUND; i++ )
	{
		dxgiAdapters.push_back( dxgiAdapter );
	}

	dxgiAdapter = dxgiAdapters[0];

	DXGI_ADAPTER_DESC dxgiAdapterDesc;
	dxgiAdapter->GetDesc( &dxgiAdapterDesc );


	// Driver Type
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_UNKNOWN,
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );


	// Feature Level
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	UINT numFeatureLevels = sizeof( featureLevels ) / sizeof( featureLevels[0] );

	D3D_FEATURE_LEVEL featureLevel;


	// Create Device
	for( UINT idx = 0; idx < numDriverTypes; ++idx )
	{
		D3D_DRIVER_TYPE driverType = driverTypes[idx];
		hr = D3D11CreateDevice(
			dxgiAdapter,
			driverType,
			0,
			flags,
			featureLevels,
			_countof( featureLevels ),
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&featureLevel,
			immediateContext.GetAddressOf()
		);
		if ( SUCCEEDED( hr ) )
		{
			break;
		}
	}
	assert( !hr && "D3D11CreateDevice	Error" );

// *******************************************************************************

// Create Swap Chain *************************************************************

	// Set Swap Chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width					 = width;
	swapChainDesc.BufferDesc.Height					 = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator	 = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format					 = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering		 = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling				 = DXGI_MODE_SCALING_UNSPECIFIED;


	// Get Num Quality Level
	UINT msaaQualityLevel;
	UINT sampleCount = subSamples;

	hr = device->CheckMultisampleQualityLevels( swapChainDesc.BufferDesc.Format, sampleCount, &msaaQualityLevel );
	assert( !hr && "CheckMultisampleQualityLevels	Error" );


	// Create Swap Chain
	swapChainDesc.SampleDesc.Count	 = enableMsaa ? sampleCount : 1;
	swapChainDesc.SampleDesc.Quality = enableMsaa ? msaaQualityLevel - 1 : 0;
	swapChainDesc.BufferUsage		 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount		 = 1;
	swapChainDesc.OutputWindow		 = hwnd;
	swapChainDesc.Windowed			 = TRUE;
	swapChainDesc.SwapEffect		 = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags				 = 0;

	hr = dxgiFactory->CreateSwapChain( device.Get(), &swapChainDesc, swapChain.GetAddressOf() );
	assert( !hr && "CreateSwapChain		Error" );

// *******************************************************************************

// Get Back Buffer and Create Render Target View *********************************

	// Get Back Buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	hr = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( backBuffer.GetAddressOf() ) );
	assert( !hr && "GetBuffer	Error" );


	// Create Render Target View
	D3D11_TEXTURE2D_DESC backBufferDesc = {};

	hr = device->CreateRenderTargetView( backBuffer.Get(), NULL, &renderTargetView );
	assert( !hr && "CreateRenderTargetView	Error" );


	// Get Back Buffer Desc
	backBuffer->GetDesc( &backBufferDesc );

// *******************************************************************************

// Create Depth Stensil View *****************************************************

	// Create Texture For Depth Stensil Buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStensilTextrue;

	D3D11_TEXTURE2D_DESC texture2dDesc = backBufferDesc;
	texture2dDesc.Width					= width;
	texture2dDesc.Height				= height;
	texture2dDesc.MipLevels				= 1;
	texture2dDesc.ArraySize				= 1;
	texture2dDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture2dDesc.SampleDesc.Count		= 1;
	texture2dDesc.SampleDesc.Quality	= 0;
	texture2dDesc.Usage					= D3D11_USAGE_DEFAULT;
	texture2dDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
	texture2dDesc.CPUAccessFlags		= 0;
	texture2dDesc.MiscFlags				= 0;

	hr = device->CreateTexture2D( &texture2dDesc, nullptr, depthStensilTextrue.GetAddressOf() );
	assert( !hr && "CreateTexture2D		Error" );


	// Create Depth Stensil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format				= texture2dDesc.Format;
	depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags				= 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView( depthStensilTextrue.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf() );
	assert( !hr && "CreateDepthStencilView		Error" );

// *******************************************************************************

// Set Rasterizer State **********************************************************

	D3D11_RASTERIZER_DESC rd;
	for (int state = 0; state < 8; state++)
	{
		switch (state)
		{
		case RS_CULL_BACK_FALSE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_BACK;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;

			break;

		case RS_WIRE_FALSE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_WIREFRAME;
			rd.CullMode = D3D11_CULL_BACK;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;
			break;

		case RS_CULL_FRONT_FALSE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_FRONT;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;

			break;

		case RS_CULL_NONE_FALSE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_NONE;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;

			break;

		case RS_CULL_BACK_TRUE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_BACK;
			rd.FrontCounterClockwise = TRUE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;

			break;

		case RS_WIRE_TRUE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_WIREFRAME;
			rd.CullMode = D3D11_CULL_BACK;
			rd.FrontCounterClockwise = TRUE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;
			break;

		case RS_CULL_FRONT_TRUE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_FRONT;
			rd.FrontCounterClockwise = TRUE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;

			break;

		case RS_CULL_NONE_TRUE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_NONE;
			rd.FrontCounterClockwise = TRUE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;

			break;
		}
		HRESULT hr = device->CreateRasterizerState(&rd, rasterizerState[state].GetAddressOf());
		//assert(FAILED(hr));

		if (FAILED(hr))
		{
			return false;
		}
	}

// *******************************************************************************

// Set View Port *****************************************************************
	
	D3D11_VIEWPORT viewPort;
	viewPort.Width	  = ( FLOAT )width;
	viewPort.Height   = ( FLOAT )height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	immediateContext->RSSetViewports( 1, &viewPort );

// *******************************************************************************

	SceneManager::GetInstance()->Init();

	return true;

}


void FrameWork::Update( float elapsedTime )
{
	SceneManager::GetInstance()->Update();
	SceneManager::GetInstance()->ImGui();
}


void FrameWork::Render( float elapsedTime )
{

// Clear *************************************************************************

	FLOAT color[4] = { 0.75f,0.75f,0.75f,1.0f };
	immediateContext->ClearRenderTargetView( renderTargetView.Get(), color );

	immediateContext->ClearDepthStencilView( depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0 );

// *******************************************************************************

// Set Target to Depth Stencil View **********************************************

	immediateContext->OMSetRenderTargets( 1, renderTargetView.GetAddressOf(), depthStencilView.Get() );

// *******************************************************************************

// Render ************************************************************************

	SetBlenderMode(BM_ALPHA);
	//static Blender blender(device.Get());
	//immediateContext->OMSetBlendState(blender.states[Blender::BS_ALPHA].Get(), nullptr, 0xFFFFFFFF);

	// ラスタライザ―設定

	//RasterizerState
	//enum { RS_CULL_BACK_FALSE, RS_WIRE_FALSE, RS_CULL_FRONT_FALSE, RS_CULL_NONE_FALSE,
	//		 RS_CULL_BACK_TRUE , RS_WIRE_TRUE , RS_CULL_FRONT_TRUE , RS_CULL_NONE_TRUE  };
	SetRasterizerState(RS_CULL_BACK_TRUE);

	SceneManager::GetInstance()->Render();

	swapChain->Present( 1, 0 );

// *******************************************************************************

}

void FrameWork::UnInit()
{
	SceneManager::GetInstance()->UnInit();
}

void SetRasterizerState(int state)
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

	//RasterizerState
	//enum { RS_CULL_BACK_FALSE, RS_WIRE_FALSE, RS_CULL_FRONT_FALSE, RS_CULL_NONE_FALSE,
	//		 RS_CULL_BACK_TRUE , RS_WIRE_TRUE , RS_CULL_FRONT_TRUE , RS_CULL_NONE_TRUE  };
	immediateContext->RSSetState(FrameWork::GetInstance().GetRasterizerState(state).Get());
}