#pragma once
#include "Camera.h"


class CameraSystem
{
public:
	Camera mainView;
	Camera enemyEditorView;
	Camera bgEditorView;

public:
	CameraSystem() {}
	~CameraSystem() {}

	void Init()
	{ 
		mainView.Init( DirectX::XMFLOAT3(-9.97f, 265.0f, -830.0f ), DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		enemyEditorView.Init( DirectX::XMFLOAT3( 270.0f, 500.0f, 1414.0f ), DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		bgEditorView.Init( DirectX::XMFLOAT3( 270.0f, 500.0f, 1414.0f ), DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	}

	static CameraSystem* GetInstance()
	{
		static CameraSystem instance;
		return &instance;
	}
};