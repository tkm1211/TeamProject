
#include "ProjectileManager.h"




ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    Clear();
}

void ProjectileManager::Update(float elapsedTime)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Update(elapsedTime);

    }

    //�j������
    // ��projectiles�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    //  �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
    for (Projectile* projectile : removes)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);

        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }

        //�e�ۂ̔j������
        delete projectile;

    }
    //�j�����X�g���N���A
    removes.clear();
}

//�e�ۍ폜
void ProjectileManager::Remove(Projectile* projectile)
{
    //�j�����X�g�ɒǉ�
    removes.emplace_back(projectile);
}


void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Projectile* projectile : projectiles)
    {
      projectile->Render(dc, shader);
    }
}

void ProjectileManager::DrawDebugPrimitive()
{
    for (Projectile* projectile : projectiles)  //�g���z��(vector)�Ő錾���ꂽprojectiles�̃A�h���X������Ă�
    {
        projectile->DrawDebugPrimitive();
    }
}

void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
    if (!projectiles.empty())
    {
        for (Projectile* projectile : projectiles)
        {
            delete projectile;
        }

    }
}























