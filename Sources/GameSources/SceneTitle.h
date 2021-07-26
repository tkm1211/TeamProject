#pragma once

#include <memory>
#include <Camera.h>
#include <Model.h>
#include <Billboard.h>
#include <GeometricPrimitive.h>

#include "Scene.h"
#include "Sprite.h"
#include "Model.h"


class SceneTitle : public Scene
{
private:
	// タイトルテクスチャ描画用
	std::unique_ptr<SpriteBatch> titleSpr;
	TexData titleSprData;

	// プレイヤーモデル描画用
	std::unique_ptr<Model> playerModel;
	OBJ3D playerModelData;

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};