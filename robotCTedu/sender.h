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
	bool socketAccept();
	void serialRcv();
	int socketRcv(int i);
	bool isAllReached();
	void serialClose();
	void socketClose();

private:
	HANDLE hComm;
	SOCKET server;
	SOCKET client[2];
	char recvData[256];
	char sendData[256];
	bool isSerialReached[2];
	bool isSocketReached[2];
};