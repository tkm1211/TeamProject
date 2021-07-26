#pragma warning( disable : 4996 )

#include <stdio.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
#include <dinput.h>

#include "InputDevice.h"

#include <Xinput.h>
#pragma comment (lib, "xinput.lib")

// ���C�u���������N
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


XINPUT xInput[PAD_NUM_MAX];
DINPUT dInput[PAD_NUM_MAX];

XINPUT xInputDummy;

#define DEADZONE 150

// #define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
// #define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
// #define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30


// 1�̃f�o�C�X���ƂɌĂяo�����R�[���o�b�N�֐�
BOOL PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	//DEBUG("�R�[���o�b�N�Ăяo��\n");

	LPDIRECTINPUTDEVICE8   _lpJoystick = NULL;

	// �W���C�X�e�B�b�N�f�o�C�X�̍쐬
	HRESULT ret = PAD.lpDI->CreateDevice(lpddi->guidInstance, &_lpJoystick, NULL);
	if (FAILED(ret)) {
		//DEBUG("�f�o�C�X�쐬���s\n");
		return DIENUM_STOP;
	}

	// ���̓f�[�^�`���̃Z�b�g
	ret = _lpJoystick->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(ret)) {
		//DEBUG("���̓f�[�^�`���̃Z�b�g���s\n");
		_lpJoystick->Release();
		return DIENUM_STOP;
	}

	// �r������̃Z�b�g
	ret = _lpJoystick->SetCooperativeLevel(PAD.hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(ret)) {
		//DEBUG("�r������̃Z�b�g���s\n");
		_lpJoystick->Release();
		return DIENUM_STOP;
	}

	// ���͔͈͂̃Z�b�g
	DIPROPRANGE	diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMax = 1000;
	diprg.lMin = -1000;

	// X��
	diprg.diph.dwObj = DIJOFS_X;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Y��
	diprg.diph.dwObj = DIJOFS_Y;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Z��
	diprg.diph.dwObj = DIJOFS_Z;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RX��
	diprg.diph.dwObj = DIJOFS_RX;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RY��
	diprg.diph.dwObj = DIJOFS_RY;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RZ��
	diprg.diph.dwObj = DIJOFS_RZ;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// �N����������
	_lpJoystick->Poll();

	// �\�z�����Ȃ�
	char inputDeviceName[260];
	WideCharToMultiByte(CP_ACP, 0, lpddi->tszProductName, -1, inputDeviceName, sizeof(inputDeviceName), NULL, NULL);

	if (strcmp(inputDeviceName, "Controller (Xbox One For Windows)") != 0)
	{
		PAD.lpJoystick.emplace_back(_lpJoystick);
	}

	// �ŏ���1�݂̂ŏI���
	return DIENUM_CONTINUE;			// ���̃f�o�C�X��񋓂���ɂ�DIENUM_CONTINUE��Ԃ�
}

void Joystick::InitInputDevice()
{
	lpJoystick.clear();

	// IDirectInput8�̍쐬
	HRESULT ret = DirectInput8Create(PAD.instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDI, NULL);
	if (FAILED(ret)) {
		// �쐬�Ɏ��s
		assert(!ret && "�쐬�Ɏ��s");
	}

	// �W���C�X�e�B�b�N�̗�
	ret = lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyDeviceProc, NULL, /*DIEDFL_FORCEFEEDBACK |*/ DIEDFL_ATTACHEDONLY);
	if (FAILED(ret)) {
		lpDI->Release();
		assert(!ret && "�񋓂Ɏ��s");
	}

	for (auto& it : lpJoystick)
	{
		if (!it)
		{
			// �W���C�X�e�B�b�N��1��������Ȃ�
			//MessageBoxW(PAD.hwnd, L"�W���C�X�e�B�b�N��1���ڑ�����Ă��܂���", L"�G���[", MB_OK | MB_ICONHAND);
			lpDI->Release();
			//assert(!ret && "�W���C�X�e�B�b�N��1��������Ȃ�");
		}
		else
		{
			// �f�o�C�X���
			dc.dwSize = sizeof(dc);
			it->GetCapabilities(&dc);

			// ����J�n
			it->Acquire();
		}
	}
}

bool GetXInputState(XINPUT *xinput, int _num)
{
	// �Q�[���p�b�h�̏�Ԃ��擾
	XINPUT_STATE state;
	DWORD connection = XInputGetState( _num, &state );

	if (connection != 0)
	{
		xinput = &xInputDummy;
		return false;
	}


	//BUTTONS STATE
	xinput->bAs = ( state.Gamepad.wButtons & XINPUT_A ) ? 1 : 0;
	xinput->bBs = ( state.Gamepad.wButtons & XINPUT_B ) ? 1 : 0;
	xinput->bXs = ( state.Gamepad.wButtons & XINPUT_X ) ? 1 : 0;
	xinput->bYs = ( state.Gamepad.wButtons & XINPUT_Y ) ? 1 : 0;

	xinput->bLBs = ( state.Gamepad.wButtons & XINPUT_LEFT_SHOULDER  ) ? 1 : 0;
	xinput->bRBs = ( state.Gamepad.wButtons & XINPUT_RIGHT_SHOULDER ) ? 1 : 0;

	xinput->bSTARTs = ( state.Gamepad.wButtons & XINPUT_START ) ? 1 : 0;
	xinput->bBACKs  = ( state.Gamepad.wButtons & XINPUT_BACK  ) ? 1 : 0;

	xinput->bL3s = ( state.Gamepad.wButtons & XINPUT_LEFT_THUMB  ) ? 1 : 0;
	xinput->bR3s = ( state.Gamepad.wButtons & XINPUT_RIGHT_THUMB ) ? 1 : 0;

	xinput->bUPs    = ( state.Gamepad.wButtons & XINPUT_DPAD_UP    ) ? 1 : 0;
	xinput->bDOWNs  = ( state.Gamepad.wButtons & XINPUT_DPAD_DOWN  ) ? 1 : 0;
	xinput->bLEFTs  = ( state.Gamepad.wButtons & XINPUT_DPAD_LEFT  ) ? 1 : 0;
	xinput->bRIGHTs = ( state.Gamepad.wButtons & XINPUT_DPAD_RIGHT ) ? 1 : 0;


	//BUTTONS TRG
	if ( xinput->bAs )
	{
		if (xinput->cnt[0] == 0) xinput->bAt = 1;
		else xinput->bAt = 0;
		xinput->cnt[0]++;
	}
	else
	{
		xinput->bAt = 0;
		xinput->cnt[0] = 0;
	}

	if ( xinput->bBs )
	{
		if ( xinput->cnt[1] == 0 ) xinput->bBt = 1;
		else xinput->bBt = 0;
		xinput->cnt[1]++;
	}
	else
	{
		xinput->bBt = 0;
		xinput->cnt[1] = 0;
	}

	if ( xinput->bXs )
	{
		if ( xinput->cnt[2] == 0 ) xinput->bXt = 1;
		else xinput->bXt = 0;
		xinput->cnt[2]++;
	}
	else
	{
		xinput->bXt = 0;
		xinput->cnt[2] = 0;
	}

	if ( xinput->bYs )
	{
		if ( xinput->cnt[3] == 0 ) xinput->bYt = 1;
		else xinput->bYt = 0;
		xinput->cnt[3]++;
	}
	else
	{
		xinput->bYt = 0;
		xinput->cnt[3] = 0;
	}

	if ( xinput->bLBs )
	{
		if ( xinput->cnt[4] == 0 ) xinput->bLBt = 1;
		else xinput->bLBt = 0;
		xinput->cnt[4]++;
	}
	else
	{
		xinput->bLBt = 0;
		xinput->cnt[4] = 0;
	}

	if ( xinput->bRBs )
	{
		if ( xinput->cnt[5] == 0 ) xinput->bRBt = 1;
		else xinput->bRBt = 0;
		xinput->cnt[5]++;
	}
	else
	{
		xinput->bRBt = 0;
		xinput->cnt[5] = 0;
	}

	if ( xinput->bSTARTs )
	{
		if ( xinput->cnt[6] == 0 ) xinput->bSTARTt = 1;
		else xinput->bSTARTt = 0;
		xinput->cnt[6]++;
	}
	else
	{
		xinput->bSTARTt = 0;
		xinput->cnt[6] = 0;
	}

	if ( xinput->bBACKs )
	{
		if ( xinput->cnt[7] == 0 ) xinput->bBACKt = 1;
		else xinput->bBACKt = 0;
		xinput->cnt[7]++;
	}
	else
	{
		xinput->bBACKt = 0;
		xinput->cnt[7] = 0;
	}

	if ( xinput->bL3s )
	{
		if ( xinput->cnt[8] == 0 ) xinput->bL3t = 1;
		else xinput->bL3t = 0;
		xinput->cnt[8]++;
	}
	else
	{
		xinput->bL3t = 0;
		xinput->cnt[8] = 0;
	}

	if ( xinput->bR3s )
	{
		if ( xinput->cnt[9] == 0 ) xinput->bR3t = 1;
		else xinput->bR3t = 0;
		xinput->cnt[9]++;
	}
	else
	{
		xinput->bR3t = 0;
		xinput->cnt[9] = 0;
	}

	if ( xinput->bUPs )
	{
		if ( xinput->cnt[10] == 0 )xinput->bUPt = 1;
		else xinput->bUPt = 0;
		xinput->cnt[10]++;
	}
	else
	{
		xinput->bUPs = 0;
		xinput->cnt[10] = 0;
	}

	if ( xinput->bDOWNs )
	{
		if ( xinput->cnt[11] == 0 )xinput->bDOWNt = 1;
		else xinput->bDOWNt = 0;
		xinput->cnt[11]++;
	}
	else
	{
		xinput->bDOWNt = 0;
		xinput->cnt[11] = 0;
	}

	if ( xinput->bLEFTs)
	{
		if ( xinput->cnt[13] == 0 )xinput->bLEFTt = 1;
		else xinput->bLEFTt = 0;
		xinput->cnt[13]++;
	}
	else
	{
		xinput->bLEFTt = 0;
		xinput->cnt[13] = 0;
	}

	if ( xinput->bRIGHTs)
	{
		if ( xinput->cnt[12] == 0 )xinput->bRIGHTt = 1;
		else xinput->bRIGHTt = 0;
		xinput->cnt[12]++;
	}
	else
	{
		xinput->bRIGHTt = 0;
		xinput->cnt[12] = 0;
	}


	// �g���K�[
	xinput->bRTs = state.Gamepad.bRightTrigger;
	xinput->bLTs = state.Gamepad.bLeftTrigger;

	if ( xinput->bRTs )
	{
		if ( xinput->cnt[15] == 0 ) xinput->bRTt = 1;
		else xinput->bRTt = 0;
		xinput->cnt[15]++;
	}
	else
	{
		xinput->bRTt = 0;
		xinput->cnt[15] = 0;
	}
	if ( xinput->bLTs )
	{
		if ( xinput->cnt[14] == 0 ) xinput->bLTt = 1;
		else xinput->bLTt = 0;
		xinput->cnt[14]++;
	}
	else
	{
		xinput->bLTt = 0;
		xinput->cnt[14] = 0;
	}

	// �X�e�B�b�N
	//XINPUT_STATE state = g_Controllers[i].state;
	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;
	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY*LY);
	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;
	float normalizedMagnitude = 0.0f;
	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{ 
		//clip the magnitude at its expected maximum value 
		if (magnitude > 32767.0f)
			magnitude = 32767.0f;
		//adjust magnitude relative to the end of the dead zone 
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		normalizedLX = LX / 32767.0f * 1000.0f;
		normalizedLY = LY / 32767.0f * 1000.0f;
		if (-DEADZONE <= normalizedLX && normalizedLX <= DEADZONE)
		{
			normalizedLX = 0.0f;
		}
		if (-DEADZONE <= normalizedLY && normalizedLY <= DEADZONE)
		{
			normalizedLY = 0.0f;
		}
		//optionally normalize the magnitude with respect to its expected range  
		//giving a magnitude value of 0.0 to 1.0 
		normalizedMagnitude = magnitude / (32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{	
		magnitude = 0.0f;
		normalizedLX = 0.0f;
		normalizedLY = 0.0f;
		normalizedMagnitude = 0.0f;
	}//repeat for right thumb stick

	xinput->sLX = static_cast<int>(normalizedLX);
	xinput->sLY = static_cast<int>(-normalizedLY);


	//XINPUT_STATE state = g_Controllers[i].state;
	float RX = state.Gamepad.sThumbRX;
	float RY = state.Gamepad.sThumbRY;
	//determine how far the controller is pushed
	magnitude = sqrt(RX * RX + RY * RY);
	//determine the direction the controller is pushed
	float normalizedRX = RX / magnitude;
	float normalizedRY = RY / magnitude;
	normalizedMagnitude = 0.0f;
	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		//clip the magnitude at its expected maximum value 
		if (magnitude > 32767.0f)
			magnitude = 32767.0f;
		//adjust magnitude relative to the end of the dead zone 
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		normalizedRX = RX / 32767.0f * 1000.0f;
		normalizedRY = RY / 32767.0f * 1000.0f;
		if (-DEADZONE <= normalizedRX && normalizedRX <= DEADZONE)
		{
			normalizedRX = 0.0f;
		}
		if (-DEADZONE <= normalizedRY && normalizedRY <= DEADZONE)
		{
			normalizedRY = 0.0f;
		}
		//optionally normalize the magnitude with respect to its expected range  
		//giving a magnitude value of 0.0 to 1.0 
		normalizedMagnitude = magnitude / (32767.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0f;
		normalizedRX = 0.0f;
		normalizedRY = 0.0f;
		normalizedMagnitude = 0.0f;
	}//repeat for right thumb stick

	xinput->sRX = static_cast<int>(normalizedRX);
	xinput->sRY = static_cast<int>(-normalizedRY);

	
	// �o�C�u���[�V�����X�V
	if (xinput->isVibration)
	{
		if (xinput->vibMaxCnt <= xinput->vibCnt++)
		{
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = 0;  // val:0~65535
			vibration.wRightMotorSpeed = 0; // val:0~65535
			XInputSetState(_num, &vibration);

			xinput->vibCnt = 0;
			xinput->vibMaxCnt = 0;
			xinput->isVibration = false;
		}
	}

	return true;
}

bool GetDInputState(DINPUT *dinput, int _num)
{
	if ((int)(PAD.lpJoystick.size() - 1) != _num) return false;


	// �f�[�^�擾�O��Poll���K�v�Ȃ�
	if (PAD.dc.dwFlags&DIDC_POLLEDDATAFORMAT) {
		PAD.lpJoystick[_num]->Poll();
	}


	// �W���C�X�e�B�b�N�̓���
	DIJOYSTATE joy;
	ZeroMemory(&joy, sizeof(joy));
	HRESULT ret = PAD.lpJoystick[_num]->GetDeviceState(sizeof(joy), &joy);
	if (FAILED(ret)) {
		// ���s�Ȃ�Ăѓ���J�n���s��
		PAD.lpJoystick[_num]->Acquire();
	}


	//STICK
	if (-DEADZONE <= joy.lX  && joy.lX  <= DEADZONE) joy.lX  = 0;
	if (-DEADZONE <= joy.lY  && joy.lY  <= DEADZONE) joy.lY  = 0;
	if (-DEADZONE <= joy.lZ  && joy.lZ  <= DEADZONE) joy.lZ  = 0;
	if (-DEADZONE <= joy.lRz && joy.lRz <= DEADZONE) joy.lRz = 0;

	dinput->sLX = joy.lX;
	dinput->sLY = joy.lY;
	dinput->sRX = joy.lZ;
	dinput->sRY = joy.lRz;


	//BUTTONS STATE
	dinput->bRECTs     = joy.rgbButtons[0] & 0x80 ? 1 : 0;
	dinput->bCROSSs    = joy.rgbButtons[1] & 0x80 ? 1 : 0;
	dinput->bCIRCLEs   = joy.rgbButtons[2] & 0x80 ? 1 : 0;
	dinput->bTRIANGLEs = joy.rgbButtons[3] & 0x80 ? 1 : 0;

	dinput->bL1s = joy.rgbButtons[4] & 0x80 ? 1 : 0;
	dinput->bR1s = joy.rgbButtons[5] & 0x80 ? 1 : 0;

	dinput->bSHAREs   = joy.rgbButtons[8] & 0x80 ? 1 : 0;
	dinput->bOPTIONSs = joy.rgbButtons[9] & 0x80 ? 1 : 0;

	dinput->bL3s = joy.rgbButtons[10] & 0x80 ? 1 : 0;
	dinput->bR3s = joy.rgbButtons[11] & 0x80 ? 1 : 0;

	if (joy.rgdwPOV[0] == 0) dinput->bUPs = 1;
	else dinput->bUPs = 0;
	if (joy.rgdwPOV[0] == 18000) dinput->bDOWNs = 1;
	else dinput->bDOWNs = 0;
	if (joy.rgdwPOV[0] == 9000) dinput->bRIGHTs = 1;
	else dinput->bRIGHTs = 0;
	if (joy.rgdwPOV[0] == 27000) dinput->bLEFTs = 1;
	else dinput->bLEFTs = 0;

#if 0
	dinput->bR2s = joy.lRx;
	dinput->bL2s = joy.lRy;
#else
	dinput->bR2s = static_cast<int>((static_cast<float>(joy.lRx) + 1000.0f) / 2000.0f * 255.0f);
	dinput->bL2s = static_cast<int>((static_cast<float>(joy.lRy) + 1000.0f) / 2000.0f * 255.0f);
#endif

	//BUTTONS TRG
	if ( dinput->bRECTs )
	{
		if ( dinput->cnt[0] == 0 ) dinput->bRECTt = 1;
		else dinput->bRECTt = 0;
		dinput->cnt[0]++;
	}
	else
	{
		dinput->bRECTt = 0;
		dinput->cnt[0] = 0;
	}

	if ( dinput->bCROSSs )
	{
		if (dinput->cnt[1] == 0) dinput->bCROSSt = 1;
		else dinput->bCROSSt = 0;
		dinput->cnt[1]++;
	}
	else
	{
		dinput->bCROSSt = 0;
		dinput->cnt[1] = 0;
	}

	if ( dinput->bCIRCLEs )
	{
		if ( dinput->cnt[2] == 0 ) dinput->bCIRCLEt = 1;
		else dinput->bCIRCLEt = 0;
		dinput->cnt[2]++;
	}
	else
	{
		dinput->bCIRCLEt = 0;
		dinput->cnt[2] = 0;
	}

	if ( dinput->bTRIANGLEs )
	{
		if ( dinput->cnt[3] == 0 ) dinput->bTRIANGLEt = 1;
		else dinput->bTRIANGLEt = 0;
		dinput->cnt[3]++;
	}
	else
	{
		dinput->bTRIANGLEt = 0;
		dinput->cnt[3] = 0;
	}

	if ( dinput->bL1s )
	{
		if ( dinput->cnt[4] == 0 ) dinput->bL1t = 1;
		else dinput->bL1t = 0;
		dinput->cnt[4]++;
	}
	else
	{
		dinput->bL1t = 0;
		dinput->cnt[4] = 0;
	}

	if ( dinput->bR1s )
	{
		if (dinput->cnt[5] == 0) dinput->bR1t = 1;
		else dinput->bR1t = 0;
		dinput->cnt[5]++;
	}
	else
	{
		dinput->bR1t = 0;
		dinput->cnt[5] = 0;
	}

	if ( dinput->bSHAREs )
	{
		if ( dinput->cnt[6] == 0 ) dinput->bSHAREt = 1;
		else dinput->bSHAREt = 0;
		dinput->cnt[6]++;
	}
	else
	{
		dinput->bSHAREt = 0;
		dinput->cnt[6] = 0;
	}

	if ( dinput->bOPTIONSs )
	{
		if ( dinput->cnt[7] == 0 ) dinput->bOPTIONSt = 1;
		else dinput->bOPTIONSt = 0;
		dinput->cnt[7]++;
	}
	else
	{
		dinput->bOPTIONSt = 0;
		dinput->cnt[7] = 0;
	}

	if ( dinput->bL3s )
	{
		if ( dinput->cnt[8] == 0 ) dinput->bL3t = 1;
		else dinput->bL3t = 0;
		dinput->cnt[8]++;
	}
	else
	{
		dinput->bL3t = 0;
		dinput->cnt[8] = 0;
	}

	if ( dinput->bR3s )
	{
		if ( dinput->cnt[9] == 0 ) dinput->bR3t = 1;
		else dinput->bR3t = 0;
		dinput->cnt[9]++;
	}
	else
	{
		dinput->bR3t = 0;
		dinput->cnt[9] = 0;
	}

	if ( dinput->bUPs )
	{
		if ( dinput->cnt[10] == 0 )dinput->bUPt = 1;
		else dinput->bUPt = 0;
		dinput->cnt[10]++;
	}
	else
	{
		dinput->bUPs = 0;
		dinput->cnt[10] = 0;
	}

	if ( dinput->bDOWNs )
	{
		if ( dinput->cnt[11] == 0 ) dinput->bDOWNt = 1;
		else dinput->bDOWNt = 0;
		dinput->cnt[11]++;
	}
	else
	{
		dinput->bDOWNt = 0;
		dinput->cnt[11] = 0;
	}

	if ( dinput->bLEFTs )
	{
		if ( dinput->cnt[13] == 0 ) dinput->bLEFTt = 1;
		else dinput->bLEFTt = 0;
		dinput->cnt[13]++;
	}
	else
	{
		dinput->bLEFTt = 0;
		dinput->cnt[13] = 0;
	}

	if ( dinput->bRIGHTs )
	{
		if ( dinput->cnt[12] == 0 ) dinput->bRIGHTt = 1;
		else dinput->bRIGHTt = 0;
		dinput->cnt[12]++;
	}
	else
	{
		dinput->bRIGHTt = 0;
		dinput->cnt[12] = 0;
	}

	if ( dinput->bR2s )
	{
		if ( dinput->cnt[15] == 0 ) dinput->bR2t = 1;
		else dinput->bR2t = 0;
		dinput->cnt[15]++;
	}
	else
	{
		dinput->bR2t = 0;
		dinput->cnt[15] = 0;
	}

	if ( dinput->bL2s )
	{
		if ( dinput->cnt[14] == 0 ) dinput->bL2t = 1;
		else dinput->bL2t = 0;
		dinput->cnt[14]++;
	}
	else
	{
		dinput->bL2t = 0;
		dinput->cnt[14] = 0;
	}

	return true;
}

void SetXInputVibration(int rVib, int lVib, int _cnt, int _num)
{
	xInput[_num].isVibration = true;
	xInput[_num].rVib = rVib;
	xInput[_num].lVib = lVib;
	xInput[_num].vibMaxCnt = _cnt;

	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = lVib;  // val:0~65535
	vibration.wRightMotorSpeed = rVib; // val:0~65535
	XInputSetState(_num, &vibration);
}

bool GetKeyboardState(int input_num)
{
	if (GetAsyncKeyState(input_num) < 0) return true;
	return false;
}

bool GetKeyboardTrg(int input_num)
{
	bool trg = false;
	if (GetKeyState(input_num) < 0)
	{
		if (PAD.cnt == 0) trg = true;
		PAD.cnt++;
	}
	else
	{
		PAD.cnt = 0;
	}
	return trg;
}


bool Mouse::InitDInput(HINSTANCE hInstApp, HWND hWnd)
{
	HRESULT ret = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& pDInput, NULL);
	if (FAILED(ret)) {
		return false;	// �쐬�Ɏ��s
	}
	return true;
}

bool Mouse::ReleaseDInput()
{
	// DirectInput�̃f�o�C�X���J��
	if (pDInput) {
		pDInput->Release();
		pDInput = NULL;
	}

	return true;
}

bool Mouse::InitDInputMouse(HWND hWnd)
{
	HRESULT ret = S_FALSE;
	if (pDInput == NULL) {
		return false;
	}

	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	ret = pDInput->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
	if (FAILED(ret)) {
		// �f�o�C�X�̍쐬�Ɏ��s
		return false;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	ret = pDIMouse->SetDataFormat(&c_dfDIMouse);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if (FAILED(ret)) {
		// �f�[�^�t�H�[�}�b�g�Ɏ��s
		return false;
	}

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	ret = pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret)) {
		// ���[�h�̐ݒ�Ɏ��s
		return false;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	ret = pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// �f�o�C�X�̐ݒ�Ɏ��s
		return false;
	}

	// ���͐���J�n
	pDIMouse->Acquire();

	return true;
}

bool Mouse::ReleaseDInputMouse()
{
	// DirectInput�̃f�o�C�X���J��
	if (pDIMouse) {
		pDIMouse->Release();
		pDIMouse = NULL;
	}

	return true;
}

void Mouse::GetMouseState(HWND hWnd)
{
	if (pDIMouse == NULL) {
		// �I�u�W�F�N�g�����O�ɌĂ΂ꂽ�Ƃ��͂����Ő���������
		InitDInputMouse(hWnd);
	}

	// �ǎ�O�̒l��ێ����܂�
	DIMOUSESTATE diMouseState_bak;	// �}�E�X���(�ω����m�p)
	memcpy(&diMouseState_bak, &diMouseState, sizeof(diMouseState_bak));

	// ��������ADirectInput�ŕK�v�ȃR�[�h -->
		// �}�E�X�̏�Ԃ��擾���܂�
	HRESULT	hr = pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &diMouseState);
	if (hr == DIERR_INPUTLOST) {
		pDIMouse->Acquire();
		hr = pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &diMouseState);
	}
	// --> �����܂ŁADirectInput�ŕK�v�ȃR�[�h

	GetCursorPos(&mousePos);

	if (memcmp(&diMouseState_bak, &diMouseState, sizeof(diMouseState_bak)) != 0) {
		// �m�F�p�̏����A�������� -->
				// �l���ς������\�����܂�
		/*char buf[128];
		wsprintf(buf, "(%5d, %5d, %5d) %s %s %s\n",
			g_zdiMouseState.lX, g_zdiMouseState.lY, g_zdiMouseState.lZ,
			(g_zdiMouseState.rgbButtons[0] & 0x80) ? "Left" : "--",
			(g_zdiMouseState.rgbButtons[1] & 0x80) ? "Right" : "--",
			(g_zdiMouseState.rgbButtons[2] & 0x80) ? "Center" : "--");
		OutputDebugString(buf);*/
		// --> �����܂ŁA�m�F�p�̏���
	}
}