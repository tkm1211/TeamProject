#include "EnemyDerived01.h"

#include "Model.h"

EnemyDerrived01::EnemyDerrived01()
	:Enemy()
{
	model = std::make_shared<Model>("Data/Assets/Model/Enemy/enemy.fbx", false);
	Init();
	color = DirectX::XMFLOAT4(1.0f, 0, 0, 1.0f);
	speed.x = 2;
	timer = 0;
}


void EnemyDerrived01::Update(const float elapsed_time)
{
	// TODO: 直線移動処理
	// ヒント : posを使用
	
<<<<<<< HEAD

	pos.z--;
	pos.x += speed.x;

	if (timer % 70 == 0)
	{
		speed.x =-speed.x;
	}
	
	timer++;
	
=======
>>>>>>> a30e87e98fc724fbd67a4ab87e056761db0424cb
}