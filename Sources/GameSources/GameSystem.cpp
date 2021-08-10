
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
    wave_manager = std::make_unique<WaveManager>();
    combo = std::make_unique<Combo>();

    // èâä˙âªèàóù
    timer.get()->Init();
    score_manager.get()->Init();
    wave_manager.get()->Init();
    combo.get()->Init();
}


void GameSystem::Update()
{
    // çXêV
    timer.get()->Update();
    combo.get()->Update();
    score_manager.get()->Update();
    wave_manager.get()->Update();
}


void GameSystem::Render()
{
    // ï`âÊ
    timer.get()->Render();
    score_manager.get()->Render();
    combo.get()->Render();
}


void GameSystem::UnInit()
{
    // èIóπèàóù
    timer.get()->UnInit();
    score_manager.get()->UnInit();
    wave_manager.get()->UnInit();
    combo.get()->UnInit();
}

void GameSystem::ImGui()
{
    ImGui::Text("timer %f", timer.get()->GetNowTime());
    combo.get()->ImGui();
    wave_manager.get()->ImGui();
}