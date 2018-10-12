
// robotCTDlg.h : 头文件
//

#pragma once

#include <vector>

#include "ExpressionDlg.h"


// CrobotCTDlg 对话框
class CrobotCTDlg : public CDialogEx
{
// 构造
public:
	CrobotCTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ROBOTCT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	std::vector<int> vec_expX;		// 存储x坐标的表达式
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
