#pragma once
#include "Enemy.h"


class EnemyDerrived02 final : public Enemy
{
public:
	EnemyDerrived02();
	void Update(const float elapsed_time) override;
};
