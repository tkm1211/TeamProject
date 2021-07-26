#include "PlayerTemplate.h"
#include "Camera.h"
#include "FrameWork.h"
#include "Shader.h"
#include "CameraSystem.h"
#include "CameraControl.h"


void PlayerTemplate::Init()
{
	pWait = std::make_unique<Model>("Data/Assets/Model/Player/MDL_Player_Wait.fbx", false);
	pRun = std::make_unique<Model>("Data/Assets/Model/Player/MDL_Player_Run.fbx", false);
	pAttack = std::make_unique<Model>("Data/Assets/Model/Player/MDL_Player_Attack.fbx", false);
	pItem = std::make_unique<Model>("Data/Assets/Model/Life.fbx", false);

	modelData.Init();
	itemData.Init();
	itemData.SetScale({ 10.0f,10.0f,10.0f });

	modelState = ModelState::WAIT;

	// New Get bonetransform
	{
		bool check = false;

		// �߂�l�́A�{�[�������݂������Ɂutrue�v�ŕԂ��Ă��܂��B����ȊO�́A�ufalse�v�B
		// ��Q�����́Amesh�C���f�b�N�X�B(�����ɃC���f�b�N�X�ԍ�������܂�)
		// ��R�����́Abone�C���f�b�N�X�B(�����ɃC���f�b�N�X�ԍ�������܂�)
		check = pRun->GetBoneTransformIndex(std::string("�{�[����"), modelIndeces[0], modelIndeces[1]);
	}


	// skinnedmeshbatch
	{
		pModelBatch = std::make_unique<Model>("Data/Assets/Model/Enemys/WarkerWait.fbx", false, true);

		// New Get bonetransform
		{
			bool check = false;

			// �߂�l�́A�{�[�������݂������Ɂutrue�v�ŕԂ��Ă��܂��B����ȊO�́A�ufalse�v�B
			// ��Q�����́Amesh�C���f�b�N�X�B(�����ɃC���f�b�N�X�ԍ�������܂�)
			// ��R�����́Abone�C���f�b�N�X�B(�����ɃC���f�b�N�X�ԍ�������܂�)
			pModelBatch->GetBoneTransformIndex(std::string("�{�[����"), instanceModelIndeces[0], instanceModelIndeces[1]);
		}

		for (int i = 0; i < MAX_INSTANCE; i++)
		{
			batchData[i].Init(); // ������
			batchData[i].SetIsAnimation(true);  // �A�j���[�V����ON
			batchData[i].SetIsAnimation(false); // �A�j���[�V����OFF
		}
	}


	pItemBatch = std::make_unique<Model>("Data/Assets/Model/Life.fbx", false, true);
	for (int i = 0; i < MAX_INSTANCE; i++)
	{
		itemBatchData[i].Init(); // ������
		itemBatchData[i].SetScale({ 10.0f,10.0f,10.0f });
	}
}
void PlayerTemplate::UnInit()
{
	pWait = nullptr;
	pRun = nullptr;
	pAttack = nullptr;

	modelData.UnInit();
}
void PlayerTemplate::Update()
{
	pWait->GetAnimationFrame();


	// Get boneTransform (�ʏ�)
	{
		// �{�[���s��擾
#if 0
		// Old Get bonetransform
		DirectX::XMFLOAT4X4 boneTransform = pRun->GetBoneTransform(std::string("�{�[����"));
#else
		// New Get bonetransform
		DirectX::XMFLOAT4X4 boneTransform = pRun->GetBoneTransform(modelIndeces[0], modelIndeces[1]);
#endif

		// �{�[���s������[���h��Ԃɕϊ�
		DirectX::XMFLOAT4X4 boneTransformWithWorld;
		DirectX::XMStoreFloat4x4(&boneTransformWithWorld, DirectX::XMLoadFloat4x4(&boneTransform) * modelData.GetWorldMatrix());

		// ���W�������o
		itemData.SetPos(DirectX::XMFLOAT3(boneTransformWithWorld._41, boneTransformWithWorld._42, boneTransformWithWorld._43));
	}

	// Get boneTransform (�C���X�^���X��)
	{
		// �{�[���s��擾
#if 0
		// Old Get bonetransform
		DirectX::XMFLOAT4X4 boneTransform = pModelBatch->GetBoneTransform(std::string("�{�[����"), batchData[10]);
#else
		// New Get bonetransform
		DirectX::XMFLOAT4X4 boneTransform = pModelBatch->GetBoneTransform(instanceModelIndeces[0], instanceModelIndeces[1], batchData[10]);
#endif

		// �{�[���s������[���h��Ԃɕϊ�
		DirectX::XMFLOAT4X4 boneTransformWithWorld;
		DirectX::XMStoreFloat4x4(&boneTransformWithWorld, DirectX::XMLoadFloat4x4(&boneTransform) * batchData[10].GetWorldMatrix());

		// ���W�������o
		itemBatchData[10].SetPos(DirectX::XMFLOAT3(boneTransformWithWorld._41, boneTransformWithWorld._42, boneTransformWithWorld._43));
	}
}
void PlayerTemplate::Draw()
{
	switch (modelState)
	{
	case PlayerTemplate::WAIT:
		pWait->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), false);
		pWait->Render(modelData.GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), modelData.GetColor(), FrameWork::GetInstance().GetElapsedTime());
		break;
	case PlayerTemplate::RUN:
		pRun->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), false);
		pRun->Render(modelData.GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), modelData.GetColor(), FrameWork::GetInstance().GetElapsedTime());
		break;
	case PlayerTemplate::ATTACK:
		pAttack->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), false);
		pAttack->Render(modelData.GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), modelData.GetColor(), FrameWork::GetInstance().GetElapsedTime());
		break;
	default: break;
	}

	pItem->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), false);
	pItem->Render(itemData.GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
		DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), itemData.GetColor(), FrameWork::GetInstance().GetElapsedTime());

	// skinnedmeshbatch
	{
		pModelBatch->Begin(ShaderSystem::GetInstance()->GetShaderOfSkinnedMeshBatch(), false);
		for (int i = 0; i < MAX_INSTANCE; i++)
		{
			pModelBatch->Render(batchData[i], /*modelBatchData[i].GetWorldMatrix(),*/ CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(), FrameWork::GetInstance().GetElapsedTime());
		}
		pModelBatch->End(DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	pItemBatch->Begin(ShaderSystem::GetInstance()->GetShaderOfSkinnedMeshBatch(), false);
	for (int i = 0; i < MAX_INSTANCE; i++)
	{
		pItemBatch->Render(itemBatchData[i], /*modelBatchData[i].GetWorldMatrix(),*/ CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(), FrameWork::GetInstance().GetElapsedTime());
	}
	pItemBatch->End(DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}
void PlayerTemplate::ImGui()
{
	int _modelState = modelState;
	DirectX::XMFLOAT3 _pos = modelData.GetPos();
	ImGui::Begin("Test Model");
	ImGui::InputInt("Model State No.", &modelState);
	DirectX::XMFLOAT4X4 boneTransform;
	ImGui::DragFloat3("Pos", &_pos.x);
	modelData.SetPos(_pos);
	/*switch (modelState)
	{
	case PlayerTemplate::WAIT:
		boneTransform = pWait->GetBoneTransform(std::string("mixamorig:Hips"), _pos);
		break;
	case PlayerTemplate::RUN:
		boneTransform = pRun->GetBoneTransform(std::string("mixamorig:Hips"), _pos);
		break;
	case PlayerTemplate::ATTACK:
		boneTransform = pAttack->GetBoneTransform(std::string("mixamorig:Hips"), _pos);
		break;
	default: break;
	}*/
	ImGui::Text("Wait Animation Frame : %d", pWait->GetAnimationFrame());
	ImGui::Text("Run Animation Frame : %d", pRun->GetAnimationFrame());
	ImGui::Text("Attack Animation Frame : %d", pAttack->GetAnimationFrame());
	ImGui::Text("pos %f %f %f", _pos.x, _pos.y, _pos.z);
	itemData.SetPos({ _pos.x, _pos.y, _pos.z });

	if (_modelState != modelState)
	{
		switch (modelState)
		{
		case PlayerTemplate::WAIT:
			pWait->StartAnimation(0, true);
			break;
		case PlayerTemplate::RUN:
			pRun->StartAnimation(0, true);
			break;
		case PlayerTemplate::ATTACK:
			pAttack->StartAnimation(0, true);
			break;
		default: break;
		}
	}

	if (ImGui::Button("Anim Start"))
	{
		switch (modelState)
		{
		case PlayerTemplate::WAIT:
			pWait->StartAnimation(0, true);
			break;
		case PlayerTemplate::RUN:
			pRun->StartAnimation(0, true);
			break;
		case PlayerTemplate::ATTACK:
			pAttack->StartAnimation(0, true);
			break;
		default: break;
		}
	}
	if (ImGui::Button("Anim ReStart"))
	{
		switch (modelState)
		{
		case PlayerTemplate::WAIT:
			pWait->ReStartAnimation();
			break;
		case PlayerTemplate::RUN:
			pRun->ReStartAnimation();
			break;
		case PlayerTemplate::ATTACK:
			pAttack->ReStartAnimation();
			break;
		default: break;
		}
	}
	if (ImGui::Button("Anim Stop"))
	{
		switch (modelState)
		{
		case PlayerTemplate::WAIT:
			pWait->PauseAnimation();
			break;
		case PlayerTemplate::RUN:
			pRun->PauseAnimation();
			break;
		case PlayerTemplate::ATTACK:
			pAttack->PauseAnimation();
			break;
		default: break;
		}
	}
	ImGui::End();
}