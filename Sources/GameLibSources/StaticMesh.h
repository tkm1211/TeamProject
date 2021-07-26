#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <shlwapi.h>

#undef max
#undef min

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

#include "Mesh.h"
#include "ResourceManager.h"
#include "WICTextureLoader.h"

#pragma comment(lib, "shlwapi.lib")


class StaticMesh : public Mesh
{
private:
	struct CBuffer
	{
		DirectX::XMFLOAT4X4	wvp;
		DirectX::XMFLOAT4X4	world;
		DirectX::XMFLOAT4	materialColor;
		DirectX::XMFLOAT4	lightDirection;
	};
	struct Vertex
	{
		DirectX::XMFLOAT3 pos = {};
		DirectX::XMFLOAT3 normal = {};
		DirectX::XMFLOAT2 tex = {};

		Vertex() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				pos.x, pos.y, pos.z,
				normal.x, normal.y, normal.z,
				tex.x, tex.y
			);
		}
	};
	struct Subset
	{
		std::string usemtl;
		unsigned int indexStart = 0;
		unsigned int indexCount = 0;

		Subset() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				usemtl,
				indexStart,
				indexCount
			);
		}
	};
	struct Material
	{
		std::string newmtl;
		DirectX::XMFLOAT3 Ka = { 0.2f, 0.2f, 0.2f };
		DirectX::XMFLOAT3 Kd = { 0.8f, 0.8f, 0.8f };
		DirectX::XMFLOAT3 Ks = { 1.0f, 1.0f, 1.0f };
		unsigned int illum = 1;
		std::string map_Kd;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

		Material() = default;

		template <class T>
		void serialize( T &archive )
		{
			archive
			(
				newmtl, 
				Ka.x, Ka.y, Ka.z,
				Kd.x, Kd.y, Kd.z,
				Ks.x, Ks.y, Ks.z,
				illum,
				map_Kd
			);
		}
	};

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Subset> subsets;
	std::vector<Material> materials;

public:
	StaticMesh() = default;
	~StaticMesh() = default;

	StaticMesh( StaticMesh & ) = delete;
	StaticMesh &operator=( StaticMesh & ) = delete;

	StaticMesh( ID3D11Device *device, const char* fileName, bool leftHandedCoordinate )/* : Mesh()*/
	{
		if ( leftHandedCoordinate ) handedCoordinateSystem = FALSE;
		else handedCoordinateSystem = TRUE;

	// Create Vertex Shader and Input Layout *****************************************

		D3D11_INPUT_ELEMENT_DESC ieDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		ResourceManager::CreateVertexShaderAndInputLayout( device, "Data/Shader/StaticMeshVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), ieDesc, ARRAYSIZE( ieDesc ) );

	// *******************************************************************************

	// Create Pixel Shader ***********************************************************

		ResourceManager::CreatePixelShader( device, "Data/Shader/StaticMeshPS.cso", pixelShader.GetAddressOf() );

	// *******************************************************************************

		Init( device, handedCoordinateSystem );
		CreateConstantBuffer( device );

		if (PathFileExistsA( ( std::string( fileName ) + ".bin" ).c_str() ) )
		{
			std::ifstream ifs;
			ifs.open( ( std::string( fileName ) + ".bin" ).c_str(), std::ios::binary );
			cereal::BinaryInputArchive inputArchive( ifs );
			inputArchive( *this );

			CreateShaderResourceView( device );
		}
		else
		{
			LoadOBJ( device, fileName );

			std::ofstream ofs;
			ofs.open( ( std::string( fileName ) + ".bin" ).c_str(), std::ios::binary );
			cereal::BinaryOutputArchive outputArchive( ofs );
			outputArchive( *this );
		}

		CreateBuffers( device, vertices.data(), static_cast<int>(vertices.size()), indices.data(), static_cast<int>(indices.size()) );
	}

public:
	void Render
	(
		ID3D11DeviceContext *immediateContext,
		const DirectX::XMFLOAT4X4 &wvp,
		const DirectX::XMFLOAT4X4 &world,
		const DirectX::XMFLOAT4 &lightDirection,
		const DirectX::XMFLOAT4 &materialColor,
		float elapsedTime,
		bool wireframe = false
	);

private:
	void LoadOBJ( ID3D11Device *device, const char* objFileName );
	void LoadMTL( ID3D11Device *device, const char* mtlFileName );
	void CreateBuffers( ID3D11Device *device, Vertex *vertices, int numVertices, u_int *indices, int numIndices );
	void CreateShaderResourceView( ID3D11Device *device )
	{

		HRESULT hr = S_OK;

		for ( auto &m : materials )
		{
			wchar_t fName[256];
			size_t length;
			mbstowcs_s( &length, fName, 256, m.map_Kd.c_str(), _TRUNCATE );

			hr = DirectX::CreateWICTextureFromFile( device, fName, NULL, m.shaderResourceView.GetAddressOf() );
			assert( !hr && "DirectX::CreateWICTextureFromFile	Error" );
		}

	}
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

	}

public:
	template <class T>
	void serialize( T &archive )
	{
		archive
		(
			vertices,
			indices,
			subsets,
			materials
		);
	}
};

CEREAL_REGISTER_TYPE( StaticMesh )