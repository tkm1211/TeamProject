#pragma once

#include <cassert>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>
#include <vector>

#define PAD_NUM_MAX 4

#define XINPUT_DPAD_UP			0x0001
#define XINPUT_DPAD_DOWN		0x0002
#define XINPUT_DPAD_LEFT		0x0004
#define XINPUT_DPAD_RIGHT		0x0008
#define XINPUT_START			0x0010
#define XINPUT_BACK				0x0020
#define XINPUT_LEFT_THUMB		0x0040
#define XINPUT_RIGHT_THUMB		0x0080
#define XINPUT_LEFT_SHOULDER	0x0100
#define XINPUT_RIGHT_SHOULDER	0x0200
#define XINPUT_A				0x1000
#define XINPUT_B				0x2000
#define XINPUT_X				0x4000
#define XINPUT_Y				0x8000


struct XINPUT
{
	// state
	int bAs; // val : 0 or 1
	int bBs; // val : 0 or 1
	int bXs; // val : 0 or 1
	int bYs; // val : 0 or 1

	int bRBs; // val : 0 or 1
	int bLBs; // val : 0 or 1
	int bRTs; // val : 0 ~ 255
	int bLTs; // val : 0 ~ 255

	int    bUPs; // val : 0 or 1
	int  bDOWNs; // val : 0 or 1
	int bRIGHTs; // val : 0 or 1
	int  bLEFTs; // val : 0 or 1

	int bSTARTs; // val : 0 or 1
	int  bBACKs; // val : 0 or 1

	int bR3s; // val : 0 or 1
	int bL3s; // val : 0 or 1

	// trg
	int bAt; // val : 0 or 1
	int bBt; // val : 0 or 1
	int bXt; // val : 0 or 1
	int bYt; // val : 0 or 1

	int bRBt; // val : 0 or 1
	int bLBt; // val : 0 or 1
	int bRTt; // val : 0 or 1
	int bLTt; // val : 0 or 1

	int    bUPt; // val : 0 or 1
	int  bDOWNt; // val : 0 or 1
	int bRIGHTt; // val : 0 or 1
	int  bLEFTt; // val : 0 or 1

	int bSTARTt; // val : 0 or 1
	int  bBACKt; // val : 0 or 1

	int bR3t; // val : 0 or 1
	int bL3t; // val : 0 or 1

	// stick
	int sLX; // val : -1000 ~ 1000 (-1000 : LEFT , 1000 : RIGHT)
	int sLY; // val : -1000 ~ 1000 (-1000 : UP   , 1000 : DOWN)
	int sRX; // val : -1000 ~ 1000 (-1000 : LEFT , 1000 : RIGHT)
	int sRY; // val : -1000 ~ 1000 (-1000 : UP   , 1000 : DOWN)

	// vibration
	bool isVibration;
	int rVib;
	int lVib;
	int vibCnt;
	int vibMaxCnt;

	// button cnt
	int cnt[16] = { 0 };
};

extern XINPUT xInput[PAD_NUM_MAX];
extern XINPUT xInputDummy;


struct DINPUT
{
	// state
	int   bCIRCLEs; // val : 0 or 1
	int    bCROSSs; // val : 0 or 1
	int bTRIANGLEs; // val : 0 or 1
	int     bRECTs; // val : 0 or 1

	int bR1s; // val : 0 or 1
	int bL1s; // val : 0 or 1
	int bR2s; // val : 0 ~ 255
	int bL2s; // val : 0 ~ 255

	int    bUPs; // val : 0 or 1
	int  bDOWNs; // val : 0 or 1
	int bRIGHTs; // val : 0 or 1
	int  bLEFTs; // val : 0 or 1

	int   bSHAREs; // val : 0 or 1
	int bOPTIONSs; // val : 0 or 1

	int bR3s; // val : 0 or 1
	int bL3s; // val : 0 or 1

	// trg
	int   bCIRCLEt; // val : 0 or 1
	int    bCROSSt; // val : 0 or 1
	int bTRIANGLEt; // val : 0 or 1
	int     bRECTt; // val : 0 or 1

	int bR1t; // val : 0 or 1
	int bL1t; // val : 0 or 1
	int bR2t; // val : 0 or 1
	int bL2t; // val : 0 or 1

	int    bUPt; // val : 0 or 1
	int  bDOWNt; // val : 0 or 1
	int bRIGHTt; // val : 0 or 1
	int  bLEFTt; // val : 0 or 1

	int   bSHAREt; // val : 0 or 1
	int bOPTIONSt; // val : 0 or 1

	int bR3t; // val : 0 or 1
	int bL3t; // val : 0 or 1

	// stick
	int sLX;
	int sLY;
	int sRX;
	int sRY;

	// button cnt
	int cnt[16] = { 0 };
};

extern DINPUT dInput[PAD_NUM_MAX];


class Joystick
{
public:
	HWND hwnd;
	HINSTANCE instance;

	int cnt = 0;

	DIDEVCAPS dc;

	LPDIRECTINPUT8          lpDI = NULL;
	std::vector<LPDIRECTINPUTDEVICE8>  lpJoystick;

	//std::vector<XINPUT_VIBRATION> vibration;
	//LPDIRECTINPUTDEVICE8    lpJoystick = NULL;      // ジョイスティックデバイス
	//LPDIRECTINPUTEFFECT     lpDIEffect = NULL;
	//DWORD					dwNumForceFeedbackAxis = NULL;		// フォースフィードバック


	//再接続時にも使用
	void InitInputDevice();

	static Joystick& getInstance()
	{
		static Joystick instance;
		return	instance;
	}
};

#define	PAD	Joystick::getInstance()


extern bool GetXInputState(XINPUT *xinput, int _num = 0);         //Xboxコントローラーの入力状態の取得
extern bool GetDInputState(DINPUT *dinput, int _num = 0);         //PS4コントローラーの入力状態の取得
extern void SetXInputVibration(int rVib, int lVib, int _cnt, int _num = 0); //Xboxコントローラーのバイブレーション値の設定


class Mouse
{
public:
	LPDIRECTINPUT8 pDInput = NULL;
	LPDIRECTINPUTDEVICE8 pDIMouse = NULL;

	DIMOUSESTATE diMouseState;
	POINT mousePos;

	bool InitDInput(HINSTANCE hInstApp, HWND hWnd);
	bool ReleaseDInput();

	bool InitDInputMouse(HWND hWnd);
	bool ReleaseDInputMouse();

	void GetMouseState(HWND hWnd);

	static Mouse& getInstance()
	{
		static Mouse instance;
		return	instance;
	}
};

#define MOUSE Mouse::getInstance()