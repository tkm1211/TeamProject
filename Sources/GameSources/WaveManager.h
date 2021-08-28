#pragma once

#include <memory>
#include <map>
#include "Sprite.h"
#include "Timer.h"
#include "Task.h"
class Enemy;


// ウェーブ管理クラス
class WaveManager
{
private:
    // 敵の出現データ
    struct SpawnEnemyData
    {
        bool has_boss = false;              // ボスがいるか
        int enemy_max = 0;                  // 最大出現数
        float spawn_cool_time = 0.0f;       // 生成して次の生成まで空ける時間
    };
    // クリアデータ
    struct ClearData
    {
        float clear_time = 0.0f;            // クリア時間
    };

public:
    WaveManager() {};
    ~WaveManager() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();
    void ImGui();


public: // Get関数
    int GetNowWave() { return wave_state; };
    int GetWaveMax() { return wave_max; }
    ClearData GetWaveClearData(int index) { return wave_resulted[index]; }

public: // Set関数
    void Clear();
    void NextWave(); 

private:
    void SetEnemyData();
    bool JudgeTaskComplete();
    void ChangeNextTask();
    void SpawnEnemy();

private:
    // 定数
    static constexpr int wave_default = 0;          // ウェーブの初期値(開始)
    static constexpr int wave_max = 3;              // 最大ウェーブ数 (0, 1 ~ 3)
    // 変数
    int wave_state = wave_default;                  // 現在のウェーブ
    float old_wave_time = 0.0f;                     // 前回出現させたときの時間
    std::unique_ptr<Timer> timer = nullptr;         // ウェーブの経過時間
    std::unique_ptr<Sprite> spr_wave = nullptr;     // ウェーブの数字用スプライト
    SpawnEnemyData wave_enemy_data[wave_max + 1] = {};      // ウェーブごとの敵の出現設定
    ClearData wave_resulted[wave_max + 1] = {};             // ウェーブごとのクリアタイムなどのデータ
    Task* p_task = nullptr;                                 // タスク
};