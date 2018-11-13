#pragma once

#include "define.h"


// CWorkThread

class CWorkThread : public CWinThread
{
	DECLARE_DYNCREATE(CWorkThread)

protected:
	CWorkThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CWorkThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	HANDLE m_hComm;						// ����ͨ��(����)���
	BYTE m_cSerialRecvData[BUFFER_SERIAL_RECEIVE];		// ���ڽ�������
	BYTE m_cSerialSendData[BUFFER_SERIAL_SEND];			// ���ڷ�������
	bool m_bSerialReached[2];			// �����Ƿ����е�λ
	CSocket m_socServer;				// ������(����)�׽��ֶ���
	CSocket m_socRobot[2];				// �ͻ���(��е��)�׽��ֶ���
	CSocket m_socDetector;				// �ͻ���(̽����ƽ��)�׽��ֶ���
	char m_cSocketRecvData[BUFFER_SOCKET_RECEIVE];	// �����������
	char m_cSocketSendData[BUFFER_SOCKET_SEND];		// ���緢������
	bool m_bSocketReached[2];			// ��е���Ƿ����е�λ
	double lastw;
	DWORD sendBytes;
	DWORD recvBytes;
};


