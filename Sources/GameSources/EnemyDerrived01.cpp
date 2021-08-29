#include "EnemyDerived01.h"
#include "Enemy.h"
#include "Model.h"

EnemyDerrived01::EnemyDerrived01()
	:Enemy()
{
	model = std::make_shared<Model>("Data/Assets/Model/Enemy/enemy.fbx", false);
	Init();


	speed.x = 1;
	A = 0.0f; B = 0.1f;

	color = DirectX::XMFLOAT4(1.0f, 0, 0, 1.0f);
	speed.x = 2;
	timer = 0;

}


void EnemyDerrived01::Update(const float elapsed_time)
{
	// TODO: ’¼üˆÚ“®ˆ—
	// ƒqƒ“ƒg : pos‚ðŽg—p

	/*A += B * elapsed_time;
	if (A >=  1.0f|| A < 0.0f)
	{
		B = -B;
	}*/
	color={ 1.0f, 0.0f, 0.0f, 1.0f };
	
	/*if (pos.x >= 40 || pos.x < -40)
	{
		speed.x = -speed.x;
	}*/

	

	pos.z--;
	pos.x += speed.x;

	if (timer % 70 == 0)
	{
		speed.x =-speed.x;
	}
	
	timer++;
	
}