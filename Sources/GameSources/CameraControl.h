#pragma once
#include "Camera.h"


class CameraControl
{
public:
	static void MouseControlUpdate( Camera* camera );
	static void PadControlUpdate( Camera* camera );
	static void CameraRotation( Camera* camera );
};