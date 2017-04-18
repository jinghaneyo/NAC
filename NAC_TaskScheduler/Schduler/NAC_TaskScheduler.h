#pragma once

#include <thread>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <chrono>
#include <future>
#include <mutex>

typedef std::function<void(void *)> callback_Task;
typedef std::chrono::time_point<std::chrono::system_clock> time_point;

struct TASK
{
	//TASK(int nInterval)
	//{
	//	task = nullptr;
	//	nInvterval_sec = std::chrono::seconds(nInterval);
	//	timeTask = std::chrono::system_clock::now() + nInvterval_sec;
	//}

	callback_Task Func_Task;
	std::chrono::seconds nInvterval;
	int nInvterval_sec;
	time_point timeTask;
};

class NAC_TaskScheduler
{
public:
	NAC_TaskScheduler();
	~NAC_TaskScheduler();

private:
	std::atomic_bool m_bRun_Ready = false;
	std::thread m_ThrReady;
	std::mutex m_mutexReady;

	std::atomic_bool m_bRun_Task = false;
	std::thread m_ThrTask;
	std::mutex m_mutexTask;

	// 키는 인터발 초
	std::vector<TASK> m_mapReady;
public:
	void Start();
	void Stop();

	void AddTask_Repeat(callback_Task callback, int nInterval_sec);
private:
};

