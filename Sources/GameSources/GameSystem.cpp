
#include "GameSystem.h"


void GameSystem::Init()
{
    // ���̂̐���
    timer = std::make_unique<Timer>();
    score_manager = std::make_unique<ScoreManager>();

    // ����������
    timer.get()->Initialize();
    score_manager.get()->Initialize();
}


void GameSystem::UnInit()
{
    // �I������
    timer.get()->UnInitialize();
    score_manager.get()->UnInitialize();
}

void GameSystem::Update()
{
    // �X�V
    timer.get()->Update();
    score_manager.get()->Update();
}


void GameSystem::Render()
{
    // �`��
    timer.get()->Render();
    score_manager.get()->Render();
}

void GameSystem::ImGui()
{
    ImGui::Text("timer %f", timer.get()->GetNowTime());
}