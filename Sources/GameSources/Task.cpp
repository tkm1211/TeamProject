
#include "Task.h"
#include "GameSystem.h"


bool TaskWave0::JudgeTaskComplete()
{
    bool hit = false;

    if (GameSystem::Instance().GetNowTime() < 300 - 10) hit = true;

    return hit;
}

bool TaskWave1::JudgeTaskComplete()
{
    bool hit = false;

    if (GameSystem::Instance().GetNowTime() < 300 - 20) hit = true;

    return hit;
}

bool TaskWave2::JudgeTaskComplete()
{
    bool hit = false;

    if (GameSystem::Instance().GetNowTime() < 300 - 30) hit = true;

    return hit;
}

bool TaskWave3::JudgeTaskComplete()
{
    bool hit = false;

    if (GameSystem::Instance().GetNowTime() < 300 - 40) hit = true;

    return hit;
}