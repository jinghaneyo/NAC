// NAC_TaskScheduler.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <chrono>
#include <ctime>
#include "NAC/TaskScheduler/NAC_Scheduler.h"

int PrintInt(int nTest)
{
	std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
	time_t tNow = std::chrono::system_clock::to_time_t(timeNow);

	printf("%I64d [%s] %d\n", tNow, __FUNCTION__, nTest );

	return nTest;
}

std::string PrintStr( std::string strTest )
{
	std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
	time_t tNow = std::chrono::system_clock::to_time_t(timeNow);

	printf("%I64d [%s] %s\n", tNow, __FUNCTION__, strTest.c_str());

	return strTest;
}

int main()
{
	NAC_Scheduler nac;
	nac.AddEvent_Task(0, 1, std::bind(PrintInt, 1), "1");
	nac.AddEvent_Task(0, 3, std::bind(PrintStr, "test"), "2");

	nac.Start(true);

	//std::chrono::seconds sec(1);
	//while (true)
	//{
	//	std::this_thread::sleep_for(sec);
	//}

    return 0;
}

