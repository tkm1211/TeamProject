
#include "EnemyManager.h"
#include "FrameWork.h"
#include "Enemy.h"
#include <algorithm>


void EnemyManager::Init()
{
    Clear();
}


void EnemyManager::UnInit()
{
    Clear();
}


void EnemyManager::Update()
{
    RemoveUpdate();
    EnemyUpdate();
}


void EnemyManager::Render()
{
    for (auto& enemy : enemies)
    {
        enemy->Render();
    }
}


void EnemyManager::EnemyUpdate()
{
    float elapsedTime = FrameWork::GetInstance().GetElapsedTime();
    for (auto& enemy : enemies)
    {
        if (!enemy) continue;
        enemy->Update(elapsedTime);   // Q : elapsedTime�������ɂ��邩�ǂ���
    }

}


void EnemyManager::RemoveUpdate()
{
    for (auto& remove : removes)
    {
        // �Y���̃C���X�^���X�̌���
        auto enemy = std::find(enemies.begin(), enemies.end(), remove);

        // �������q�b�g������
        if (enemy != enemies.end())
        {
            // �폜
            enemies.erase(enemy);
        }

    }
    removes.clear();
}

void EnemyManager::Clear()
{
    // vector�R���e�i�̉��
    enemies.clear();
    removes.clear();
}