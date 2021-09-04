#include"Player.h"
#include <GameLibSources/InputDevice.h>
#include <GameLibSources/FrameWork.h>
#include <GameSources/CameraSystem.h>
#include"Camera.h"



Player::Player() : OBJ3D()
{

	// ���f���̏�����
	testModel = std::make_unique<Model>("Data/Assets/Model/Player/EX/test500 kai.fbx", false);
	{
		testModel->StartAnimation(0, true); // ���� : FBX���̃A�j���[�V�����ԍ�, ���[�v�Đ����邩�H
		Init();
	}

	//testModel = std::make_unique<Model>("Data/Assets/Model/player_anime.fbx", false);
	//{
	//	testModel->StartAnimation(2, true);
	//	//testModelData.Init();
	//	//testModelData.SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
	//}




 }


Player::~Player() 
{
	UnInit();
}


void Player::Update(float elapsedTime)
{
	InputMove(elapsedTime);

	InputJump(elapsedTime);
}


void Player::Render()
{
	// ���f���̕`��
	{
		testModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), true);
		testModel->Render(GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), GetColor(), FrameWork::GetInstance().GetElapsedTime());
	}

}


void Player::InputProjectile()
{





}

//�ړ�����
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//���͏����擾
	//GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = xInput[0].sLX;
	float ay = xInput[0].sLY;

	//�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
	Camera::GetInstance()->GetRight();

	//Camera& camera = Camera::Instance();

	const DirectX::XMFLOAT3& cameraRight = Camera::GetInstance()->GetRight();
	const DirectX::XMFLOAT3& cameraFront = Camera::GetInstance()->GetFront();


	//�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���Ɂu�Ȃ�悤�ɂ���

	//�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf((cameraRightX * cameraRightX) + (cameraRightZ * cameraRightZ));//�����̌v�Z

	if (cameraRightLength > 0.0f)
	{
		//�P�ʃx�N�g����
	 //   float mag = cameraRightLength / cameraRightLength;//1
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}
	//�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLenght = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLenght > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraFrontX /= cameraFrontLenght;
		cameraFrontZ /= cameraFrontLenght;
	}
	//�X�e�B�b�N�̐��������l�ɃJ�����E�����ɔ��f��
	//�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f��
	//�i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	//�O�L�[��cameraFrontX =���x�N�g���̋������L�[��cameraRightX =���x�N�g���̋���
	vec.x = ay * cameraFrontX + ax * cameraRightX;//
	//�O�L�[��cameraFrontZ =�O�x�N�g���̋��� ���L�[��cameraRightZ =�O�x�N�g���̋���
	vec.z = ay * cameraFrontZ + ax * cameraRightZ;//
	//Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;

}

void Player::InputMove(float elapsedTime)
{
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//�ړ�����
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
	//��]����
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	//if (xInput[0].bRBs) {
	//	dash = 3;
	//}
	//else {
	//	dash = 1;
	//}
	//���X�e�b�N���͏���
	//SetPos(
	//	{ PMDx += (xInput[0].sLX / 1000.0f) * 5.0f * dash,
	//		PMDy ,
	//		PMDz -= (xInput[0].sLY / 1000.0f) * 5.0f * dash }
	//);
}

//�ړ�����
void Player::Move(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	position.x += vx * speed;
	position.z += vz * speed;
}

//��]����
void Player::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	//float TurnSpeed = 2.0f;
	//�i�s�x�N�g����0�x�N�g���̏ꍇ��������K�v����
   //  if (vz == 0.0f && vx == 0.0f)return;//���Ɠ����Ӗ�
	float lengse = sqrtf(vx * vx + vz * vz);//�������v�Z
	if (lengse == 0.0f)return;

	//�M�x�N�g����P�ʃx�N�g����
	vz /= lengse;
	vx /= lengse;

	//���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);//���������̉E��錾
	float frontZ = cosf(angle.y);//���������̑O��錾

	//��]�l�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (vx * frontX) + (vz * frontZ);//����

	//���σ`��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
	//�������ق�1.0�ɋ߂Â������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;//�␳�l
	if (speed > rot)speed = rot;//

	//���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	//�O�ς̌v�Z��:float  cross = (vec1.x * vec2.z) - (vec1.z * vec2.x);
	float cross = (vx * frontZ) - (vz * frontX);

	//�QD�̊O�ϒl�����̏ꍇ||���̏ꍇ�ɂ���č��E���肪�s����
	//���E������s�����Ƃō��E��]��I������
	if (cross < 0.0f) {
		//����]
		angle.y -= speed;
		SetAngleY(angle.y);
	}
	else if (cross > 0.0f)//��̕��Ői�s�����x�N�g�����O�̎�rerun���Ă���̂Ŗ{���� else if�͂���Ȃ�
	{
		//�E��]
		angle.y += speed;
		SetAngleY(angle.y);
	}

}

void Player::InputJump(float elapsedTime)
{
	float ElapsedTime = elapsedTime;

	//�EB�{�^�����͏���
	if (xInput[0].bAt)
	{
		//���g��speed��ݒ�
		SetSpeed({ 0,15,0 });
	}

	Jgravity = GetSpeed().y;
	PMDy += Jgravity;
	SetPos({ PMDx ,PMDy ,PMDz });
	//���C���n�ʔ���
	if (PMDy <= 0) {
		PMDy = 0;
		ElapsedTime = 0;
		gravity = -1;
	}
	if (PMDy > 0)
	{
		//�󒆂ɂ���Ƃ��ɏ��X�ɋ����Ȃ�
		Jgravity += gravity * Ti;
		SetSpeed({ 0,Jgravity  ,0 });
	}
}





