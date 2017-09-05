//

#include <WinSock2.h>


class Sender
{
public:
	Sender();
	~Sender();

public:
	bool serialSetup();
	bool socketSetup();
	bool socketAccept(int i);
	void serialRcv();
	int socketRcv(int i);


	HANDLE hComm;
	SOCKET server;
	SOCKET client[2];
	char recvData[256];
	char sendData[256];
};