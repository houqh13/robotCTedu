#pragma once

#include "define.h"


// CWorkThread

class CWorkThread : public CWinThread
{
	DECLARE_DYNCREATE(CWorkThread)

protected:
	CWorkThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CWorkThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	HANDLE m_hComm;						// 串口通信(导轨)句柄
	BYTE m_cSerialRecvData[BUFFER_SERIAL_RECEIVE];		// 串口接收数据
	BYTE m_cSerialSendData[BUFFER_SERIAL_SEND];			// 串口发送数据
	bool m_bSerialReached[2];			// 导轨是否运行到位
	CSocket m_socServer;				// 服务器(本机)套接字对象
	CSocket m_socRobot[2];				// 客户端(机械臂)套接字对象
	CSocket m_socDetector;				// 客户端(探测器平板)套接字对象
	char m_cSocketRecvData[BUFFER_SOCKET_RECEIVE];	// 网络接收数据
	char m_cSocketSendData[BUFFER_SOCKET_SEND];		// 网络发送数据
	bool m_bSocketReached[2];			// 机械臂是否运行到位
	double lastw;
	DWORD sendBytes;
	DWORD recvBytes;
};


