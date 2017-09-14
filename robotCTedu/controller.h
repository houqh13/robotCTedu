//

#include <WinSock2.h>


class Controller
{
public:
	Controller();
	~Controller();

public:
	bool serialSetup();
	bool socketSetup();
	bool socketAccept();
	void serialSend(double w);
	void socketSend(double* data);
	bool serialReached();
	bool socketReached();
	void serialClose();
	void socketClose();

public:
	int double2String(double* d, char* str, int prec);
	BYTE calcCheckBit(BYTE* data);

private:
	HANDLE hComm;
	SOCKET server;
	SOCKET client[2];
	BYTE serialRecvData[256];
	BYTE serialSendData[10];
	char socketRecvData[256];
	char* socketSendData;
	bool isSerialReached[2];
	bool isSocketReached[2];
	double lastw;
	DWORD sendBytes;
	DWORD recvBytes;
};