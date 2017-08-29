// 


#include <iostream>
#include "timer.h"


Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::CreateTimerThread(int* pint)
{
	CreateThread(NULL, 0, Timer::ThreadProc, pint, 0, NULL);
}

DWORD WINAPI Timer::ThreadProc(PVOID pvoid)
{
	//int millTime = *(int*)pvoid;
	MSG msg;
	//PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	std::cout << "Thread ends." << std::endl;
	return msg.wParam;
}