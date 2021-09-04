#pragma once
#include "Camera.h"


class CameraControl
{
public:
	static void MouseControlUpdate( Camera* camera );
	static void PadControlUpdate( Camera* camera );
	static void CameraRotation( Camera* camera );


private:
	DirectX::XMFLOAT3   target = { 0, 0, 0 };
	DirectX::XMFLOAT3   angle = { 0, 0, 0 };
	float               rollSpeed = DirectX::XMConvertToRadians(90);


};