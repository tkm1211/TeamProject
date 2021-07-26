#pragma once
#include "Model.h"
#include "OBJ3D.h"


class PlayerTemplate
{
private:
	std::unique_ptr<Model> pWait;
	std::unique_ptr<Model> pRun;
	std::unique_ptr<Model> pAttack;
	std::unique_ptr<Model> pItem;

	OBJ3D modelData;
	OBJ3D itemData;

	// skinnedmeshbatch
	std::unique_ptr<Model> pModelBatch;
	static const int MAX_INSTANCE = 150;
	OBJ3DInstance batchData[MAX_INSTANCE];

	std::unique_ptr<Model> pItemBatch;
	OBJ3DInstance itemBatchData[MAX_INSTANCE];

	// New Get bonetransform
	int modelIndeces[2];
	int instanceModelIndeces[2];


	enum ModelState
	{
		WAIT,
		RUN,
		ATTACK
	};
#if 0
	ModelState modelState;
#else
	int modelState;
#endif

public:
	PlayerTemplate() {}
	~PlayerTemplate() {}

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
	void ImGui();
};