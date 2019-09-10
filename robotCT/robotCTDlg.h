
// robotCTDlg.h : 头文件
//

#pragma once

#include <vector>

#include "ExpressionDlg.h"
#include "WorkThread.h"
#include "afxwin.h"


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
	// 欧拉角坐标的表达式
	std::vector<int> vec_expX;
	std::vector<int> vec_expY;
	std::vector<int> vec_expZ;
	std::vector<int> vec_expRx;
	std::vector<int> vec_expRy;
	std::vector<int> vec_expRz;
	std::vector<POSE_R> vec_poseR;	// 欧拉角坐标值(未添加过渡点, 实时更新)
	std::vector<POSE_Q> vec_poseQ;	// 四元数坐标值(含外部轴, 添加过渡点, 非实时更新)
	CWorkThread* th_workThread;		// 通讯线程
	bool b_connectSerial;			// 串口通讯连接状态
	bool b_connectRobot[2];			// 机械臂网络通讯连接状态
	bool b_connectDetector;			// 探测器网络通讯连接状态
	void SetupPos();				// 位姿初始化

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
	LRESULT OnError(WPARAM wParam, LPARAM lParam);		// 工作线程错误消息相应函数
	LRESULT OnConnect(WPARAM wParam, LPARAM lParam);	// 工作线程设备连接消息相应函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
