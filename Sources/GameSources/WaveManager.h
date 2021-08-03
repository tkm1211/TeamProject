#pragma once

#include <memory>
#include "Sprite.h"
#include "Timer.h"


// ウェーブのクリアデータ
struct WaveClearData
{
    float clear_time = 0.0f;            // クリア時間
};


// ウェーブ管理クラス "シングルトン"
class WaveManager
{
private:
    WaveManager() {};
    ~WaveManager() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();
    void ImGui();

    static WaveManager& Instance()
    {
        static WaveManager instance;
        return instance;
    }

public: // Get関数
    int GetNowWave() { return wave_state; };
    WaveClearData GetWaveClearData(int index) { return wave_resulted[index]; }

public: // Set関数
    void Clear();
    void NextWave(); 

private:
    bool JudgeTaskComplete();

private:
    static constexpr int wave_default = 0;          // ウェーブの初期値(開始)
    static constexpr int wave_max = 2;              // 最大ウェーブ数 (0 ~ 2)
    int wave_state = wave_default;                  // 現在のウェーブ
    std::unique_ptr<Timer> timer = nullptr;         // ウェーブの経過時間
    std::unique_ptr<Sprite> spr_wave = nullptr;     // ウェーブの数字用スプライト
    WaveClearData wave_resulted[wave_max+1] = {};   // ウェーブごとのクリアタイムなどのデータ
};