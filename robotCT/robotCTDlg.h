
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
	std::vector<expUnit> vec_expX;	// �洢x�ı��ʽ

public:
	afx_msg void OnBnClickedButtonX();
};
