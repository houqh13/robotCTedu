#pragma once

// CAsyncClientSocket ����Ŀ��

class CAsyncClientSocket : public CAsyncSocket
{
public:
	CAsyncClientSocket();
	virtual ~CAsyncClientSocket();
	virtual void OnReceive(int nErrorCode);

public:
	int deviceID;
	char recvData[BUFFER_SOCKET_RECEIVE];	// �����������
	char sendData[BUFFER_SOCKET_SEND];		// ���緢������
};


