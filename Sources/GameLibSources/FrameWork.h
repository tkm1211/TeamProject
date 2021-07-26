#pragma once

#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include <memory>
#include <cassert>

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

#include <SceneManager.h>

#include "Misc.h"
#include "HighResolutionTimer.h"

#include "Sprite.h"
#include "OBJ3D.h"

#include "AssetLoader.h"
#include "InputDevice.h"



#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )


#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	 720
#define DEBUG_MODE		( defined( DEBUG ) || defined( _DEBUG ) )


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);


class FrameWork
{
public:
	CONST HWND hwnd;

public:
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState[8];

	float elapsedTime = 0.0f;

	//RasterizerState
	enum RS_State { RS_CULL_BACK_FALSE, RS_WIRE_FALSE, RS_CULL_FRONT_FALSE, RS_CULL_NONE_FALSE,
		   RS_CULL_BACK_TRUE , RS_WIRE_TRUE , RS_CULL_FRONT_TRUE , RS_CULL_NONE_TRUE  };

public:
	FrameWork( HWND hwnd ) : hwnd( hwnd )
	{
		
	}
	~FrameWork()
	{
		
	}

	static FrameWork& GetInstance( HWND hwnd = nullptr )
	{

		static FrameWork instance( hwnd );
		return instance;

	}

	int run()
	{

		MSG msg = {};

		if ( !Initialize() ) return 0;

#if DEBUG_MODE

		ImGui::SetCurrentContext(ImGui::CreateContext());
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device.Get(), immediateContext.Get());
		ImGui::StyleColorsDark();

		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, glyphRangesJapanese);
		//io.Fonts->AddFontFromFileTTF(".\\consolab.ttf", 10.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//io.Fonts->AddFontFromFileTTF(".\\Inconsolata-Bold.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

#endif // DEBUG_MODE

		while ( WM_QUIT != msg.message )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				timer.Tick();
				CalculateFrameStats();
				elapsedTime = timer.TimeInterval();
				MOUSE.GetMouseState( hwnd );
				Update( elapsedTime );
				Render( elapsedTime );
			}
		}

#if DEBUG_MODE

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

#endif // DEBUG_MODE

		MOUSE.ReleaseDInputMouse();
		MOUSE.ReleaseDInput();

		UnInit();

		return static_cast<int>( msg.wParam );

	}

	LRESULT CALLBACK HandleMessage( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		{
			return 1;
		}

		switch ( msg )
		{
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint( hwnd, &ps );
			EndPaint( hwnd, &ps );
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if ( wparam == VK_ESCAPE )  PostMessage( hwnd, WM_CLOSE, 0, 0 );
			break;
		case WM_ENTERSIZEMOVE:
			timer.Stop();
			break;
		case WM_EXITSIZEMOVE:
			timer.Start();
			break;
		case WM_DROPFILES:
			HDROP hdrop;
			WCHAR filename[MAX_PATH];
			int num, i;

			hdrop = ( HDROP )wparam;

			// ドロップされたファイルの個数を得る
			num = DragQueryFile( hdrop, -1, NULL, 0 );

			// ファイルを処理
			for ( i = 0; i < num; i++ )
			{
				DragQueryFile( hdrop, i, filename, sizeof( filename ) );

				// filename にファイル名が入っているので、ここで処理を行う。
				assetLoader.Loader( filename );
			}
			DragFinish( hdrop );
			break;
		default:
			return DefWindowProc( hwnd, msg, wparam, lparam );
		}

		return 0;

	}
private:
	bool Initialize( bool enableMsaa = false, int subSamples = 1 );
	void Update( float elapsedTime );
	void Render( float elapsedTime );
	void UnInit();

private:
	HighResolutionTimer timer;
	void CalculateFrameStats()
	{

		static int frames = 0;
		static float timeTlapsed = 0.0f;

		frames++;

		if ( ( timer.timeStamp() - timeTlapsed ) >= 1.0f )
		{
			float fps = static_cast<float>( frames );
			float mspf = 1000.0f / fps;
			std::ostringstream outs;
			outs.precision( 6 );
			outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
			SetWindowTextA( hwnd, outs.str().c_str() );

			frames = 0;
			timeTlapsed += 1.0f;
		}

	}

public:
	inline static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice()						{ return FrameWork::GetInstance().device; };
	inline static Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext()				{ return FrameWork::GetInstance().immediateContext; };
	inline static Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView()	{ return FrameWork::GetInstance().depthStencilView; };
	inline static Microsoft::WRL::ComPtr<ID3D11RasterizerState> GetRasterizerState(int state) { return FrameWork::GetInstance().rasterizerState[state]; }
	float GetElapsedTime() { return elapsedTime; }
};

void SetRasterizerState(int state);