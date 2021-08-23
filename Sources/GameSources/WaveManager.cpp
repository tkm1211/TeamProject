
#include "WaveManager.h"
#include "EnemyManager.h"
#include "EnemyDerived01.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void WaveManager::Init()
{
    // 実体の生成
    //spr_wave = std::make_unique<Sprite>("");
    timer = std::make_unique<Timer>();
    p_task = new TaskWave0();

    // 初期化
    SetEnemyData();
    SetCreateEnemy();
    for (ClearData& data : wave_resulted) data = {};
    Clear();

}


void WaveManager::Update()
{
    // 最終ウェーブをクリアすれば更新しない
    if (wave_state >= wave_max) return;

    // 経過時間の更新
    timer.get()->Update();

    SpawnEnemy();

    // タスクを完了したら次のウェーブへ
    if (JudgeTaskComplete()) NextWave();

}


void WaveManager::Render()
{
    // 現在のウェーブ数を表示
}


void WaveManager::ImGui()
{
    ImGui::Text("wave_state : %d", wave_state);
}


void WaveManager::UnInit()
{
    Clear();
    create_enemy.clear();
}


void WaveManager::Clear()
{
    timer.get()->Clear();
    wave_state = wave_default;
}


void WaveManager::SetEnemyData()
{
    wave_enemy_data[0] =
    {
        false,
        3,
        3.0f
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


    // 前回出現させたときの時間
    static float old_wave_time = 0.0f;

    float subtruct_time = timer.get()->GetNowTime() - old_wave_time;

    // クールタイムを過ぎていたら
    if (subtruct_time < wave_enemy_data[wave_state].spawn_cool_time) return;

    // TODO : 生成する敵の種類を増やす
    constexpr int enemy_of_kinds = 1;
    int random = rand() % enemy_of_kinds;
    //　出現させる
    EnemyManager::Instance().Spawn(create_enemy[random]);

    old_wave_time = timer.get()->GetNowTime();
}


void WaveManager::SetCreateEnemy()
{
    std::shared_ptr<Enemy> enemy = nullptr;

    // 横移動
    enemy = std::make_shared<EnemyDerrived01>();
    create_enemy.emplace(0, enemy);

    //enemy = std::make_shared< >();
    //create_enemy.emplace(1, enemy);
}