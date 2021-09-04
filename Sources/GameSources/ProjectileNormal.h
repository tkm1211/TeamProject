#pragma once

#include "Projectile.h"
#include <d3d11.h>
#include <GameLibSources/Shader.h>
#include <memory>
#include "OBJ3D.h"
#include "Model.h"

//�e��
class  ProjectileNormal : public Projectile
{
public:
	 ProjectileNormal(ProjectileManager* manager);
	~ ProjectileNormal()override;

	//�X�V����
	void Update(float elapsedTime) override;

	//�`�揈��
	//void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//����
	void Launch(const DirectX::XMFLOAT3& direction,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& target);



//�ϐ��̐錾
	std::unique_ptr<Model>  ProjectileModel;//���j�[�N�|�C���^
private:
	

	DirectX::XMFLOAT3  target = { 0, 0, 0 };
	float moveSpeed = 10.0f;
	float turnSpeed = DirectX::XMConvertToRadians(180);
	float lifeTimer = 3.0f;
};

