#include <FrameWork.h>
#include <string>

#include "SceneTitle.h"
#include "InputDevice.h"
#include "SoundLoader.h"
#include "CameraSystem.h"
#include "SceneGame.h"
#include "StageTitle.h"



void SceneTitle::Init()
{
	// テクスチャの初期化
	titleSpr = std::make_unique<SpriteBatch>(L"Data/Assets/Texture/Title/title.png", 1); // 引数 : ファイルパス, 同時描画数
	{
		titleSprData.pos = { 400, 50 };
		titleSprData.size = { 1280.0f, 720.0f };
		titleSprData.texPos = { 0, 0 };
		titleSprData.texSize = { 1920.0f, 1080.0f };
	}

	titleSpr2 = std::make_unique<SpriteBatch>(L"Data/Assets/Texture/Title/start.png", 1); // 引数 : ファイルパス, 同時描画数
	{
		titleSprData2.pos = { 550, 550 };
		titleSprData2.size = { 1280.0f, 720.0f };
		titleSprData2.texPos = { 0, 0 };
		titleSprData2.texSize = { 1920.0f, 1080.0f };
	}

	titleSpr3 = std::make_unique<SpriteBatch>(L"Data/Assets/Texture/blackFade.png", 1); // 引数 : ファイルパス, 同時描画数
	{
		titleSprData3.pos = { 0, 0 };
		titleSprData3.size = { 1280.0f, 720.0f };
		titleSprData3.texPos = { 0, 0 };
		titleSprData3.texSize = { 1920.0f, 1080.0f };
	}

	// モデルの初期化
	stageTitle.Init();

	// すべてのサウンドを停止
	{
		AllSoundStop();
		AllBgmSoundStop();
	}
	
	// サウンドの再生
	{
		PlaySoundMem(SoundLoader::GetInstance()->bgm1.get()); // ループ再生
		PlaySoundMem(SoundLoader::GetInstance()->se.get()); // 単発再生
	}
}

void SceneTitle::Update()
{
	if (Fade::GetInstance()->loading) return;

	float elapsedtime = FrameWork::GetInstance().elapsedTime;

	// シーン遷移
	if (GetAsyncKeyState(VK_SPACE) < 0/*xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt*/)
	{
		//Fade::GetInstance()->onFadeFlg = true;
		//Fade::GetInstance()->loading = true;
		//Fade::GetInstance()->SetNextScene(new SceneGame());
		flag = true;
	}
	if (flag == true)
	{
		A += 1.0f * elapsedtime;
	}
	if (A > 1.0f)
	{
		Fade::GetInstance()->onFadeFlg = true;
		Fade::GetInstance()->loading = true;
		Fade::GetInstance()->SetNextScene(new SceneGame());
	}

	stageTitle.Update();
}

void SceneTitle::Render()
{
	// テクスチャの描画
	{
		titleSpr3->Begin();
		titleSpr3->Draw(titleSprData3, 0, { 1, 1, 1, 1 });
		titleSpr3->End();
		// モデルの描画
		{
			stageTitle.Render();
		}
		titleSpr2->Begin();
		titleSpr2->Draw(titleSprData2, 0, { 1, 1, 1, 1 });
		titleSpr2->End();
		titleSpr->Begin();
		titleSpr->Draw(titleSprData, 0, { 1, 1, 1, 1 });
		titleSpr->End();
		titleSpr3->Begin();
		titleSpr3->Draw(titleSprData3, 0, { 1, 1, 1, A });
		titleSpr3->End();
	}
}

void SceneTitle::ImGui()
{
	ImGui::Text("Scene : Title");
}

void SceneTitle::UnInit()
{
	stageTitle.UnInit();
}