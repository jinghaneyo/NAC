#include "stdafx.h"
#include <thread>
#include <mutex>
#include <condition_variable>

#include "NAC_Timeout.h"

NAC_Timeout::NAC_Timeout()
{
	m_Func = NULL;
}

NAC_Timeout::~NAC_Timeout()
{
}

bool NAC_Timeout::SetTimer( __in stdBind_Func _Func, __in int _nTime_Sec  )
{
	bool bRet = true;
	std::mutex Mutex;
	std::condition_variable cv;

	std::thread t([&_Func, &cv]()
	{
		_Func();

		cv.notify_one();
	});
	t.detach();

	{
		std::chrono::seconds sec(_nTime_Sec);
		std::unique_lock < std::mutex > gaudLock(Mutex);

		if (cv.wait_for(gaudLock,sec) == std::cv_status::timeout)
		{
			bRet = false;
		}
	}

	return bRet;
}
