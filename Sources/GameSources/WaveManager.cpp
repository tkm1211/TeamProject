
#include "WaveManager.h"
#include "EnemyManager.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void WaveManager::Init()
{
    // 実体の生成
    //spr_wave = std::make_unique<Sprite>("");
    timer = std::make_unique<Timer>();

    // 初期化
    for (WaveClearData& data : wave_resulted) data = {};
    Clear();
}


void WaveManager::Update()
{
    // 最終ウェーブをクリアすれば更新しない
    if (wave_state >= wave_max) return;

    // 経過時間の更新
    timer.get()->Update();


    // タスクを完了したら次のウェーブへ
    //if (JudgeTaskComplete()) NextWave();

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
}


void WaveManager::Clear()
{
    timer.get()->Clear();
    wave_state = wave_default;
}


void WaveManager::NextWave()
{
    // ウェーブ単位のクリア結果の保存
    wave_resulted[wave_state].clear_time = timer.get()->GetNowTime();

    // 敵の全削除
    EnemyManager::Instance().Clear();
    // 時間のリセット
    timer.get()->Clear();

    // 次のウェーブにする
    wave_state++;
}


bool WaveManager::JudgeTaskComplete()
{
    // 0 wave, Tutorial
    if (wave_state == wave_default)
    {

        return true;
    }

    // 1 wave, First Task
    if (wave_state == 1)
    {

        return true;
    }

    // 2 wave, Second Task
    if (wave_state == 2)
    {

        return true;
    }

    // 3 wave, Final Task
    if (wave_state == wave_max)
    {

        return true;
    }


    return false;
}