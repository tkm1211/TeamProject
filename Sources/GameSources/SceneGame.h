#pragma once

#include <memory>
#include <Camera.h>
#include <Billboard.h>
#include <GeometricPrimitive.h>

#include "Scene.h"
#include "Model.h"
<<<<<<< HEAD
#include"Player.h"
=======

#include "Stage.h"
>>>>>>> 107a1b7bfc157684b8b04fe72abdfe36722547e3

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

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();
};