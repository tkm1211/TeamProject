
#include "ProjectileManager.h"




ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    Clear();
}

void ProjectileManager::Update(float elapsedTime)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Update(elapsedTime);

    }

    //破棄処理
    // ※projectilesの範囲for分中でerase()すると不具合が発生してしまうため、
    //  更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
    for (Projectile* projectile : removes)
    {
        //std::vectorから要素を削除する場合はイテレーターで削除しなければならない
        std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);

        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }

        //弾丸の破棄処理
        delete projectile;

    }
    //破棄リストをクリア
    removes.clear();
}

//弾丸削除
void ProjectileManager::Remove(Projectile* projectile)
{
    //破棄リストに追加
    removes.emplace_back(projectile);
}


void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Projectile* projectile : projectiles)
    {
      projectile->Render(dc, shader);
    }
}

void ProjectileManager::DrawDebugPrimitive()
{
    for (Projectile* projectile : projectiles)  //拡張配列(vector)で宣言されたprojectilesのアドレスをいれてる
    {
        projectile->DrawDebugPrimitive();
    }
}

void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
    if (!projectiles.empty())
    {
        for (Projectile* projectile : projectiles)
        {
            delete projectile;
        }

    }
}























