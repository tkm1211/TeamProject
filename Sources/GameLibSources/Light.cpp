#include "Light.h"
#include <Windows.h>
#include "FrameWork.h"


void Light::Init()
{
	ZeroMemory(pointLight, sizeof(POINTLIGHT) * POINTMAX);
	//ZeroMemory(SpotLight, sizeof(SPOTLIGHT) * SPOTMAX);

	lightAngle = 4.16f;
	posY = -0.36f;
	Light::SetAmbient(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	//ライト方向
#if 0
	lightDir.x = sinf(lightAngle);
	lightDir.y = posY;
	lightDir.z = cosf(lightAngle);
#elif 1
	lightDir.x = 1.0f;
	lightDir.y = 55.0f;
	lightDir.z = 0.0f;
#endif

#if 1
	// SetPointLight(0, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 50.0f);
#else
	for (int i = 0; i < POINTMAX; i++)
	{
		float randX = rand() % 1000;
		float randZ = rand() % 1000;
		SetPointLight(i, { randX, 11.0f, randZ }, { 54.0f, 96.0f, 110.0f }, 900.0f);
	}
#endif
	SetSpotLight(0, DirectX::XMFLOAT3(-5.0f, 5.0f, -5.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, -0.2f, 0.0f), 50.0f, 0.99f, 0.9f);

	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<ID3D11Device> device = FrameWork::GetInstance().GetDevice();

	D3D11_BUFFER_DESC bufferDescLight = {};
	bufferDescLight.ByteWidth = sizeof(CBufferLight01);
	bufferDescLight.Usage = D3D11_USAGE_DEFAULT;
	bufferDescLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescLight.CPUAccessFlags = 0;
	bufferDescLight.MiscFlags = 0;
	bufferDescLight.StructureByteStride = 0;

	hr = device->CreateBuffer(&bufferDescLight, nullptr, constantBufferLight1.GetAddressOf());
	assert(!hr && "CreateBufferLight1	Error");

	ZeroMemory(&bufferDescLight, sizeof(bufferDescLight));
	bufferDescLight.ByteWidth = sizeof(CBufferLight02);
	bufferDescLight.Usage = D3D11_USAGE_DEFAULT;
	bufferDescLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescLight.CPUAccessFlags = 0;
	bufferDescLight.MiscFlags = 0;
	bufferDescLight.StructureByteStride = 0;

	hr = device->CreateBuffer(&bufferDescLight, nullptr, constantBufferLight2.GetAddressOf());
	assert(!hr && "CreateBufferLight2	Error");

	pointLightNo = 0;
}

void Light::Update()
{
	//if (GetKeyState('A') < 0) lightAngle -= 2.0f * 0.01745f;
	//if (GetKeyState('D') < 0) lightAngle += 2.0f * 0.01745f;
	//if (GetKeyState('W') < 0) posY -= 2.0f * 0.01745f;
	//if (GetKeyState('S') < 0) posY += 2.0f * 0.01745f;
	//SetAmbient(DirectX::XMFLOAT3(0.75f, 0.75f, 0.75f));

	////ライト方向
	//lightDir.x = sinf(lightAngle);
	//lightDir.y = posY;
	//lightDir.z = cosf(lightAngle);
	//SetDirLight(lightDir, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
}

void Light::ImGui()
{
	static int lightNo = 0;
	ImGui::Begin("Point Light");
	ImGui::InputInt("lightNo", &lightNo);
	ImGui::InputFloat("ON", &pointLight[lightNo].type);
	ImGui::DragFloat3("pos", &pointLight[lightNo].pos.x);
	ImGui::DragFloat("range", &pointLight[lightNo].range);
	ImGui::DragFloat3("ambient color", &ambient.x);
	ImGui::DragFloat3("dir", &lightDir.x);
	ImGui::DragFloat4("light color", &pointLight[lightNo].color.x);
	ImGui::End();

	static int spotLightNo = 0;
	ImGui::Begin("Spot Light");
	ImGui::InputInt("lightNo", &spotLightNo);
	ImGui::InputFloat("ON", &spotLight[spotLightNo].type);
	ImGui::DragFloat3("pos", &spotLight[spotLightNo].pos.x);
	ImGui::DragFloat3("dir", &spotLight[spotLightNo].dir.x);
	ImGui::DragFloat("range", &spotLight[spotLightNo].range);
	ImGui::DragFloat("near", &spotLight[spotLightNo].inner_corn);
	ImGui::DragFloat("far", &spotLight[spotLightNo].outer_corn);
	ImGui::DragFloat3("ambient color", &ambient.x);
	ImGui::DragFloat4("light color", &spotLight[spotLightNo].color.x);
	ImGui::End();
}

void Light::SetDirLight(DirectX::XMFLOAT4 dir, DirectX::XMFLOAT3 color)
{
	float d = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

	if (d > 0) { dir.x /= d, dir.y /= d, dir.z /= d; }
	lightDir = DirectX::XMFLOAT4(dir.x, dir.y, dir.z, 0);
	lightColor = DirectX::XMFLOAT4(color.x, color.y, color.z, 1);
}
void Light::SetAmbient(DirectX::XMFLOAT3 amb)
{
	ambient = { amb.x,amb.y,amb.z,0 };
}
void Light::SetPointLight(int index, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, float range)
{
	if (index < 0) return;
	if (index >= POINTMAX)return;
	pointLight[index].index = (float)index;
	pointLight[index].range = range;
	pointLight[index].type = 1.0f;
	pointLight[index].dumy = 0.0f;
	pointLight[index].pos = DirectX::XMFLOAT4(pos.x, pos.y, pos.z, 0);
	pointLight[index].color = DirectX::XMFLOAT4(color.x, color.y, color.z, 0);
}
void Light::ReSetPointLight(int index)
{
	if (index < 0) return;
	if (index >= POINTMAX)return;
	pointLight[index].type = 0.0f;
}
void Light::SetPointLight(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, float range)
{
	if (pointLightNo < 0) return;
	if (pointLightNo >= POINTMAX)return;
	pointLight[pointLightNo].index = (float)pointLightNo;
	pointLight[pointLightNo].range = range;
	pointLight[pointLightNo].type = 1.0f;
	pointLight[pointLightNo].dumy = 0.0f;
	pointLight[pointLightNo].pos = DirectX::XMFLOAT4(pos.x, pos.y, pos.z, 0);
	pointLight[pointLightNo].color = DirectX::XMFLOAT4(color.x, color.y, color.z, 0);

	pointLightNo++;
}
void Light::SetSpotLight(int index, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 dir,
	float range, float near_area, float far_area)
{
	if (index < 0) return;
	if (index >= SPOTMAX)return;
	float d = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	if (d > 0) {
		dir.x /= d; dir.y /= d; dir.z /= d;
	}

	spotLight[index].index = (float)index;
	spotLight[index].range = range;
	spotLight[index].type = 1.0f;
	spotLight[index].inner_corn = near_area;
	spotLight[index].outer_corn = far_area;
	spotLight[index].dumy0 = 0.0f;
	spotLight[index].dumy1 = 0.0f;
	spotLight[index].dumy2 = 0.0f;

	spotLight[index].pos = DirectX::XMFLOAT4(pos.x, pos.y, pos.z, 0);
	spotLight[index].color = DirectX::XMFLOAT4(color.x, color.y, color.z, 0);
	spotLight[index].dir = DirectX::XMFLOAT4(dir.x, dir.y, dir.z, 0);
}
