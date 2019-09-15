#pragma once

// CAsyncClientSocket 命令目标

class CAsyncClientSocket : public CAsyncSocket
{
public:
	CAsyncClientSocket();
	virtual ~CAsyncClientSocket();
	virtual void OnReceive(int nErrorCode);

public:
	int deviceID;
	char recvData[BUFFER_SOCKET_RECEIVE];	// 网络接收数据
	char sendData[BUFFER_SOCKET_SEND];		// 网络发送数据
};


