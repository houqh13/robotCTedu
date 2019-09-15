#pragma once

#include "define.h"

#include "AsyncClientSocket.h"
#include "AsyncServerSocket.h"

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
	CAsyncServerSocket m_socServer;		// ������(����)�׽��ֶ���
	CAsyncClientSocket m_socRobot[2];	// �ͻ���(��е��)�׽��ֶ���
	CAsyncClientSocket m_socDetector;	// �ͻ���(̽����ƽ��)�׽��ֶ���
	BYTE m_cSerialRecvData[BUFFER_SERIAL_RECEIVE];		// ���ڽ�������
	BYTE m_cSerialSendData[BUFFER_SERIAL_SEND];			// ���ڷ�������
	bool m_bSerialReached[2];			// �����Ƿ����е�λ
	char m_cSocketRecvData[BUFFER_SOCKET_RECEIVE];		// �����������
	char m_cSocketSendData[BUFFER_SOCKET_SEND];			// ���緢������
	bool m_bSocketReached[2];			// ��е���Ƿ����е�λ
	DWORD sendBytes;
	DWORD recvBytes;
	double lastw;						// ǰһ������λ��
	int double2String(POSE_Q* pose, char* str, int precision);
										// (double��)������ת(char*��)�ַ���, �������е�۴�������
	BYTE calcCheckBit(BYTE* data);		// ����У��λ, �����򵼹촫������

public:
	afx_msg void OnThreadTimer(WPARAM wParam, LPARAM lParam);	// ���̶߳�ʱ����Ϣ��Ӧ����
	afx_msg void OnStart(WPARAM wParam, LPARAM lParam);			// ��ʼ�˶���Ϣ��Ӧ����
	afx_msg void OnShow(WPARAM wParam, LPARAM lParam);			// ֪ͨƽ����ʾ��Ϣ��Ӧ����
};


