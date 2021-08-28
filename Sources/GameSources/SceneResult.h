#pragma once

#include <memory>

#include "Scene.h"
#include "Sprite.h"


class SceneResult : public Scene
{
private:
	// タイトルテクスチャ描画用
	std::unique_ptr<Sprite> spr_back;

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};