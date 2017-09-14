//

#include <iostream>
#include "controller.h"


// guide & motor parameters
#define LEAD			5
#define	DIVIDE_2		2
#define DIVIDE_4		4
#define DIVIDE_8		8
#define DIVIDE_16		16
#define STEPPR_50		50		// step angle 7.2
#define STEPPR_100		100		// step angle 3.6
#define STEPPR_200		200		// step angle 1.8
#define STEPPR_400		400		// step angle 0.9


// construction & destruction

Controller::Controller()
	: hComm(INVALID_HANDLE_VALUE)
	, server(INVALID_SOCKET)
	, lastw(0)
{
	for (int i = 0; i < 2; i++)
	{
		client[i] = INVALID_SOCKET;
		isSerialReached[i] = false;
		isSocketReached[i] = false;
	}
	serialSendData[0] = (BYTE)0xFF;
	serialSendData[1] = (BYTE)0xAA;
	serialSendData[2] = (BYTE)0x00;
	serialSendData[8] = (BYTE)0x00;
}

Controller::~Controller()
{
	CloseHandle(hComm);
	closesocket(server);
	closesocket(client[0]);
	closesocket(client[1]);
}


// communication

bool Controller::serialSetup()
{
	// create the handle of serial port
	hComm = CreateFile(L"COM NO.", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hComm == INVALID_HANDLE_VALUE)
	{
		int error = GetLastError();
		printf("Open Comm failed with error %d.\n", error);
		return false;
	}
	printf("Open Comm succeed!\n");

	// configure the serial port
	SetupComm(hComm, 256, 256);
	DCB dcb;
	GetCommState(hComm, &dcb);
	dcb.BaudRate = CBR_9600;		// baud rate 9600
	dcb.ByteSize = 8;				// 8 bits for one data byte
	dcb.Parity = NOPARITY;			// no parity bit
	dcb.StopBits = ONESTOPBIT;		// one stop bit
	SetCommState(hComm, &dcb);
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);

	return true;
}

bool Controller::socketSetup()
{
	// load Winsock library
	WSADATA wsaData;
	int error = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (error != 0)
	{
		printf("WSAStartup failed: %d.\n", error);
		return false;
	}
	printf("WSAStartup succeed!\n");

	// open the socket
	server = socket(AF_INET, SOCK_STREAM, 0);

	// bind local address with the socket
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr("input ip address here");	// ip address
	addr.sin_family = AF_INET;		// windows only
	addr.sin_port = htons(6000);	// port number
	bind(server, (SOCKADDR*)&addr, sizeof(SOCKADDR));

	// listen the connection
	if (error = listen(server, 5))
	{
		printf("Listen failed: %d.\n", error);
		return false;
	}

	printf("Server socket setup complete!\n");
	printf("Waiting for client connecting...\n");
	return true;
}

bool Controller::socketAccept()
{
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	for (int i = 0; i < 2; i++)
	{
		// accept the link request from the client
		client[i] = accept(server, (SOCKADDR*)&addr, &addrlen);
		printf("Client[%d] accepts a connection, ip address: %s\n", i, inet_ntoa(addr.sin_addr));

		// receive the setup complete message from the client
		while (true)
		{
			int ret = recv(client[i], socketRecvData, 256, 0);
			if (ret == SOCKET_ERROR)
			{
				printf("Receiving data from client[%d] failed!\n", i);
				Sleep(500);
			}
			else if (ret > 0)
			{
				socketRecvData[ret] = 0x00;
				printf("Data from client[%d] : %s\n", i, socketRecvData);
				if (socketRecvData[0] == 'a' && socketRecvData[1] == '0')
				{
					printf("Robot[%d] setup complete!", i);
					break;
				}
			}
		}
	}
	return true;
}

void Controller::serialSend(double w)
{
	double distance = w - lastw;
	
	// set running distance
	double temp = abs(distance);
	int pulse = int(temp * STEPPR_100 * DIVIDE_8 / LEAD);
	serialSendData[4] = (BYTE)0x03;
	serialSendData[5] = (BYTE)pulse;
	serialSendData[6] = (BYTE)(pulse / 256);
	serialSendData[7] = (BYTE)(pulse / 65536);
	for (int i = 1; i < 3; i++)
	{
		serialSendData[3] = (BYTE)i;
		serialSendData[9] = calcCheckBit(serialSendData);
		WriteFile(hComm, &serialSendData, 10, &sendBytes, NULL);
		while (ReadFile(hComm, serialRecvData, 256, &recvBytes, NULL))
		{
			if (serialRecvData[0] == (BYTE)0xFF && serialRecvData[1] == (BYTE)0xAA
				&& serialRecvData[2] == (BYTE)0x00 && serialRecvData[4] == (BYTE)0x03
				&& serialRecvData[5] == (BYTE)0x00 && serialRecvData[6] == (BYTE)0x00)
			{
				printf("Pulse number for guide %d is %d.\n", (int)serialSendData[3], pulse);
				break;
			}
		}
	}
	
	// set move direction, guide 2 is opposite of guide 1
	serialSendData[4] = (BYTE)0x04;
	serialSendData[6] = (BYTE)0x32;		// start frequency 50Hz
	serialSendData[7] = (BYTE)0x00;
	for (int i = 1; i < 3; i++)
	{
		serialSendData[3] = (BYTE)i;
		if (distance >= 0)
		{
			serialSendData[5] = (BYTE)((i + 1) % 2);		// 0 for forward, 1 for backward
		}
		else
		{
			serialSendData[5] = (BYTE)(i % 2);
		}
		serialSendData[9] = calcCheckBit(serialSendData);
		WriteFile(hComm, &serialSendData, 10, &sendBytes, NULL);
		while (ReadFile(hComm, serialRecvData, 256, &recvBytes, NULL))
		{
			if (serialRecvData[0] == (BYTE)0xFF && serialRecvData[1] == (BYTE)0xAA
				&& serialRecvData[2] == (BYTE)0x00 && serialRecvData[4] == (BYTE)0x04
				&& serialRecvData[6] == (BYTE)0x00)
			{
				if (serialSendData[5] == (BYTE)0x00)
				{
					printf("Guide %d will move forward.\n", (int)serialSendData[3]);
				}
				else
				{
					printf("Guide %d will move backward.\n", (int)serialSendData[3]);
				}
				break;
			}
		}
	}

	// start moving
	serialSendData[3] = (BYTE)0x09;
	serialSendData[4] = (BYTE)0x09;
	serialSendData[5] = (BYTE)0x00;
	serialSendData[6] = (BYTE)0x00;
	serialSendData[7] = (BYTE)0x00;
	serialSendData[9] = calcCheckBit(serialSendData);
	WriteFile(hComm, &serialSendData, 10, &sendBytes, NULL);
	while (ReadFile(hComm, serialRecvData, 256, &recvBytes, NULL))
	{
		if (serialRecvData[0] == (BYTE)0xFF && serialRecvData[1] == (BYTE)0xAA
			&& serialRecvData[2] == (BYTE)0x00)
		{
			if (serialRecvData[3] == (BYTE)0x09 && serialRecvData[4] == (BYTE)0x09
				&& serialRecvData[5] == (BYTE)0x00 && serialRecvData[6] == (BYTE)0x00)
			{
				printf("All guide start moving.\n");
				break;
			}
		}
	}
}

void Controller::socketSend(double* data)
{
	char* flag = "A\r";
	int len = double2String(data, socketSendData, 8);
	for (int i = 0; i < 2; i++)
	{
		int byte;
		byte = send(client[i], flag, 2, 0);		// flag
		printf("send %d bytes\n", byte);
		Sleep(20);
		byte = send(client[i], socketSendData, len, 0);		// data
		printf("send %d bytes\n", byte);
	}
}

bool Controller::serialReached()
{
	while (ReadFile(hComm, serialRecvData, 256, &recvBytes, NULL))
	{
		if (serialRecvData[0] == (BYTE)0xFF && serialRecvData[1] == (BYTE)0xAA
			&& serialRecvData[2] == (BYTE)0x00 && serialRecvData[4] == (BYTE)0x09
			&& serialRecvData[5] == (BYTE)0x01 && serialRecvData[6] == (BYTE)0x00)
		{
			printf("Guide %d finished moving!\n", (int)serialSendData[3]);
			isSerialReached[(int)serialSendData[3]] = true;
			if (isSerialReached[0] && isSerialReached[1])
			{
				isSerialReached[0] = false;
				isSerialReached[1] = false;
				break;
			}
		}
	}
	return true;
}

bool Controller::socketReached()
{
	for (int i = 0; i < 2; i++)
	{
		// receive the move complete message from the client
		while (true)
		{
			int ret = recv(client[i], socketRecvData, 256, 0);
			if (ret == SOCKET_ERROR)
			{
				printf("Receiving data from client[%d] failed!\n", i);
				Sleep(500);
			}
			else if (ret > 0)
			{
				socketRecvData[ret] = 0x00;
				printf("Data from client[%d] : %s\n", i, socketRecvData);
				if (socketRecvData[0] == 'a' && socketRecvData[1] == '1')
				{
					printf("Robot[%d] finished moving!", i);
					break;
				}
			}
		}
	}
	return true;
}


// necessary algorithm

int Controller::double2String(double* d, char* str, int prec)
{
	int len = 0;
	for (int i = 0; i < 7; i++)
	{
		double divide = 1;

		// negative number
		if (d[i] < 0)
		{
			str[len++] = '-';
			d[i] = 0 - d[i];
		}

		// get the highest digit of the number
		while (d[i] / divide > 10)
		{
			divide *= 10;
		}

		bool first = false;
		while (prec)
		{
			int number = int(d[i] / divide);
			if (!first && number)
			{
				first = true;		// first effective number
			}
			if (first)
			{
				prec--;
			}
			str[len++] = number + 48;
			d[i] -= number * divide;
			if (divide == 1.0 && prec)
			{
				str[len++] = '.';		// decimal point
			}
			divide /= 10;
		}

		// when digit of the number is bigger than precision we want, fill zero in low digit
		while (divide >= 1)
		{
			str[len++] = '0';
			divide /= 10;
		}

		str[len++] = ',';		// seperator between two numbers
	}
	str[len - 1] = '\r';		// flag for the end
	str[len] = '\0';
	printf(str);
	return len;
}

BYTE Controller::calcCheckBit(BYTE* data)
{
	BYTE sum = (BYTE)0;
	for (int i = 0; i < 9; i++)
	{
		sum += data[i];
	}
	return sum;
}