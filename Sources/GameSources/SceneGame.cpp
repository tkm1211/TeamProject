#include <FrameWork.h>
#include <string>

#include "SceneGame.h"
#include "InputDevice.h"
#include "SoundLoader.h"
#include "CameraSystem.h"
#include "SceneTitle.h"
#include "GameSystem.h"
// �ǉ�
#include "EnemyManager.h"
#include "EnemyDerived01.h"


void SceneGame::Init()
{
	//testModel = std::make_unique<Model>("Data/Assets/Model/player_anime.fbx", false);
	//{
	//	testModel->StartAnimation(2, true);
	//	testModelData.Init();
	//	testModelData.SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
	//}


		// ���f���̏�����
	testModel = std::make_unique<Model>("Data/Assets/Model/Player/EX/bullet_box_pi.fbx", false);
	{
		//testModel->StartAnimation(1, true); // ���� : FBX���̃A�j���[�V�����ԍ�, ���[�v�Đ����邩�H
		testModelData.Init();
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

	EnemyManager::Instance().Init();

	std::shared_ptr<Enemy> enemy_00 = std::make_shared<EnemyDerrived01>();
	EnemyManager::Instance().Spawn(enemy_00);
}


void SceneGame::Update()
{
	if (Fade::GetInstance()->loading) return;

	// �V�[���J��
	//if (GetAsyncKeyState(VK_SPACE) < 0/*xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt*/)
	//{
	//	Fade::GetInstance()->onFadeFlg = true;
	//	Fade::GetInstance()->loading = true;
	//	Fade::GetInstance()->SetNextScene(new SceneTitle());
	//}




	EnemyManager::Instance().Update();

	// �X�V
	GameSystem::Instance().Update();
}


void SceneGame::Render()
{
	EnemyManager::Instance().Render();

	// ���f���̕`��
	{
		testModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), true);
		testModel->Render(testModelData.GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), testModelData.GetColor(), FrameWork::GetInstance().GetElapsedTime());
	}


}


void SceneGame::ImGui()
{
	ImGui::Text("Scene : Game");
	GameSystem::Instance().ImGui();
}


void SceneGame::UnInit()
{
	GameSystem::Instance().UnInit();
	testModelData.UnInit();
}