// complication of CommThread class

#define	COMM	L"Com3"

#include <iostream>
#include "CommThread.h"


CommThread::CommThread()
{
	hComm = INVALID_HANDLE_VALUE;
}

CommThread::~CommThread()
{
}

bool CommThread::CreateCommThread()
{
	CreateThread(NULL, 0, CommThread::ThreadProc, NULL, 0, NULL);
}

DWORD WINAPI CommThread::ThreadProc(LPVOID lpParameter)
{
	// create the handle of serial port
	hComm = CreateFile(COMM, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	
	// check the connection of serial port
	if (hComm == INVALID_HANDLE_VALUE)
	{
		int errorCode = GetLastError();
		std::cout << "Open " << COMM << " failed with error " << errorCode << "." << std::endl;
		return 1;
	}

	// configure the serial port
	SetupComm(hComm, 1024, 1024);
	DCB dcb;
	GetCommState(hComm, &dcb);
	//dcb.BaudRate = CBR_115200;
	//dcb.ByteSize = 8;
	//dcb.Parity = NOPARITY;
	//dcb.StopBits = ONESTOPBIT;
	SetCommState(hComm, &dcb);

	// clear the buffer
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
}