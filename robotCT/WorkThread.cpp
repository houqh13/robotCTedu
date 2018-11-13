// WorkThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "robotCT.h"
#include "WorkThread.h"

#include "define.h"


// CWorkThread

IMPLEMENT_DYNCREATE(CWorkThread, CWinThread)

CWorkThread::CWorkThread()
{
}

CWorkThread::~CWorkThread()
{
}

BOOL CWorkThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	m_hComm = INVALID_HANDLE_VALUE;
	for (int i = 0; i < 2; i++)
	{
		m_bSerialReached[i] = false;
		m_bSocketReached[i] = false;
	}
	m_cSerialSendData[0] = (BYTE)0xFF;
	m_cSerialSendData[1] = (BYTE)0xAA;
	m_cSerialSendData[2] = (BYTE)0x00;
	m_cSerialSendData[8] = (BYTE)0x00;

	// �򿪴���
	m_hComm = CreateFileW(
		L"COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		int error = GetLastError();
		PostMessage(GetMainWnd()->GetSafeHwnd(), WM_ERROR, DEVICE_SERIAL, (LPARAM)error);
		return FALSE;
	}

	// ���ô���
	SetupComm(m_hComm, 256, 256);
	DCB dcb;
	GetCommState(m_hComm, &dcb);
	dcb.BaudRate = CBR_9600;		// ������9600
	dcb.ByteSize = 8;				// ����λ8λ
	dcb.Parity = NOPARITY;			// ����żУ��λ
	dcb.StopBits = ONESTOPBIT;		// ֹͣλ1λ
	SetCommState(m_hComm, &dcb);
	COMMTIMEOUTS timeouts;			// �޳�ʱ����
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(m_hComm, &timeouts);
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	PostMessage(GetMainWnd()->GetSafeHwnd(), WM_CONNECT, DEVICE_SERIAL, NULL);

	// �򿪷����������粢���ӿͻ���
	if (m_socServer.Create(6000) && m_socServer.Listen() == FALSE)
	{
		int error = GetLastError();
		PostMessage(GetMainWnd()->GetSafeHwnd(), WM_ERROR, DEVICE_SERVER, (LPARAM)error);
		return FALSE;
	}
	while (m_socRobot[0].m_hSocket 
		&& m_socRobot[1].m_hSocket && m_socDetector.m_hSocket == NULL)
	{
		CSocket tempsoc;
		SOCKADDR_IN tempaddr;
		int len = sizeof(tempaddr);
		m_socServer.Accept(tempsoc, (SOCKADDR*)&tempaddr, &len);
		switch (tempaddr.sin_addr.S_un.S_addr & 0xFF)
		{
		case 1:
			m_socRobot[0].Attach(tempsoc.Detach());
			while (true)
			{
				int ret = m_socRobot[0].Receive(m_cSocketRecvData, BUFFER_SOCKET_RECEIVE);
				if (ret == SOCKET_ERROR)
				{
					PostMessage(GetMainWnd()->GetSafeHwnd(), 
						WM_ERROR, DEVICE_ROBOT_0, (LPARAM)ret);
					return false;
				}
				else if (strcmp(m_cSocketRecvData, PROTOCAL_CONNECT) == 0)
				{
					PostMessage(GetMainWnd()->GetSafeHwnd(), 
						WM_CONNECT, DEVICE_ROBOT_0, NULL);
				}
			}
			break;
		case 2:
			m_socRobot[1].Attach(tempsoc.Detach());
			while (true)
			{
				int ret = m_socRobot[1].Receive(m_cSocketRecvData, BUFFER_SOCKET_RECEIVE);
				if (ret == SOCKET_ERROR)
				{
					PostMessage(GetMainWnd()->GetSafeHwnd(), 
						WM_ERROR, DEVICE_ROBOT_1, (LPARAM)ret);
					return false;
				}
				else if (strcmp(m_cSocketRecvData, PROTOCAL_CONNECT) == 0)
				{
					PostMessage(GetMainWnd()->GetSafeHwnd(), 
						WM_CONNECT, DEVICE_ROBOT_1, NULL);
				}
			}
			break;
		case 13:
			m_socDetector.Attach(tempsoc.Detach());
			while (true)
			{
				int ret = m_socDetector.Receive(m_cSocketRecvData, BUFFER_SOCKET_RECEIVE);
				if (ret == SOCKET_ERROR)
				{
					PostMessage(GetMainWnd()->GetSafeHwnd(), 
						WM_ERROR, DEVICE_DETECTOR, (LPARAM)ret);
					return false;
				}
				else if (strcmp(m_cSocketRecvData, PROTOCAL_CONNECT) == 0)
				{
					PostMessage(GetMainWnd()->GetSafeHwnd(), 
						WM_CONNECT, DEVICE_DETECTOR, NULL);
				}
			}
			break;
		default:
			break;
		}
	}

	return TRUE;
}

int CWorkThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	CloseHandle(m_hComm);
	m_socServer.Close();
	m_socRobot[0].Close();
	m_socRobot[1].Close();
	m_socDetector.Close();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
END_MESSAGE_MAP()


// CWorkThread ��Ϣ�������
