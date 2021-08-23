#pragma once

#include <memory>
#include <map>
#include "Sprite.h"
#include "Timer.h"
#include "Task.h"



// �E�F�[�u�Ǘ��N���X
class WaveManager
{
private:
    // �G�̏o���f�[�^
    struct SpawnEnemyData
    {
        bool has_boss = false;              // �{�X�����邩
        int enemy_max = 0;                  // �ő�o����
        float spawn_cool_time = 0.0f;       // �������Ď��̐����܂ŋ󂯂鎞��
    };
    // �N���A�f�[�^
    struct ClearData
    {
        float clear_time = 0.0f;            // �N���A����
    };

public:
    WaveManager() {};
    ~WaveManager() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();
    void ImGui();


public: // Get�֐�
    int GetNowWave() { return wave_state; };
    ClearData GetWaveClearData(int index) { return wave_resulted[index]; }

public: // Set�֐�
    void Clear();
    void NextWave(); 

private:
    void SetEnemyData();
    bool JudgeTaskComplete();
    void ChangeNextTask();
    void SpawnEnemy();
    void SetCreateEnemy();

private:
    // �萔
    static constexpr int wave_default = 0;          // �E�F�[�u�̏����l(�J�n)
    static constexpr int wave_max = 3;              // �ő�E�F�[�u�� (0, 1 ~ 3)
    // �ϐ�
    int wave_state = wave_default;                  // ���݂̃E�F�[�u
    std::unique_ptr<Timer> timer = nullptr;         // �E�F�[�u�̌o�ߎ���
    std::unique_ptr<Sprite> spr_wave = nullptr;     // �E�F�[�u�̐����p�X�v���C�g
    std::map<int, std::shared_ptr<Enemy>> create_enemy = {}; // �G�̐����p
    SpawnEnemyData wave_enemy_data[wave_max + 1] = {};      // �E�F�[�u���Ƃ̓G�̏o���ݒ�
    ClearData wave_resulted[wave_max + 1] = {};             // �E�F�[�u���Ƃ̃N���A�^�C���Ȃǂ̃f�[�^
    Task* p_task = nullptr;                                 // �^�X�N
};