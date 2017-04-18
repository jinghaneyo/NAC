#include "stdafx.h"
#include "NAC_Scheduler.h"
#include <chrono>
#include <thread>

#define CONVERT_STR(func)	#func

NAC_Scheduler::NAC_Scheduler()
{
}


NAC_Scheduler::~NAC_Scheduler()
{
}

void NAC_Scheduler::Start(__in bool bWait)
{
	// jingtest 10���� �ϴ� �׽�Ʈ�� 
	m_ThreadPool.Start(10);

	std::thread thr([this]
	{
		Thread_Scheduler();
	}
	);

	if (true == bWait)
		thr.join();
	else
		thr.detach();
}

void NAC_Scheduler::Stop()
{
	m_ThreadPool.Stop();

	m_bContinue_Scheduler = false;
}

void NAC_Scheduler::AddEvent_Task(__in time_t nStartTime, 
								  __in time_t nInterval, 
								  __in call_Func func, 
								  __in std::string strFuncKey, 
								  __in bool bRepeat, 
								  __in int nTimeout_sec )
{
	std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
	time_t tNow = std::chrono::system_clock::to_time_t(timeNow);

	// ������ �ð��� ���簡 �������� ���� �� �ʿ� ���� 
	if (nStartTime > 0)
	{
		if (tNow > nStartTime)
			return;

		nStartTime += nInterval;
	}
	else  // 0 ���ϴ� ���� �ð� ���� ���� 
	{
		nStartTime = tNow + nInterval + 2;	// 2�ʴ� ������ 
	}

	std::lock_guard<std::mutex> guard(m_MutexMapFunc);

	TASK_INFO task;
	task.bRepeat = bRepeat;
	task.func = std::move(func);
	task.timeInterval = nInterval;
	task.timeout = nTimeout_sec;

	auto itr = m_mapFunc.find(nStartTime);
	if (itr == m_mapFunc.end())
	{
		std::map<std::string, TASK_INFO> mapTask;
		mapTask.emplace(std::make_pair(strFuncKey, task));
		
		m_mapFunc.emplace(std::make_pair(nStartTime, mapTask));
	}
	else
	{
		auto data = m_mapFunc[nStartTime].find(strFuncKey);
		if (data == m_mapFunc[nStartTime].end())
			m_mapFunc[nStartTime].emplace(std::make_pair(strFuncKey, task));
		else
		{
			m_mapFunc[nStartTime][strFuncKey] = task;
		}
	}
}

void NAC_Scheduler::Do_Task(__in time_t tNow, __inout std::map<std::string, TASK_INFO> &taskMap)
{
	time_t tNext = 0;

	for (auto &task : taskMap)
	{
		if (nullptr != task.second.func)
		{
			m_ThreadPool.Call_Function(std::move(task.second.func));
			//task.second.func();

			// �ݺ��� �ƴϸ� ���� �����ٸ����� �̵� 
			if (true == task.second.bRepeat)
			{
				tNext = tNow + task.second.timeInterval;

				InsertScheduleMap(tNext, task.first, task.second);
			}
		}
	}

	// ���ݰŴ� ���� 
	std::lock_guard<std::mutex> guard(m_MutexMapFunc);
	m_mapFunc.erase(tNow);
}

void NAC_Scheduler::InsertScheduleMap(__in time_t timeTask,
									  __in const std::string &strKey,
									  __in TASK_INFO &task)
{
	std::lock_guard<std::mutex> guard(m_MutexMapFunc);

	auto taskNext = m_mapFunc.find(timeTask);
	if (taskNext == m_mapFunc.end())
	{
		std::map<std::string, TASK_INFO> taskNew;
		taskNew.emplace(std::make_pair(strKey, task));

		m_mapFunc.emplace(std::make_pair(timeTask, taskNew));
	}
	else
	{
		auto data = (*taskNext).second.find(strKey);
		if (data == (*taskNext).second.end())
		{
			(*taskNext).second.emplace(std::make_pair(strKey, task));
		}
		else
		{
			(*data).second = task;
		}
	}
}

void NAC_Scheduler::Thread_Scheduler()
{
	m_bContinue_Scheduler = true;

	std::chrono::seconds sec(1);
	std::chrono::system_clock::time_point timeNow;
	time_t tNow = 0;

	while (true == m_bContinue_Scheduler)
	{
		timeNow = std::chrono::system_clock::now();
		tNow = std::chrono::system_clock::to_time_t(timeNow);

		auto taskNow = m_mapFunc.find(tNow);
		if (taskNow != m_mapFunc.end())
		{
			Do_Task(tNow, (*taskNow).second);
		}

		std::this_thread::sleep_for(sec);
	}
}
