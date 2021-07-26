#include <DirectXTex.h>
#include <DirectXMath.h>

#include "SkinnedMesh.h"
#include "WICTextureLoader.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"
#include "CameraSystem.h"

using namespace fbxsdk;


void SkinnedMesh::LoadFBX( ID3D11Device *device, const char* fileName )
{

	// Create the FBX SDK manager
	FbxManager* manager = FbxManager::Create();

	// Create an IOSettings object. IOSROOT is defined in Fbxiosettingspath.h.
	manager->SetIOSettings( FbxIOSettings::Create( manager, IOSROOT ) );

	// Create an importer.
	FbxImporter* importer = FbxImporter::Create( manager, "" );

	// Initialize the importer.
	bool importStatus = false;
	importStatus = importer->Initialize( fileName, -1, manager->GetIOSettings() );
	_ASSERT_EXPR_A( importStatus, importer->GetStatus().GetErrorString() );


	// Create a new scene so it can be populated by the imported file.
	FbxScene* scene = FbxScene::Create( manager, "" );

	// Import the contents of the file into the scene.
	importStatus = importer->Import( scene );
	_ASSERT_EXPR_A( importStatus, importer->GetStatus().GetErrorString() );

	importer->Destroy();

	// Convert mesh, NURBS and patch into triangle mesh
	FbxGeometryConverter geometryConverter( manager );

	geometryConverter.Triangulate( scene, true );

	// Fetch node attributes and materials under this node recursively. Currently only mesh.
	std::vector<FbxNode*> fetchedMeshes;
	std::vector<FbxNode*> fetchedSkeletons;

	std::function<void( FbxNode* )> traverse = [&]( FbxNode* node )
	{
		if ( node )
		{
			FbxNodeAttribute *fbxNodeAttribute = node->GetNodeAttribute();

			if ( fbxNodeAttribute )
			{
				switch ( fbxNodeAttribute->GetAttributeType() ) 
				{
				case FbxNodeAttribute::eMesh:
					fetchedMeshes.push_back( node );
					break;
				/*case FbxNodeAttribute::eSkeleton:
					fetchedSkeletons.push_back( node );
					break;*/
				}
			}

			for ( int i = 0; i < node->GetChildCount(); i++ )
			{
				traverse( node->GetChild( i ) );
			}
		}
	};

	//std::vector<subset> subsets;
	traverse( scene->GetRootNode() );

//	std::string skeletonName = fetchedSkeletons.at(0)->GetName();
	//FbxMesh *fbx_mesh = fetched_meshes.at(0)->GetMesh(); // Currently only one mesh.
	meshes.resize( fetchedMeshes.size() );

	for ( size_t i = 0; i < fetchedMeshes.size(); i++ )
	{
		FbxMesh *fbxMesh = fetchedMeshes.at( i )->GetMesh();

		MeshData &mesh = meshes.at( i );

		mesh.name = fetchedMeshes.at( i )->GetName();

		FbxAMatrix globalTransform = fbxMesh->GetNode()->EvaluateGlobalTransform( 0 );
		FbxAMatrixToXMFLOAT4X4(globalTransform, mesh.globalTransform);

		FbxTime::EMode timeMode = fbxMesh->GetScene()->GetGlobalSettings().GetTimeMode();

		FbxTime frameTime;
		frameTime.SetTime( 0, 0, 0, 1, 0, timeMode );

		FetchAnimations( device, fbxMesh, mesh );
		FetchMaterials( device, fileName, fbxMesh, mesh );
		FetchVertecesAndIndeces( device, fbxMesh, mesh );
	}

	manager->Destroy();

}

void SkinnedMesh::Preparation( ID3D11DeviceContext* immediateContext, Shader shader, bool wireframe )
{
	// プリミティブモードの設定
	immediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// シェーダーの設定
	immediateContext->VSSetShader( shader.GetVertexShader().Get(), nullptr, 0 );
	immediateContext->PSSetShader( shader.GetPixelShader().Get(), nullptr, 0 );

	// 入力レイアウトの設定
	immediateContext->IASetInputLayout( shader.GetInputLayout().Get() );

	// 深度テストの設定
	immediateContext->OMSetDepthStencilState( depthStencilState.Get(), 0 );
	
	// サンプラーの設定
	immediateContext->PSSetSamplers( 0, 1, samplerState.GetAddressOf() );

	// コンスタントバッファ設定
	Light::CBufferLight01 cb1;
	cb1.ambientColor = Light::GetInstance()->ambient;
	cb1.lightDir = Light::GetInstance()->lightDir;
	cb1.lightColor = Light::GetInstance()->lightColor;
	DirectX::XMFLOAT3 _viewPos = CameraSystem::GetInstance()->mainView.GetPos();
	cb1.eyePos.x = _viewPos.x;
	cb1.eyePos.y = _viewPos.y;
	cb1.eyePos.z = _viewPos.z;
	cb1.eyePos.w = wireframe;

	Light::CBufferLight02 cb2;
	memcpy( cb2.pointLight, Light::GetInstance()->pointLight, sizeof( Light::POINTLIGHT ) * Light::POINTMAX );
	memcpy( cb2.spotLight, Light::GetInstance()->spotLight, sizeof( Light::SPOTLIGHT ) * Light::SPOTMAX );
	
	immediateContext->UpdateSubresource(Light::GetInstance()->constantBufferLight1.Get(), 0, 0, &cb1, 0, 0);
	immediateContext->UpdateSubresource(Light::GetInstance()->constantBufferLight2.Get(), 0, 0, &cb2, 0, 0);

	immediateContext->VSSetConstantBuffers(2, 1, Light::GetInstance()->constantBufferLight1.GetAddressOf());
	immediateContext->VSSetConstantBuffers(3, 1, Light::GetInstance()->constantBufferLight2.GetAddressOf());
	immediateContext->PSSetConstantBuffers(2, 1, Light::GetInstance()->constantBufferLight1.GetAddressOf());
	immediateContext->PSSetConstantBuffers(3, 1, Light::GetInstance()->constantBufferLight2.GetAddressOf());
}

void SkinnedMesh::Render
  ( ID3D11DeviceContext *immediateContext,
	const DirectX::XMFLOAT4X4 &wvp,
	const DirectX::XMFLOAT4X4 &world,
	const DirectX::XMFLOAT4 &lightDirection,
	const DirectX::XMFLOAT4 &materialColor,
	float elapsedTime,
	bool inCamara,
	bool solid )
{

	int cnt = 0;
	for ( auto& mesh : meshes )
	{
		//if (cnt++ <= 0) continue;

		//頂点バッファのバインド
		UINT stride = sizeof( Vertex );
		UINT offset = 0;
		immediateContext->IASetVertexBuffers( 0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset );

		//インデックスバッファのバインド
		immediateContext->IASetIndexBuffer( mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0 );

		// ラスタライザーの設定
		//if ( !solid )	immediateContext->RSSetState( wireframeRasterizerState.Get() );
		//else				immediateContext->RSSetState( solidRasterizerState.Get() );

		//プリミティブの描画(index付き)
		for ( auto &subset : mesh.subsets )
		{
			CBuffer data;

			if ( isAnimation )
			{
				if ( animationNumber < mesh.skeletalAnimations.size() && mesh.skeletalAnimations.at( animationNumber ).skeletel.size() > 0 )
				{
					if (subset.animCnt == 0)
					{
						isFinishAnimation = false;
					}

					int frame = static_cast<int>( mesh.skeletalAnimations.at( animationNumber ).animationTick / mesh.skeletalAnimations.at( animationNumber ).samplingTime );
					animationFrame = frame;
					int maxSize = static_cast<int>(mesh.skeletalAnimations.at(animationNumber).skeletel.size()) - 1;
					if ( frame > static_cast<int>( mesh.skeletalAnimations.at( animationNumber ).skeletel.size() ) - 1 )
					{
						frame = 0;
						animationFrame = 0;
						mesh.skeletalAnimations.at( animationNumber ).animationTick = 0.0f;
						if ( !isLoopAnimation ) isAnimation = false;
					}
					if (maxSize <= subset.animCnt++)
					{
						subset.animCnt = 0;
						isFinishAnimation = true;
					}
				
					std::vector<Bone> &skeletal = mesh.skeletalAnimations.at( animationNumber ).skeletel.at( animationFrame ).bone;
					size_t number_of_bones = skeletal.size();
					_ASSERT_EXPR( number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES." );
				
				
					for ( size_t i = 0; i < number_of_bones; i++ )
					{
						XMStoreFloat4x4( &data.boneTransforms[i], XMLoadFloat4x4( &skeletal.at( i ).transform ) );
					}
					mesh.skeletalAnimations.at( animationNumber ).animationTick += elapsedTime;
				}
			}
			else
			{
				if (mesh.skeletalAnimations.size() <= 0)
				{
					for (size_t i = 0; i < MAX_BONES; i++)
					{
						DirectX::XMStoreFloat4x4(&data.boneTransforms[i], DirectX::XMLoadFloat4x4(&unitMatrix[i]));
					}
				}
				else if (mesh.skeletalAnimations.at(animationNumber).skeletel.size() <= 0)
				{
					for (size_t i = 0; i < MAX_BONES; i++)
					{
						DirectX::XMStoreFloat4x4(&data.boneTransforms[i], DirectX::XMLoadFloat4x4(&unitMatrix[i]));
					}
				}
				else if (mesh.skeletalAnimations.at(animationNumber).skeletel.at(animationFrame).bone.size() <= 0)
				{
					for (size_t i = 0; i < MAX_BONES; i++)
					{
						DirectX::XMStoreFloat4x4(&data.boneTransforms[i], DirectX::XMLoadFloat4x4(&unitMatrix[i]));
					}
				}
				else if ( animationNumber < mesh.skeletalAnimations.size() && mesh.skeletalAnimations.at( animationNumber ).skeletel.size() > 0 )
				{
					std::vector<Bone>& skeletal = mesh.skeletalAnimations.at( animationNumber ).skeletel.at( animationFrame ).bone;
					size_t number_of_bones = skeletal.size();
					_ASSERT_EXPR( number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES." );


					for ( size_t i = 0; i < number_of_bones; i++ )
					{
						XMStoreFloat4x4( &data.boneTransforms[i], XMLoadFloat4x4( &skeletal.at( i ).transform ) );
					}
				}
			}

			if ( inCamara )
			{
				/*if (cnt == 1)
				{
					DirectX::XMMATRIX M = DirectX::XMMatrixTranslation(25.0f, 0.0f, 0.0f);
					if (!handedCoordinateSystem)
					{
						DirectX::XMStoreFloat4x4(&data.wvp,
							DirectX::XMLoadFloat4x4(&mesh.globalTransform) *
							M *
							DirectX::XMLoadFloat4x4(&wvp));

						DirectX::XMStoreFloat4x4(&data.world,
							DirectX::XMLoadFloat4x4(&mesh.globalTransform) *
							M *
							DirectX::XMLoadFloat4x4(&world));
					}
					else
					{
						DirectX::XMStoreFloat4x4(&data.wvp,
							DirectX::XMLoadFloat4x4(&coordinateConversion) *
							DirectX::XMLoadFloat4x4(&mesh.globalTransform) *
							M *
							DirectX::XMLoadFloat4x4(&wvp));

						DirectX::XMStoreFloat4x4(&data.world,
							DirectX::XMLoadFloat4x4(&coordinateConversion) *
							DirectX::XMLoadFloat4x4(&mesh.globalTransform) *
							 M *
							DirectX::XMLoadFloat4x4(&world));
					}
				}
				else*/
				{
					if (!handedCoordinateSystem)
					{
						DirectX::XMStoreFloat4x4(&data.wvp,
							DirectX::XMLoadFloat4x4(&mesh.globalTransform) *
							DirectX::XMLoadFloat4x4(&wvp));

						DirectX::XMStoreFloat4x4(&data.world,
							DirectX::XMLoadFloat4x4(&mesh.globalTransform) *
							DirectX::XMLoadFloat4x4(&world));
					}
					else
					{
						DirectX::XMStoreFloat4x4(&data.wvp,
							DirectX::XMLoadFloat4x4(&coordinateConversion) *
							DirectX::XMLoadFloat4x4(&mesh.globalTransform) *
							DirectX::XMLoadFloat4x4(&wvp));

						DirectX::XMStoreFloat4x4(&data.world,
							DirectX::XMLoadFloat4x4(&coordinateConversion) *
							DirectX::XMLoadFloat4x4(&mesh.globalTransform) *
							DirectX::XMLoadFloat4x4(&world));
					}
				}

				// コンスタントバッファの設定
				data.lightDirection = lightDirection;
				data.materialColor.x = materialColor.x;
				data.materialColor.y = materialColor.y;
				data.materialColor.z = materialColor.z;
				data.materialColor.w = materialColor.w;

				immediateContext->UpdateSubresource( constantBuffer.Get(), 0, 0, &data, 0, 0 );

				immediateContext->VSSetConstantBuffers( 0, 1, constantBuffer.GetAddressOf() );
				immediateContext->PSSetConstantBuffers( 0, 1, constantBuffer.GetAddressOf() );

				immediateContext->PSSetShaderResources( 0, 1, subset.diffuse.shaderResourceView.GetAddressOf() );

				// Draw
				immediateContext->DrawIndexed( subset.indexCount, subset.indexStart, 0 );
			}
		}
		// if (1 <= cnt++) break;
		cnt++;
	}

}

void SkinnedMesh::CreateBuffer( ID3D11Device *device, MeshData &m, Vertex* vertices, unsigned int* indices, int numV, int numI )
{

	HRESULT hr = S_OK;

	
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof( Vertex ) * numV;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer( &bd, &InitData, m.vertexBuffer.GetAddressOf() );
	assert( !hr && "CreateBuffer	Error" );

	
	if ( indices )
	{
		ZeroMemory( &bd, sizeof( bd ) );
		bd.ByteWidth = sizeof( unsigned int ) * numI;
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		ZeroMemory( &InitData, sizeof( InitData ) );
		InitData.pSysMem = indices;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		hr = device->CreateBuffer( &bd, &InitData, m.indexBuffer.GetAddressOf() );
		assert( !hr && "CreateBuffer	Error" );

		numIndex = numI;
	}

}


void SkinnedMesh::CreateShaderResourceView( ID3D11Device *device )
{
	 
	for ( auto& m : meshes )
	{
		for ( auto& s : m.subsets )
		{
			if ( s.diffuse.texture[0] == NULL )
			{
				Texture::CreateDummyTexture( device, L"dummy_texture", s.diffuse.shaderResourceView.GetAddressOf() );
			}
			else
			{
				Texture::LoadTextureFromFile( device, s.diffuse.texture.c_str(), s.diffuse.shaderResourceView.GetAddressOf() );
			}
			if ( s.ambient.texture[0] == NULL )
			{
				Texture::CreateDummyTexture( device, L"dummy_texture", s.ambient.shaderResourceView.GetAddressOf() );
			}
			else
			{
				Texture::LoadTextureFromFile( device, s.ambient.texture.c_str(), s.ambient.shaderResourceView.GetAddressOf() );
			}
			if ( s.specular.texture[0] == NULL )
			{
				Texture::CreateDummyTexture( device, L"dummy_texture", s.specular.shaderResourceView.GetAddressOf() );
			}
			else
			{
				Texture::LoadTextureFromFile( device, s.specular.texture.c_str(), s.specular.shaderResourceView.GetAddressOf() );
			}
			if ( s.bump.texture[0] == NULL )
			{
				Texture::CreateDummyTexture( device, L"dummy_texture", s.bump.shaderResourceView.GetAddressOf() );
			}
			else
			{
				Texture::LoadTextureFromFile( device, s.bump.texture.c_str(), s.bump.shaderResourceView.GetAddressOf() );
			}
			if ( s.normalMap.texture[0] == NULL )
			{
				Texture::CreateDummyTexture( device, L"dummy_texture", s.normalMap.shaderResourceView.GetAddressOf() );
			}
			else
			{
				Texture::LoadTextureFromFile( device, s.normalMap.texture.c_str(), s.normalMap.shaderResourceView.GetAddressOf() );
			}
		}
	}

}

void SkinnedMesh::FbxAMatrixToXMFLOAT4X4( const FbxAMatrix& fbxamatrix, DirectX::XMFLOAT4X4& xmfloat4x4 )
{

	for ( int row = 0; row < 4; row++ )
	{
		for ( int column = 0; column < 4; column++ )
		{
			xmfloat4x4.m[row][column] = static_cast<float>( fbxamatrix[row][column] );
		}
	}

}

void SkinnedMesh::FetchAnimations( ID3D11Device* device, FbxMesh* fbxMesh, MeshData& mesh )
{

	// Get the list of all the animation stack.
	FbxArray<FbxString*> arrayOfAnimationStackNames;
	fbxMesh->GetScene()->FillAnimStackNameArray( arrayOfAnimationStackNames );

	// Get the number of animations.
	int numberOfAnimations = arrayOfAnimationStackNames.Size();

	for ( int i = 0; i < numberOfAnimations; i++ )
	{
		delete arrayOfAnimationStackNames[i];
	}

	mesh.skeletalAnimations.resize( numberOfAnimations );

	numberOfAnimations = 0;

	for ( size_t numberOfAnimations = 0; numberOfAnimations < mesh.skeletalAnimations.size(); numberOfAnimations++ )
	{
		SkeletalAnimation& skeletalAnimation = mesh.skeletalAnimations.at( numberOfAnimations );
		FetchAnimations( fbxMesh, skeletalAnimation, static_cast<u_int>(numberOfAnimations) );
	}

}

void SkinnedMesh::FetchMaterials( ID3D11Device* device, const char* fileName, FbxMesh* fbxMesh, MeshData& mesh )
{

	// Fetch material properties.
	const int numberOfMaterials = fbxMesh->GetNode()->GetMaterialCount();
	mesh.subsets.resize( numberOfMaterials );

	for ( int indexOfMaterial = 0; indexOfMaterial < numberOfMaterials; ++indexOfMaterial )
	{
		Subset& subset = mesh.subsets.at( indexOfMaterial );

		const FbxSurfaceMaterial* surfaceMaterial = fbxMesh->GetNode()->GetMaterial( indexOfMaterial );

		std::function<void( Material&, const char*, const char* )> FetchMaterial = [&]( Material& material, const char* propertyName, const char* factorName )
		{
			const FbxProperty property = surfaceMaterial->FindProperty( propertyName );
			const FbxProperty factor = surfaceMaterial->FindProperty( factorName );

			if ( property.IsValid() && factor.IsValid() )
			{
				FbxDouble4 color = property.Get<FbxDouble4>();
				double f = factor.Get<FbxDouble>();

				material.color.x = static_cast<float>( color[0] );
				material.color.y = static_cast<float>( color[1] );
				material.color.z = static_cast<float>( color[2] );
				material.color.w = 1.0f;
			}

			if ( property.IsValid() )
			{
				const int numberOfTextures = property.GetSrcObjectCount<FbxFileTexture>();

				if ( numberOfTextures )
				{
					const FbxFileTexture* fileTexture = property.GetSrcObject<FbxFileTexture>();

					if ( fileTexture )
					{
						const char* _fileName = fileTexture->GetRelativeFileName();

						if (_fileName[0] != NULL)
						{
							// Create "diffuse.shader_resource_view" from "filename".
							char allName[256] = { 0 };
							char fbxFileName[64] = { 0 };
							char textureFileName[64] = { 0 };

							_splitpath_s(fileName, NULL, NULL, fbxFileName, sizeof(fbxFileName), NULL, NULL, NULL, NULL);
							_makepath_s(allName, 256, NULL, fbxFileName, _fileName, NULL);

							material.texture = allName;
						}
					}
				}
			}
		};
		FetchMaterial( subset.diffuse,		FbxSurfaceMaterial::sDiffuse,	FbxSurfaceMaterial::sDiffuseFactor	);
		FetchMaterial( subset.ambient,		FbxSurfaceMaterial::sAmbient,	FbxSurfaceMaterial::sAmbientFactor	);
		FetchMaterial( subset.specular,		FbxSurfaceMaterial::sSpecular,	FbxSurfaceMaterial::sSpecularFactor	);
		FetchMaterial( subset.normalMap,	FbxSurfaceMaterial::sNormalMap,	FbxSurfaceMaterial::sBumpFactor		);
		FetchMaterial( subset.bump,			FbxSurfaceMaterial::sBump,		FbxSurfaceMaterial::sBumpFactor		);

	}

	// Count the polygon count of each material
	if ( numberOfMaterials > 0 )
	{
		// Count the faces of each material
		const int number_of_polygons = fbxMesh->GetPolygonCount();

		for ( int index_of_polygon = 0; index_of_polygon < number_of_polygons; ++index_of_polygon )
		{
			const u_int materialIndex = fbxMesh->GetElementMaterial()->GetIndexArray().GetAt( index_of_polygon );

			mesh.subsets.at( materialIndex ).indexCount += 3;
		}

		// Record the offset (how many vertex)
		int offset = 0;

		for ( Subset& subset : mesh.subsets )
		{
			subset.indexStart = offset;

			offset += subset.indexCount;

			// This will be used as counter in the following procedures, reset to zero
			subset.indexCount = 0;
		}
	}

}

void SkinnedMesh::FetchVertecesAndIndeces( ID3D11Device* device, FbxMesh* fbxMesh, MeshData& mesh )
{

	// Fetch mesh data
	std::vector<Vertex> vertices;
	std::vector<u_int> indices;
	u_int vertexCount = 0;
	FbxStringList uvNames;


	const int numberOfPolygons = fbxMesh->GetPolygonCount();
	indices.resize( numberOfPolygons * 3 );

	const int numberOfMaterials = fbxMesh->GetNode()->GetMaterialCount();
	const FbxVector4* arrayOfControlPoints = fbxMesh->GetControlPoints();

	fbxMesh->GetUVSetNames( uvNames );

	std::vector<BoneInfluencesPerControlPoint> boneInfluences;
	FetchBoneInfluences( fbxMesh, boneInfluences );


	for ( int indexOfPolygon = 0; indexOfPolygon < numberOfPolygons; indexOfPolygon++ )
	{
		// The material for current face.
		int indexOfMaterial = 0;

		if ( numberOfMaterials > 0 )
		{
			indexOfMaterial = fbxMesh->GetElementMaterial()->GetIndexArray().GetAt( indexOfPolygon );
		}

		// Where should I save the vertex attribute index, according to the material
		Subset& subset = mesh.subsets.at( indexOfMaterial );
		const int indexOffset = subset.indexStart + subset.indexCount;

		// 保存用ポリゴンデータ
	//	Face face;
	//	face.materialIndex = indexOfMaterial;

		for ( int indexOfVertex = 0; indexOfVertex < 3; indexOfVertex++ )
		{
			const int indexOfControlPoint = fbxMesh->GetPolygonVertex( indexOfPolygon, indexOfVertex );

			Vertex vertex;
			vertex.pos.x = static_cast<float>( arrayOfControlPoints[indexOfControlPoint][0] );
			vertex.pos.y = static_cast<float>( arrayOfControlPoints[indexOfControlPoint][1] );
			vertex.pos.z = static_cast<float>( arrayOfControlPoints[indexOfControlPoint][2] );

			FbxVector4 normal;
			fbxMesh->GetPolygonVertexNormal( indexOfPolygon, indexOfVertex, normal );
			vertex.normal.x = static_cast<float>( normal[0] );
			vertex.normal.y = static_cast<float>( normal[1] );
			vertex.normal.z = static_cast<float>( normal[2] );

			// 面ごとの頂点データ
			//face.pos[indexOfVertex] = vertex.pos;

			if ( fbxMesh->GetElementUVCount() != 0 )
			{
				FbxVector2 uv;
				bool unmappedUV;

				fbxMesh->GetPolygonVertexUV( indexOfPolygon, indexOfVertex, uvNames[0], uv, unmappedUV );

				vertex.tex.x = static_cast<float>( uv[0] );
				vertex.tex.y = 1.0f - static_cast<float>( uv[1] );

				indices.at( indexOffset + indexOfVertex ) = static_cast<u_int>( vertexCount );
				vertexCount += 1;
			}

			// ボーン影響度の所得
			for ( int i = 0; i < static_cast<int>( boneInfluences.at( indexOfControlPoint ).size() ); i++ )
			{
				if ( i < MAX_BONE_INFLUENCES )
				{
					vertex.boneWeights[i] = boneInfluences.at( indexOfControlPoint ).at( i ).weight;
					vertex.boneIndices[i] = boneInfluences.at( indexOfControlPoint ).at( i ).index;
				}
				else
				{
					vertex.boneWeights[0] += boneInfluences.at(indexOfControlPoint).at(i).weight;
				}
			}

			std::vector<float> _boneWeights;
			std::vector<int>   _boneIndeces;

			_boneWeights.resize(MAX_BONE_INFLUENCES);
			_boneIndeces.resize(MAX_BONE_INFLUENCES);

			for (int i = 0; i < MAX_BONE_INFLUENCES; i++)
			{
				_boneWeights[i] = vertex.boneWeights[i];
				_boneIndeces[i] = vertex.boneIndices[i];
			}

			VectexPos vectexPos;
			vectexPos.pos = { vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f };
			mesh.vectexPos.push_back( vectexPos );

			mesh.boneWeights.push_back(_boneWeights);
			mesh.boneIndeces.push_back(_boneIndeces);

			vertices.push_back( vertex );
		}
		subset.indexCount += 3;

		// 面データ保存
		//faces.push_back( face );
	}
	integratedVertex.push_back( vertices );
	integratedIndex.push_back( indices );


	CreateBuffer( device, mesh, vertices.data(), indices.data(), static_cast<int>(vertices.size()), static_cast<int>(indices.size()) );

}

void SkinnedMesh::FetchBoneInfluences( const FbxMesh *fbxMesh, std::vector<BoneInfluencesPerControlPoint> &influences )
{

	const int numberOfControlPoints = fbxMesh->GetControlPointsCount();
	influences.resize( numberOfControlPoints );

	const int numberOfDeformers = fbxMesh->GetDeformerCount( FbxDeformer::eSkin );

	for ( int indexOfDeformer = 0; indexOfDeformer < numberOfDeformers; ++indexOfDeformer )
	{
		FbxSkin *skin = static_cast<FbxSkin *>( fbxMesh->GetDeformer( indexOfDeformer, FbxDeformer::eSkin ) );
		const int numberOfClusters = skin->GetClusterCount();

		for ( int indexOfCluster = 0; indexOfCluster < numberOfClusters; ++indexOfCluster )
		{
			FbxCluster* cluster = skin->GetCluster( indexOfCluster );
			const int numberOfControlPointIndices = cluster->GetControlPointIndicesCount();
			const int *arrayOfControlPointIndices = cluster->GetControlPointIndices();
			const double *arrayOfControlPointWeights = cluster->GetControlPointWeights();

			for ( int i = 0; i < numberOfControlPointIndices; ++i )
			{
				BoneInfluencesPerControlPoint &influencesPerControlPoint
					= influences.at( arrayOfControlPointIndices[i] );

				BoneInfluence influence;
				influence.index = indexOfCluster;
				influence.weight = static_cast<float>( arrayOfControlPointWeights[i] );

				influencesPerControlPoint.push_back( influence );
			}
		}
	}

}

void SkinnedMesh::FetchBoneMatrices( FbxMesh *fbxMesh, std::vector<SkinnedMesh::Bone> &skeletal, FbxTime time )
{

	const int numberOfDeformers = fbxMesh->GetDeformerCount( FbxDeformer::eSkin );

	for ( int indexOfDeformer = 0; indexOfDeformer < numberOfDeformers; ++indexOfDeformer )
	{
		FbxSkin *skin = static_cast<FbxSkin *>( fbxMesh->GetDeformer( indexOfDeformer, FbxDeformer::eSkin ) );

		const int numberOfClusters = skin->GetClusterCount();
		skeletal.resize( numberOfClusters );

		for ( int indexOfCluster = 0; indexOfCluster < numberOfClusters; ++indexOfCluster )
		{
			Bone &bone = skeletal.at( indexOfCluster );
			FbxCluster *cluster = skin->GetCluster( indexOfCluster );

			// Get Bone Name
			std::string _name = cluster->GetName();
			bone.name = cluster->GetLink()->GetName();

			// this matrix trnasforms coordinates of the initial pose from mesh space to global space
			FbxAMatrix referenceGlobalInitPosition;
			cluster->GetTransformMatrix( referenceGlobalInitPosition );

			// this matrix trnasforms coordinates of the initial pose from bone space to global space
			FbxAMatrix clusterGlobalInitPosition;
			cluster->GetTransformLinkMatrix( clusterGlobalInitPosition );

			// this matrix trnasforms coordinates of the current pose from bone space to global space
			FbxAMatrix clusterGlobalCurrentPosition;
			clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform( time );
			FbxAMatrixToXMFLOAT4X4( clusterGlobalCurrentPosition, bone.globalTransform );

			// this matrix trnasforms coordinates of the current pose from mesh space to global space
			FbxAMatrix referenceGlobalCurrentPosition;
			referenceGlobalCurrentPosition = fbxMesh->GetNode()->EvaluateGlobalTransform( time );

			// Matrices are defined using the Column Major scheme. When a FbxAMatrix represents a transformation
			// (translation, rotation and scale), the last row of the matrix represents the translation part of the
			// transformation.
			FbxAMatrix transform = referenceGlobalCurrentPosition.Inverse() * clusterGlobalCurrentPosition
				* clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;

			// convert FbxAMatrix(transform) to XMFLOAT4X4(bone.transform)
			FbxAMatrixToXMFLOAT4X4( transform, bone.transform );

			// Get ParentBoneTransform
			bone.transformToParent = DirectX::XMFLOAT4X4( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
			FbxNode* parentNode = cluster->GetLink()->GetParent();
			if ( parentNode )
			{
				FbxAMatrix parentClusterGlobalPosition;
				parentClusterGlobalPosition = parentNode->EvaluateGlobalTransform( time );
				FbxAMatrixToXMFLOAT4X4( parentClusterGlobalPosition.Inverse() * clusterGlobalCurrentPosition, bone.transformToParent );
			}
		}
	}

}

void SkinnedMesh::FetchAnimations( FbxMesh *fbxMesh, SkinnedMesh::SkeletalAnimation &skeletalAnimation, u_int numOfAnimation, u_int samplingRate )
{

	// Get the list of all the animation stack.
	FbxArray<FbxString *> arrayOfAnimationStackNames;
	fbxMesh->GetScene()->FillAnimStackNameArray( arrayOfAnimationStackNames );

	// // Get the number of animations.
	int numberOfAnimations = arrayOfAnimationStackNames.Size();

	if ( numberOfAnimations > 0 )
	{
		// Get the FbxTime per animation's frame.
		FbxTime::EMode timeMode = fbxMesh->GetScene()->GetGlobalSettings().GetTimeMode();

		FbxTime frameTime;
		frameTime.SetTime( 0, 0, 0, 1, 0, timeMode );

		samplingRate = samplingRate > 0 ? samplingRate : static_cast<u_int>( frameTime.GetFrameRate( timeMode ) );
		float samplingTime = 1.0f / samplingRate;

		skeletalAnimation.samplingTime = samplingTime;
		skeletalAnimation.animationTick = 0.0f;

		FbxString *animationStackName = arrayOfAnimationStackNames.GetAt( numOfAnimation );
		FbxAnimStack * currentAnimationStack
			= fbxMesh->GetScene()->FindMember<FbxAnimStack>( animationStackName->Buffer() );
		fbxMesh->GetScene()->SetCurrentAnimationStack( currentAnimationStack );

		FbxTakeInfo *takeInfo = fbxMesh->GetScene()->GetTakeInfo( animationStackName->Buffer() );

		FbxTime startTime = takeInfo->mLocalTimeSpan.GetStart();
		FbxTime endTime = takeInfo->mLocalTimeSpan.GetStop();

		FbxTime samplingStep;
		samplingStep.SetTime( 0, 0, 1, 0, 0, timeMode );
		samplingStep = static_cast<FbxLongLong>( samplingStep.Get() * samplingTime );

		for ( FbxTime currentTime = startTime; currentTime < endTime; currentTime += samplingStep )
		{
			SkinnedMesh::Skeletal skeletal;

			FetchBoneMatrices( fbxMesh, skeletal.bone, currentTime );
			skeletalAnimation.skeletel.push_back(skeletal);
		}
	}
	for ( int i = 0; i < numberOfAnimations; i++ )
	{
		delete arrayOfAnimationStackNames[i];
	}

	numberOfAnimations = 0;
}

//int SkinnedMesh::RayPick(const DirectX::XMFLOAT3& startPosition, const DirectX::XMFLOAT3& endPosition, DirectX::XMFLOAT3* outPosition, DirectX::XMFLOAT3* outNormal, float* outLength)
//{
//	int ret = -1;
//	DirectX::XMVECTOR start = DirectX::XMLoadFloat3(&startPosition);
//	DirectX::XMVECTOR end = DirectX::XMLoadFloat3(&endPosition);
//	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);
//	DirectX::XMVECTOR length = DirectX::XMVector3Length(vec);
//	DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(vec);
//	float neart; // 一番近い距離を保持
//	DirectX::XMStoreFloat(&neart, length);
//
//	DirectX::XMVECTOR position, normal;
//	for (const auto& it : faces)
//	{
//		// 面頂点取得
//		DirectX::XMVECTOR a = DirectX::XMLoadFloat3(&it.pos[0]);
//		DirectX::XMVECTOR b = DirectX::XMLoadFloat3(&it.pos[1]);
//		DirectX::XMVECTOR c = DirectX::XMLoadFloat3(&it.pos[2]);
//
//		// 3辺算出
//		DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(b, a);
//		DirectX::XMVECTOR bc = DirectX::XMVectorSubtract(c, b);
//		DirectX::XMVECTOR ca = DirectX::XMVectorSubtract(a, c);
//
//		// 2辺の外積による法線算出
//		DirectX::XMVECTOR n = DirectX::XMVector3Cross(ab, bc);
//
//		// nとrey(dir)との内積の結果がプラスならば裏向き
//		float dot = 0.0f;
//		DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(n, dir));
//
//		if (0.0f < dot)// その場合、これ以上処理しない
//		{
//			continue;
//		}
//
//		// 交点算出	
//		// 交点までの距離(t)を算出の事
//		DirectX::XMVECTOR t;
//		DirectX::XMVECTOR v0 = DirectX::XMVectorSubtract(a, start);
//		DirectX::XMVECTOR vn_dot = DirectX::XMVector3Dot(v0, n);
//		DirectX::XMVECTOR dn_dot = DirectX::XMVector3Dot(dir, n);
//		t = DirectX::XMVectorDivide(vn_dot, dn_dot);
//
//		float ft;
//		DirectX::XMStoreFloat(&ft, t);
//		if (ft < 0.0f || ft > neart)
//		{
//			continue;
//		}
//
//		// 交点
//		DirectX::XMVECTOR cp = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, t), start);
//
//		// 内点判定
//		DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(a, cp);
//		DirectX::XMVECTOR temp = DirectX::XMVector3Cross(v1, ab);
//		DirectX::XMVECTOR work = DirectX::XMVector3Dot(temp, n);
//		float fwork;
//		DirectX::XMStoreFloat(&fwork, work);
//		if (fwork < 0.0f)
//		{
//			continue;
//		}
//		DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(b, cp);
//		temp = DirectX::XMVector3Cross(v2, bc);
//		work = DirectX::XMVector3Dot(temp, n);
//		DirectX::XMStoreFloat(&fwork, work);
//		if (fwork < 0.0f)
//		{
//			continue;
//		}
//		DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(c, cp);
//		temp = DirectX::XMVector3Cross(v3, ca);
//		work = DirectX::XMVector3Dot(temp, n);
//		DirectX::XMStoreFloat(&fwork, work);
//		if (fwork < 0.0f)
//		{
//			continue;
//		}
//
//		// 情報保存
//		position = cp;			// 当たった場所
//		normal = n;				// 法線
//		neart = ft;				// 距離
//		ret = it.materialIndex; // インデックス(戻り値)
//	}
//
//	if (ret != -1) // どこでも当たらなかったら
//	{
//		DirectX::XMStoreFloat3(outPosition, position);
//		DirectX::XMStoreFloat3(outNormal, normal);
//	}
//
//	// 最も近いヒット位置までの距離
//	*outLength = neart;
//	return ret;
//}