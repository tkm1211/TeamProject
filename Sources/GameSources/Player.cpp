#include"Player.h"
#include <GameLibSources/InputDevice.h>
#include <GameLibSources/FrameWork.h>
#include <GameSources/CameraSystem.h>




Player::Player() : OBJ3D()
{

	// モデルの初期化
	testModel = std::make_unique<Model>("Data/Assets/Model/Player/EX/test500 kai.fbx", false);
	{
		testModel->StartAnimation(0, true); // 引数 : FBX内のアニメーション番号, ループ再生するか？
		Init();
	}





 }


Player::~Player() 
{
	UnInit();
}


void Player::Update(float elapsedTime)
{

	//test

	Ti += 1 * elapsedTime;//FrameWork::GetInstance().elapsedTime;

	if (xInput[0].bRBs) {
		dash = 3;
	}
	else {
		dash = 1;
	}

	//左ステック入力処理


		SetPos(
			{ PMDx += (xInput[0].sLX / 1000.0f) * 5.0f * dash,
				PMDy ,
				PMDz -= (xInput[0].sLY / 1000.0f) * 5.0f * dash }
		);
	

	//右Bボタン入力処理
	if (xInput[0].bAt)
	{
		//自身にspeedを設定
		SetSpeed({ 0,15,0 });
	}

	Jgravity = GetSpeed().y;
	PMDy += Jgravity;
	SetPos({ PMDx ,PMDy ,PMDz });
	//レイ無地面判定
	if (PMDy <= 0) {
		PMDy = 0;
		Ti = 0;
		gravity = -1;
	}
	if (PMDy > 0)
	{
		//空中にいるときに徐々に強くなる
		Jgravity += gravity * Ti;
		SetSpeed({ 0,Jgravity  ,0 });
	}




}


void Player::Render()
{
	// モデルの描画
	{
		testModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), true);
		testModel->Render(GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), GetColor(), FrameWork::GetInstance().GetElapsedTime());
	}






}













