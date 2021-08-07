#include "Enemy.h"

#include "Model.h"
#include "Shader.h"
#include "CameraSystem.h"
#include "FrameWork.h"

void Enemy::Render()
{
	model->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::PHONE), true);
	model->Render(GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
		DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), GetColor(), FrameWork::GetInstance().GetElapsedTime());
}
