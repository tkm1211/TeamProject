#pragma once

#include <memory>
#include <vector>
class Enemy;


// 敵管理クラス "シングルトン"
class EnemyManager
{
private:
    EnemyManager() {};
    ~EnemyManager() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();

    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

public: // Get関数
    Enemy* GetEnemy(int index) { return enemies.at(index).get(); }
    int GetEnemyTotalCount() { return enemies.size(); }

public: // Set関数
    void Clear();
    void Spawn(Enemy* enemy_) { enemies.emplace_back(enemy_); }
    void Remove(Enemy* enemy_) { removes.emplace_back(enemy_); }

private:
    void EnemyUpdate();
    void RemoveUpdate();

private:
    std::vector<std::shared_ptr<Enemy>> enemies = {};
    std::vector<std::shared_ptr<Enemy>> removes = {};
};