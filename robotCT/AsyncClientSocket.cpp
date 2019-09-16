// AsyncClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "robotCT.h"
#include "robotCTDlg.h"
#include "AsyncClientSocket.h"


// CAsyncClientSocket

CAsyncClientSocket::CAsyncClientSocket()
{
}

CAsyncClientSocket::~CAsyncClientSocket()
{
}


// CAsyncClientSocket 成员函数


void CAsyncClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int ret = Receive(recvData, BUFFER_SOCKET_RECEIVE);
	CrobotCTDlg* mainDlg = (CrobotCTDlg*)(AfxGetApp()->m_pMainWnd);
	if (strcmp(recvData, PROTOCAL_ROBOT_REACH) && (deviceID == DEVICE_ROBOT_0 || deviceID == DEVICE_ROBOT_1))
	{
		mainDlg->m_bSocketReached[deviceID - DEVICE_ROBOT_0] = true;
	}
	else if (strcmp(recvData, PROTOCAL_DETECTOR_FINISH) && (deviceID == DEVICE_DETECTOR))
	{
		mainDlg->PostMessage(WM_FINISH, NULL, NULL);
	}

	CAsyncSocket::OnReceive(nErrorCode);
}
