#include"Player.h"
#include <GameLibSources/InputDevice.h>
#include <GameLibSources/FrameWork.h>
#include <GameSources/CameraSystem.h>
#include"Camera.h"



Player::Player() : OBJ3D()
{

	// モデルの初期化
	testModel = std::make_unique<Model>("Data/Assets/Model/Player/EX/test500 kai.fbx", false);
	{
		testModel->StartAnimation(0, true); // 引数 : FBX内のアニメーション番号, ループ再生するか？
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
	// モデルの描画
	{
		testModel->Preparation(ShaderSystem::GetInstance()->GetShaderOfSkinnedMesh(ShaderSystem::DEFAULT), true);
		testModel->Render(GetWorldMatrix(), CameraSystem::GetInstance()->mainView.GetViewMatrix(), CameraSystem::GetInstance()->mainView.GetProjectionMatrix(),
			DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f), GetColor(), FrameWork::GetInstance().GetElapsedTime());
	}

}


void Player::InputProjectile()
{





}

//移動処理
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//入力情報を取得
	//GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = xInput[0].sLX;
	float ay = xInput[0].sLY;

	//カメラ方向とスティックの入力値によって進行方向を計算する
	Camera::GetInstance()->GetRight();

	//Camera& camera = Camera::Instance();

	const DirectX::XMFLOAT3& cameraRight = Camera::GetInstance()->GetRight();
	const DirectX::XMFLOAT3& cameraFront = Camera::GetInstance()->GetFront();


	//移動ベクトルはXZ平面に水平なベクトルに「なるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf((cameraRightX * cameraRightX) + (cameraRightZ * cameraRightZ));//距離の計算

	if (cameraRightLength > 0.0f)
	{
		//単位ベクトル化
	 //   float mag = cameraRightLength / cameraRightLength;//1
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}
	//カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLenght = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLenght > 0.0f)
	{
		//単位ベクトル化
		cameraFrontX /= cameraFrontLenght;
		cameraFrontZ /= cameraFrontLenght;
	}
	//スティックの水平方向値にカメラ右方向に反映し
	//スティックの垂直入力値をカメラ前方向に反映し
	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	//前キー時cameraFrontX =横ベクトルの強さ横キー時cameraRightX =横ベクトルの強さ
	vec.x = ay * cameraFrontX + ax * cameraRightX;//
	//前キー時cameraFrontZ =前ベクトルの強さ 横キー時cameraRightZ =前ベクトルの強さ
	vec.z = ay * cameraFrontZ + ax * cameraRightZ;//
	//Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;

}

void Player::InputMove(float elapsedTime)
{
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//移動処理
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
	//回転処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	//if (xInput[0].bRBs) {
	//	dash = 3;
	//}
	//else {
	//	dash = 1;
	//}
	//左ステック入力処理
	//SetPos(
	//	{ PMDx += (xInput[0].sLX / 1000.0f) * 5.0f * dash,
	//		PMDy ,
	//		PMDz -= (xInput[0].sLY / 1000.0f) * 5.0f * dash }
	//);
}

//移動処理
void Player::Move(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	position.x += vx * speed;
	position.z += vz * speed;
}

//回転処理
void Player::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	//float TurnSpeed = 2.0f;
	//進行ベクトルが0ベクトルの場合処理する必要無し
   //  if (vz == 0.0f && vx == 0.0f)return;//↓と同じ意味
	float lengse = sqrtf(vx * vx + vz * vz);//距離を計算
	if (lengse == 0.0f)return;

	//信仰ベクトルを単位ベクトル化
	vz /= lengse;
	vx /= lengse;

	//自身の回転値から前方向を求める
	float frontX = sinf(angle.y);//水平方向の右を宣言
	float frontZ = cosf(angle.y);//水平方向の前を宣言

	//回転値を求めるため、2つの単位ベクトルの内積を計算する
	float dot = (vx * frontX) + (vz * frontZ);//内積

	//内積チは-1.0~1.0で表現されており、2つの単位ベクトルの角度が
	//小さいほど1.0に近づく性質を利用して回転速度を調整する
	float rot = 1.0f - dot;//補正値
	if (speed > rot)speed = rot;//

	//左右判定を行うために２つの単位ベクトルの外積を計算する
	//外積の計算式:float  cross = (vec1.x * vec2.z) - (vec1.z * vec2.x);
	float cross = (vx * frontZ) - (vz * frontX);

	//２Dの外積値が正の場合||負の場合によって左右判定が行える
	//左右判定を行うことで左右回転を選択する
	if (cross < 0.0f) {
		//左回転
		angle.y -= speed;
		SetAngleY(angle.y);
	}
	else if (cross > 0.0f)//上の方で進行方向ベクトルが０の時rerunしているので本当は else ifはいらない
	{
		//右回転
		angle.y += speed;
		SetAngleY(angle.y);
	}

}

void Player::InputJump(float elapsedTime)
{
	float ElapsedTime = elapsedTime;

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
		ElapsedTime = 0;
		gravity = -1;
	}
	if (PMDy > 0)
	{
		//空中にいるときに徐々に強くなる
		Jgravity += gravity * Ti;
		SetSpeed({ 0,Jgravity  ,0 });
	}
}





