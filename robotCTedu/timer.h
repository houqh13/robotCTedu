// 


#include <Windows.h>


class Timer
{
public:
	Timer();
	~Timer();
	void CreateTimerThread(int* pint);
	static DWORD WINAPI ThreadProc(PVOID pvoid);
	static void CALLBACK TimeProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
};