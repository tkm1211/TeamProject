#pragma once

#include <memory>
#include <Camera.h>
#include <Billboard.h>
#include <GeometricPrimitive.h>

#include "Scene.h"
#include "Model.h"


class SceneGame : public Scene
{
private:
	// •Ï”‚ÌéŒ¾
	std::unique_ptr<Model> testModel;
	OBJ3D testModelData;

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};