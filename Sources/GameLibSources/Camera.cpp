#include "camera.h"
#include "framework.h"
#include "InputDevice.h"


void Camera::Init( const DirectX::XMFLOAT3& _pos, const DirectX::XMFLOAT3& _target )
{
	pos    = originPos = _pos;
	target = originTarget = _target;

	up     = originUp;
	right  = originRight;

	float x = target.x - pos.x;
	float y = target.y - pos.y;
	float z = target.z - pos.z;

	distance = sqrtf(x * x + y * y + z * z);

	fov = DirectX::XMConvertToRadians(30.0f);
	float	aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	//SetOrthographicMatrix(50.0f, 50.0f, 0.1f, 100000.0f);
	SetPerspectiveMatrix(fov, aspect, 0.1f, 5000.0f);

	rotateX = 0.0f;
	rotateY = 0.0f;

	angleY = 0.0f;

	foward.x = target.x - pos.x;
	foward.y = target.y - pos.y;
	foward.z = target.z - pos.z;

	updateLook = false;
	updateNow = false;
}


DirectX::XMMATRIX	Camera::SetOrthographicMatrix(float w, float h, float znear, float zfar)
{
	projection = DirectX::XMMatrixOrthographicLH(w, h, znear, zfar);

	return	projection;
}


DirectX::XMMATRIX	Camera::SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar)
{
	projection = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, znear, zfar);

	return	projection;
}


DirectX::XMMATRIX	Camera::GetViewMatrix()
{
	if (!updateLook)
	{
		DirectX::XMVECTOR _pos = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
		DirectX::XMVECTOR _target = DirectX::XMVectorSet(target.x, target.y, target.z, 1.0f);
		DirectX::XMVECTOR _up = DirectX::XMVectorSet(up.x, up.y, up.z, 0.0f);

		view = DirectX::XMMatrixLookAtLH(_pos, _target, _up);
	}

	return	view;
}