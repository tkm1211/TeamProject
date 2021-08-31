#pragma once

#include <memory>
#include <Camera.h>
#include <Model.h>
#include <Billboard.h>
#include <GeometricPrimitive.h>

#include "Scene.h"
#include "Sprite.h"
#include "StageTitle.h"

class SceneTitle : public Scene
{
private:
	// タイトルテクスチャ描画用
	std::unique_ptr<SpriteBatch> titleSpr, titleSpr2, titleSpr3;
	TexData titleSprData, titleSprData2, titleSprData3;

	// モデル描画用
	StageTitle stageTitle;

	float A = 0.0f;
	bool flag = false;

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};