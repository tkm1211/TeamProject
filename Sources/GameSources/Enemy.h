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

protected:
	std::shared_ptr<Model> model{ nullptr };

	int timer=0;
};