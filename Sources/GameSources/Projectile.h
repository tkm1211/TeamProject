#pragma once
#include <DirectXMath.h>


//�e��
class Projectile
{
public:


    //�X�V����
    virtual void Update(float elapsedTime) = 0;


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


protected:
    //�s��X�V����
    void UpdateTransform();



protected:
    DirectX::XMFLOAT3  position = { 0, 0, 0 };
    DirectX::XMFLOAT3  direction = { 0, 0, 1 };
    DirectX::XMFLOAT3  scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4  transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

    float radius = 0.5f;



};


























