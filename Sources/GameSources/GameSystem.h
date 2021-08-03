#pragma once

#include <memory>

#include "Timer.h"
#include "ScoreManager.h"


// �Q�[���V�X�e���Ǘ��N���X "�V���O���g��"
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

public: // Get�֐�
    Timer* GetTimer() { return timer.get(); }
    ScoreManager* GetScoreManager() { return score_manager.get(); }

public: // Set�֐�
    void Clear()
    {
        
    }

private:
    std::unique_ptr<Timer> timer = nullptr;
    std::unique_ptr<ScoreManager> score_manager = nullptr;

};