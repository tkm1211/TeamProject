#include <Windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

#include "FrameWork.h"
#include "InputDevice.h"

#pragma comment( lib, "winmm.lib" )


LRESULT CALLBACK FnWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{

	FrameWork *f = reinterpret_cast<FrameWork *>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );

	return f ? f->HandleMessage( hwnd, msg, wparam, lparam ) : DefWindowProc( hwnd, msg, wparam, lparam );

}


INT WINAPI wWinMain( HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, INT cmdShow )
{
	srand(static_cast<unsigned>(time(nullptr)));

#ifdef USE_DEBUG

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(87481);

#endif

	HRESULT hr = CoInitializeEx( 0, COINITBASE_MULTITHREADED );
	assert( !hr );

	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof( WNDCLASSEX );
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= FnWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= instance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= _T( "Phoenix" );
	wcex.hIconSm		= 0;

	RegisterClassEx( &wcex );


	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	HWND hwnd = CreateWindow( _T( "Phoenix" ), _T( "" ),
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME
		| WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, instance, NULL );

	SetMenu(hwnd, NULL);	//メニューを隠す
	//SetWindowLong(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);//ウィンドウのスタイルを変更
	/*MoveWindow(hwnd, GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN), TRUE);*/

	ShowWindow( hwnd, cmdShow );

	DragAcceptFiles(hwnd, TRUE);


	PAD.hwnd = hwnd;
	PAD.instance = instance;

	PAD.InitInputDevice();

	MOUSE.InitDInput( instance, hwnd );
	MOUSE.InitDInputMouse( hwnd );

	FrameWork& f = FrameWork::GetInstance( hwnd );

	SetWindowLongPtr( hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( &f ) );

	return f.run();

}