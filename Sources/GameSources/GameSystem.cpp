
#include "GameSystem.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void GameSystem::Init()
{
    // é¿ëÃÇÃê∂ê¨
    timer = std::make_unique<Timer>();
    score_manager = std::make_unique<ScoreManager>();

    // èâä˙âªèàóù
    timer.get()->Initialize();
    score_manager.get()->Initialize();
}


void GameSystem::UnInit()
{
    // èIóπèàóù
    timer.get()->UnInitialize();
    score_manager.get()->UnInitialize();
}

void GameSystem::Update()
{
    // çXêV
    timer.get()->Update();
    score_manager.get()->Update();
}


void GameSystem::Render()
{
    // ï`âÊ
    timer.get()->Render();
    score_manager.get()->Render();
}

void GameSystem::ImGui()
{
    ImGui::Text("timer %f", timer.get()->GetNowTime());
}