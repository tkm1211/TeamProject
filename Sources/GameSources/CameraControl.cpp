#include "CameraControl.h"
#include "InputDevice.h"


void CameraControl::MouseControlUpdate( Camera* camera )
{
#if 0
	ImGui::Begin(u8"Camera");

	ImGui::Text("pos.x : %f", pos.x);
	ImGui::Text("pos.y : %f", pos.y);
	ImGui::Text("pos.z : %f", pos.z);

	ImGui::Text("target.x : %f", target.x);
	ImGui::Text("target.y : %f", target.y);
	ImGui::Text("target.z : %f", target.z);
	ImGui::Text("angle : %f", angleY);

	// ImGui::Text("MOUSE.diMouseState.lX : %f", static_cast<float>(MOUSE.diMouseState.lX));
	// ImGui::Text("MOUSE.diMouseState.lY : %f", static_cast<float>(MOUSE.diMouseState.lY));
	// ImGui::Text("MOUSE.diMouseState.lZ : %f", static_cast<float>(MOUSE.diMouseState.lZ));

	if (ImGui::Button(u8"‰Šú‰»"))
	{
		Init();
	}

	ImGui::End();
#endif

	if (camera->updateLook) return;


	POINT cursor;
	GetCursorPos(&cursor);

	camera->oldCursor = camera->newCursor;
	camera->newCursor = DirectX::XMFLOAT2(static_cast<float>(cursor.x), static_cast<float>(cursor.y));

	float moveX = (camera->newCursor.x - camera->oldCursor.x) * 0.02f;
	float moveY = (camera->newCursor.y - camera->oldCursor.y) * 0.02f;

	if (GetKeyState(VK_MENU) < 0)
	{
		if (GetKeyState(VK_LBUTTON) < 0)
		{
			camera->updateNow = true;
			camera->rotateY += moveX * 0.5f;
			camera->rotateX += moveY * 0.5f;
		}
		else if (GetKeyState(VK_MBUTTON) < 0)
		{
			float s = camera->distance * 0.035f;
			float x = moveX * s;
			float y = moveY * s;

			camera->target.x += camera->right.x * x;
			camera->target.y += camera->right.y * x;
			camera->target.z += camera->right.z * x;

			camera->target.x += camera->up.x * y;
			camera->target.y += camera->up.y * y;
			camera->target.z += camera->up.z * y;
		}
		else if (GetKeyState(VK_RBUTTON) < 0)
		{
			camera->distance += (-moveY - moveX) * camera->distance * 0.1f;
		}
		else
		{
			camera->updateNow = false;
			camera->distance -= (float)MOUSE.diMouseState.lZ / 1.0f;
		}
	}
	else
	{
		if (!(GetKeyState(VK_LBUTTON) < 0))
		{
			camera->updateNow = false;
		}
	}

	float xSin = sinf(camera->rotateX);
	float xCos = cosf(camera->rotateX);
	float ySin = sinf(camera->rotateY);
	float yCos = cosf(camera->rotateY);

	DirectX::XMVECTOR front = DirectX::XMVectorSet(-xCos * ySin, -xSin, -xCos * yCos, 0.0f);
	DirectX::XMVECTOR _right = DirectX::XMVectorSet(yCos, 0.0f, -ySin, 0.0f);
	DirectX::XMVECTOR _up = DirectX::XMVector3Cross(_right, front);

	DirectX::XMVECTOR _target = DirectX::XMLoadFloat3(&camera->target);
	DirectX::XMVECTOR _distance = DirectX::XMVectorSet(camera->distance, camera->distance, camera->distance, 0.0f);
	DirectX::XMVECTOR _pos = DirectX::XMVectorSubtract(_target, DirectX::XMVectorMultiply(front, _distance));

	DirectX::XMStoreFloat3(&camera->pos, _pos);
	DirectX::XMStoreFloat3(&camera->up, _up);
	DirectX::XMStoreFloat3(&camera->right, _right);

	DirectX::XMVECTOR _foward = DirectX::XMVectorSubtract(_target, _pos);
	DirectX::XMStoreFloat3(&camera->foward, _foward);
}

void CameraControl::PadControlUpdate( Camera* camera )
{
	camera->rotateY += (static_cast<float>(xInput[0].sRX) / 1000.0f) * 2 * 0.01745f;
	camera->rotateX += (static_cast<float>(xInput[0].sRY) / 1000.0f)  * 2 * 0.01745f;
	if (0.3f < camera->rotateX)
	{
		camera->rotateX = 0.3f;
	}
	if (camera->rotateX < 0.05f)
	{
		camera->rotateX = 0.05f;
	}
}

void CameraControl::CameraRotation( Camera* camera )
{
	camera->rotateY += 1 * 0.01745f;
}