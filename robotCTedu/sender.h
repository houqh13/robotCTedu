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
	void serialRcv();
	void socketRcv();


	HANDLE hComm;
	SOCKET sock;
};