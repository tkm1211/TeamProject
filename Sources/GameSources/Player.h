#pragma once
#include <memory>
#include "OBJ3D.h"
#include "Model.h"



class Player : public OBJ3D
{
public:
	Player();
	~Player();

	void Update(float elapsedTime);

	void Render();

	void InputProjectile();

	void InputMove(float elapsedTime);

	void Move(float elapsedTime, float vx, float vz, float speed);
	//��]����
	void Turn(float elapsedTime, float vx, float vz, float speed);

	void InputJump(float elapsedTime);

private:
	//�ϐ��̐錾
	std::unique_ptr<Model> testModel;//���j�[�N�|�C���^

	//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;

	float  Ti = 0;
	float PMDx = GetPos().x;
	float PMDy = GetPos().y;
	float PMDz = GetPos().z;
	/*static*/ float Jgravity = GetSpeed().y;
	float gravity = -1;
	int dash = 1;
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	DirectX::XMFLOAT3                  position = { 0,0,0 };



};























