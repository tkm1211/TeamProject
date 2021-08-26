#include <FrameWork.h>
#include <string>

#include "SceneGame.h"
#include "InputDevice.h"
#include "SoundLoader.h"
#include "CameraSystem.h"
#include "SceneTitle.h"
#include "GameSystem.h"
// 追加
#include "EnemyManager.h"
#include "EnemyDerived01.h"


void SceneGame::Init()
{

	playerModel = std::make_unique<Player>();
	// すべてのサウンドを停止
	{
		AllSoundStop();
		AllBgmSoundStop();
	}

	// サウンドの再生
	{
		PlaySoundMem(SoundLoader::GetInstance()->bgm.get()); // ループ再生
		PlaySoundMem(SoundLoader::GetInstance()->se.get()); // 単発再生
	}

	GameSystem::Instance().Init();

	EnemyManager::Instance().Init();
	stage.Init();

	std::shared_ptr<Enemy> enemy_00 = std::make_shared<EnemyDerrived01>();
	EnemyManager::Instance().Spawn(enemy_00);
}


void SceneGame::Update()
{
	if (Fade::GetInstance()->loading) return;

	// シーン遷移
	if (GetAsyncKeyState(VK_SPACE) < 0/*xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt*/)
	{
		Fade::GetInstance()->onFadeFlg = true;
		Fade::GetInstance()->loading = true;
		Fade::GetInstance()->SetNextScene(new SceneTitle());
	}

	playerModel->Update(FrameWork::GetInstance().GetElapsedTime());

	EnemyManager::Instance().Update();

	// 更新
	GameSystem::Instance().Update();
}


void SceneGame::Render()
{

	playerModel->Render();

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