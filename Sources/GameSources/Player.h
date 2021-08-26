#pragma once
#include <memory>
#include "OBJ3D.h"
#include "Model.h"



class Player : public OBJ3D
{
public:
	Player();
	~Player();

	void Update(float elapsedTime);

	void Render();


private:
	//変数の宣言
	std::unique_ptr<Model> testModel;//ユニークポインタ

	float  Ti = 0;
	float PMDx = GetPos().x;
	float PMDy = GetPos().y;
	float PMDz = GetPos().z;
	/*static*/ float Jgravity = GetSpeed().y;
	float gravity = -1;
	int dash = 1;





};























