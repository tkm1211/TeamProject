#pragma once
#include <memory>
#include "Model.h"
#include "OBJ3D.h"


class Stage : public OBJ3D
{
public:
	void Init();
	void UnInit();
	void Render();

protected:
	std::shared_ptr<Model> StageModel{ nullptr };
	OBJ3D StageData;
};