#pragma once

#ifndef _NAC_SCHEDULER_H_
#define _NAC_SCHEDULER_H_

#include <functional>
#include <map>
#include <list>
#include <vector>
#include <mutex>
#include <ctime>
#include "NAC/Thread/NAC_ThreadPool_Task.h"



///> @brief : it made for controlling user function by setting time.
///> @class : NAC_Scheduler
///> @date : 2017-04-01
///> @auth : lee,giljae
class NAC_Scheduler
{
public:
	NAC_Scheduler();
	~NAC_Scheduler();

	void Start(__in bool bWait);
	void Stop();

	///> @brief : register user functin
	///> @param nStartTime : it is time for start execute
	///> @param nInterval : it is time for execute.(unit second)
	///> @param func : it is user function for execute.
	///> @param pszFuncKey : key of map, using user function
	///> @param bRepeat : repeat for calling user fucntion
	///> @param nTimeout_sec : time out (unit second)
	///>						if zero is, it wait infinitly.
	void AddEvent_Task(__in time_t nStartTime, 
					   __in time_t nInterval, 
					   __in call_Func call, 
					   __in std::string strFuncKey,
					   __in bool bRepeat = true, 
					   __in int nTimeout_sec  = 0);
private:
	typedef struct _TASK_INFO
	{
		call_Func func = nullptr;		///> call user function
		std::time_t timeInterval = 0;	///> time for execute
		std::time_t timeout = 0;		///> time out
		bool bRepeat = true;			///> repate user function
	}TASK_INFO;

	void Do_Task(__in time_t tNow,
				 __inout std::map<std::string, TASK_INFO> &taskMap);

	void InsertScheduleMap(__in time_t timeTask,
						   __in const std::string &strKey,
						   __in TASK_INFO &task);

	void Thread_Scheduler();

	bool m_bContinue_Scheduler = false;

	std::mutex m_MutexMapFunc;
	std::map<std::time_t, std::map<std::string, TASK_INFO>> m_mapFunc; // Key is time of execute

	NAC_ThreadPool_Task m_ThreadPool;
};

#endif
