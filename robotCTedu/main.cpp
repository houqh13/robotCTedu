#include <iostream>
#include <Windows.h>
using namespace std;

#define	COMM	L"Com3"

int main()
{
	// create the handle of serial port
	HANDLE hComm = INVALID_HANDLE_VALUE;
	hComm = CreateFile(COMM, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	
	// check the connection of serial port
	if (hComm == INVALID_HANDLE_VALUE)
	{
		int errorCode = GetLastError();
		cout << "Open " << COMM << " failed with error " << errorCode << "." << endl;
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


	return 0;
}