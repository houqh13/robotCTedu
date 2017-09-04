//

#include <iostream>
#include "sender.h"


Sender::Sender()
	: hComm(INVALID_HANDLE_VALUE)
	, sock(INVALID_SOCKET)
{
}

Sender::~Sender()
{
}

bool Sender::serialSetup()
{
	//
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
	printf("WSAStartup success!\n", error);

	// open the socket
	sock = socket(AF_INET, SOCK_STREAM, 0);

	// bind local address with the socket
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr("input ip address here");	// ip address
	addr.sin_family = AF_INET;		// windows only
	addr.sin_port = htons(6000);	// port number
	bind(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR));

	// listen the connection
	listen(sock, 2);

	return true;
}

void Sender::serialRcv()
{
	//
}

void Sender::socketRcv()
{
	//
}