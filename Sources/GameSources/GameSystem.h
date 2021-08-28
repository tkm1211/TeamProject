#pragma once

#include <memory>

#include "Timer.h"
#include "ScoreManager.h"
#include "WaveManager.h"
#include "Combo.h"


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
    float GetNowTime() { return timer.get()->GetNowTime(); }
    float GetScored() { return score_manager.get()->GetScored(); }
    int GetCombo() { return combo.get()->GetCombo(); }
    int GetMaxCombo() { return combo.get()->GetMaxCombo(); }
    float GetWaveClearTime(int index) { return wave_manager.get()->GetWaveClearData(index).clear_time; }

public: // Set�֐�
    void Clear()
    {
        timer.get()->Clear();
        score_manager.get()->Clear();
        wave_manager.get()->Clear();
        combo.get()->Clear();
    }
    void KilledEnemy(float enemy_score);    // �G��|�����Ƃ��̃X�R�A�ƃR���{�̉��Z����
    void DamagedPlayer();

private:
    std::unique_ptr<Timer> timer = nullptr;
    std::unique_ptr<ScoreManager> score_manager = nullptr;
    std::unique_ptr<WaveManager> wave_manager = nullptr;
    std::unique_ptr<Combo> combo = nullptr;

    std::unique_ptr<Sprite> spr_ui_time_score = nullptr;
};