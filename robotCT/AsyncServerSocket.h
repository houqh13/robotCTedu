#pragma once

// CAsyncServerSocket ����Ŀ��

class CAsyncServerSocket : public CAsyncSocket
{
public:
	CAsyncServerSocket();
	virtual ~CAsyncServerSocket();
	virtual void OnAccept(int nErrorCode);
};


