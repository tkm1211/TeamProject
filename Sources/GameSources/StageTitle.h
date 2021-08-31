#pragma once
#include <memory>
#include "Model.h"
#include "OBJ3D.h"

class StageTitle : public OBJ3D
{
public:
	void Init();
	void UnInit();
	void Update();
	void Render();

protected:
	std::shared_ptr<Model> StageModel{ nullptr };
};