#pragma once

#include "Projectile.h"
#include <d3d11.h>
#include <GameLibSources/Shader.h>
#include <memory>
#include "OBJ3D.h"
#include "Model.h"

//弾丸
class  ProjectileNormal : public Projectile
{
public:
	 ProjectileNormal(ProjectileManager* manager);
	~ ProjectileNormal()override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	//void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//発射
	void Launch(const DirectX::XMFLOAT3& direction,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& target);



//変数の宣言
	std::unique_ptr<Model>  ProjectileModel;//ユニークポインタ
private:
	

	DirectX::XMFLOAT3  target = { 0, 0, 0 };
	float moveSpeed = 10.0f;
	float turnSpeed = DirectX::XMConvertToRadians(180);
	float lifeTimer = 3.0f;
};

