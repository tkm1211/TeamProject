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
	//�ォ�牺��
	//if (pos.z >= -500.0f )
	//{
	//	pos.z -= speed.z * elapsed_time;
	//}
	//��������
	//else if (pos.z <= 500.0f )
	//{
	//	pos.z += speed.z * elapsed_time;
	//}
	//�E���獶��
	//else if ( pos.x >= 500.0f)
	//{
	//	pos.x -= speed.x * elapsed_time;
	//}
	//������E��
	//else if ( pos.x <= 500.0f)
	//{
	//	pos.x += speed.x * elapsed_time;
	//}
}