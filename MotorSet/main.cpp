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
	BYTE readBuffer[7];
	BYTE writeBuffer[10];
	DWORD readBytes;
	DWORD writeBytes;

	writeBuffer[0] = (BYTE)0xFF;
	writeBuffer[1] = (BYTE)0xAA;
	writeBuffer[2] = (BYTE)0x00;
	writeBuffer[8] = (BYTE)0x00;

	hComm = CreateFile(L"COM1", GENERIC_READ | GENERIC_WRITE,
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
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(hComm, &timeouts);
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	
	for (int i = 1; i < 3; i++)
	{
		// set divide and step
		writeBuffer[3] = (BYTE)i;
		writeBuffer[4] = (BYTE)0x01;
		writeBuffer[5] = (BYTE)0x08;		// divide = 8
		writeBuffer[6] = (BYTE)0x00;
		writeBuffer[7] = (BYTE)0xB4;		// step = 1.8 degree
		writeBuffer[9] = calcCheckBit(writeBuffer);
		WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[2] == (BYTE)0x00 && readBuffer[4] == (BYTE)0x01
				&& readBuffer[5] == (BYTE)0x00 && readBuffer[6] == (BYTE)0x00)
			{
				printf("Set divide & step for guide %d succeed!\n", (int)readBuffer[3]);
				readBuffer[0] = 0xCC;
				break;
			}
		}

		// set pulse number per round
		writeBuffer[3] = (BYTE)i;
		writeBuffer[4] = (BYTE)0x02;
		writeBuffer[5] = (BYTE)0x40;		// 1600 pulses per round
		writeBuffer[6] = (BYTE)0x06;
		writeBuffer[7] = (BYTE)0x00;
		writeBuffer[9] = calcCheckBit(writeBuffer);
		WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[2] == (BYTE)0x00 && readBuffer[4] == (BYTE)0x02
				&& readBuffer[5] == (BYTE)0x00 && readBuffer[6] == (BYTE)0x00)
			{
				printf("Set pulse number per round for guide %d succeed!\n", (int)readBuffer[3]);
				readBuffer[0] = 0xCC;
				break;
			}
		}

		// set direction & start frequency
		writeBuffer[3] = (BYTE)i;
		writeBuffer[4] = (BYTE)0x04;
		writeBuffer[5] = (BYTE)(i % 2);		// 0 for moving forward, 1 for moving backward
		writeBuffer[6] = (BYTE)0x28;		// start frequency = 40 Hz
		writeBuffer[7] = (BYTE)0x00;
		writeBuffer[9] = calcCheckBit(writeBuffer);
		WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[2] == (BYTE)0x00 && readBuffer[4] == (BYTE)0x04
				&& readBuffer[5] == (BYTE)0x00 && readBuffer[6] == (BYTE)0x00)
			{
				printf("Set direction & start frequency for guide %d succeed!\n", (int)readBuffer[3]);
				readBuffer[0] = 0xCC;
				break;
			}
		}

		// set acceleration & max speed
		writeBuffer[3] = (BYTE)i;
		writeBuffer[4] = (BYTE)0x05;
		writeBuffer[5] = (BYTE)0x14;		// start frequancy = 20 Hz
		writeBuffer[6] = (BYTE)0x00;
		writeBuffer[7] = (BYTE)0x78;		// max speed = 120 rpm
		writeBuffer[9] = calcCheckBit(writeBuffer);
		WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[2] == (BYTE)0x00 && readBuffer[4] == (BYTE)0x05
				&& readBuffer[5] == (BYTE)0x00 && readBuffer[6] == (BYTE)0x00)
			{
				printf("Set acceleration & max speed for guide %d succeed!\n", (int)readBuffer[3]);
				readBuffer[0] = 0xCC;
				break;
			}
		}

		// set reply for finishing moving
		writeBuffer[3] = (BYTE)i;
		writeBuffer[4] = (BYTE)0x0D;
		writeBuffer[5] = (BYTE)0x01;		// 0 for no reply, 1 for having reply
		writeBuffer[6] = (BYTE)0x00;
		writeBuffer[7] = (BYTE)0x00;
		writeBuffer[9] = calcCheckBit(writeBuffer);
		WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[2] == (BYTE)0x00 && readBuffer[4] == (BYTE)0x0D
				&& readBuffer[5] == (BYTE)0x00 && readBuffer[6] == (BYTE)0x00)
			{
				printf("Set finishing moving reply for guide %d succeed!\n", (int)readBuffer[3]);
				readBuffer[0] = 0xCC;
				break;
			}
		}
	}

	// save the settings
	writeBuffer[3] = (BYTE)0xBC;
	writeBuffer[4] = (BYTE)0x00;
	writeBuffer[5] = (BYTE)0x00;
	writeBuffer[6] = (BYTE)0x00;
	writeBuffer[7] = (BYTE)0x00;
	writeBuffer[9] = (BYTE)0x65;
	WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
	while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
	{
		if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
			&& readBuffer[2] == (BYTE)0x00 && readBuffer[3] == (BYTE)0xBC
			&& readBuffer[4] == (BYTE)0x00 && readBuffer[5] == (BYTE)0x00
			&& readBuffer[6] == (BYTE)0x00)
		{
			printf("Settings saved!\n");
			readBuffer[0] = 0xCC;
			break;
		}
	}

	// check the settings
	writeBuffer[5] = (BYTE)0x40;
	writeBuffer[6] = (BYTE)0x36;
	writeBuffer[7] = (BYTE)0x00;
	for (int i = 1; i < 3; i++)
	{
		writeBuffer[3] = (BYTE)i;
		writeBuffer[4] = (BYTE)((i % 2) * 0x10 + 0x1F);
		writeBuffer[9] = calcCheckBit(writeBuffer);
		WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[2] == (BYTE)0x00 && readBuffer[4] == writeBuffer[4]
				&& readBuffer[5] == (BYTE)0x00 && readBuffer[6] == (BYTE)0x00)
			{
				printf("Guide %d starts moving!\n", (int)readBuffer[3]);
				readBuffer[0] = 0xCC;
				break;
			}
		}
	}
	for (int i = 1; i < 3; i++)
	{
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[3] == (BYTE)0x3F)
			{
				printf("Guide %d completes moving! Total pulse number is %d.\n", (int)readBuffer[2],
					(int)readBuffer[4] + 256 * (int)readBuffer[5] + 65536 * (int)readBuffer[6]);
				readBuffer[0] = 0xCC;
				break;
			}
		}
	}
	// move back
	for (int i = 1; i < 3; i++)
	{
		writeBuffer[3] = (BYTE)i;
		writeBuffer[4] = (BYTE)(((i + 1) % 2) * 0x10 + 0x1F);
		writeBuffer[9] = calcCheckBit(writeBuffer);
		WriteFile(hComm, &writeBuffer, 10, &writeBytes, NULL);
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[2] == (BYTE)0x00 && readBuffer[4] == writeBuffer[4]
				&& readBuffer[5] == (BYTE)0x00 && readBuffer[6] == (BYTE)0x00)
			{
				printf("Guide %d starts moving!\n", (int)readBuffer[3]);
				readBuffer[0] = 0xCC;
				break;
			}
		}
	}
	for (int i = 1; i < 3; i++)
	{
		while (ReadFile(hComm, readBuffer, 7, &readBytes, NULL))
		{
			if (readBuffer[0] == (BYTE)0xFF && readBuffer[1] == (BYTE)0xAA
				&& readBuffer[3] == (BYTE)0x3F)
			{
				printf("Guide %d completes moving! Total pulse number is %d.\n", (int)readBuffer[2],
					(int)readBuffer[4] + 256 * (int)readBuffer[5] + 65536 * (int)readBuffer[6]);
				readBuffer[0] = 0xCC;
				break;
			}
		}
	}

	printf("All settings are successfully saved!\n");

	CloseHandle(hComm);
	return 0;
}