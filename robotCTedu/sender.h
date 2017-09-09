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
	void serialSend(int i);
	void socketSend(double* data);
	bool isAllReached();
	void serialClose();
	void socketClose();

public:
	int double2String(double* d, char* str, int prec);

private:
	HANDLE hComm;
	SOCKET server;
	SOCKET client[2];
	char recvData[256];
	char* sendData;
	bool isSerialReached[2];
	bool isSocketReached[2];
};