#pragma once

#include <memory>
#include "Sprite.h"
#include "Timer.h"


// �E�F�[�u�̃N���A�f�[�^
struct WaveClearData
{
    float clear_time = 0.0f;            // �N���A����
};


// �E�F�[�u�Ǘ��N���X "�V���O���g��"
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

public: // Get�֐�
    int GetNowWave() { return wave_state; };
    WaveClearData GetWaveClearData(int index) { return wave_resulted[index]; }

public: // Set�֐�
    void Clear();
    void NextWave(); 

private:
    bool JudgeTaskComplete();

private:
    static constexpr int wave_default = 0;          // �E�F�[�u�̏����l(�J�n)
    static constexpr int wave_max = 2;              // �ő�E�F�[�u�� (0 ~ 2)
    int wave_state = wave_default;                  // ���݂̃E�F�[�u
    std::unique_ptr<Timer> timer = nullptr;         // �E�F�[�u�̌o�ߎ���
    std::unique_ptr<Sprite> spr_wave = nullptr;     // �E�F�[�u�̐����p�X�v���C�g
    WaveClearData wave_resulted[wave_max+1] = {};   // �E�F�[�u���Ƃ̃N���A�^�C���Ȃǂ̃f�[�^
};