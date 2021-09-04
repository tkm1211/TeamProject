#include "EnemyDerrived02.h"
#include "Model.h"

EnemyDerrived02::EnemyDerrived02()
	:Enemy()
{
	model = std::make_shared<Model>("Data/Assets/Model/Enemy/enemy.fbx", false);
	Init();
	speed = { 100.0f, 0.0f, 100.0f };
	color = { 0.0f, 0.0f, 1.0f, 1.0f };
	pos = { 300.0f, 0.0f, -300.0f };
}


void EnemyDerrived02::Update(const float elapsed_time)
{
	//è„Ç©ÇÁâ∫Ç÷
	//if (pos.z >= -500.0f )
	//{
	//	pos.z -= speed.z * elapsed_time;
	//}
	//â∫Ç©ÇÁè„Ç÷
	//else if (pos.z <= 500.0f )
	//{
	//	pos.z += speed.z * elapsed_time;
	//}
	//âEÇ©ÇÁç∂Ç÷
	//else if ( pos.x >= 500.0f)
	//{
	//	pos.x -= speed.x * elapsed_time;
	//}
	//ç∂Ç©ÇÁâEÇ÷
	//else if ( pos.x <= 500.0f)
	//{
	//	pos.x += speed.x * elapsed_time;
	//}
}