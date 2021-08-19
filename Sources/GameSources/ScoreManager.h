#pragma once

#include <memory>
#include "Sprite.h"



class ScoreManager
{
public:
    ScoreManager() {};
    ~ScoreManager() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();

public: // Getä÷êî
    float GetScored() { return scored; }

public: // Setä÷êî
    void Clear() { scored = 0.0f; }
    void AddScore(float add_score_) { scored += add_score_; }

private:
    std::unique_ptr<Sprite> spr_score = nullptr;
    float scored = 0;

};
