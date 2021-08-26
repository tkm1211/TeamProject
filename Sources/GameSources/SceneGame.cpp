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

	EnemyManager::Instance().Init(/*TODO: �v���C���[�̌��݈ʒu�̃A�h���X������*/);
	stage.Init();
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

	EnemyManager::Instance().Update();

	// �X�V
	GameSystem::Instance().Update();
}


void SceneGame::Render()
{
	EnemyManager::Instance().Render();
	stage.Render();
	GameSystem::Instance().Render();
}


void SceneGame::ImGui()
{
	ImGui::Text("Scene : Game");
	GameSystem::Instance().ImGui();
}


void SceneGame::UnInit()
{
	GameSystem::Instance().UnInit();
	stage.UnInit();
}