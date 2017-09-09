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
	printf("WSAStartup success!\n");

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
		client[i] = accept(server, (SOCKADDR*)&addr, &addrlen);
		printf("Client[%d] accepts a connection, ip address: %s\n", i, inet_ntoa(addr.sin_addr));

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
	/*client[i] = accept(server, (SOCKADDR*)&addr, &addrlen);
	if (client[i] == INVALID_SOCKET)
	{
		printf("Client[%d] accept error!\n", i);
		return false;
	}
	printf("Client[%d] accepts a connection, ip address: %s\n", i, inet_ntoa(addr.sin_addr));*/
	return true;
}

void Sender::serialRcv()
{
	//
}

int Sender::socketRcv(int i)
{
	return recv(client[i], recvData, 256, 0);
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