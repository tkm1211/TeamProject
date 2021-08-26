#pragma once
#include "Enemy.h"

class EnemyDerrived01 final : public Enemy
{
public:
	EnemyDerrived01();
	void Update(const float elapsed_time) override;
	float A = 0, B = 0.1f;
};


