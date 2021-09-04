#include "ProjectileNormal.h"
#include "OBJ3D.h"
//�R���X�g���N�^
ProjectileNormal::ProjectileNormal(ProjectileManager* manager)
    : Projectile(manager)
{
    // ���f���̏�����
    ProjectileModel = std::make_unique<Model>("Data/Assets/Model/Player/EX/test500 kai.fbx", false);
    {
        ProjectileModel->StartAnimation(0, true); // ���� : FBX���̃A�j���[�V�����ԍ�, ���[�v�Đ����邩�H
        //Init();
        //OBJ3D::Init();
    }

}

//�f�X�g���N�^
ProjectileNormal::~ProjectileNormal()
{
    //UnInit();
    //delete ProjectileModel;
}

void ProjectileNormal::Update(float elapsedTime)
{
    //��������
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        //�������폜
        Destroy();
    }

    //�ړ�
    {
        float moveSpeed = this->moveSpeed * elapsedTime;
        position.x += direction.x * moveSpeed; //direction�͒e�̐i�s�����x�N�g��;
        position.y += direction.y * moveSpeed;
        position.z += direction.z * moveSpeed; //�i�s�����x�N�g��.���̎� * speed;
    }
    //����
    {
        float turnSpeed = this->turnSpeed * elapsedTime;

        //�^�[�Q�b�g�܂ł̃x�N�g�����Z�o
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        //�[���x�N�g���łȂ��Ȃ��]����
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        if (lengthSq > 0.00001f)
        {
            //�^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
            Vec = DirectX::XMVector3Normalize(Vec);

            //�����Ă�������x�N�g�����Z�o
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

            //  Direction = DirectX::XMVector3Normalize(Direction);

              //�O�����x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓���(�p�x)���Z�o
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Direction);
            // DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            //2�̒P�ʃx�N�g���̊p�x���������ق�
            //1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
            float rot = 1.0 - dot;  //�␳�l(���ϒl-1.0�`1.0���p�x���F0.0�`2.0�F�p�x��ɕ␳���܂�)
            //���ϒl��-1.0�`1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
            //�������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
            if (rot > turnSpeed)
            {
                rot = turnSpeed;
            }


            //��]�p�x������Ȃ��]����������
            if (fabsf(rot) > 0.00001f)
            {
                //��]�����Z�o
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Vec, Direction);
                //��]���Ɖ�]�ʂ����]�s����Z�o
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

                //���݂̍s�����]������
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                //Transform = Rotation * Transform;
                Transform = DirectX::XMMatrixMultiply(Rotation, Transform);

                //��]��̑O���������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);
                DirectX::XMStoreFloat3(&direction, Direction);
            }
        }

    }

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

}

//void ProjectileNormal::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //shader->Draw(dc, ProjectileModel);
//}

void ProjectileNormal::Launch(const DirectX::XMFLOAT3& direction,
    const DirectX::XMFLOAT3& position,
    const DirectX::XMFLOAT3& target)
{
    this->direction = direction;
    this->position = position;
    this->target = target;

    // DirectX::XMVECTOR Direction 

    UpdateTransform();
}







