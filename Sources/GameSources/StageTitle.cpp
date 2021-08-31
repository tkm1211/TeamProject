#include "StageTitle.h"
#include "Model.h"
#include "Shader.h"
#include "CameraSystem.h"
#include "FrameWork.h"

void StageTitle::Init()
{
	// ƒ‚ƒfƒ‹‚Ì‰Šú‰»
	StageModel = std::make_unique<Model>("./Data/Assets/Model/Stage/stage.fbx", false);
	{
		OBJ3D::Init();
		scale = { 0.1f, 0.1f, 0.1f };
		pos = { 0.0f, -75.0f, 0.0f };
		angle = { 0.0f, 0.0f, 0.0f };
	}
}

void StageTitle::UnInit()
{
	OBJ3D::UnInit();
}

void StageTitle::Update()
{
	angle.y += 0.01f;
}

void StageTitle::Render()
{
	// ƒ‚ƒfƒ‹‚Ì•`‰æ
	{
		StageModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), false);
		StageModel->Render(GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), GetColor(), FrameWork::GetInstance().GetElapsedTime());
	}
}