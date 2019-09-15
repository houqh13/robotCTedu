// WorkThread.cpp : 实现文件
//

#include "stdafx.h"
#include "robotCT.h"
#include "WorkThread.h"


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
	lastw = 0;

	// 打开串口
	m_hComm = CreateFile(
		L"COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		int error = GetLastError();
		PostMessage(GetMainWnd()->GetSafeHwnd(), WM_ERROR, (WPARAM)DEVICE_SERIAL, (LPARAM)error);
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
	PostMessage(GetMainWnd()->GetSafeHwnd(), WM_CONNECT, (WPARAM)DEVICE_SERIAL, NULL);

	// 打开服务器端网络并等待连接
	if (!AfxSocketInit())
	{
		PostMessage(GetMainWnd()->GetSafeHwnd(), WM_ERROR, (WPARAM)SOCKET_INITIAL, (LPARAM)GetLastError());
		return FALSE;
	}
	if (!m_socServer.Create(SERVER_PORT, SOCK_STREAM, FD_ACCEPT) || !m_socServer.Listen())
	{
		PostMessage(GetMainWnd()->GetSafeHwnd(), WM_ERROR, (WPARAM)DEVICE_SERVER, (LPARAM)GetLastError());
		m_socServer.Close();
		return FALSE;
	}
	/*while (m_socRobot[0].m_hSocket && m_socRobot[1].m_hSocket && m_socDetector.m_hSocket == NULL)
	{
		CAsyncSocket tempsoc;
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
	}*/

	return TRUE;
}

int CWorkThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	CloseHandle(m_hComm);
	m_socRobot[0].Close();
	m_socRobot[1].Close();
	m_socDetector.Close();
	m_socServer.Close();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
	ON_THREAD_MESSAGE(WM_THREADTIMER, OnThreadTimer)
	ON_THREAD_MESSAGE(WM_START, OnStart)
	ON_THREAD_MESSAGE(WM_SHOW, OnShow)
END_MESSAGE_MAP()


// CWorkThread 消息处理程序

void CWorkThread::OnThreadTimer(WPARAM wParam, LPARAM lParam)
{
	ReadFile(m_hComm, &m_cSerialRecvData, BUFFER_SERIAL_RECEIVE, &recvBytes, NULL);
	if (m_cSerialRecvData[0] == (BYTE)0xFF && m_cSerialRecvData[1] == (BYTE)0xAA && m_cSerialRecvData[3] == (BYTE)0x3F)
	{
		m_bSerialReached[(int)m_cSerialRecvData[2] - 1] = true;
	}
	memset(m_cSerialRecvData, 0, BUFFER_SERIAL_RECEIVE);
	if (m_bSerialReached[0] && m_bSerialReached[1] && m_bSocketReached[0] && m_bSocketReached[1])
	{
		PostMessage(GetMainWnd()->GetSafeHwnd(), WM_REACH, NULL, NULL);
		m_bSerialReached[0] = false;
		m_bSerialReached[1] = false;
		m_bSocketReached[0] = false;
		m_bSocketReached[1] = false;
	}
}


void CWorkThread::OnStart(WPARAM wParam, LPARAM lParam)
{
	POSE_Q* pose = (POSE_Q*)wParam;

	// 导轨运动通过串口发送
	double distance = pose->w - lastw;
	lastw = pose->w;
	if (distance == 0)
	{
		m_bSerialReached[0] = true;
		m_bSerialReached[1] = true;
	}
	else
	{
		int pulse = int(abs(distance) * STEPPR_200 * DIVIDE_8 / LEAD);
		m_cSerialSendData[5] = (BYTE)pulse;
		m_cSerialSendData[6] = (BYTE)(pulse / 256);
		m_cSerialSendData[7] = (BYTE)(pulse / 65536);
		// 导轨 1
		m_cSerialSendData[3] = (BYTE)1;
		if (distance > 0)
		{
			m_cSerialSendData[4] = (BYTE)0x2F;
		}
		else
		{
			m_cSerialSendData[4] = (BYTE)0x1F;
		}
		m_cSerialSendData[9] = calcCheckBit(m_cSerialSendData);
		WriteFile(m_hComm, &m_cSerialSendData, BUFFER_SERIAL_SEND, &sendBytes, NULL);
		// 导轨 2
		m_cSerialSendData[3] = (BYTE)2;
		if (distance > 0)
		{
			m_cSerialSendData[4] = (BYTE)0x1F;
		}
		else
		{
			m_cSerialSendData[4] = (BYTE)0x2F;
		}
		m_cSerialSendData[9] = calcCheckBit(m_cSerialSendData);
		WriteFile(m_hComm, &m_cSerialSendData, BUFFER_SERIAL_SEND, &sendBytes, NULL);
	}

	// 机械臂运动通过网络发送
	int len = double2String(pose, m_cSocketSendData, PRECISION_6);
	m_socRobot[0].Send(PROTOCAL_ROBOT_MOVE, strlen(PROTOCAL_ROBOT_MOVE));
	m_socRobot[0].Send(m_cSocketSendData, len);
	m_socRobot[0].Send(PROTOCAL_ROBOT_MOVE, strlen(PROTOCAL_ROBOT_MOVE));
	m_socRobot[1].Send(m_cSocketSendData, len);
}


void CWorkThread::OnShow(WPARAM wParam, LPARAM lParam)
{
	char s[4];
	sprintf_s(s, sizeof(s), "%d", (int)wParam);
	m_socDetector.Send(PROTOCAL_DETECTOR_SHOW, strlen(PROTOCAL_DETECTOR_SHOW));
	m_socDetector.Send(s, strlen(s));
}


// CWorkThread 计算程序

int CWorkThread::double2String(POSE_Q* pose, char* str, int precision)
{
	char temp[256];
	_gcvt_s(str, BUFFER_SOCKET_SEND, pose->x, precision);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->y, precision);
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->z, precision);
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->q0, precision);
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->q1, precision);
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->q2, precision);
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->q3, precision);
	strcat_s(str, BUFFER_SOCKET_SEND, temp);

	return strlen(str);
}


BYTE CWorkThread::calcCheckBit(BYTE* data)
{
	BYTE sum = (BYTE)0;
	for (int i = 0; i < BUFFER_SERIAL_SEND - 1; i++)
	{
		sum += data[i];
	}
	return sum;
}