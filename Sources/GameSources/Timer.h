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

public: // Getä÷êî
    float GetNowTime() { return time; }

public: // Setä÷êî
    void Clear() { time = 0.0f; }

private:
    float time = 0.0f;
    std::unique_ptr<Sprite> spr_timer = nullptr;
};