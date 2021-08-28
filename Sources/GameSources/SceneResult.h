#pragma once

#include <memory>

#include "Scene.h"
#include "Sprite.h"


class SceneResult : public Scene
{
private:
	// �^�C�g���e�N�X�`���`��p
	std::unique_ptr<Sprite> spr_back;

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};