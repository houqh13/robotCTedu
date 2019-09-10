
// robotCTDlg.h : ͷ�ļ�
//

#pragma once

#include <vector>

#include "ExpressionDlg.h"
#include "WorkThread.h"
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
	std::vector<POSE_R> vec_poseR;	// ŷ��������ֵ(δ��ӹ��ɵ�, ʵʱ����)
	std::vector<POSE_Q> vec_poseQ;	// ��Ԫ������ֵ(���ⲿ��, ��ӹ��ɵ�, ��ʵʱ����)
	CWorkThread* th_workThread;		// ͨѶ�߳�
	bool b_connectSerial;			// ����ͨѶ����״̬
	bool b_connectRobot[2];			// ��е������ͨѶ����״̬
	bool b_connectDetector;			// ̽��������ͨѶ����״̬
	void SetupPos();				// λ�˳�ʼ��

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
