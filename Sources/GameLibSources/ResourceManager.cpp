#include <string>
#include "ResourceManager.h"

void ResourceManager::CreateVertexShaderAndInputLayout
  ( ID3D11Device *device,
	const char *csoName,
	ID3D11VertexShader **vertexShader,
	ID3D11InputLayout **inputLayout,
	D3D11_INPUT_ELEMENT_DESC *inputElementDesc,
	UINT numElements )
{

	HRESULT hr = S_OK;


	struct SetOfVertexShaderAndInputLayout
	{
		SetOfVertexShaderAndInputLayout( ID3D11VertexShader  *_vertexShader, ID3D11InputLayout *_inputLayout ) : _vertexShader( _vertexShader ), _inputLayout( _inputLayout ) {}
		Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>  _inputLayout;
	};


	static std::map<std::string, SetOfVertexShaderAndInputLayout> cache;


	auto it = cache.find( csoName );
	if ( it != cache.end() )
	{
		*vertexShader = it->second._vertexShader.Get();
		( *vertexShader )->AddRef();
		*inputLayout = it->second._inputLayout.Get();
		( *inputLayout )->AddRef();
	}


	//Load CSO (VS)
	FILE* fp = 0;
	fopen_s( &fp, csoName, "rb" );

	
	fseek( fp, 0, SEEK_END );
	long csoSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	
	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>( csoSize );
	fread( csoData.get(), csoSize, 1, fp );
	fclose( fp );


	hr = device->CreateVertexShader( csoData.get(), csoSize, NULL, vertexShader );
	assert( !hr && "Errer device->CreateVertexShader" );


	hr = device->CreateInputLayout( inputElementDesc, numElements, csoData.get(), csoSize, inputLayout );
	assert( !hr && "Errer device->CreateInputLayout" );


	cache.insert( std::make_pair( csoName, SetOfVertexShaderAndInputLayout( *vertexShader, *inputLayout ) ) );

}

void ResourceManager::CreatePixelShader
  ( ID3D11Device *device,
	const char *csoName,
	ID3D11PixelShader **pixelShader )
{

	HRESULT hr = S_OK;


	static std::map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> cache;


	auto it = cache.find( csoName );
	if ( it != cache.end() )
	{
		*pixelShader = it->second.Get();
		( *pixelShader )->AddRef();
	}


	//Load CSO (ps)
	FILE* fp = 0;
	fopen_s( &fp, csoName, "rb" );


	fseek( fp, 0, SEEK_END );
	long csoSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );


	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>( csoSize );
	fread( csoData.get(), csoSize, 1, fp );
	fclose( fp );

	fp = nullptr;


	hr = device->CreatePixelShader( csoData.get(), csoSize, NULL, pixelShader );
	assert( !hr && "Errer device->CreatePixelShader" );


	cache.insert( std::make_pair( csoName, *pixelShader ) );

}

void ResourceManager::CreateComputeShader
  ( ID3D11Device *device,
	const char *csoName,
	ID3D11ComputeShader **computeShader )
{

	HRESULT hr = S_OK;


	static std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ComputeShader>> cache;


	auto it = cache.find( csoName );
	if ( it != cache.end() )
	{
		*computeShader = it->second.Get();
		( *computeShader )->AddRef();
	}


	//Load CSO (ps)
	FILE* fp = 0;
	fopen_s( &fp, csoName, "rb" );


	fseek( fp, 0, SEEK_END );
	long csoSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );


	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>( csoSize );
	fread( csoData.get(), csoSize, 1, fp );
	fclose( fp );

	fp = nullptr;


	hr = device->CreateComputeShader( csoData.get(), csoSize, NULL, computeShader );
	assert( !hr && "Errer device->CreatePixelShader" );


	cache.insert( std::make_pair( csoName, *computeShader ) );

}