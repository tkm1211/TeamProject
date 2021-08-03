#pragma once

#include <memory>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

#include "Timer.h"
#include "ScoreManager.h"


class GameSystem
{
private:
    GameSystem() {};
    ~GameSystem() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();
    void ImGui();

    static GameSystem& Instance()
    {
        static GameSystem instance;
        return instance;
    }

public: // Getä÷êî
    Timer* GetTimer() { return timer.get(); }
    ScoreManager* GetScoreManager() { return score_manager.get(); }

public: // Setä÷êî
    void Clear()
    {
        
    }

private:
    std::unique_ptr<Timer> timer = nullptr;
    std::unique_ptr<ScoreManager> score_manager = nullptr;

};