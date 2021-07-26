#include "StaticMesh.h"


void StaticMesh::LoadOBJ( ID3D11Device *device, const char* objFileName )
{
	
	u_int currentIndex = 0;

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> texcoord;

	Subset _subset;
	u_int indexCnt = 0;

	std::ifstream fin( objFileName );
	_ASSERT_EXPR( fin, L"'OBJ file not fuund." );


	char command[256];

	while ( fin )
	{
		fin >> command;

		if ( 0 == strcmp( command, "mtllib" ) )
		{
			char allName[256];

			char dir[64] = { NULL };
			_splitpath_s( objFileName, NULL, NULL, dir, 64, NULL, NULL, NULL, NULL );

			char name[256] = { NULL };
			fin >> name;

			_makepath_s( allName, 256, NULL, dir, name, NULL );

			LoadMTL( device, allName );

			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "v" ) )
		{
			float x, y, z;
			fin >> x >> y >> z;

			positions.push_back( DirectX::XMFLOAT3( x, y, z ) );

			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "vn" ) )
		{
			FLOAT i, j, k;
			fin >> i >> j >> k;

			normals.push_back( DirectX::XMFLOAT3( i, j, k ) );

			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "vt" ) )
		{
			FLOAT u, v;
			fin >> u >> v;

			if ( v < 0 ) v = -v;
			else v = 1 - v;

			texcoord.push_back( DirectX::XMFLOAT2( u, v ) );

			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "usemtl" ) )
		{
			if ( indexCnt != 0 )
			{
				_subset.indexCount = indexCnt;
				indexCnt = 0;

				subsets.push_back( _subset );
			}

			char name[256];
			fin >> name;

			_subset.usemtl = name;
			_subset.indexStart = currentIndex;

			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "f" ) )
		{
			static u_int index = 0;

			for ( u_int i = 0; i < 3; i++ )
			{
				u_int v, vt, vn;
				fin >> v;

				Vertex vertex;
				vertex.pos = positions[v - 1];

				if ( '/' == fin.peek() )
				{
					fin.ignore();

					if ( '/' != fin.peek() )
					{
						fin >> vt;
						vertex.tex = texcoord[vt - 1];
					}

					if ( '/' == fin.peek() )
					{
						fin.ignore();
						fin >> vn;

						vertex.normal = normals[vn - 1];
					}
				}
				vertices.push_back( vertex );
				indices.push_back( currentIndex++ );
				indexCnt++;
			}

			fin.ignore( 1024, '\n' );
		}
		else
		{
			fin.ignore( 1024, '\n' );
		}
	}

	if ( indexCnt != 0 )
	{
		_subset.indexCount = indexCnt;
		indexCnt = 0;

		subsets.push_back( _subset );
	}

	fin.close();

}


void StaticMesh::LoadMTL( ID3D11Device *device, const char* mtlFileName )
{

	HRESULT hr = S_OK;


	Material _material;

	std::ifstream fin( mtlFileName );
	_ASSERT_EXPR( fin, L"'OBJ file not fuund." );


	char command[256];

	while ( fin )
	{
		fin >> command;

		if ( 0 == strcmp( command, "newmtl" ) )
		{
			char name[256];
			fin >> name;

			_material.newmtl = name;

			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "Ka" ) )
		{
			DirectX::XMFLOAT3 ka;
			fin >> ka.x >> ka.y >> ka.z;

			_material.Ka = ka;
			
			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "Kd" ) )
		{
			DirectX::XMFLOAT3 kd;
			fin >> kd.x >> kd.y >> kd.z;

			_material.Kd = kd;
		
			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "Ks" ) )
		{
			DirectX::XMFLOAT3 ks;
			fin >> ks.x >> ks.y >> ks.z;

			_material.Ks = ks;

			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "illum" ) )
		{
			int i;
			fin >> i;

			_material.illum = i;

			fin.ignore( 1024, '\n' );
		}
		else if ( 0 == strcmp( command, "map_Kd" ) )
		{
			char allName[256] = { NULL };

			
			char dir[64] = { NULL };
			_splitpath_s( mtlFileName, NULL, NULL, dir, 64, NULL, NULL, NULL, NULL );
			
			char name[256] = { NULL };
			fin >> name;
			_makepath_s( allName, _MAX_FNAME, NULL, dir, name, NULL );


			_material.map_Kd = allName;


			wchar_t fName[256];
			size_t length;
			mbstowcs_s( &length, fName, 256, _material.map_Kd.c_str(), _TRUNCATE );

			hr = DirectX::CreateWICTextureFromFile( device, fName, NULL, _material.shaderResourceView.GetAddressOf() );
			assert( !hr && "DirectX::CreateWICTextureFromFile	Error" );


			materials.push_back( _material );

			fin.ignore( 1024, '\n' );
		}
		else
		{
			fin.ignore( 1024, '\n' );
		}
	}

	fin.close();

}


void StaticMesh::CreateBuffers( ID3D11Device *device, Vertex *vertices, int numVertices, u_int *indices, int numIndices )
{

	HRESULT hr;


	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof( Vertex ) * numVertices;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;
	subresourceData.SysMemPitch = 0; //Not use for vertex buffers.mm 
	subresourceData.SysMemSlicePitch = 0; //Not use for vertex buffers.

	hr = device->CreateBuffer(&bufferDesc, &subresourceData, vertexBuffer.ReleaseAndGetAddressOf());
	assert( !hr && "CreateBuffer	Error" );


	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.ByteWidth = sizeof( u_int ) * numIndices;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	ZeroMemory( &subresourceData, sizeof( subresourceData ) );
	subresourceData.pSysMem = indices;
	subresourceData.SysMemPitch = 0; //Not use for index buffers.
	subresourceData.SysMemSlicePitch = 0; //Not use for index buffers.

	hr = device->CreateBuffer( &bufferDesc, &subresourceData, indexBuffer.ReleaseAndGetAddressOf() );
	assert( !hr && "CreateBuffer	Error" );

}


void StaticMesh::Render
  ( ID3D11DeviceContext *immediateContext,
	const DirectX::XMFLOAT4X4 &wvp,
	const DirectX::XMFLOAT4X4 &world,
	const DirectX::XMFLOAT4 &lightDirection,
	const DirectX::XMFLOAT4 &materialColor,
	float elapsedTime,
	bool wireframe )
{

	u_int stride = sizeof( Vertex );
	u_int offset = 0;


	immediateContext->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddressOf(), &stride, &offset );
	immediateContext->IASetIndexBuffer( indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0 );

	immediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	immediateContext->IASetInputLayout( inputLayout.Get() );

	immediateContext->VSSetShader( vertexShader.Get(), nullptr, 0 );
	immediateContext->PSSetShader( pixelShader.Get(), nullptr, 0 );

	immediateContext->OMSetDepthStencilState( depthStencilState.Get(), 1 );

	if ( wireframe )
	{
		immediateContext->RSSetState( wireframeRasterizerState.Get() );
	}
	else
	{
		immediateContext->RSSetState( solidRasterizerState.Get() );
	}


	for ( auto &material : materials )
	{
		CBuffer data;
		data.wvp = wvp;
		data.world = world;
		data.lightDirection = lightDirection;
		data.materialColor.x = materialColor.x * material.Kd.x;
		data.materialColor.y = materialColor.y * material.Kd.y;
		data.materialColor.z = materialColor.z * material.Kd.z;
		data.materialColor.w = materialColor.w;

		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);

		immediateContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

		immediateContext->PSSetShaderResources(0, 1, material.shaderResourceView.GetAddressOf());

		immediateContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());


		for ( auto &subset : subsets )
		{
			if ( material.newmtl == subset.usemtl )
			{
				immediateContext->DrawIndexed( subset.indexCount, subset.indexStart, 0 );
			}
		}
	}

}