#include "EnemyDerived01.h"

#include "Model.h"

EnemyDerrived01::EnemyDerrived01()
	:Enemy()
{
	model = std::make_shared<Model>("Data/Assets/Model/Enemy/enemy.fbx", false);
	Init();

	speed.x = 1;
	A = 0.0f; B = 0.1f;
}


void EnemyDerrived01::Update(const float elapsed_time)
{
	// TODO: 直線移動処理
	// ヒント : posを使用
    pos.x += speed.x;
	//pos.z--;
	timer++;
	
	A += B * elapsed_time;
	if (A >=  1.0f|| A < 0.0f)
	{
		B = -B;
	}
	color={ 1.0f, 1.0f, 1.0f, A };
	//0.5f以下になると消えた
	//Aの初期が０だからはじめ消えるはずなのに一瞬だけ表示される
	if (timer % 70 == 0)
	{
		speed.x = -speed.x;
	}
	
	/*if (pos.x >= 40 || pos.x < -40)
	{
		speed.x = -speed.x;
	}*/
}