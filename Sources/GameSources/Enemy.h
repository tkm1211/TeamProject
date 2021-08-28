#pragma once
#include <memory>
#include "OBJ3D.h"

class Model;

class Enemy : public OBJ3D
{
public:
	explicit Enemy() {}
	virtual void Update(const float elapsed_time) = 0;
	void Render();

public: // Getä÷êî

public: // Setä÷êî
	void SetPlayerPosition(DirectX::XMFLOAT3* player_pos) { player_position = player_pos; }

protected:
	DirectX::XMFLOAT3 GetPlayerPosition() { return *player_position; }

protected:
	std::shared_ptr<Model> model{ nullptr };
	DirectX::XMFLOAT3* player_position = {};

	int timer=0;
};