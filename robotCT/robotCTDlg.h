
// robotCTDlg.h : ͷ�ļ�
//

#pragma once

#include <vector>

#include "ExpressionDlg.h"


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
	std::vector<int> vec_expX;		// �洢x����ı��ʽ
	std::vector<int> vec_expY;
	std::vector<int> vec_expZ;
	std::vector<int> vec_expRx;
	std::vector<int> vec_expRy;
	std::vector<int> vec_expRz;

public:
	CString s_expX;
	CString s_expY;
	CString s_expZ;
	CString s_expRx;
	CString s_expRy;
	CString s_expRz;
	afx_msg void OnBnClickedButtonX();
	afx_msg void OnBnClickedButtonY();
	afx_msg void OnBnClickedButtonZ();
	afx_msg void OnBnClickedButtonRx();
	afx_msg void OnBnClickedButtonRy();
	afx_msg void OnBnClickedButtonRz();
};
