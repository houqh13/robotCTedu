//

#include <iostream>
#include "sender.h"


Sender::Sender()
	: hComm(INVALID_HANDLE_VALUE)
	, server(INVALID_SOCKET)
{
	for (int i = 0; i < 2; i++)
	{
		client[i] = INVALID_SOCKET;
		isSerialReached[i] = false;
		isSocketReached[i] = false;
	}
}

Sender::~Sender()
{
}

bool Sender::serialSetup()
{
	//
	return true;
}

bool Sender::socketSetup()
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

bool Sender::socketAccept()
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
			int ret = recv(client[i], recvData, 256, 0);
			if (ret > 0)
			{
				recvData[ret] = 0x00;
				printf("Data from client[%d] : %s\n", i, recvData);
				if (recvData[0] == 'A' && recvData[1] == '0')
				{
					printf("Robot[%d] setup complete!", i);
					break;
				}
			}
		}
	}
	return true;
}

void Sender::serialRcv()
{
	//
}

void Sender::serialSend(int cmd)
{
	//
}

void Sender::socketSend(double* data)
{
	char* flag = "A\r";
	int len = double2String(data, sendData, 8);
	for (int i = 0; i < 2; i++)
	{
		int byte;
		byte = send(client[i], flag, 2, 0);		// flag
		printf("send %d bytes\n", byte);
		Sleep(20);
		byte = send(client[i], sendData, len, 0);		// data
		printf("send %d bytes\n", byte);
	}
}

bool Sender::isAllReached()
{
	for (int i = 0; i < 2; i++)
	{
		if (true)
		{
			// serialRcv...
		}
		if (recv(client[i], recvData, 256, 0))
		{
			if (recvData[0] == 'A' && recvData[1] == '1')
			{
				isSocketReached[i] = true;
			}
		}
	}
	if (isSerialReached[0] && isSerialReached[1] && isSocketReached[0] && isSocketReached[1])
	{
		for (int i = 0; i < 2; i++)
		{
			isSerialReached[i] = false;
			isSocketReached[i] = false;
		}
		return true;
	}
	return false;
}

int Sender::double2String(double* d, char* str, int prec)
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