// class for the communication thread

#pragma once

//#include <thread>
#include <Windows.h>

class CommThread
{
public:
	CommThread();
	~CommThread();

private:
	static HANDLE hComm;

public:
	bool CreateCommThread();		// create the thread
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);		// process function of the thread

};