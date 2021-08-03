
#include "Timer.h"
#include "FrameWork.h"

void Timer::Initialize()
{
    Clear();
    //spr_timer = std::make_unique<Sprite>("");
}

void Timer::UnInitialize()
{
    Clear();
}

void Timer::Update()
{
    float elapsedTime = FrameWork::GetInstance().GetElapsedTime();

}

void Timer::Render()
{

}