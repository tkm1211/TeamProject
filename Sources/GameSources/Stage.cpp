#include "Stage.h"
#include "Model.h"
#include "Shader.h"
#include "CameraSystem.h"
#include "FrameWork.h"

void Stage::Init()
{
	// モデルの初期化
	StageModel = std::make_unique<Model>("./Data/Assets/Model/Stage/stage.fbx", false);
	{
		OBJ3D::Init();
	}
}

void Stage::UnInit()
{
	OBJ3D::UnInit();
}

void Stage::Render()
{
	// モデルの描画
	{
		StageModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), false);
		StageModel->Render(GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), GetColor(), FrameWork::GetInstance().GetElapsedTime());
	}
}