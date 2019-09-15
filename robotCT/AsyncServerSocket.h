#pragma once

// CAsyncServerSocket ÃüÁîÄ¿±ê

class CAsyncServerSocket : public CAsyncSocket
{
public:
	CAsyncServerSocket();
	virtual ~CAsyncServerSocket();
	virtual void OnAccept(int nErrorCode);
};


