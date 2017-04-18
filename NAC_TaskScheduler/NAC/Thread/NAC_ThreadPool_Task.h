#pragma once

#ifndef _NAC_THREAD_POOL_TASK_H_ 
#define _NAC_THREAD_POOL_TASK_H_ 

#include <functional>

typedef std::function<void()> call_Func;

class NAC_ThreadPool_Task
{
public:
	NAC_ThreadPool_Task();
	~NAC_ThreadPool_Task();

	void Start( __in int nThreadCount );
	void Stop();
	void Call_Function( __in call_Func &&func );
private:
	bool m_bContinue;
};

#endif
