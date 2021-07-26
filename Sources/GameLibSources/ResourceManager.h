#pragma once

#include <windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <map>
#include <memory>
#include <cassert>


class ResourceManager
{
public:
	static void CreateVertexShaderAndInputLayout
	(
		ID3D11Device *device,
		const char *csoName,
		ID3D11VertexShader **vertexShader,
		ID3D11InputLayout **inputLayout,
		D3D11_INPUT_ELEMENT_DESC *inputElementDesc,
		UINT num_elements
	);

	static void CreatePixelShader
	(
		ID3D11Device *device,
		const char *csoName,
		ID3D11PixelShader **pixelShader
	);

	static void CreateComputeShader
	(
		ID3D11Device* device,
		const char* csoName,
		ID3D11ComputeShader** computeShader
	);
};
