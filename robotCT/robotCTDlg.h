
// robotCTDlg.h : ͷ�ļ�
//

#pragma once

#include <vector>

#include "ExpressionDlg.h"
//#include "WorkThread.h"
#include "AsyncClientSocket.h"
#include "AsyncServerSocket.h"
#include "afxwin.h"


// CrobotCTDlg �Ի���
class CrobotCTDlg : public CDialogEx
{
// ����
public:
	CrobotCTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ROBOTCT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// ŷ��������ı��ʽ
	std::vector<int> vec_expX;
	std::vector<int> vec_expY;
	std::vector<int> vec_expZ;
	std::vector<int> vec_expRx;
	std::vector<int> vec_expRy;
	std::vector<int> vec_expRz;
	std::vector<POSE_R> vec_poseR;		// ŷ��������ֵ(δ��ӹ��ɵ�, ʵʱ����)
	std::vector<POSE_Q> vec_poseQ;		// ��Ԫ������ֵ(���ⲿ��, ��ӹ��ɵ�, ��ʵʱ����)
	int i_progress;						// ɨ�����
	//CWorkThread* th_workThread;		// ͨѶ�߳�
	bool b_connectSerial;				// ����ͨѶ����״̬
	bool b_connectRobot[2];				// ��е������ͨѶ����״̬
	bool b_connectDetector;				// ̽��������ͨѶ����״̬
	HANDLE m_hComm;						// ����ͨ��(����)���
	CAsyncServerSocket m_socServer;		// ������(����)�׽��ֶ���
	CAsyncClientSocket m_socRobot[2];	// �ͻ���(��е��)�׽��ֶ���
	CAsyncClientSocket m_socDetector;	// �ͻ���(̽����ƽ��)�׽��ֶ���
	BYTE m_cSerialRecvData[BUFFER_SERIAL_RECEIVE];		// ���ڽ�������
	BYTE m_cSerialSendData[BUFFER_SERIAL_SEND];			// ���ڷ�������
	char m_cSocketRecvData[BUFFER_SOCKET_RECEIVE];		// �����������
	char m_cSocketSendData[BUFFER_SOCKET_SEND];			// ���緢������
	bool m_bSerialReached[2];			// �����Ƿ����е�λ
	bool m_bSocketReached[2];			// ��е���Ƿ����е�λ
	DWORD sendBytes;
	DWORD recvBytes;
	//double lastw;						// ǰһ������λ��
	void SetupPos();					// λ�˳�ʼ��
	int double2String(POSE_Q* pose, char* str, int precision);
										// (double��)������ת(char*��)�ַ���, �������е�۴�������
	BYTE calcCheckBit(BYTE* data);		// ����У��λ, �����򵼹촫������

public:
	CString m_sExpX;
	CString m_sExpY;
	CString m_sExpZ;
	CString m_sExpRx;
	CString m_sExpRy;
	CString m_sExpRz;
	afx_msg void OnBnClickedButtonX();
	afx_msg void OnBnClickedButtonY();
	afx_msg void OnBnClickedButtonZ();
	afx_msg void OnBnClickedButtonRx();
	afx_msg void OnBnClickedButtonRy();
	afx_msg void OnBnClickedButtonRz();
	afx_msg void OnBnClickedButtonStart();
	LRESULT OnError(WPARAM wParam, LPARAM lParam);		// �����̴߳�����Ϣ��Ӧ����
	LRESULT OnConnect(WPARAM wParam, LPARAM lParam);	// �����߳��豸������Ϣ��Ӧ����
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);		// �����߳��豸������Ϣ��Ӧ����
	LRESULT OnReach(WPARAM wParam, LPARAM lParam);		// �˶��豸����λ����Ϣ��Ӧ����
	LRESULT OnFinish(WPARAM wParam, LPARAM lParam);		// ��ǰλ��ɨ�������Ϣ��Ӧ����
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
