
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
    //if (JudgeTaskComplete()) NextWave();

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
}


bool WaveManager::JudgeTaskComplete()
{
    // 0 wave, Tutorial
    if (wave_state == wave_default)
    {

        return true;
    }

    // 1 wave, First Task
    if (wave_state == 1)
    {

        return true;
    }

    // 2 wave, Second Task
    if (wave_state == 2)
    {

        return true;
    }

    // 3 wave, Final Task
    if (wave_state == wave_max)
    {

        return true;
    }


    return false;
}