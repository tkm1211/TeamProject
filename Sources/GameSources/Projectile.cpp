#include "Projectile.h"
#include "ProjectileManager.h"

Projectile::Projectile(ProjectileManager* manager)
    : manager(manager)
{
    //manager->Register(this);
}

void Projectile::DrawDebugPrimitive()
{

}

void Projectile::Destroy()
{
   // manager->Remove(this);
}


void Projectile::UpdateTransform()
{
    DirectX::XMVECTOR Front, Up, Right;

    //前ベクトルを算出
//Front = { direction.x, direction.y, direction.z };
    Front = DirectX::XMLoadFloat3(&direction);
    Front = DirectX::XMVector3Normalize(Front);

    //仮の上ベクトルを算出
    Up = DirectX::XMVectorSet(0.01f, 1, 0, 0);
    Up = DirectX::XMVector3Normalize(Up);

    //右ベクトル算出
    Right = DirectX::XMVector3Cross(Front, Up);
    Right = DirectX::XMVector3Normalize(Right);

    //上ベクトル算出
    Up = DirectX::XMVector3Cross(Front, Right);
    Up = DirectX::XMVector3Normalize(Up);

    //計算結果を取り出し
    DirectX::XMFLOAT3 right, up, front;
    DirectX::XMStoreFloat3(&right, Right);
    DirectX::XMStoreFloat3(&up, Up);
    DirectX::XMStoreFloat3(&front, Front);


    //算出した軸ベクトルから行列を作成
    transform._11 = right.x * scale.x;
    transform._12 = right.y * scale.x;
    transform._13 = right.z * scale.x;
    transform._14 = 0;
    transform._21 = up.x * scale.y;
    transform._22 = up.y * scale.y;
    transform._23 = up.z * scale.y;
    transform._24 = 0;
    transform._31 = front.x * scale.z;
    transform._32 = front.y * scale.z;
    transform._33 = front.z * scale.z;
    transform._34 = 0;
    transform._41 = position.x;
    transform._42 = position.y;
    transform._43 = position.z;
    transform._44 = 1;

    this->direction = front;

}








