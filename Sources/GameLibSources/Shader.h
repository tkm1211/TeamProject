#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>


class Shader
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;

public:
	 Shader() {}
	~Shader() {}

public:
	void LoadCSO( ID3D11Device* device, const char* csoNameOfVertexShader, const char* csoNameOfPixelShader, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements );
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader()	{ return vertexShader; }
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  GetPixelShader()		{ return pixelShader;  }
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  GetInputLayout()		{ return inputLayout;  }
};


class ShaderSystem
{
public:
	enum ShaderOfSkinnedMesh
	{
		DEFAULT,
		PHONE,
		NORMAL_MAP,
		END
	};

private:
	Shader shaderOfSkinnedMesh[ShaderOfSkinnedMesh::END];
	Shader shaderOfSkinnedMeshBatch;
	Shader shaderOfStaticMeshBatch;
	Shader shadowMapShader;

public:
	 ShaderSystem() {}
	~ShaderSystem() {}

	static ShaderSystem* GetInstance()
	{
		static ShaderSystem instance;
		return &instance;
	}

public:
	void Init();
	void UnInit() {}
	Shader GetShaderOfSkinnedMesh( ShaderOfSkinnedMesh no ) { return shaderOfSkinnedMesh[no]; }
	Shader GetShaderOfSkinnedMeshBatch() { return shaderOfSkinnedMeshBatch; }
	Shader GetShaderOfStaticMeshBatch() { return shaderOfStaticMeshBatch; }
	Shader GetShadowMapShader() { return shadowMapShader; }
};


class ComputeShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> computeShader;

public:
	 ComputeShader() {}
	~ComputeShader() {}

public:
	void LoadCSO( ID3D11Device* device, const char* csoNameOfComputeShader );
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> GetComputeShader() { return computeShader; }
};

class ComputeShaderSystem
{
private:
	ComputeShader gpuCaring;

public:
	 ComputeShaderSystem() {}
	~ComputeShaderSystem() {}

	static ComputeShaderSystem* GetInstance()
	{
		static ComputeShaderSystem instance;
		return &instance;
	}

public:
	void Init();
	void UnInit() {}
	ComputeShader GetGPUCaring() { return gpuCaring; }
};