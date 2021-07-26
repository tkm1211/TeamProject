#pragma once

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


// SceneƒNƒ‰ƒX
class Scene
{
private:

public:
	virtual void Init() {}
	virtual void UnInit() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void ImGui() {}
};