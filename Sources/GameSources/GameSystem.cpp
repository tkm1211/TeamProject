
#include "GameSystem.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void GameSystem::Init()
{
    // 実体の生成
    timer               = std::make_unique<Timer>(true);
    score_manager       = std::make_unique<ScoreManager>();
    wave_manager        = std::make_unique<WaveManager>();
    combo               = std::make_unique<Combo>();
    spr_ui_time_score   = std::make_unique<Sprite>(L"Data/Assets/Texture/time_score.png");

    // 初期化処理
    timer.get()->Init();
    timer.get()->SetTime(300);
    score_manager.get()->Init();
    wave_manager.get()->Init();
    combo.get()->Init();
}


void GameSystem::Update()
{
    // 更新
    timer.get()->Update();
    combo.get()->Update();
    score_manager.get()->Update();
    wave_manager.get()->Update();
}


void GameSystem::Render()
{
    // 描画
    spr_ui_time_score.get()->Draw2(
        920, 90,
        220, 120,
        0, 0, 220, 120,
        0,
        1, 1, 1, 1);

    wave_manager.get()->Render();
    timer.get()->Render({ 1040.0f,40.0f });
    score_manager.get()->Render({ 1040.0f,40.0f + 80.0f });
    combo.get()->Render();
}


void GameSystem::UnInit()
{
    // 終了処理
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


void GameSystem::KilledEnemy(float enemy_score)
{
    combo.get()->AddCombo();

    // コンボ数で倍率変化 (1コンボ増えるたびに0.25倍スコアが上昇する)
    float now_combo = static_cast<float>(combo.get()->GetCombo());
    float score_ratio = 1.0f + 0.25f * (1.0f - now_combo);

    score_manager.get()->AddScore(score_ratio * enemy_score);
}

void GameSystem::DamagedPlayer()
{
    constexpr int minus_time = -5;
    timer.get()->AddTime(minus_time);

    if (timer.get()->GetNowTime() < 0.0f) timer.get()->SetTime(0);
}