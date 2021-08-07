#include <FrameWork.h>
#include <string>

#include "SceneGame.h"
#include "InputDevice.h"
#include "SoundLoader.h"
#include "CameraSystem.h"
#include "SceneTitle.h"
#include "GameSystem.h"
#include "WaveManager.h"


void SceneGame::Init()
{
	testModel = std::make_unique<Model>("Data/Assets/Model/player_anime.fbx", false);
	{
		testModel->StartAnimation(2, true);
		testModelData.Init();
		testModelData.SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
	}

	// ���ׂẴT�E���h���~
	{
		AllSoundStop();
		AllBgmSoundStop();
	}

	// �T�E���h�̍Đ�
	{
		PlaySoundMem(SoundLoader::GetInstance()->bgm.get()); // ���[�v�Đ�
		PlaySoundMem(SoundLoader::GetInstance()->se.get()); // �P���Đ�
	}

	GameSystem::Instance().Init();
	WaveManager::Instance().Init();
}


void SceneGame::Update()
{
	if (Fade::GetInstance()->loading) return;

	// �V�[���J��
	if (GetAsyncKeyState(VK_SPACE) < 0/*xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt*/)
	{
		Fade::GetInstance()->onFadeFlg = true;
		Fade::GetInstance()->loading = true;
		Fade::GetInstance()->SetNextScene(new SceneTitle());
	}

	// �X�V
	GameSystem::Instance().Update();
	WaveManager::Instance().Update();
}


void SceneGame::Render()
{
	testModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), true);
	testModel->Render(testModelData.GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
		DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), testModelData.GetColor(), FrameWork::GetInstance().GetElapsedTime());
}


void SceneGame::ImGui()
{
	ImGui::Text("Scene : Game");
	GameSystem::Instance().ImGui();
	WaveManager::Instance().ImGui();
}


void SceneGame::UnInit()
{
	GameSystem::Instance().UnInit();
	WaveManager::Instance().UnInit();
	testModelData.UnInit();
}