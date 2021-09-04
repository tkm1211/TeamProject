#pragma once
#include <DirectXMath.h>
#include <memory>
#include "OBJ3D.h"
#include "./GameLibSources/Shader.h"

class ProjectileManager;  //�O���錾�͑��݃C���N���[�h�h�~�ׂ̈ɂ��

//�e��
class Projectile :public OBJ3D
{
public:

    Projectile(ProjectileManager* manager);  //�o�^�����}�l�[�W���[��ێ�����悤�ɂ���
    virtual ~Projectile() {}


    //�X�V����
    virtual void Update(float elapsedTime) = 0;

    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    //�f�o�b�N�v���~�e�B�u�`��
    virtual void DrawDebugPrimitive();


    //�ʒu���擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    //�����擾
    const DirectX::XMFLOAT3& GetDirection() const { return direction; }

    //�X�P�[���擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    //���a�擾
    float GetRadius() const { return radius; }


    //�j��
    void Destroy();

protected:
    //�s��X�V����
    void UpdateTransform();



protected:
    DirectX::XMFLOAT3  position = { 0, 0, 0 };
    DirectX::XMFLOAT3  direction = { 0, 0, 1 };
    DirectX::XMFLOAT3  scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4  transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

    float radius = 0.5f;

    ProjectileManager* manager = nullptr;

};


























