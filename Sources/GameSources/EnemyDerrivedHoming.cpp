#include "EnemyDerrivedHoming.h"

#include "Model.h"
#include "EnemyDerived01.h"

EnemyDerrivedHoming::EnemyDerrivedHoming()
	:Enemy()
{
	model = std::make_shared<Model>("Data/Assets/Model/Enemy/enemy.fbx", false);
	Init();
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
	pos.x += VN.x * moveSpeed * elapsed_time;
	pos.y += VN.y * moveSpeed * elapsed_time;
	pos.z += VN.z * moveSpeed * elapsed_time;
}