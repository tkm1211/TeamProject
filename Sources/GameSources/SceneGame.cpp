#include <FrameWork.h>
#include <string>

#include "SceneGame.h"
#include "InputDevice.h"
#include "SoundLoader.h"
#include "CameraSystem.h"
#include "SceneTitle.h"
#include "GameSystem.h"
#include "WaveManager.h"
// 追加
#include "EnemyManager.h"
#include "EnemyDerived01.h"


void SceneGame::Init()
{
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
	WaveManager::Instance().Init();

	EnemyManager::Instance().Init();

	std::shared_ptr<Enemy> enemy_00 = std::make_shared<EnemyDerrived01>();
	EnemyManager::Instance().Spawn(enemy_00.get());
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

	// 更新
	GameSystem::Instance().Update();
	WaveManager::Instance().Update();
}


void SceneGame::Render()
{
	EnemyManager::Instance().Render();
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
}