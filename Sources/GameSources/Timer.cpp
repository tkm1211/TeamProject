
#include "Timer.h"
#include "FrameWork.h"

void Timer::Init()
{
    Clear();
    //spr_timer = std::make_unique<Sprite>("");
}

void Timer::UnInit()
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