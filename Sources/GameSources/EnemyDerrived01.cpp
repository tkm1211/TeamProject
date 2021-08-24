#include "EnemyDerived01.h"

#include "Model.h"

EnemyDerrived01::EnemyDerrived01()
	:Enemy()
{
	model = std::make_shared<Model>("Data/Assets/Model/Enemy/enemy.fbx", false);
	Init();
<<<<<<< HEAD

	speed.x = 1;
	A = 0.0f; B = 0.1f;
=======
	color = DirectX::XMFLOAT4(1.0f, 0, 0, 1.0f);
	speed.x = 2;
	timer = 0;
>>>>>>> 38e2fae63bf96a464e169d65347fd1938a25bec3
}


void EnemyDerrived01::Update(const float elapsed_time)
{
	// TODO: ’¼üˆÚ“®ˆ—
	// ƒqƒ“ƒg : pos‚ğg—p
<<<<<<< HEAD
    pos.x += speed.x;
	//pos.z--;
	timer++;
	
	A += B * elapsed_time;
	if (A >=  1.0f|| A < 0.0f)
	{
		B = -B;
	}
	color={ 1.0f, 1.0f, 1.0f, A };
	//0.5fˆÈ‰º‚É‚È‚é‚ÆÁ‚¦‚½
	//A‚Ì‰Šú‚ª‚O‚¾‚©‚ç‚Í‚¶‚ßÁ‚¦‚é‚Í‚¸‚È‚Ì‚Éˆêu‚¾‚¯•\¦‚³‚ê‚é
	if (timer % 70 == 0)
	{
		speed.x = -speed.x;
	}
	
	/*if (pos.x >= 40 || pos.x < -40)
	{
		speed.x = -speed.x;
	}*/
=======
	
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
>>>>>>> 38e2fae63bf96a464e169d65347fd1938a25bec3
}