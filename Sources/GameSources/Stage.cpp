#include "Stage.h"
#include "Model.h"
#include "Shader.h"
#include "CameraSystem.h"
#include "FrameWork.h"

void Stage::Init()
{
	// ƒ‚ƒfƒ‹‚Ì‰Šú‰»
	StageModel = std::make_unique<Model>("./Data/Assets/Model/Stage/stage.fbx", false);
	{
		StageData.Init();
	}
}

void Stage::UnInit()
{
	StageData.UnInit();
}

void Stage::Render()
{
	// ƒ‚ƒfƒ‹‚Ì•`‰æ
	{
		StageModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), false);
		StageModel->Render(GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), GetColor(), FrameWork::GetInstance().GetElapsedTime());
	}
}