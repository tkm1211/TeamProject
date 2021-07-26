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

	// Scene* scene : 次のシーン, bool nowSceneStack : 現在のシーンを残したいかどうか？
	void SetScene(Scene* scene, bool nowSceneStack = false);

	// スタックしたシーンを消す関数
	void ReSetStackScene();


	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}
};