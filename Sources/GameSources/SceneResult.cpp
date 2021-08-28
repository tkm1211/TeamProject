#include <FrameWork.h>
#include <string>

#include "SceneResult.h"
#include "InputDevice.h"
#include "SoundLoader.h"
#include "SceneTitle.h"
#include "GameSystem.h"


void SceneResult::Init()
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

	spr_back = std::make_unique<Sprite>();
}


void SceneResult::Update()
{
	// �V�[���J��
	if (GetAsyncKeyState(VK_SPACE) < 0/*xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt*/)
	{
		Fade::GetInstance()->onFadeFlg = true;
		Fade::GetInstance()->loading = true;
		Fade::GetInstance()->SetNextScene(new SceneTitle());
	}
}


void SceneResult::Render()
{
	spr_back.get()->Draw({ 0.0f,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0.0f,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT }, 0,
		{ 1, 0, 0, 1 });
}


void SceneResult::ImGui()
{
	ImGui::Text("Scene : Result");
	for (int i = 0; i < GameSystem::Instance().GetWaveMax(); ++i)
	{
		ImGui::Text("wave : %d, time %f",i ,GameSystem::Instance().GetWaveClearTime(i));
	}

	ImGui::Text("total score : %d", GameSystem::Instance().GetScored());
}


void SceneResult::UnInit()
{
	
}