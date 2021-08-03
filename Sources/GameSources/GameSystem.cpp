
#include "GameSystem.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void GameSystem::Init()
{
    // 実体の生成
    timer = std::make_unique<Timer>();
    score_manager = std::make_unique<ScoreManager>();

    // 初期化処理
    timer.get()->Initialize();
    score_manager.get()->Initialize();
}


void GameSystem::UnInit()
{
    // 終了処理
    timer.get()->UnInitialize();
    score_manager.get()->UnInitialize();
}

void GameSystem::Update()
{
    // 更新
    timer.get()->Update();
    score_manager.get()->Update();
}


void GameSystem::Render()
{
    // 描画
    timer.get()->Render();
    score_manager.get()->Render();
}

void GameSystem::ImGui()
{
    ImGui::Text("timer %f", timer.get()->GetNowTime());
}