#include <DirectXTex.h>
#include <DirectXMath.h>

#include "SkinnedMeshBatch.h"
#include "WICTextureLoader.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"

using namespace fbxsdk;


void SkinnedMeshBatch::LoadFBX( ID3D11Device *device, const char* fileName )
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

void SkinnedMeshBatch::Begin( ID3D11DeviceContext* immediateContext, Shader shader, bool wireframe )
{
	// プリミティブモードの設定
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ラスタライザーの設定
	if (wireframe)	immediateContext->RSSetState(wireframeRasterizerState.Get());
	else			immediateContext->RSSetState(solidRasterizerState.Get());

	// シェーダーの設定
	immediateContext->VSSetShader(shader.GetVertexShader().Get(), nullptr, 0);
	immediateContext->PSSetShader(shader.GetPixelShader().Get(), nullptr, 0);

	// 入力レイアウトの設定
	immediateContext->IASetInputLayout(shader.GetInputLayout().Get());

	// 深度テストの設定
	//immediateContext->OMSetDepthStencilState( depthStencilState.Get(), 0 );

	// サンプラーの設定
	immediateContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	instance = 0;
}
void SkinnedMeshBatch::Render( ID3D11DeviceContext *immediateContext, OBJ3DInstance& obj, const DirectX::XMFLOAT4X4 &wvp, const DirectX::XMFLOAT4X4 &world, float elapsedTime )
{
	if (MAX_INSTANCE <= instance) return;

	Instance _instatnceData;
	if (!handedCoordinateSystem)
	{
		DirectX::XMStoreFloat4x4(&_instatnceData.wvp,
			DirectX::XMLoadFloat4x4(&wvp));

		DirectX::XMStoreFloat4x4(&_instatnceData.world,
			DirectX::XMLoadFloat4x4(&world));
	}
	else
	{
		DirectX::XMStoreFloat4x4(&_instatnceData.wvp,
			DirectX::XMLoadFloat4x4(&coordinateConversion) *
			DirectX::XMLoadFloat4x4(&wvp));

		DirectX::XMStoreFloat4x4(&_instatnceData.world,
			DirectX::XMLoadFloat4x4(&coordinateConversion) *
			DirectX::XMLoadFloat4x4(&world));
	}

	instatnceData.push_back(_instatnceData);


	AnimationData _animationData;

	_animationData.isAnimation = obj.GetIsAnimation();
	_animationData.isLoopAnimation = obj.GetIsLoopAnimation();

	if ( _animationData.isAnimation )
	{
		_animationData.animationTick = obj.GetAnimationTick() + elapsedTime;
		int frame = static_cast<int>(_animationData.animationTick / meshes.at(0).skeletalAnimations.at(animationNumber).samplingTime);
		if (frame > static_cast<int>(meshes.at(0).skeletalAnimations.at(animationNumber).skeletel.size()) - 1)
		{
			_animationData.animationTick = 0.0f;
		}
		obj.SetAnimationTick(_animationData.animationTick);
	}
	else
	{
		_animationData.animationTick = obj.GetAnimationTick();
		obj.SetAnimationTick(_animationData.animationTick);
	}

	animationData.push_back(_animationData);


	instance++;
}
void SkinnedMeshBatch::End( ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& materialColor )
{
	if (instance <= 0) return;

	HRESULT hr = S_OK;

	for (auto& mesh : meshes)
	{
		//頂点バッファのバインド
		UINT strides[2] = { sizeof( Vertex ), sizeof( Instance ) };
		UINT offsets[2] = { 0, 0 };
		ID3D11Buffer* vbs[2] = { mesh.vertexBuffer.Get(), instanceBuffer.Get() };

		immediateContext->IASetVertexBuffers(0, 2, vbs, strides, offsets);
		immediateContext->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;

		hr = immediateContext->Map(instanceBuffer.Get(), 0, map, 0, &mappedBuffer);
		assert(!hr && "Map		Error");

		Instance* instances = nullptr;
		instances = static_cast<Instance *>(mappedBuffer.pData);

		for (UINT i = 0; i < instance; i++)
		{
			instances[i].wvp = instatnceData[i].wvp;
			instances[i].world = instatnceData[i].world;

			if (animationNumber < mesh.skeletalAnimations.size() && mesh.skeletalAnimations.at(animationNumber).skeletel.size() > 0)
			{
				int frame = static_cast<int>(animationData[i].animationTick / mesh.skeletalAnimations.at(animationNumber).samplingTime);
				instances[i].frame = frame;
			}
		}

		immediateContext->Unmap(instanceBuffer.Get(), 0);

		// コンスタントバッファの設定
		CBuffer data;
		data.lightDirection = lightDirection;
		data.materialColor.x = materialColor.x;
		data.materialColor.y = materialColor.y;
		data.materialColor.z = materialColor.z;
		data.materialColor.w = materialColor.w;

		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);

		immediateContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		immediateContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());


		BoneTransformBuffer boneTransformData[MAX_BUFFER_SIZE];
		int maxFrame = mesh.skeletalAnimations.size() != 0 ? static_cast<int>( mesh.skeletalAnimations.at( animationNumber ).skeletel.size()) : 0 ;
		int frameCnt = 0;

		for( int j = 0; j < MAX_BUFFER_SIZE; j++ )
		{
			for ( int k = 0; k < MAX_FRAME; k++ )
			{
				for (size_t l = 0; l < MAX_BONES; l++)
				{
					DirectX::XMStoreFloat4x4(&boneTransformData[j].transform[k][l], DirectX::XMLoadFloat4x4(&originUnitMatrix));
				}
				if ( maxFrame <= frameCnt )
				{
					break;
				}

				std::vector<Bone>& skeletal = mesh.skeletalAnimations.at( animationNumber ).skeletel.at( k ).bone;
				size_t numberOfBones = skeletal.size();
				_ASSERT_EXPR( numberOfBones < MAX_BONES, L"'the numberOfBones' exceeds MAX_BONES." );

				for ( size_t l = 0; l < numberOfBones; l++ )
				{
					DirectX::XMStoreFloat4x4( &boneTransformData[j].transform[k][l], XMLoadFloat4x4( &skeletal.at( l ).transform ) );
				}

				frameCnt++;
			}
			immediateContext->UpdateSubresource( boneTransformBuffer[j].Get(), 0, 0, &boneTransformData[j], 0, 0 );

			immediateContext->VSSetConstantBuffers( 7 + j, 1, boneTransformBuffer[j].GetAddressOf() );
			immediateContext->PSSetConstantBuffers( 7 + j, 1, boneTransformBuffer[j].GetAddressOf() );
		}

		for (auto& subset : mesh.subsets)
		{
			immediateContext->PSSetShaderResources( 0, 1, subset.diffuse.shaderResourceView.GetAddressOf() );

			// Draw
#if 1
			immediateContext->DrawIndexedInstanced( subset.indexCount - subset.indexStart, instance, 0, 0, 0 );
#else
			immediateContext->DrawIndexedInstanced( subset.indexCount, instance, subset.indexStart, 0, 0 );
#endif
		}
	}

	animationData.clear();
	instatnceData.clear();
	instance = 0;
}

void SkinnedMeshBatch::CreateBuffer( ID3D11Device *device, MeshData &m, Vertex* vertices, unsigned int* indices, int numV, int numI )
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


void SkinnedMeshBatch::CreateShaderResourceView( ID3D11Device *device )
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

void SkinnedMeshBatch::FbxAMatrixToXMFLOAT4X4( const FbxAMatrix& fbxamatrix, DirectX::XMFLOAT4X4& xmfloat4x4 )
{

	for ( int row = 0; row < 4; row++ )
	{
		for ( int column = 0; column < 4; column++ )
		{
			xmfloat4x4.m[row][column] = static_cast<float>( fbxamatrix[row][column] );
		}
	}

}

void SkinnedMeshBatch::FetchAnimations( ID3D11Device* device, FbxMesh* fbxMesh, MeshData& mesh )
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

void SkinnedMeshBatch::FetchMaterials( ID3D11Device* device, const char* fileName, FbxMesh* fbxMesh, MeshData& mesh )
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

						// Create "diffuse.shader_resource_view" from "filename".
						char allName[256] = { 0 };
						char fbxFileName[64] = { 0 };
						char textureFileName[64] = { 0 };

						_splitpath_s( fileName, NULL, NULL, fbxFileName, sizeof( fbxFileName ), NULL, NULL, NULL, NULL );
						_makepath_s( allName, 256, NULL, fbxFileName, _fileName, NULL );

						material.texture = allName;
					}
				}
			}
		};
		FetchMaterial(subset.diffuse,   FbxSurfaceMaterial::sDiffuse,   FbxSurfaceMaterial::sDiffuseFactor);
		FetchMaterial(subset.ambient,   FbxSurfaceMaterial::sAmbient,   FbxSurfaceMaterial::sAmbientFactor);
		FetchMaterial(subset.specular,  FbxSurfaceMaterial::sSpecular,  FbxSurfaceMaterial::sSpecularFactor);
		FetchMaterial(subset.normalMap, FbxSurfaceMaterial::sNormalMap, FbxSurfaceMaterial::sBumpFactor);
		FetchMaterial(subset.bump,      FbxSurfaceMaterial::sBump,      FbxSurfaceMaterial::sBumpFactor);

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

void SkinnedMeshBatch::FetchVertecesAndIndeces( ID3D11Device* device, FbxMesh* fbxMesh, MeshData& mesh )
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
			for (int i = 0; i < 4; i++)
			{
				mesh.boneWeights[i] = vertex.boneWeights[i];
				mesh.boneIndeces[i] = vertex.boneIndices[i];
				mesh.pos = { vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f };
			}
			vertices.push_back( vertex );
		}
		subset.indexCount += 3;
	}
	integratedVertex.push_back( vertices );
	integratedIndex.push_back( indices );


	CreateBuffer( device, mesh, vertices.data(), indices.data(), static_cast<int>(vertices.size()), static_cast<int>(indices.size()) );

}

void SkinnedMeshBatch::FetchBoneInfluences( const FbxMesh *fbxMesh, std::vector<BoneInfluencesPerControlPoint> &influences )
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

void SkinnedMeshBatch::FetchBoneMatrices( FbxMesh *fbxMesh, std::vector<Bone> &skeletal, FbxTime time )
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

void SkinnedMeshBatch::FetchAnimations( FbxMesh *fbxMesh, SkinnedMeshBatch::SkeletalAnimation &skeletalAnimation, u_int numOfAnimation, u_int samplingRate )
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
			Skeletal skeletal;

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