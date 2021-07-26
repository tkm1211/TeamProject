#pragma once

#include	<DirectXMath.h>

#define PI 3.14159265358979f


class Camera
{
private:
	DirectX::XMFLOAT3	originPos;
	DirectX::XMFLOAT3	originTarget;

	const DirectX::XMFLOAT3	originUp     = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	const DirectX::XMFLOAT3	originRight  = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

public:
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 right;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 foward;

	DirectX::XMFLOAT2 oldCursor;
	DirectX::XMFLOAT2 newCursor;

	float rotateX;
	float rotateY;
	float distance;
	float angleY;
	float fov;

	bool updateLook;
	bool updateNow;

public:
	Camera() {}
	~Camera() {}

	void Init( const DirectX::XMFLOAT3& _pos, const DirectX::XMFLOAT3& _target );

public: // Getä÷êî
	DirectX::XMMATRIX	GetViewMatrix();
	DirectX::XMMATRIX	GetProjectionMatrix(){ return	projection; }
	DirectX::XMFLOAT3	GetPos()	{ return pos;	 }
	DirectX::XMFLOAT3	GetTarget() { return target; }
	DirectX::XMFLOAT3	GetFoward() { return foward; }
	float GetRotateX()	{ return rotateX; }
	float GetRotateY()	{ return rotateY; }
	float GetAngleY()	{ return angleY;  }
	float GetFov()		{ return fov;	  }

	bool GetUpdateLook() { return updateLook; }
	bool GetUpdateNow() { return updateNow; }

public: // Setä÷êî
	DirectX::XMMATRIX	SetOrthographicMatrix(float w, float h, float znear, float zfar);
	DirectX::XMMATRIX	SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar);

	void Set( DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _target, DirectX::XMFLOAT3 _up ) { pos = _pos; target = _target; up = _up; }
	void SetTarget(DirectX::XMFLOAT3 _target) { target = _target; }
	void SetRotateX( float _rotateX ) { rotateX = _rotateX; }
	void SetRotateY( float _rotateY ) { rotateY = _rotateY; }
	void SetPos(DirectX::XMFLOAT3 _pos) { pos = _pos; }
	void SetUpdateLook(bool _updateLook) { updateLook = _updateLook; }

	void SetViewMatrix(const DirectX::XMMATRIX& _view) { view = _view; }
	void SetProjectionMatrix(const DirectX::XMMATRIX& _projection) { projection = _projection; }
};