#pragma once

#include <memory>
#include <Camera.h>
#include <Billboard.h>
#include <GeometricPrimitive.h>
#include "Scene.h"
#include "Model.h"
#include "Player.h"

#include "Stage.h"

class SceneGame : public Scene
{
private:
	// •Ï”‚ÌéŒ¾

	std::unique_ptr<Player> playerModel;
	std::unique_ptr<Camera> playerCamera;

	 float  Ti = 0;

	 float gravity = -1;
	 int dash = 1;

	Stage stage;

	DirectX::XMFLOAT3 position = {100, 0, 100};

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};