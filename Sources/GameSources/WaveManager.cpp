
#include "WaveManager.h"
#include "EnemyManager.h"
#include "EnemyDerived01.h"
#include "EnemyDerrivedHoming.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void WaveManager::Init()
{
    // ���̂̐���
    spr_wave = std::make_unique<Sprite>(L"Data/Assets/Texture/wave.png");
    timer = std::make_unique<Timer>();
    p_task = new TaskWave0();

    // ������
    SetEnemyData();
    for (ClearData& data : wave_resulted) data = {};
    Clear();

}


void WaveManager::Update()
{
    // �ŏI�E�F�[�u���N���A����΍X�V���Ȃ�
    if (wave_state > wave_max) return;

    // �o�ߎ��Ԃ̍X�V
    timer.get()->Update();

    SpawnEnemy();

    // �^�X�N�����������玟�̃E�F�[�u��
    if (JudgeTaskComplete()) NextWave();

}


void WaveManager::Render()
{
    constexpr float end_time_second = 2.0f;
    if (wave_state - 1 < 0)return;
    if (timer.get()->GetNowTime() > end_time_second) return;


    constexpr float width = 400.0f;
    constexpr float height = 280.0f;

    // ���݂̃E�F�[�u����\��
    spr_wave.get()->Draw2(
        640, 300,
        width, height,
        0.0f, 0.0f + height * static_cast<float>(wave_state - 1),
        width, height,
        0,
        1, 1, 1, 1);
}


void WaveManager::ImGui()
{
    ImGui::Text("wave_state : %d", wave_state);
    ImGui::Text("total enemy : %d", EnemyManager::Instance().GetEnemyTotalCount());
}


void WaveManager::UnInit()
{
    Clear();
}


void WaveManager::Clear()
{
    timer.get()->Clear();
    old_wave_time = 0.0f;
    wave_state = wave_default;
}


void WaveManager::SetEnemyData()
{
    wave_enemy_data[0] =
    {
        false,
        3,
        2.0f
    };

    wave_enemy_data[1] =
    {
        false,
        4,
        2.0f
    };

    wave_enemy_data[2] =
    {
        false,
        5,
        2.0f
    };

    wave_enemy_data[3] =
    {
        true,
        6,
        1.0f
    };
}


void WaveManager::NextWave()
{
    // �E�F�[�u�̃N���A���ʂ̕ۑ�
    wave_resulted[wave_state].clear_time = timer.get()->GetNowTime();

    // �G�̑S�폜
    EnemyManager::Instance().Clear();
    // ���Ԃ̃��Z�b�g
    timer.get()->Clear();
    old_wave_time = 0.0f;
     
    // ���̃E�F�[�u�ɂ���
    wave_state++;
    // �^�X�N�̐i�s
    ChangeNextTask();
}


bool WaveManager::JudgeTaskComplete()
{
    if (!p_task) return false;

    return p_task->JudgeTaskComplete();
}


void WaveManager::ChangeNextTask()
{
    // �O��^�X�N�̍폜����
    if (p_task)
    {
        delete p_task;
        p_task = nullptr;
    }

    // �^�X�N���w�肵�Ȃ�
    if (wave_state >= wave_max)return;

    // �E�F�[�u1�̃^�X�N
    if (wave_state == 1)
    {
        p_task = new TaskWave1();
        return;
    }

    // �E�F�[�u2�̃^�X�N
    if (wave_state == 2)
    {
        p_task = new TaskWave2();
        return;
    }

    // �E�F�[�u3�̃^�X�N
    if (wave_state == wave_max)
    {
        p_task = new TaskWave3();
        return;
    }

}


void WaveManager::SpawnEnemy()
{
    // �G���o�������Ȃ�
    if (EnemyManager::Instance().GetEnemyTotalCount() >= wave_enemy_data[wave_state].enemy_max) return;



    float subtruct_time = timer.get()->GetNowTime() - old_wave_time;

    // �N�[���^�C�����߂��Ă�����
    if (subtruct_time < wave_enemy_data[wave_state].spawn_cool_time) return;

    // TODO : ��������G�̎�ނ𑝂₷
    constexpr int enemy_of_kinds = 2;
    int random = rand() % enemy_of_kinds;

    // TODO : �G�̎�ނ��������瑝�₷�I�I�I
    std::shared_ptr<Enemy> enemy = nullptr;
    if (random == 0) enemy = std::make_shared<EnemyDerrived01>();
    if (random == 1) enemy = std::make_shared<EnemyDerrivedHoming>();

    if (!enemy) return;
    //�@�o��������
    EnemyManager::Instance().Spawn(enemy);

    old_wave_time = timer.get()->GetNowTime();
}