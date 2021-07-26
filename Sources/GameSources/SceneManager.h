#pragma once

#include "Scene.h"
#include "SceneTitle.h"
#include "Fade.h"

#include <string>
#include <WinUser.h>


class SceneManager
{
private:
	Scene* pScene;
	Scene* pNext;
	Scene* pStackScene;

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();

	// Scene* scene : ���̃V�[��, bool nowSceneStack : ���݂̃V�[�����c���������ǂ����H
	void SetScene(Scene* scene, bool nowSceneStack = false);

	// �X�^�b�N�����V�[���������֐�
	void ReSetStackScene();


	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}
};