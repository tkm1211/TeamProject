#include "OBJ3D.h"
#include "FrameWork.h"


DirectX::XMMATRIX OBJ3D::GetWorldMatrix()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

	//	������
	world = DirectX::XMMatrixIdentity();

	//	�g��E�k��
	S = DirectX::XMMatrixScaling( scale.x, scale.y, scale.z );

	//	��]
	Rx = DirectX::XMMatrixRotationX( angle.x );	// X������Ƃ�����]�s��
	Ry = DirectX::XMMatrixRotationY( angle.y );	// Y������Ƃ�����]�s��
	Rz = DirectX::XMMatrixRotationZ( angle.z );	// Z������Ƃ�����]�s��
	R = DirectX::XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, 0.0f );
	R = Rz * Ry * Rx;

	//	���s�ړ�
	T = DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );

	//	���[���h�ϊ��s��
	world = S * R * T;


	return	world;
}


DirectX::XMMATRIX OBJ3DInstance::GetWorldMatrix()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

	//	������
	world = DirectX::XMMatrixIdentity();

	//	�g��E�k��
	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//	��]
	Rx = DirectX::XMMatrixRotationX(angle.x);	// X������Ƃ�����]�s��
	Ry = DirectX::XMMatrixRotationY(angle.y);	// Y������Ƃ�����]�s��
	Rz = DirectX::XMMatrixRotationZ(angle.z);	// Z������Ƃ�����]�s��
	R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	R = Rz * Ry * Rx;

	//	���s�ړ�
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	//	���[���h�ϊ��s��
	world = S * R * T;


	return	world;
}