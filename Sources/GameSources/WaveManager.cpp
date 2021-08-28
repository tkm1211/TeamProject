
#include "WaveManager.h"
#include "EnemyManager.h"
#include "EnemyDerived01.h"
#include "EnemyDerrivedHoming.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void WaveManager::Init()
{
    // 実体の生成
    spr_wave = std::make_unique<Sprite>(L"Data/Assets/Texture/wave.png");
    timer = std::make_unique<Timer>();
    p_task = new TaskWave0();

    // 初期化
    SetEnemyData();
    for (ClearData& data : wave_resulted) data = {};
    Clear();

}


void WaveManager::Update()
{
    // 最終ウェーブをクリアすれば更新しない
    if (wave_state > wave_max) return;

    // 経過時間の更新
    timer.get()->Update();

    SpawnEnemy();

    // タスクを完了したら次のウェーブへ
    if (JudgeTaskComplete()) NextWave();

}


void WaveManager::Render()
{
    constexpr float end_time_second = 2.0f;
    if (wave_state - 1 < 0)return;
    if (timer.get()->GetNowTime() > end_time_second) return;


    constexpr float width = 400.0f;
    constexpr float height = 280.0f;

    // 現在のウェーブ数を表示
    spr_wave.get()->Draw2(
        640, 300,
        width, height,
        0.0f, 0.0f + height * static_cast<float>(wave_state - 1),
        width, height,
        0,
        1, 1, 1, 1);
}


void WaveManager::ImGui()
{
    ImGui::Text("wave_state : %d", wave_state);
    ImGui::Text("total enemy : %d", EnemyManager::Instance().GetEnemyTotalCount());
}


void WaveManager::UnInit()
{
    Clear();
}


void WaveManager::Clear()
{
    timer.get()->Clear();
    old_wave_time = 0.0f;
    wave_state = wave_default;
}


void WaveManager::SetEnemyData()
{
    wave_enemy_data[0] =
    {
        false,
        3,
        2.0f
    };

    wave_enemy_data[1] =
    {
        false,
        4,
        2.0f
    };

    wave_enemy_data[2] =
    {
        false,
        5,
        2.0f
    };

    wave_enemy_data[3] =
    {
        true,
        6,
        1.0f
    };
}


void WaveManager::NextWave()
{
    // ウェーブのクリア結果の保存
    wave_resulted[wave_state].clear_time = timer.get()->GetNowTime();

    // 敵の全削除
    EnemyManager::Instance().Clear();
    // 時間のリセット
    timer.get()->Clear();
    old_wave_time = 0.0f;
     
    // 次のウェーブにする
    wave_state++;
    // タスクの進行
    ChangeNextTask();
}


bool WaveManager::JudgeTaskComplete()
{
    if (!p_task) return false;

    return p_task->JudgeTaskComplete();
}


void WaveManager::ChangeNextTask()
{
    // 前回タスクの削除処理
    if (p_task)
    {
        delete p_task;
        p_task = nullptr;
    }

    // タスクを指定しない
    if (wave_state >= wave_max)return;

    // ウェーブ1のタスク
    if (wave_state == 1)
    {
        p_task = new TaskWave1();
        return;
    }

    // ウェーブ2のタスク
    if (wave_state == 2)
    {
        p_task = new TaskWave2();
        return;
    }

    // ウェーブ3のタスク
    if (wave_state == wave_max)
    {
        p_task = new TaskWave3();
        return;
    }

}


void WaveManager::SpawnEnemy()
{
    // 敵を出現させない
    if (EnemyManager::Instance().GetEnemyTotalCount() >= wave_enemy_data[wave_state].enemy_max) return;



    float subtruct_time = timer.get()->GetNowTime() - old_wave_time;

    // クールタイムを過ぎていたら
    if (subtruct_time < wave_enemy_data[wave_state].spawn_cool_time) return;

    // TODO : 生成する敵の種類を増やす
    constexpr int enemy_of_kinds = 2;
    int random = rand() % enemy_of_kinds;

    // TODO : 敵の種類が増えたら増やす！！！
    std::shared_ptr<Enemy> enemy = nullptr;
    if (random == 0) enemy = std::make_shared<EnemyDerrived01>();
    if (random == 1) enemy = std::make_shared<EnemyDerrivedHoming>();

    if (!enemy) return;
    //　出現させる
    EnemyManager::Instance().Spawn(enemy);

    old_wave_time = timer.get()->GetNowTime();
}