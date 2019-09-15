// AsyncServerSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "robotCT.h"
#include "robotCTDlg.h"
#include "AsyncClientSocket.h"
#include "AsyncServerSocket.h"


// CAsyncServerSocket

CAsyncServerSocket::CAsyncServerSocket()
{
}

CAsyncServerSocket::~CAsyncServerSocket()
{
}


// CAsyncServerSocket 成员函数


void CAsyncServerSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode == 0)
	{
		CAsyncClientSocket tempsoc;
		SOCKADDR_IN tempaddr;
		int len = sizeof(tempaddr);
		Accept(tempsoc, (SOCKADDR*)&tempaddr, &len);
		tempsoc.AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
		CrobotCTDlg* mainDlg = (CrobotCTDlg*)(AfxGetApp()->m_pMainWnd);
		switch (tempaddr.sin_addr.S_un.S_addr & 0xFF)
		{
		case 1:
			mainDlg->th_workThread->m_socRobot[0].Attach(tempsoc.Detach());
			mainDlg->th_workThread->m_socRobot[0].deviceID = DEVICE_ROBOT_0;
			mainDlg->PostMessage(WM_CONNECT, (WPARAM)DEVICE_ROBOT_0, NULL);
			break;
		case 2:
			mainDlg->th_workThread->m_socRobot[1].Attach(tempsoc.Detach());
			mainDlg->th_workThread->m_socRobot[1].deviceID = DEVICE_ROBOT_1;
			mainDlg->PostMessage(WM_CONNECT, (WPARAM)DEVICE_ROBOT_1, NULL);
			break;
		case 13:
			mainDlg->th_workThread->m_socDetector.Attach(tempsoc.Detach());
			mainDlg->th_workThread->m_socDetector.deviceID = DEVICE_DETECTOR;
			mainDlg->PostMessage(WM_CONNECT, (WPARAM)DEVICE_DETECTOR, NULL);
			break;
		default:
			break;
		}
	}

	CAsyncSocket::OnAccept(nErrorCode);
}
