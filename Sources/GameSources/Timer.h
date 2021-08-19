#pragma once

#include <memory>
#include "Sprite.h"


class Timer
{
public:
    Timer() {};
    ~Timer() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();

public: // Get関数
    float GetNowTime() { return time; }

public: // Set関数
    void Clear() { time = 0.0f; }
    void SetTime(int time_) { time = time_; }

private:
    // クラス内だけで使える関数 (クラスの外では関数が呼び出せない、使えない)
    void SliceDigits(float digits_, float& ones_place, float& tens_place, float& hundreds_place);
    void CountUp();
    void CountDown();

private:
    float time = 0.0f;
    std::unique_ptr<Sprite> spr_timer = nullptr;
    bool is_countdown = false;
};