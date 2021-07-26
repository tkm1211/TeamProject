#include "Model.h"
#include "FrameWork.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "SkinnedMeshBatch.h"
#include "CameraSystem.h"


Model::Model
 (  const char* fileName,
	bool leftHandedCoordinate,
	bool instancing )
{

	Microsoft::WRL::ComPtr<ID3D11Device> device = FrameWork::GetInstance().GetDevice();

	char ext[32];
	_splitpath_s( fileName, NULL, NULL, NULL, NULL, NULL, NULL, ext, sizeof( ext ) );

	if ( strcmp( ext, ".obj" ) == 0 || strcmp( ext, ".OBJ" ) == 0 )
	{
		pMesh = std::make_unique<StaticMesh>( device.Get(), fileName, leftHandedCoordinate );
	}
	else if ( strcmp( ext, ".fbx" ) == 0 || strcmp( ext, ".FBX" ) == 0 )
	{
		if ( !instancing ) pMesh = std::make_unique<SkinnedMesh>( device.Get(), fileName, leftHandedCoordinate );
		else pMesh = std::make_unique<SkinnedMeshBatch>( device.Get(), fileName, leftHandedCoordinate );
	}

}

void Model::Preparation( Shader shader, bool wireframe )
{

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

	if ( pMesh )
	{
		pMesh->Preparation( immediateContext.Get(), shader, wireframe );
	}

}

void Model::Render
 (  const DirectX::XMMATRIX& world,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& lightDirection,
	const DirectX::XMFLOAT4& color,
	float elapsedTime,
	bool solid,
	float radius )
{

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

	// カメラの中に入っているか判定
	bool inCamera = true;
	if (0.0f < radius)
	{
		DirectX::XMFLOAT4X4 _world;
		DirectX::XMStoreFloat4x4(&_world, world);
		inCamera = JudgeInCamera( {_world._41, _world._42, _world._43} );
	}

	//	ワールド変換行列取得
	DirectX::XMMATRIX worldM = world;

	//	Matrix -> Float4x4 変換
	DirectX::XMFLOAT4X4 wvp;
	DirectX::XMFLOAT4X4 _world;

	DirectX::XMStoreFloat4x4( &wvp, DirectX::XMLoadFloat4x4( &addGlobalTransform ) * worldM * view * projection );
	DirectX::XMStoreFloat4x4( &_world, worldM );

	//	描画
	if ( pMesh )
	{
		pMesh->Render( immediateContext.Get(), wvp, _world, lightDirection, color, elapsedTime, inCamera, solid );
	}

}

void Model::Begin( Shader shader, bool wireframe )
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

	if ( pMesh )
	{
		pMesh->Begin( immediateContext.Get(), shader, wireframe );
	}
}

void Model::Render
 (  OBJ3DInstance& obj,
	//const DirectX::XMMATRIX& world,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	float elapsedTime )
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

	//	ワールド変換行列取得
	DirectX::XMMATRIX worldM = obj.GetWorldMatrix();

	//	Matrix -> Float4x4 変換
	DirectX::XMFLOAT4X4 wvp;
	DirectX::XMFLOAT4X4 _world;

	DirectX::XMStoreFloat4x4( &wvp, worldM * view * projection );
	DirectX::XMStoreFloat4x4( &_world, worldM );

	//	描画
	if ( pMesh )
	{
		pMesh->Render( immediateContext.Get(), obj, wvp, _world, elapsedTime );
	}
}

void Model::End
 (  const DirectX::XMFLOAT4& lightDirection,
	const DirectX::XMFLOAT4& materialColor )
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = FrameWork::GetInstance().GetContext();

	if ( pMesh )
	{
		pMesh->End( immediateContext.Get(), lightDirection, materialColor );
	}
}

bool Model::JudgeInCamera( DirectX::XMFLOAT3 pos )
{
	DirectX::XMFLOAT3 _cameraFoward = CameraSystem::GetInstance()->mainView.GetFoward();
	DirectX::XMVECTOR cameraFowardVec = DirectX::XMLoadFloat2( &DirectX::XMFLOAT2( _cameraFoward.x, _cameraFoward.z ) );

	DirectX::XMFLOAT3 _cameraPos = CameraSystem::GetInstance()->mainView.GetPos();
	DirectX::XMVECTOR modelToCamaraVec = DirectX::XMLoadFloat2( &DirectX::XMFLOAT2( pos.x - _cameraPos.x, pos.z - _cameraPos.z ) );

	// ベクトルの長さ
	float cameraFowardLength = 0.0f;
	float modelToCamaraLength = 0.0f;
	DirectX::XMVECTOR cameraFowardVecN = DirectX::XMVector2Normalize( cameraFowardVec );
	DirectX::XMVECTOR modelToCamaraVecN = DirectX::XMVector2Normalize( modelToCamaraVec );
	DirectX::XMStoreFloat( &cameraFowardLength,  DirectX::XMVector2Length( cameraFowardVecN  ) );
	DirectX::XMStoreFloat( &modelToCamaraLength, DirectX::XMVector2Length( modelToCamaraVecN ) );

	// 内積
	float dot = 0.0f;
	DirectX::XMStoreFloat( &dot, DirectX::XMVector2Dot( cameraFowardVecN, modelToCamaraVecN ) );

	// cosΘ
	float cosSita = dot / ( cameraFowardLength * modelToCamaraLength );

	// cosΘからΘ
	if ( 1.0f < cosSita )
	{
		cosSita = 1.0f;
	}
	float sita = acosf( cosSita );

	//角度(degree)
	float degreeSita = sita * 180.0f / PI;

	// カメラに映っているか判定
	float degreeFov = ( CameraSystem::GetInstance()->mainView.GetFov() * 180.0f / PI );
	if ( -degreeFov <= degreeSita && degreeSita <= degreeFov )
	{
		return true;
	}

	return false;
}