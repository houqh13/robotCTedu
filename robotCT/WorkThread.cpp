// WorkThread.cpp : 实现文件
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
	// TODO: 在此执行任意逐线程初始化
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

	// 打开串口
	m_hComm = CreateFileW(
		L"COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		int error = GetLastError();
		PostMessage(GetMainWnd()->GetSafeHwnd(), WM_ERROR, DEVICE_SERIAL, (LPARAM)error);
		return FALSE;
	}

	// 配置串口
	SetupComm(m_hComm, 256, 256);
	DCB dcb;
	GetCommState(m_hComm, &dcb);
	dcb.BaudRate = CBR_9600;		// 波特率9600
	dcb.ByteSize = 8;				// 数据位8位
	dcb.Parity = NOPARITY;			// 无奇偶校验位
	dcb.StopBits = ONESTOPBIT;		// 停止位1位
	SetCommState(m_hComm, &dcb);
	COMMTIMEOUTS timeouts;			// 无超时设置
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(m_hComm, &timeouts);
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	PostMessage(GetMainWnd()->GetSafeHwnd(), WM_CONNECT, DEVICE_SERIAL, NULL);

	// 打开服务器端网络并连接客户端
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
	// TODO: 在此执行任意逐线程清理
	CloseHandle(m_hComm);
	m_socServer.Close();
	m_socRobot[0].Close();
	m_socRobot[1].Close();
	m_socDetector.Close();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
END_MESSAGE_MAP()


// CWorkThread 消息处理程序
