#pragma once

#include <memory>
#include <vector>
#include "Mesh.h"


class Model
{
private:
	std::unique_ptr<Mesh> pMesh;
	DirectX::XMFLOAT4X4 addGlobalTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

public: // コンストラクタ・デストラクタ
	Model() {}

	// （ const char* fileName : モデルファイルパス, bool leftHandedCoordinate : true = 左手座標系 false = 右手座標系, bool instancing : true = インスタンス化 false = 通常 ）
	Model
	(
		const char* fileName,
		bool leftHandedCoordinate = true,
		bool instancing = false
	);


	~Model() {}

public:
	/*void Init();
	void Release();*/

	void Preparation( Shader shader, bool wireframe = false );
	void Render
	(
		const DirectX::XMMATRIX& world,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& lightDirection,
		const DirectX::XMFLOAT4& color,
		float elapsedTime,
		bool solid = true,
		float radius = 0.0f
	);
	void Begin(Shader shader, bool wireframe = false);
	void Render
	(
		OBJ3DInstance& obj,
		//const DirectX::XMMATRIX& world,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		float elapsedTime
	);
	void End
	(
		const DirectX::XMFLOAT4& lightDirection,
		const DirectX::XMFLOAT4& materialColor
	);

public: // アニメーション関数
	// 初めからアニメーションを始める関数
	void StartAnimation( u_int _animationNumber, bool _isloopAnimation )
	{
		pMesh->StartAnimation( _animationNumber, _isloopAnimation );
	}

	// PauseAnimation()呼ばれた時のアニメーションフレームからアニメーションを始める関数
	void ReStartAnimation()
	{
		pMesh->ReStartAnimation();
	}

	// アニメーションを一時停止する関数
	void PauseAnimation()
	{
		pMesh->PauseAnimation();
	}

	// ※工事中
	void SetAnimation( bool startAnimation, bool isLoopAnimation, bool forcedExecution )
	{
		if (!startAnimation) return;
		if (startAnimation && !forcedExecution) return;

		pMesh->StartAnimation( 0, isLoopAnimation );
	}

	// 現在アニメーションしているかを取得する関数
	bool GetAnimatingFlg()
	{
		return pMesh->GetAnimatingFlg();
	}

	// 現在のアニメーションフレームを取得する関数
	int GetAnimationFrame()
	{
		return pMesh->GetAnimationFrame();
	}

	// アニメーションが終わった瞬間を取得する関数
	bool GetFinishAnimation()
	{
		return pMesh->GetFinishAnimation();
	}

	DirectX::XMFLOAT4X4 GetBoneTransform( std::string name )
	{
		return pMesh->GetBoneTransform( name );
	}
	DirectX::XMFLOAT4X4 GetBoneTransform( std::string name, OBJ3DInstance& obj )
	{
		return pMesh->GetBoneTransform( name, obj );
	}
	DirectX::XMFLOAT4X4 GetBoneTransform( int meshIndex, int boneIndex )
	{
		return pMesh->GetBoneTransform( meshIndex, boneIndex );
	}
	DirectX::XMFLOAT4X4 GetBoneTransform( int meshIndex, int boneIndex, OBJ3DInstance& obj )
	{
		return pMesh->GetBoneTransform( meshIndex, boneIndex, obj );
	}
	bool GetBoneTransformIndex( std::string name, int& meshIndex, int& boneIndex )
	{
		return pMesh->GetBoneTransformIndex( name, meshIndex, boneIndex );
	}
	DirectX::XMFLOAT3 GetVectexPos( std::string name, const DirectX::XMFLOAT3& pos, const DirectX::XMMATRIX& myWorldTransform, int vectexPosNo = 0 )
	{
		DirectX::XMFLOAT3 _pos = pMesh->GetVectexPos( name, pos, vectexPosNo );

		DirectX::XMMATRIX M = DirectX::XMMatrixTranslation( _pos.x, _pos.y, _pos.z );
		DirectX::XMFLOAT4X4 _M;
		DirectX::XMStoreFloat4x4( &_M, M * myWorldTransform );

		return DirectX::XMFLOAT3( _M._41, _M._42, _M._43 );
	}

	/*std::vector<Mesh::Face> GetFaces()
	{
		return pMesh->GetFaces();
	}*/

	void SetAddGlobalTransform(const DirectX::XMFLOAT4X4& _addGlobalTransform)
	{
		addGlobalTransform = _addGlobalTransform;
	}

private:
	bool JudgeInCamera( DirectX::XMFLOAT3 pos );

public:
	// レイピック関数
	int RayPick
	(
		const DirectX::XMFLOAT3& startPosition,
		const DirectX::XMFLOAT3& endPosition,
		const DirectX::XMMATRIX& modelWorldMatrix,
		DirectX::XMFLOAT3* outPosition,
		DirectX::XMFLOAT3* outNormal,
		float* outLength
	)
	{
#if 1
		// レイをモデル空間に変換
		DirectX::XMVECTOR rayStartVec = DirectX::XMLoadFloat3(&startPosition);
		DirectX::XMVECTOR rayEndVec = DirectX::XMLoadFloat3(&endPosition);
		
		DirectX::XMVECTOR rayStartVecModelSpace;
		DirectX::XMVECTOR rayEndVecModelSpace;
		
		DirectX::XMMATRIX groundMatrixInverse = DirectX::XMMatrixInverse(nullptr, modelWorldMatrix);
		rayStartVecModelSpace = DirectX::XMVector3TransformCoord(rayStartVec, groundMatrixInverse);
		rayEndVecModelSpace = DirectX::XMVector3TransformCoord(rayEndVec, groundMatrixInverse);
		
		DirectX::XMFLOAT3 rayStartFloat3;
		DirectX::XMFLOAT3 rayEndFloat3;
		
		DirectX::XMStoreFloat3(&rayStartFloat3, rayStartVecModelSpace);
		DirectX::XMStoreFloat3(&rayEndFloat3, rayEndVecModelSpace);
		
		
		return pMesh->RayPick(rayStartFloat3, rayEndFloat3, outPosition, outNormal, outLength);
#else
		return pMesh->RayPick(startPosition, endPosition, outPosition, outNormal, outLength);
#endif
	}
};