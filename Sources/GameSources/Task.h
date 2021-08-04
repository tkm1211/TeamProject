#pragma once


class Task
{
public:
    Task() {};
    virtual ~Task() {};

public:
    virtual bool JudgeTaskComplete() = 0;

};



class TaskWave0 : public Task
{
public:
    TaskWave0() {};
    virtual ~TaskWave0() {};

public:
    bool JudgeTaskComplete() override;

};


class TaskWave1 : public Task
{
public:
    TaskWave1() {};
    virtual ~TaskWave1() {};

public:
    bool JudgeTaskComplete() override;

};