#include "ProjectileNormal.h"
#include "OBJ3D.h"
//コンストラクタ
ProjectileNormal::ProjectileNormal(ProjectileManager* manager)
    : Projectile(manager)
{
    // モデルの初期化
    ProjectileModel = std::make_unique<Model>("Data/Assets/Model/Player/EX/test500 kai.fbx", false);
    {
        ProjectileModel->StartAnimation(0, true); // 引数 : FBX内のアニメーション番号, ループ再生するか？
        //Init();
        //OBJ3D::Init();
    }

}

//デストラクタ
ProjectileNormal::~ProjectileNormal()
{
    //UnInit();
    //delete ProjectileModel;
}

void ProjectileNormal::Update(float elapsedTime)
{
    //寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        //自分を削除
        Destroy();
    }

    //移動
    {
        float moveSpeed = this->moveSpeed * elapsedTime;
        position.x += direction.x * moveSpeed; //directionは弾の進行方向ベクトル;
        position.y += direction.y * moveSpeed;
        position.z += direction.z * moveSpeed; //進行方向ベクトル.その軸 * speed;
    }
    //旋回
    {
        float turnSpeed = this->turnSpeed * elapsedTime;

        //ターゲットまでのベクトルを算出
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        //ゼロベクトルでないなら回転処理
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        if (lengthSq > 0.00001f)
        {
            //ターゲットまでのベクトルを単位ベクトル化
            Vec = DirectX::XMVector3Normalize(Vec);

            //向いている方向ベクトルを算出
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

            //  Direction = DirectX::XMVector3Normalize(Direction);

              //前方向ベクトルとターゲットまでのベクトルの内積(角度)を算出
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Direction);
            // DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            //2つの単位ベクトルの角度が小さいほど
            //1.0に近づくという性質を利用して回転速度を調整する
            float rot = 1.0 - dot;  //補正値(内積値-1.0～1.0を角度小：0.0～2.0：角度大に補正します)
            //内積値は-1.0～1.0で表現されており、2つの単位ベクトルの角度が
            //小さいほど1.0に近づくという性質を利用して回転速度を調整する
            if (rot > turnSpeed)
            {
                rot = turnSpeed;
            }


            //回転角度があるなら回転処理をする
            if (fabsf(rot) > 0.00001f)
            {
                //回転軸を算出
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Vec, Direction);
                //回転軸と回転量から回転行列を算出
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

                //現在の行列を回転させる
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                //Transform = Rotation * Transform;
                Transform = DirectX::XMMatrixMultiply(Rotation, Transform);

                //回転後の前方向を取り出し、単位ベクトル化する
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);
                DirectX::XMStoreFloat3(&direction, Direction);
            }
        }

    }

    //オブジェクト行列を更新
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







