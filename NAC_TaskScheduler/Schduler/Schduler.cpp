// Schduler.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "NAC_TaskScheduler.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

void Print_30sec(void *)
{
	printf("%s\n",__FUNCTION__);
}

void Print_1min(void *)
{
	printf("%s\n",__FUNCTION__);
}

void Print_1min30sec(void *)
{
	printf("%s\n",__FUNCTION__);
}

void Print_2min(void *)
{
	printf("%s\n",__FUNCTION__);
}

int main()
{
	NAC_TaskScheduler shceduler;
	shceduler.AddTask_Repeat(Print_30sec, 30);  
	shceduler.AddTask_Repeat(Print_1min, 60);
	shceduler.AddTask_Repeat(Print_1min30sec, 90);
	shceduler.AddTask_Repeat(Print_2min, 120);

	shceduler.Start();

	int nCount = 0;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
		//std::cout << ++nCount << "=> " << std::put_time(std::localtime(&now_c), "%F %T") << '\n';
	}

	//while (1)
	//{
	//	int a = nCount % 10;

	//	if (0 == nCount%1)
	//		Print_1sec(nullptr);

	//	if (9 == nCount%10)
	//		Print_10sec(nullptr);

	//	if (29 == nCount%30)
	//		Print_30sec(nullptr);

	//	if (59 == nCount%60)
	//		Print_60sec(nullptr);

	//	std::this_thread::sleep_for(std::chrono::seconds(1));

	//	nCount++;
	//}

    return 0;
}

