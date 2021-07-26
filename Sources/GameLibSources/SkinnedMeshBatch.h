#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <shlwapi.h>
#include <string>

#undef max
#undef min

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

#include "misc.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "fbxsdk.h"
#include "Shader.h"
#include "OBJ3D.h"


#define USE_CEREAL


class SkinnedMeshBatch : public Mesh
{
public:
	static const int MAX_FRAME = 32;
	static const int MAX_BONE_INFLUENCES = 4;
	static const int MAX_BONES = 32;
	static const int MAX_BUFFER_SIZE = 3;

private:
	struct CBuffer
	{
		DirectX::XMFLOAT4	materialColor;
		DirectX::XMFLOAT4	lightDirection;
	};

	struct BoneTransformBuffer
	{
		DirectX::XMFLOAT4X4 transform[MAX_FRAME][MAX_BONES];
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> boneTransformBuffer[MAX_BUFFER_SIZE];

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
		float boneWeights[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
		int   boneIndices[MAX_BONE_INFLUENCES] = {};

		Vertex() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				CEREAL_NVP( pos.x ),	CEREAL_NVP( pos.y ),	CEREAL_NVP( pos.z ),
				CEREAL_NVP( normal.x ), CEREAL_NVP( normal.y ), CEREAL_NVP( normal.z ),
				CEREAL_NVP( tex.x ),	CEREAL_NVP( tex.y ),
				CEREAL_NVP( boneWeights ),
				CEREAL_NVP( boneIndices )
			);
		}
	};

	struct Material
	{
		DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		std::string texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

		Material() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				CEREAL_NVP( color.x ), CEREAL_NVP( color.y ), CEREAL_NVP( color.z ), CEREAL_NVP( color.w ),
				CEREAL_NVP( texture )
			);
		}
	};

	struct Subset
	{
		unsigned int indexStart = 0; // start number of index buffer
		unsigned int indexCount = 0; // number of vertices (indices)

		Material ambient;
		Material diffuse;
		Material specular;
		Material bump;
		Material normalMap;


		Subset() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				CEREAL_NVP( indexStart ),
				CEREAL_NVP( indexCount ),
				CEREAL_NVP( ambient ),
				CEREAL_NVP( diffuse ),
				CEREAL_NVP( specular ),
				CEREAL_NVP( bump ),
				CEREAL_NVP( normalMap )
			);
		}
	};

	struct Bone
	{
		std::string name;
		DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		DirectX::XMFLOAT4X4 transformToParent = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		DirectX::XMFLOAT4X4 globalTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

		Bone() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				CEREAL_NVP( name ),
				CEREAL_NVP( transform._11 ), CEREAL_NVP( transform._12 ), CEREAL_NVP( transform._13 ), CEREAL_NVP( transform._14 ),
				CEREAL_NVP( transform._21 ), CEREAL_NVP( transform._22 ), CEREAL_NVP( transform._23 ), CEREAL_NVP( transform._24 ),
				CEREAL_NVP( transform._31 ), CEREAL_NVP( transform._32 ), CEREAL_NVP( transform._33 ), CEREAL_NVP( transform._34 ),
				CEREAL_NVP( transform._41 ), CEREAL_NVP( transform._42 ), CEREAL_NVP( transform._43 ), CEREAL_NVP( transform._44 ),
				CEREAL_NVP( transformToParent._11 ), CEREAL_NVP( transformToParent._12 ), CEREAL_NVP( transformToParent._13 ), CEREAL_NVP( transformToParent._14 ),
				CEREAL_NVP( transformToParent._21 ), CEREAL_NVP( transformToParent._22 ), CEREAL_NVP( transformToParent._23 ), CEREAL_NVP( transformToParent._24 ),
				CEREAL_NVP( transformToParent._31 ), CEREAL_NVP( transformToParent._32 ), CEREAL_NVP( transformToParent._33 ), CEREAL_NVP( transformToParent._34 ),
				CEREAL_NVP( transformToParent._41 ), CEREAL_NVP( transformToParent._42 ), CEREAL_NVP( transformToParent._43 ), CEREAL_NVP( transformToParent._44 ),
				CEREAL_NVP( globalTransform._11 ), CEREAL_NVP( globalTransform._12 ), CEREAL_NVP( globalTransform._13 ), CEREAL_NVP( globalTransform._14 ),
				CEREAL_NVP( globalTransform._21 ), CEREAL_NVP( globalTransform._22 ), CEREAL_NVP( globalTransform._23 ), CEREAL_NVP( globalTransform._24 ),
				CEREAL_NVP( globalTransform._31 ), CEREAL_NVP( globalTransform._32 ), CEREAL_NVP( globalTransform._33 ), CEREAL_NVP( globalTransform._34 ),
				CEREAL_NVP( globalTransform._41 ), CEREAL_NVP( globalTransform._42 ), CEREAL_NVP( globalTransform._43 ), CEREAL_NVP( globalTransform._44 )
			);
		}
	};

	struct Skeletal
	{
		std::vector<Bone> bone;

		Skeletal() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				CEREAL_NVP( bone )
			);
		}
	};

	struct SkeletalAnimation
	{
		std::vector<Skeletal> skeletel;
		float samplingTime = 1 / 24.0f;
		float animationTick = 0.0f;

		SkeletalAnimation() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				CEREAL_NVP( skeletel ),
				CEREAL_NVP( samplingTime ),
				CEREAL_NVP( animationTick )
			);
		}
	};

	struct MeshData
	{
		std::string name;
		DirectX::XMFLOAT4 pos;
		float boneWeights[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
		int boneIndeces[MAX_BONE_INFLUENCES] = {};
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		std::vector<Subset> subsets;
		DirectX::XMFLOAT4X4 globalTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		std::vector<SkeletalAnimation> skeletalAnimations;

		MeshData() = default;

		template <class T>
		void serialize(T &archive)
		{
			archive
			(
				CEREAL_NVP( name ),
				CEREAL_NVP( pos.x ), CEREAL_NVP( pos.y ), CEREAL_NVP( pos.z ), CEREAL_NVP( pos.w ),
				CEREAL_NVP( boneWeights ),
				CEREAL_NVP( boneIndeces ),
				CEREAL_NVP( subsets ),
				CEREAL_NVP( globalTransform._11 ), CEREAL_NVP( globalTransform._12 ), CEREAL_NVP( globalTransform._13 ), CEREAL_NVP( globalTransform._14 ),
				CEREAL_NVP( globalTransform._21 ), CEREAL_NVP( globalTransform._22 ), CEREAL_NVP( globalTransform._23 ), CEREAL_NVP( globalTransform._24 ),
				CEREAL_NVP( globalTransform._31 ), CEREAL_NVP( globalTransform._32 ), CEREAL_NVP( globalTransform._33 ), CEREAL_NVP( globalTransform._34 ),
				CEREAL_NVP( globalTransform._41 ), CEREAL_NVP( globalTransform._42 ), CEREAL_NVP( globalTransform._43 ), CEREAL_NVP( globalTransform._44 ),
				CEREAL_NVP( skeletalAnimations )
			);
		}
	};

	struct BoneInfluence
	{
		int index; // index of bone
		float weight; // weight of bone
	};
	typedef std::vector<BoneInfluence> BoneInfluencesPerControlPoint;

public:

	std::vector<std::vector<Vertex>> integratedVertex;
	std::vector<std::vector<unsigned int>>  integratedIndex;
	std::vector<MeshData> meshes;
	int numIndex;

private:
	DirectX::XMFLOAT4X4 originUnitMatrix =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	DirectX::XMFLOAT4X4 unitMatrix[MAX_BONES];

	DirectX::XMFLOAT4X4 coordinateConversion = {
		-1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	};

	u_int animationNumber = 0;
	int animationFrame = 0;
	bool isAnimation = false;
	bool isLoopAnimation = false;

public:
	SkinnedMeshBatch() = default;
	~SkinnedMeshBatch() {}

	SkinnedMeshBatch( SkinnedMeshBatch & ) = delete;
	SkinnedMeshBatch&operator=( SkinnedMeshBatch & ) = delete;

	SkinnedMeshBatch( ID3D11Device *device, const char* fileName, bool leftHandedCoordinate )/* : Mesh()*/
	{
		if ( leftHandedCoordinate ) handedCoordinateSystem = FALSE;
		else handedCoordinateSystem = TRUE;

		for (size_t i = 0; i < MAX_BONES; i++)
		{
			DirectX::XMStoreFloat4x4( &unitMatrix[i], DirectX::XMLoadFloat4x4( &originUnitMatrix ) );
		}

	/*
	// Create Vertex Shader and Input Layout *****************************************

		D3D11_INPUT_ELEMENT_DESC ieDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES"   , 0, DXGI_FORMAT_R32G32B32A32_UINT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		ResourceManager::CreateVertexShaderAndInputLayout( device, "Data/Shader/SkinnedMeshVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), ieDesc, ARRAYSIZE( ieDesc ) );

	// *******************************************************************************

	// Create Pixel Shader ***********************************************************

		ResourceManager::CreatePixelShader( device, "Data/Shader/SkinnedMeshPS.cso", pixelShader.GetAddressOf() );

	// *******************************************************************************
	*/

		Init( device, handedCoordinateSystem );
		CreateConstantBuffer( device );


		char fbxFileName[] = "Data/Assets/Model/Serialized/";
		char fbxName[64] = { 0 };
		_splitpath_s( fileName, NULL, NULL, NULL, NULL, fbxName, sizeof( fbxName ), NULL, NULL );


#ifdef USE_CEREAL
		if ( PathFileExistsA( ( std::string( fbxFileName ) + std::string( fbxName ) + ".bin" ).c_str() ) )
		{
			std::ifstream ifs;
			ifs.open( ( std::string( fbxFileName ) + std::string( fbxName ) + ".bin" ).c_str(), std::ios::binary );
			cereal::BinaryInputArchive inputArchive( ifs );
			inputArchive( *this );

			for ( size_t i = 0; i < meshes.size(); i++ )
			{
				CreateBuffer( device, meshes.at( i ), integratedVertex.at( i ).data(), integratedIndex.at( i ).data(), static_cast<int>(integratedVertex.at( i ).size()), static_cast<int>(integratedIndex.at( i ).size()) );
			}
		}
		else
		{
			LoadFBX( device, fileName );

			std::ofstream ofsBin;
			ofsBin.open( ( std::string( fbxFileName ) + std::string( fbxName ) + ".bin" ).c_str(), std::ios::binary );
			cereal::BinaryOutputArchive outputBinArchive( ofsBin );
			outputBinArchive( *this );

			/*std::ofstream ofsJson;
			ofsJson.open( ( std::string( fbxFileName ) + std::string( fbxName ) + ".json" ).c_str(), std::ios::out );
			cereal::JSONOutputArchive outputJsonArchive( ofsJson );
			outputJsonArchive( *this );*/
		}
#else
		LoadFBX(device, fileName);
#endif
		CreateShaderResourceView( device );

		HRESULT hr = S_OK;
		Instance* _instances = new Instance[MAX_INSTANCE];

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(Instance) * MAX_INSTANCE;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = _instances;

		hr = device->CreateBuffer(&bufferDesc, &subresourceData, &instanceBuffer);
		assert(!hr && "CreateBuffer		Error");

		delete[] _instances;
	}

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> instanceBuffer;
	static const UINT MAX_INSTANCE = 150;
	UINT instance;
	struct Instance
	{
		DirectX::XMFLOAT4X4 wvp;
		DirectX::XMFLOAT4X4 world;
		UINT frame;
	};
	std::vector<Instance> instatnceData;
	struct AnimationData
	{
		float animationTick;
		bool isAnimation;
		bool isLoopAnimation;
	};
	std::vector<AnimationData> animationData;
	void Begin(ID3D11DeviceContext* immediateContext, Shader shader, bool wireframe = false);
	void Render
	(
		ID3D11DeviceContext *immediateContext,
		OBJ3DInstance& obj,
		const DirectX::XMFLOAT4X4 &wvp,
		const DirectX::XMFLOAT4X4 &world,
		float elapsedTime
	);
	void End
	(
		ID3D11DeviceContext* immediateContext,
		const DirectX::XMFLOAT4& lightDirection,
		const DirectX::XMFLOAT4& materialColor
	);


	void StartAnimation( u_int _animationNumber, bool _isloopAnimation )
	{
		animationNumber = _animationNumber;
		animationFrame = 0;
		isAnimation = true;
		isLoopAnimation = _isloopAnimation;

		for ( auto& mash : meshes )
		{
			for ( auto& skeletalAnimation : mash.skeletalAnimations )
			{
				skeletalAnimation.animationTick = 0.0f;
			}
		}
	}
	// void StopAnimation()
	// {

	// }
	void ReStartAnimation()
	{
		isAnimation = true;
	}
	void PauseAnimation()
	{
		isAnimation = false;
	}
	int GetAnimationFrame()
	{
		return animationFrame;
	}

private:
	void LoadFBX( ID3D11Device *device, const char* fileName );
	void FbxAMatrixToXMFLOAT4X4( const FbxAMatrix& fbxamatrix, DirectX::XMFLOAT4X4& xmfloat4x4 );
	void FetchAnimations( ID3D11Device* device, FbxMesh* fbxMesh, MeshData& mesh );
	void FetchMaterials( ID3D11Device* device, const char* fileName, FbxMesh* fbxMesh, MeshData& md );
	void FetchVertecesAndIndeces( ID3D11Device* device, FbxMesh* fbxMesh, MeshData& mesh );
	void FetchBoneInfluences( const FbxMesh *fbxMesh, std::vector<BoneInfluencesPerControlPoint> &influences );
	void FetchBoneMatrices( FbxMesh *fbxMesh, std::vector<Bone> &skeletal, FbxTime time );
	void FetchAnimations( FbxMesh *fbxMesh, SkeletalAnimation &skeletalAnimation, u_int numOfAnimation, u_int samplingRate = 0 );
	void CreateBuffer( ID3D11Device *device, MeshData &m, Vertex* v, unsigned int* i, int numV, int numI );
	void CreateShaderResourceView( ID3D11Device *device );
	void CreateConstantBuffer( ID3D11Device *device )
	{

		HRESULT hr = S_OK;

		// Create Constant Buffer ********************************************************

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof( CBuffer );
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		hr = device->CreateBuffer( &bufferDesc, nullptr, constantBuffer.GetAddressOf() );
		assert( !hr && "CreateBuffer	Error" );

		// *******************************************************************************

		for ( int i = 0; i < MAX_BUFFER_SIZE; i++ )
		{
			ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
			bufferDesc.ByteWidth = sizeof( BoneTransformBuffer );
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			hr = device->CreateBuffer( &bufferDesc, nullptr, boneTransformBuffer[i].GetAddressOf() );
			assert( !hr && "BoneTransformBuffer	Error" );
		}

	}
	bool GetBoneTransformIndex( std::string name, int& meshIndex, int& boneIndex )
	{
		for ( size_t i = 0; i < meshes.size(); i++ )
		{
			std::vector<Bone>& skeletal = meshes[i].skeletalAnimations.at( animationNumber ).skeletel.at( animationFrame ).bone;
			size_t number_of_bones = skeletal.size();
			_ASSERT_EXPR( number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES." );

			for ( size_t j = 0; j < skeletal.size(); j++ )
			{
				if ( skeletal[j].name != name ) continue;

				meshIndex = static_cast<int>(i);
				boneIndex = static_cast<int>(j);

				return true;
			}
		}

		return false;
	}
	DirectX::XMFLOAT4X4 GetBoneTransform( int meshIndex, int boneIndex, OBJ3DInstance& obj )
	{
		int frame = static_cast<int>( obj.GetAnimationTick() / meshes[meshIndex].skeletalAnimations.at( animationNumber ).samplingTime );

		std::vector<Bone>& skeletal = meshes[meshIndex].skeletalAnimations.at( animationNumber ).skeletel.at( frame ).bone;
		size_t number_of_bones = skeletal.size();
		_ASSERT_EXPR( number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES." );

		DirectX::XMFLOAT4X4 boneTransform;
		if ( !handedCoordinateSystem )
		{
			DirectX::XMStoreFloat4x4( &boneTransform, DirectX::XMLoadFloat4x4( &skeletal[boneIndex].globalTransform ) );
		}
		else
		{
			DirectX::XMStoreFloat4x4( &boneTransform, DirectX::XMLoadFloat4x4( &skeletal[boneIndex].globalTransform ) * DirectX::XMLoadFloat4x4( &coordinateConversion ) );
		}

		return boneTransform;
	}
	DirectX::XMFLOAT4X4 GetBoneTransform( std::string name, OBJ3DInstance& obj )
	{
		for ( auto& mesh : meshes )
		{
			int frame = static_cast<int>( obj.GetAnimationTick() / mesh.skeletalAnimations.at( animationNumber ).samplingTime );

			std::vector<Bone>& skeletal = mesh.skeletalAnimations.at( animationNumber ).skeletel.at( frame ).bone;
			size_t number_of_bones = skeletal.size();
			_ASSERT_EXPR( number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES." );

			for ( auto& bone : skeletal )
			{
				if ( bone.name != name ) continue;

				DirectX::XMFLOAT4X4 boneTransform;
				if ( !handedCoordinateSystem )
				{
					DirectX::XMStoreFloat4x4( &boneTransform, DirectX::XMLoadFloat4x4( &bone.globalTransform ) );
				}
				else
				{
					DirectX::XMStoreFloat4x4( &boneTransform, DirectX::XMLoadFloat4x4( &bone.globalTransform ) * DirectX::XMLoadFloat4x4( &coordinateConversion ) );
				}

				return boneTransform;
			}
		}

		return DirectX::XMFLOAT4X4();
	}

public:
	template <class T>
	void serialize(T &archive)
	{
		archive
		(
			CEREAL_NVP( integratedVertex ),
			CEREAL_NVP( integratedIndex ),
			CEREAL_NVP( meshes ),
			CEREAL_NVP( numIndex )
		);
	}
};

CEREAL_REGISTER_TYPE( SkinnedMeshBatch )