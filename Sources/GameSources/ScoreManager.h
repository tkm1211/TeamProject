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

public: // Get�֐�
    int GetScored() { return scored; }

public: // Set�֐�
    void Clear() { scored = 0; }

private:
    std::unique_ptr<Sprite> spr_score = nullptr;
    int scored = 0;

};
