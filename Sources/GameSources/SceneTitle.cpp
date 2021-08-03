#include <FrameWork.h>
#include <string>

#include "SceneTitle.h"
#include "InputDevice.h"
#include "SoundLoader.h"
#include "CameraSystem.h"


void SceneTitle::Init()
{
	// �e�N�X�`���̏�����
	titleSpr = std::make_unique<SpriteBatch>(L"Data/Assets/Texture/Title/TitleLogo.png", 2); // ���� : �t�@�C���p�X, �����`�搔
	{
		titleSprData.pos = { 0, 0 };
		titleSprData.size = { 1280.0f, 720.0f };
		titleSprData.texPos = { 0, 0 };
		titleSprData.texSize = { 1920.0f, 1080.0f };
	}

	// ���f���̏�����
	playerModel = std::make_unique<Model>("Data/Assets/Model/Player/Test/Mma_Kick.fbx", false);
	{
		playerModel->StartAnimation(1, true); // ���� : FBX���̃A�j���[�V�����ԍ�, ���[�v�Đ����邩�H
		playerModelData.Init();
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
}

void SceneTitle::Update()
{
	if (Fade::GetInstance()->loading) return;

	if (xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt)
	{
		Fade::GetInstance()->onFadeFlg = true;
		Fade::GetInstance()->loading = true;
		//Fade::GetInstance()->SetNextScene(new SceneGame()); // TODO : ���̃V�[����ǉ�
	}
}

void SceneTitle::Render()
{
	// �e�N�X�`���̕`��
	{
		titleSpr->Begin();
		titleSpr->Draw(titleSprData, 0, { 1, 1, 1, 1 });
		titleSpr->Draw(titleSprData, 1.5f, { 1, 1, 1, 1 });//
		titleSpr->End();
	}

	// ���f���̕`��
	{
		playerModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), true);
		playerModel->Render(playerModelData.GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), playerModelData.GetColor(), FrameWork::GetInstance().GetElapsedTime());
	}
}

void SceneTitle::ImGui()
{
}

void SceneTitle::UnInit()
{
	playerModelData.UnInit();
}