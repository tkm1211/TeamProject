#include"SoundLoader.h"
//	CreateSoundBuffer(&TitleBGM, "Data/Sounds/title.wav");
//ボリューム DSBVOLUME_MAX ~ DSBVOLUME_MIN

//***************************************************************************************************
//
//サウンド読み込み
//
//***************************************************************************************************
void SoundLoader::load()
{
	bgm1 = std::make_unique<CXAudio2>(L"Data/Assets/Sound/BGM/Title.wav", 1);
	bgm2 = std::make_unique<CXAudio2>(L"Data/Assets/Sound/BGM/Stage.wav", 1);
	se = std::make_unique<CXAudio2>(L"Data/Assets/Sound/SE/KO/ko.wav", 0);
}


//***************************************************************************************************
//
//更新
//
//***************************************************************************************************
void SoundLoader::Update()
{
	bgm1->Update();
	bgm2->Update();
	se->Update();
}

//***************************************************************************************************
//
//サウンド解放
//
//***************************************************************************************************
void SoundLoader::Release()
{
	bgm1->Relese();
	bgm2->Relese();
	se->Relese();
}

void SoundLoader::ImGui()
{

}

void AllSoundStop()
{
	StopSoundMem(SoundLoader::GetInstance()->se.get());
}

void AllBgmSoundStop()
{
	StopSoundMem(SoundLoader::GetInstance()->bgm1.get());
	StopSoundMem(SoundLoader::GetInstance()->bgm2.get());
}


//***************************************************************************************************
//サウンド再生
//***************************************************************************************************
void PlaySoundMem(CXAudio2* sound_name)
{
	sound_name->PlayWAV();
}

//***************************************************************************************************
//サウンド停止
//***************************************************************************************************
void StopSoundMem(CXAudio2* sound_name)
{
	if (!sound_name) return;
	sound_name->StopWAV();
}

//***************************************************************************************************
//サウンド停止
//***************************************************************************************************

void SetVolume(CXAudio2* sound_name, float volume)
{
	sound_name->Volume(volume);
}



