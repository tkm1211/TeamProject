#pragma once

#include <memory>
#include "Sprite.h"


class Timer
{
public:
    Timer(bool is_countdown_ = false) : is_countdown(is_countdown_) {};
    ~Timer() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render(const DirectX::XMFLOAT2 pos = {});

public: // Get関数
    float GetNowTime() { return time; }

public: // Set関数
    void Clear() { time = 0.0f; }
    void SetTime(int time_) { time = static_cast<float>(time_); }

private:
    // クラス内だけで使える関数 (クラスの外では関数が呼び出せない、使えない)
    void SliceDigits(float digits_, float& ones_place, float& tens_place, float& hundreds_place);
    void CountUp(float elapsedTime);
    void CountDown(float elapsedTime);

private:
    float time = 0.0f;
    std::unique_ptr<Sprite> spr_timer = nullptr;
    bool is_countdown = false;
};