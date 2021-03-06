#pragma once
#include <DirectXMath.h>
#include <memory>
#include "OBJ3D.h"
#include "./GameLibSources/Shader.h"

class ProjectileManager;  //前方宣言は相互インクルード防止の為にやる

//弾丸
class Projectile :public OBJ3D
{
public:

    Projectile(ProjectileManager* manager);  //登録されるマネージャーを保持するようにする
    virtual ~Projectile() {}


    //更新処理
    virtual void Update(float elapsedTime) = 0;

    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    //デバックプリミティブ描画
    virtual void DrawDebugPrimitive();


    //位置情報取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    //方向取得
    const DirectX::XMFLOAT3& GetDirection() const { return direction; }

    //スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    //半径取得
    float GetRadius() const { return radius; }


    //破棄
    void Destroy();

protected:
    //行列更新処理
    void UpdateTransform();



protected:
    DirectX::XMFLOAT3  position = { 0, 0, 0 };
    DirectX::XMFLOAT3  direction = { 0, 0, 1 };
    DirectX::XMFLOAT3  scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4  transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

    float radius = 0.5f;

    ProjectileManager* manager = nullptr;

};


























