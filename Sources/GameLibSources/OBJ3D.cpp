#include "OBJ3D.h"
#include "FrameWork.h"


DirectX::XMMATRIX OBJ3D::GetWorldMatrix()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

	//	初期化
	world = DirectX::XMMatrixIdentity();

	//	拡大・縮小
	S = DirectX::XMMatrixScaling( scale.x, scale.y, scale.z );

	//	回転
	Rx = DirectX::XMMatrixRotationX( angle.x );	// X軸を基準とした回転行列
	Ry = DirectX::XMMatrixRotationY( angle.y );	// Y軸を基準とした回転行列
	Rz = DirectX::XMMatrixRotationZ( angle.z );	// Z軸を基準とした回転行列
	R = DirectX::XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, 0.0f );
	R = Rz * Ry * Rx;

	//	平行移動
	T = DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );

	//	ワールド変換行列
	world = S * R * T;


	return	world;
}


DirectX::XMMATRIX OBJ3DInstance::GetWorldMatrix()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

	//	初期化
	world = DirectX::XMMatrixIdentity();

	//	拡大・縮小
	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//	回転
	Rx = DirectX::XMMatrixRotationX(angle.x);	// X軸を基準とした回転行列
	Ry = DirectX::XMMatrixRotationY(angle.y);	// Y軸を基準とした回転行列
	Rz = DirectX::XMMatrixRotationZ(angle.z);	// Z軸を基準とした回転行列
	R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	R = Rz * Ry * Rx;

	//	平行移動
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	//	ワールド変換行列
	world = S * R * T;


	return	world;
}