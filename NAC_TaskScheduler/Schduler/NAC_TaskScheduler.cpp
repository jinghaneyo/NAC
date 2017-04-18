#include "stdafx.h"
#include "NAC_TaskScheduler.h"

NAC_TaskScheduler::NAC_TaskScheduler()
{
}


NAC_TaskScheduler::~NAC_TaskScheduler()
{
}

void NAC_TaskScheduler::Start()
{
	m_bRun_Ready = true;

	m_ThrReady = std::thread([this]()
	{
		long long TimeStart = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch().count();

		while (true == m_bRun_Ready)
		{
			long long TimeNow = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch().count();

			TimeNow = TimeNow - TimeStart;

			for (auto task : m_mapReady)
			{
				if ( 0 == (TimeNow % task.nInvterval_sec) )
				{
					task.Func_Task(nullptr);
				}
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});
	m_ThrReady.detach();
}

void NAC_TaskScheduler::Stop()
{
	m_bRun_Ready.store(false);
}
	
void NAC_TaskScheduler::AddTask_Repeat(callback_Task callback, int nInterval_sec)
{
	std::lock_guard<std::mutex> guard(m_mutexReady);

	TASK task;
	task.Func_Task = std::move(callback);
	task.nInvterval_sec = nInterval_sec;

	m_mapReady.emplace_back(task);

	// 초로 변환 
	//time_point time = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()) + std::chrono::seconds(nInterval_sec);

	//if(itr == m_mapReady.end())
	//	m_mapReady.emplace(std::pair<time_point, callback_Task>(time, std::move(callback)));
}
