#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include "Shader.h"
#include "OBJ3D.h"

#undef max
#undef min

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>


class Mesh
{
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		wireframeRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		solidRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;

	// TRUE : right handed , FALSE : left handed
	BOOL handedCoordinateSystem = FALSE;

public:
	/*struct Face
	{
		DirectX::XMFLOAT3 pos[3];
		int materialIndex;

		Face() = default;

		template <class T>
		void serialize(T& archive)
		{
			archive
			(
				CEREAL_NVP(pos[0].x), CEREAL_NVP(pos[0].y), CEREAL_NVP(pos[0].z),
				CEREAL_NVP(pos[1].x), CEREAL_NVP(pos[1].y), CEREAL_NVP(pos[1].z),
				CEREAL_NVP(pos[2].x), CEREAL_NVP(pos[2].y), CEREAL_NVP(pos[2].z),
				CEREAL_NVP(materialIndex)
			);
		}
	};*/

public:
	Mesh() {}
	virtual ~Mesh() {}

public:
	virtual void Preparation(ID3D11DeviceContext* immediateContext, Shader shader, bool wireframe = false) {}
	//virtual void Render
	//(
	//	ID3D11DeviceContext *immediateContext,
	//	const DirectX::XMFLOAT4X4 &wvp,
	//	const DirectX::XMFLOAT4X4 &world,
	//	const DirectX::XMFLOAT4 &lightDirection,
	//	const DirectX::XMFLOAT4 &materialColor,
	//	float elapsedTime,
	//	bool wireframe = false
	//) {}
	virtual void Render
	(
		ID3D11DeviceContext* immediateContext,
		const DirectX::XMFLOAT4X4& wvp,
		const DirectX::XMFLOAT4X4& world,
		const DirectX::XMFLOAT4& lightDirection,
		const DirectX::XMFLOAT4& materialColor,
		float elapsedTime,
		bool inCamera,
		bool solid
	) {}
	virtual void Begin(ID3D11DeviceContext* immediateContext, Shader shader, bool wireframe = false) {}
	virtual void Render
	(
		ID3D11DeviceContext* immediateContext,
		OBJ3DInstance& obj,
		const DirectX::XMFLOAT4X4& wvp,
		const DirectX::XMFLOAT4X4& world,
		float elapsedTime
	) {}
	virtual void End
	(
		ID3D11DeviceContext* immediateContext,
		const DirectX::XMFLOAT4& lightDirection,
		const DirectX::XMFLOAT4& materialColor
	) {}

	//virtual void Release() {}

	virtual void StartAnimation(u_int _animationNumber, bool _isloopAnimation) {}
	// virtual void StopAnimation() {}
	virtual void ReStartAnimation() {}
	virtual void PauseAnimation() {}
	virtual bool GetAnimatingFlg() { return false; }
	virtual int GetAnimationFrame() { return 0; }
	virtual bool GetFinishAnimation() { return false; }
	virtual DirectX::XMFLOAT3 GetVectexPos( std::string name, const DirectX::XMFLOAT3& pos, int vectexPosNo ) { return DirectX::XMFLOAT3(); }
	virtual DirectX::XMFLOAT4X4 GetBoneTransform( std::string name ) { return DirectX::XMFLOAT4X4(); }
	virtual DirectX::XMFLOAT4X4 GetBoneTransform( std::string name, OBJ3DInstance& obj ) { return DirectX::XMFLOAT4X4(); }
	virtual DirectX::XMFLOAT4X4 GetBoneTransform( int meshIndex, int boneIndex ) { return DirectX::XMFLOAT4X4(); }
	virtual DirectX::XMFLOAT4X4 GetBoneTransform( int meshIndex, int boneIndex, OBJ3DInstance& obj ) { return DirectX::XMFLOAT4X4(); }
	virtual bool GetBoneTransformIndex( std::string name, int& meshIndex, int& boneIndex ) { return false; }
	virtual int RayPick
	(
		const DirectX::XMFLOAT3& startPosition,
		const DirectX::XMFLOAT3& endPosition,
		DirectX::XMFLOAT3* outPosition,
		DirectX::XMFLOAT3* outNormal,
		float* outLength
	) { return 0; }
	//virtual std::vector<Face> GetFaces() { return std::vector<Face>(); }

	template <class T>
	void serialize(T& archive)
	{
		return;
	}

protected:
	void Init(ID3D11Device* device, BOOL handedCoordinateSystem);
};