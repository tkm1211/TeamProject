#pragma once
#include "Enemy.h"


class EnemyDerrivedHoming final : public Enemy
{
public:
	EnemyDerrivedHoming();
	void Update(const float elapsed_time) override;
};
