#pragma once

#include <memory>
#include <Camera.h>
#include <Billboard.h>
#include <GeometricPrimitive.h>

#include "Scene.h"


class SceneGame : public Scene
{
private:
	// •Ï”‚ÌéŒ¾

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};