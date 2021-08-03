#include <FrameWork.h>
#include <string>

#include "SceneGame.h"
#include "InputDevice.h"
#include "SoundLoader.h"
#include "CameraSystem.h"
#include "GameSystem.h"


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
}


void SceneGame::Update()
{

	GameSystem::Instance().Update();
}


void SceneGame::Render()
{

}


void SceneGame::ImGui()
{
	ImGui::Text("Scene : Game");
	GameSystem::Instance().ImGui();
}


void SceneGame::UnInit()
{
	GameSystem::Instance().UnInit();
}