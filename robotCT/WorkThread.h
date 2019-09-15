#pragma once

#include "define.h"

#include "AsyncClientSocket.h"
#include "AsyncServerSocket.h"

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
	CAsyncServerSocket m_socServer;		// 服务器(本机)套接字对象
	CAsyncClientSocket m_socRobot[2];	// 客户端(机械臂)套接字对象
	CAsyncClientSocket m_socDetector;	// 客户端(探测器平板)套接字对象
	BYTE m_cSerialRecvData[BUFFER_SERIAL_RECEIVE];		// 串口接收数据
	BYTE m_cSerialSendData[BUFFER_SERIAL_SEND];			// 串口发送数据
	bool m_bSerialReached[2];			// 导轨是否运行到位
	char m_cSocketRecvData[BUFFER_SOCKET_RECEIVE];		// 网络接收数据
	char m_cSocketSendData[BUFFER_SOCKET_SEND];			// 网络发送数据
	bool m_bSocketReached[2];			// 机械臂是否运行到位
	DWORD sendBytes;
	DWORD recvBytes;
	double lastw;						// 前一步导轨位置
	int double2String(POSE_Q* pose, char* str, int precision);
										// (double型)浮点数转(char*型)字符串, 用于向机械臂传递数据
	BYTE calcCheckBit(BYTE* data);		// 计算校验位, 用于向导轨传递数据

public:
	afx_msg void OnThreadTimer(WPARAM wParam, LPARAM lParam);	// 主线程定时器消息响应函数
	afx_msg void OnStart(WPARAM wParam, LPARAM lParam);			// 开始运动消息响应函数
	afx_msg void OnShow(WPARAM wParam, LPARAM lParam);			// 通知平板显示消息响应函数
};


