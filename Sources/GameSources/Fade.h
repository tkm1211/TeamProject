#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "Model.h"
#include "OBJ3D.h"

#include <mutex>
#include <thread>


class Fade
{

private:
	std::unique_ptr<SpriteBatch> fade_spr;
	TexData fade_spr_data;
	int state;
	float alpha;

	Scene* scene;

	std::unique_ptr<Sprite> nowLoading;

	TexData nowLoadingData;
	TexData dotData[3];

	int cnt;
	int loadingState;

private:
	std::unique_ptr<std::thread> loadingThread;
	std::mutex loadingMutex;
	bool IsNowLoading()
	{
		if (loadingThread && loadingMutex.try_lock())
		{
			loadingMutex.unlock();
			return false;
		}
		return true;
	}


public:
	bool onFadeFlg;
	bool loading;
	bool nowLoad;

	Fade() {}
	~Fade() {}

	void Init();
	void Update();
	void Draw();
	void UnInit();
	void LoadModelsInit();
	void LoadModels();
	
	//Fadeå„ÇÃÉVÅ[Éì
	void SetNextScene(Scene* s) { scene = s; }

	static Fade* GetInstance()
	{
		static Fade instance;
		return &instance;
	}


};