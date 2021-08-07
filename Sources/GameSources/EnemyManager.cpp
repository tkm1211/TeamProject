
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
        enemy->Update(elapsedTime);   // Q : elapsedTimeを引数にするかどうか
    }

}


void EnemyManager::RemoveUpdate()
{
    for (auto& remove : removes)
    {
        // 該当のインスタンスの検索
        auto enemy = std::find(enemies.begin(), enemies.end(), remove);

        // 検索がヒットしたら
        if (enemy != enemies.end())
        {
            // 削除
            enemies.erase(enemy);
        }

    }
    removes.clear();
}

void EnemyManager::Clear()
{
    // vectorコンテナの解放
    enemies.clear();
    removes.clear();
}