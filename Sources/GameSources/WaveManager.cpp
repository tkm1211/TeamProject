
#include "WaveManager.h"
#include "EnemyManager.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void WaveManager::Init()
{
    // ���̂̐���
    //spr_wave = std::make_unique<Sprite>("");
    timer = std::make_unique<Timer>();
    p_task = new TaskWave0();

    // ������
    for (WaveClearData& data : wave_resulted) data = {};
    Clear();

}


void WaveManager::Update()
{
    // �ŏI�E�F�[�u���N���A����΍X�V���Ȃ�
    if (wave_state >= wave_max) return;

    // �o�ߎ��Ԃ̍X�V
    timer.get()->Update();


    // �^�X�N�����������玟�̃E�F�[�u��
    if (JudgeTaskComplete()) NextWave();

}


void WaveManager::Render()
{
    // ���݂̃E�F�[�u����\��
}


void WaveManager::ImGui()
{
    ImGui::Text("wave_state : %d", wave_state);
}


void WaveManager::UnInit()
{
    Clear();
}


void WaveManager::Clear()
{
    timer.get()->Clear();
    wave_state = wave_default;
}


void WaveManager::NextWave()
{
    // �E�F�[�u�P�ʂ̃N���A���ʂ̕ۑ�
    wave_resulted[wave_state].clear_time = timer.get()->GetNowTime();

    // �G�̑S�폜
    EnemyManager::Instance().Clear();
    // ���Ԃ̃��Z�b�g
    timer.get()->Clear();
     
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

    if (wave_state == 2)
    {
        p_task = new TaskWave2();
        return;
    }

    if (wave_state == wave_max)
    {
        p_task = new TaskWave3();
        return;
    }

}