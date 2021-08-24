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
<<<<<<< HEAD
	int timer=0;
=======
	int timer;
>>>>>>> 38e2fae63bf96a464e169d65347fd1938a25bec3
};