#include "EnemyDerrivedHoming.h"
#include "Model.h"

EnemyDerrivedHoming::EnemyDerrivedHoming()
	:Enemy()
{
	model = std::make_shared<Model>("Data/Assets/Model/Enemy/enemy.fbx", false);
	Init();
	speed = { 100.0f, 0.0f, 100.0f };
	color = { 0.0f, 1.0f, 0.0f, 1.0f };
	pos = { 200.0f, 0.0f, 200.0f };
}


void EnemyDerrivedHoming::Update(const float elapsed_time)
{
	//プレイヤーをターゲットにする
	float dist = FLT_MAX;

	//プレイヤーとの距離判定
	DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(player_position);
	DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
	V = DirectX::XMVector3Normalize(V);
	DirectX::XMFLOAT3 VN;
	DirectX::XMStoreFloat3(&VN, V);
	pos.x += VN.x * speed.x * elapsed_time;
	pos.y += VN.y * speed.y * elapsed_time;
	pos.z += VN.z * speed.z * elapsed_time;
}