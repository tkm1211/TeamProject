#include "EnemyDerived01.h"

#include "Model.h"

EnemyDerrived01::EnemyDerrived01()
	:Enemy()
{
	model = std::make_shared<Model>("Data/Assets/Model/Enemy/enemy.fbx", false);
	Init();
}


void EnemyDerrived01::Update(const float elapsed_time)
{

}