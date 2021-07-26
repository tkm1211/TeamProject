#pragma once
#include <DirectXMath.h>


// ���[���h�s�񂩂�ړ������݂̂𒊏o����
DirectX::XMMATRIX ExtractOffset(const DirectX::XMMATRIX& mWorld);

// ���[���h�s�񂩂�傫�������݂̂𒊏o����
DirectX::XMMATRIX ExtractScaling(const DirectX::XMMATRIX& mWorld);

// ���[���h�s�񂩂��]�����݂̂𒊏o����
DirectX::XMMATRIX ExtractRotation(const DirectX::XMMATRIX& mWorld);

// �X�N���[�����W�����[���h���W�ɕϊ�
DirectX::XMFLOAT3* CalcScreenToWorld(DirectX::XMFLOAT3* pout, int screenX, int screenY, float fZ, int screenW, int screenH, DirectX::XMMATRIX* view, DirectX::XMMATRIX* prj);

// ���ʐ��`���
DirectX::XMFLOAT3 SphereLinear(DirectX::XMFLOAT3 originPos, DirectX::XMFLOAT3 oldPos, DirectX::XMFLOAT3 nowPos, float t);