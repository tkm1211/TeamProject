#pragma once
#include <DirectXMath.h>


// ワールド行列から移動成分のみを抽出する
DirectX::XMMATRIX ExtractOffset(const DirectX::XMMATRIX& mWorld);

// ワールド行列から大きさ成分のみを抽出する
DirectX::XMMATRIX ExtractScaling(const DirectX::XMMATRIX& mWorld);

// ワールド行列から回転成分のみを抽出する
DirectX::XMMATRIX ExtractRotation(const DirectX::XMMATRIX& mWorld);

// スクリーン座標をワールド座標に変換
DirectX::XMFLOAT3* CalcScreenToWorld(DirectX::XMFLOAT3* pout, int screenX, int screenY, float fZ, int screenW, int screenH, DirectX::XMMATRIX* view, DirectX::XMMATRIX* prj);

// 球面線形補間
DirectX::XMFLOAT3 SphereLinear(DirectX::XMFLOAT3 originPos, DirectX::XMFLOAT3 oldPos, DirectX::XMFLOAT3 nowPos, float t);