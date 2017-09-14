#include <iostream>
#include <Windows.h>

using namespace std;

BYTE calcCheckBit(BYTE* data)
{
	BYTE sum = (BYTE)0;
	for (int i = 0; i < 9; i++)
	{
		sum += data[i];
	}
	return sum;
}

int main()
{
	HANDLE hComm = INVALID_HANDLE_VALUE;
	BYTE readBuffer[32];
	BYTE writeBuffer[10];
	DWORD readBytes;
	DWORD writeBytes;

	writeBuffer[0] = (BYTE)0xFF;
	writeBuffer[1] = (BYTE)0xAA;
	writeBuffer[2] = (BYTE)0x00;
	writeBuffer[8] = (BYTE)0x00;

	hComm = CreateFile(L"COM2", GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hComm == INVALID_HANDLE_VALUE)
	{
		int errorCode = GetLastError();
		printf("Open Com failed with error %d.\n", errorCode);
		return -1;
	}
	else
	{
		printf("Open Com succeed!\n");
	}

	SetupComm(hComm, 1024, 1024);
	DCB dcb;
	GetCommState(hComm, &dcb);
	dcb.BaudRate = CBR_9600;      	// 波特率
	dcb.ByteSize = 8;				// 数据位
	dcb.Parity = NOPARITY;			// 校验位
	dcb.StopBits = ONESTOPBIT;		// 停止位
	SetCommState(hComm, &dcb);
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);

	// set divide and step
	writeBuffer[3] = (BYTE)0x01;
	writeBuffer[5] = (BYTE)0x08;		// divide = 8
	writeBuffer[6] = (BYTE)0x00;
	writeBuffer[7] = (BYTE)0xB4;		// step = 1.8 degree
	for (int i = 1; i < 3; i++)
	{
		writeBuffer[4] = (BYTE)i;
		writeBuffer[9] = calcCheckBit(writeBuffer);
		WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
		while (ReadFile(hComm, readBuffer, 32, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[2] == (BYTE)0x00 && readBuffer[4] == (BYTE)0x01
				&& readBuffer[5] == (BYTE)0x00 && readBuffer[6] == (BYTE)0x00)
			{
				printf("Set divide & step for guide %d succeed!\n", (int)readBuffer[3]);
				break;
			}
		}
	}

	CloseHandle(hComm);
	return 0;
}