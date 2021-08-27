#pragma once

#include <memory>
#include "Sprite.h"
#include "Timer.h"


class Combo
{
public:
    Combo() {};
    ~Combo() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();
    void ImGui();

public: // Get関数
    int GetCombo() { return combo; }
    int GetMaxCombo() { return max_comboed; }

public: // Set関数
    void Clear()
    {
        is_combo = false;
        combo = 0;
        max_comboed = FLT_MIN;
        combo_duration_timer = 0.0f;
    }
    void AddCombo()
    {
        is_combo = true;
        ++combo;
        combo_duration_timer = set_combo_duration_timer;
    }
private:
    void Prepare()  // コンボ一時終了 コンボ情報のリセット
    {
        is_combo = false;
        combo = 0;
        combo_duration_timer = 0.0f;
    }

private:
    // 定数
    static constexpr float set_combo_duration_timer = 2.0f; // 連続コンボの持続時間

    // 変数
    bool is_combo = false;                      // コンボ中
    int combo = 0;                              // 現在コンボ数
    int max_comboed = INT_MIN;                  // 過去最大コンボ
    float combo_duration_timer = 0.0f;          // 仮タイマー
    std::unique_ptr<Timer> timer = nullptr;
    std::unique_ptr<Sprite> spr_icon = nullptr;
    std::unique_ptr<Sprite> spr_combo = nullptr;
};
