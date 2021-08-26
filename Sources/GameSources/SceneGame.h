#pragma once

#include <memory>
#include <Camera.h>
#include <Billboard.h>
#include <GeometricPrimitive.h>

#include "Scene.h"
#include "Model.h"

#include "Stage.h"

class SceneGame : public Scene
{
private:
	// •Ï”‚ÌéŒ¾
	Stage stage;

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};