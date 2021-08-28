#pragma once
#include "Enemy.h"


class EnemyDerrivedHoming final : public Enemy
{
public:
	EnemyDerrivedHoming();
	void Update(const float elapsed_time) override;

private:
	float			  moveSpeed = 100.0f;

protected:
	DirectX::XMFLOAT3	direction = { 0, 0, 1 };
};
