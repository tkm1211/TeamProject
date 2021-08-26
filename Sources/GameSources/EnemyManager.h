#pragma once

#include <memory>
#include <vector>
#include "Enemy.h"


// �G�Ǘ��N���X "�V���O���g��"
class EnemyManager
{
private:
    EnemyManager() {};
    ~EnemyManager() {};

public:
    void Init(DirectX::XMFLOAT3* player_pos = nullptr);
    void UnInit();
    void Update();
    void Render();

    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

public: // Get�֐�
    Enemy* GetEnemy(int index) { return enemies.at(index).get(); }
    int GetEnemyTotalCount() { return enemies.size(); }

public: // Set�֐�
    void Clear();
    void Spawn(std::shared_ptr<Enemy>& enemy_);
    void Remove(Enemy* enemy_) { removes.emplace_back(enemy_); }


private:
    void EnemyUpdate();
    void RemoveUpdate();

private:
    std::vector<std::shared_ptr<Enemy>> enemies = {};
    std::vector<std::shared_ptr<Enemy>> removes = {};

    DirectX::XMFLOAT3* player_position = {};
};