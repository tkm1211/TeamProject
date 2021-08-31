#include "Fade.h"
#include "SceneManager.h"
#include "Blender.h"
#include "CameraSystem.h"
#include "SoundLoader.h"


void Fade::Init()
{
	fade_spr = std::make_unique<SpriteBatch>(L"Data/Assets/Texture/blackFade.png");
	fade_spr_data.texPos = { 0, 0 };		//テクスチャの左上s
	fade_spr_data.size = { 1920, 1080 };	//テクスチャの幅、高さ

	onFadeFlg = false;
	loading = false;
	state = 0;
	alpha = 0;
	scene = nullptr;
}

void Fade::LoadModelsInit()
{
	//nowLoading = std::make_unique<Sprite>(L"Data/Assets/Texture/text03.png");

	nowLoadingData.pos = DirectX::XMFLOAT2(1300.0f, 1080.0f - 110.0f);
	nowLoadingData.texPos = DirectX::XMFLOAT2(0.0f, 110.0f);
	nowLoadingData.size = DirectX::XMFLOAT2(440.0f, 110.0f);

	for (int i = 0; i < 3; i++)
	{
		dotData[i].pos = DirectX::XMFLOAT2(1300.0f + 450.0f + 50.0f * i, 1080.0f - 110.0f);
		dotData[i].texPos = DirectX::XMFLOAT2(10013.8f, 110.0f);
		dotData[i].size = DirectX::XMFLOAT2(30.0f, 110.0f);
	}

	cnt = 0;
	loadingState = 0;
	nowLoad = false;
}
void Fade::LoadModels()
{
	loadingThread = std::make_unique<std::thread>([&]()
	{
		std::lock_guard<std::mutex> lock(loadingMutex);

		/*CharacterSystem::GetInstance()->Init();
		ObjectSystem::GetInstance()->Init();
		ParticleSystem::GetInstance()->Init();
		UiSystem::GetInstance()->Init();
		Ranking::GetInstance()->Init();
		CrystalSystem::GetInstance()->Init();*/
	});
}

void Fade::UnInit()
{

}

void Fade::Update()
{
	if (!onFadeFlg/* || waveMgr.GetWaveNowIndex() == (int)waveMgr.GetWaves().size()*/) return;

	if (3 <= state && loading)
	{
		if (IsNowLoading())
		{
			return; //ロードが完了していなかったら即return
		}

		if (loadingThread && loadingThread->joinable())
		{
			loadingThread->join();
			nowLoad = false;
		}
	}

	switch (state)
	{
	case 0:
		if (loading)
		{
			LoadModelsInit();
		}

		alpha = 0;
		state++;
		break;
	case 1:
		alpha += 0.01f;
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
			state++;
		}
		break;
	case 2:
		if (loading)
		{
			nowLoad = true;
			LoadModels();
		}
		if (scene)
		{
			SceneManager::GetInstance()->SetScene(scene, false);
		}
		scene = nullptr;
		state++;
		break;
	case 3:
		alpha -= 0.01f;
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			state++;
		}
		break;
	case 4:
		alpha = 0;
		state = 0;
		onFadeFlg = false;
		loading = false;
		break;
	}

}

void Fade::Draw()
{
	SetBlenderMode(BM_ALPHA);
	if (fade_spr)
	{
		fade_spr->Begin();
		fade_spr->Draw({ 0, 0 }, { 1920, 1080 }, fade_spr_data.texPos, fade_spr_data.size, 0, { 1, 1, 1, alpha });
		fade_spr->End();
	}

	if (IsNowLoading() && loading && nowLoading)
	{
		//ロード中
		switch (loadingState)
		{
		case 0:
			nowLoading->Draw(nowLoadingData.pos, nowLoadingData.size, nowLoadingData.texPos, nowLoadingData.size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			if (10 <= cnt++)
			{
				cnt = 0;
				loadingState++;
			}
			break;
		case 1:
			nowLoading->Draw(nowLoadingData.pos, nowLoadingData.size, nowLoadingData.texPos, nowLoadingData.size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
			nowLoading->Draw(dotData[0].pos, dotData[0].size, dotData[0].texPos, dotData[0].size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			if (10 <= cnt++)
			{
				cnt = 0;
				loadingState++;
			}
			break;
		case 2:
			nowLoading->Draw(nowLoadingData.pos, nowLoadingData.size, nowLoadingData.texPos, nowLoadingData.size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
			nowLoading->Draw(dotData[0].pos, dotData[0].size, dotData[0].texPos, dotData[0].size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
			nowLoading->Draw(dotData[1].pos, dotData[1].size, dotData[1].texPos, dotData[1].size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			if (10 <= cnt++)
			{
				cnt = 0;
				loadingState++;
			}
			break;
		case 3:
			nowLoading->Draw(nowLoadingData.pos, nowLoadingData.size, nowLoadingData.texPos, nowLoadingData.size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
			nowLoading->Draw(dotData[0].pos, dotData[0].size, dotData[0].texPos, dotData[0].size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
			nowLoading->Draw(dotData[1].pos, dotData[1].size, dotData[1].texPos, dotData[1].size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
			nowLoading->Draw(dotData[2].pos, dotData[2].size, dotData[2].texPos, dotData[2].size, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			if (10 <= cnt++)
			{
				cnt = 0;
				loadingState = 0;
			}
			break;
		}

		return;
	}
	SetBlenderMode(BM_NONE);
}