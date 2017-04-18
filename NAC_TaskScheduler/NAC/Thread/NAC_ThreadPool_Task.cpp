#include "stdafx.h"
#include "NAC_ThreadPool_Task.h"
#include "NAC_Timeout.h"

NAC_ThreadPool_Task::NAC_ThreadPool_Task()
{
}

NAC_ThreadPool_Task::~NAC_ThreadPool_Task()
{
}

void NAC_ThreadPool_Task::Start(__in int nThreadCount)
{

}

void NAC_ThreadPool_Task::Stop()
{

}

void NAC_ThreadPool_Task::Call_Function(__in call_Func &&func)
{
	NAC_Timeout task;
	task.SetTimer(func, 60);
}