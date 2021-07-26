#pragma once

#include <memory>

#include "Sprite.h"
#include "Model.h"
#include "OBJ3D.h"


struct ModelAnimationData
{
	bool isAnim = false;
	int animNum = 0;
};

class AssetLoader
{
private:
	static const int MAX_SIZE = 10;

public:
	//std::unique_ptr<SpriteBatch> sprite[MAX_SIZE];
	std::unique_ptr<Model> model[MAX_SIZE];

private:
	ModelAnimationData modelAnim[MAX_SIZE];
	OBJ3D			   modelData[MAX_SIZE];

public:
	AssetLoader() {}
	~AssetLoader()
	{
		model;
	}

	void Loader( const wchar_t* _fileName );

	void Update();

	void Render
	(
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& lightDirection,
		float elapsedTime
	);

private:
	void LoadSprite( const wchar_t* _fileName );

	void LoadOBJ( const wchar_t* _fileName );
	void LoadFBX( const wchar_t* _fileName );

	void FileWrite( const wchar_t* _fileName );

};

extern AssetLoader assetLoader;

extern char* modelNames[];