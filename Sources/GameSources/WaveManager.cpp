
#include "WaveManager.h"
#include "EnemyManager.h"
#include "EnemyDerived01.h"
#include "EnemyDerrived02.h"
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

    // �G�̏o��
    if (CoolTime()) SpawnEnemy();

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

    int enemy_total = EnemyManager::Instance().GetEnemyTotalCount();
    ImGui::Text("total enemy : %d", enemy_total);
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


    // TODO : �G�̎�ނ��������瑝�₷�I�I�I
    constexpr int enemy_of_kinds = 2;
    int random = Random(0, enemy_of_kinds);


    // TODO : �G�̎�ނ��������瑝�₷�I�I�I
    std::shared_ptr<Enemy> enemy = nullptr;
    if (random == 0) enemy = std::make_shared<EnemyDerrived01>();
    if (random == 1) enemy = std::make_shared<EnemyDerrived02>();
    if (random == 2) enemy = std::make_shared<EnemyDerrivedHoming>();

    if (!enemy) return;

    
    constexpr int pos_of_kinds = 4;
    random = Random(0, pos_of_kinds);

    // �G�̏o���ʒu�̌���
    DirectX::XMFLOAT3 pos = SetRamdomPosition(random);

    enemy.get()->SetPos(pos);


    //�@�ݒ肵���G���o��������
    EnemyManager::Instance().Spawn(enemy);

    old_wave_time = timer.get()->GetNowTime();
}


bool WaveManager::CoolTime()
{
    float subtruct_time = timer.get()->GetNowTime() - old_wave_time;

    // �N�[���^�C�����߂��Ă�����
    return (subtruct_time >= wave_enemy_data[wave_state].spawn_cool_time);
}


int WaveManager::Random(const int min, const int range)
{
    return (rand() % range) + min;
}


float WaveManager::fRandom(const int min, const int range)
{
    return (float)rand() / (float)RAND_MAX * range + min;
}


DirectX::XMFLOAT3 WaveManager::SetRamdomPosition(int index)
{
    constexpr int pos_of_kinds = 4;

    constexpr float out_range = 600.0f;                         // �K���Ȑ��l
    constexpr int random_min = -300;
    constexpr int random_range = 600;

    DirectX::XMFLOAT3 random_pos[pos_of_kinds] =
    {
        {  fRandom(random_min, random_range),   0,   out_range      },                      // x,z =  random,  out
        { -out_range,                           0,   fRandom(random_min, random_range)},    // x,z = -out   ,  random
        {  out_range,                           0,  -fRandom(random_min, random_range)},    // x,z =  out   , -random
        { -fRandom(random_min, random_range),   0,  -out_range      }                       // x,z = -random, -out
    };

    return random_pos[index];
}