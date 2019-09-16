
// robotCTDlg.h : 头文件
//

#pragma once

#include <vector>

#include "ExpressionDlg.h"
//#include "WorkThread.h"
#include "AsyncClientSocket.h"
#include "AsyncServerSocket.h"
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
	std::vector<POSE_R> vec_poseR;		// 欧拉角坐标值(未添加过渡点, 实时更新)
	std::vector<POSE_Q> vec_poseQ;		// 四元数坐标值(含外部轴, 添加过渡点, 非实时更新)
	int i_progress;						// 扫描进度
	//CWorkThread* th_workThread;		// 通讯线程
	bool b_connectSerial;				// 串口通讯连接状态
	bool b_connectRobot[2];				// 机械臂网络通讯连接状态
	bool b_connectDetector;				// 探测器网络通讯连接状态
	HANDLE m_hComm;						// 串口通信(导轨)句柄
	CAsyncServerSocket m_socServer;		// 服务器(本机)套接字对象
	CAsyncClientSocket m_socRobot[2];	// 客户端(机械臂)套接字对象
	CAsyncClientSocket m_socDetector;	// 客户端(探测器平板)套接字对象
	BYTE m_cSerialRecvData[BUFFER_SERIAL_RECEIVE];		// 串口接收数据
	BYTE m_cSerialSendData[BUFFER_SERIAL_SEND];			// 串口发送数据
	char m_cSocketRecvData[BUFFER_SOCKET_RECEIVE];		// 网络接收数据
	char m_cSocketSendData[BUFFER_SOCKET_SEND];			// 网络发送数据
	bool m_bSerialReached[2];			// 导轨是否运行到位
	bool m_bSocketReached[2];			// 机械臂是否运行到位
	DWORD sendBytes;
	DWORD recvBytes;
	//double lastw;						// 前一步导轨位置
	void SetupPos();					// 位姿初始化
	int double2String(POSE_Q* pose, char* str, int precision);
										// (double型)浮点数转(char*型)字符串, 用于向机械臂传递数据
	BYTE calcCheckBit(BYTE* data);		// 计算校验位, 用于向导轨传递数据

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
	LRESULT OnError(WPARAM wParam, LPARAM lParam);		// 工作线程错误消息响应函数
	LRESULT OnConnect(WPARAM wParam, LPARAM lParam);	// 工作线程设备连接消息响应函数
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);		// 工作线程设备连接消息响应函数
	LRESULT OnReach(WPARAM wParam, LPARAM lParam);		// 运动设备到达位置消息响应函数
	LRESULT OnFinish(WPARAM wParam, LPARAM lParam);		// 当前位置扫描结束消息响应函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
