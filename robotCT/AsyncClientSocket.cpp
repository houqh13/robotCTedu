// AsyncClientSocket.cpp : ʵ���ļ�
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


// CAsyncClientSocket ��Ա����


void CAsyncClientSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
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
