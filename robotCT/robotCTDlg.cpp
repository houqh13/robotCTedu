
// robotCTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "robotCT.h"
#include "robotCTDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CrobotCTDlg 对话框



CrobotCTDlg::CrobotCTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CrobotCTDlg::IDD, pParent)
	, m_sExpX(_T("0"))
	, m_sExpY(_T("0"))
	, m_sExpZ(_T("0"))
	, m_sExpRx(_T("0"))
	, m_sExpRy(_T("0"))
	, m_sExpRz(_T("0"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrobotCTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_sExpX);
	DDX_Text(pDX, IDC_EDIT_Y, m_sExpY);
	DDX_Text(pDX, IDC_EDIT_Z, m_sExpZ);
	DDX_Text(pDX, IDC_EDIT_RX, m_sExpRx);
	DDX_Text(pDX, IDC_EDIT_RY, m_sExpRy);
	DDX_Text(pDX, IDC_EDIT_RZ, m_sExpRz);
}

BEGIN_MESSAGE_MAP(CrobotCTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_X, &CrobotCTDlg::OnBnClickedButtonX)
	ON_BN_CLICKED(IDC_BUTTON_Y, &CrobotCTDlg::OnBnClickedButtonY)
	ON_BN_CLICKED(IDC_BUTTON_Z, &CrobotCTDlg::OnBnClickedButtonZ)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CrobotCTDlg::OnBnClickedButtonRx)
	ON_BN_CLICKED(IDC_BUTTON_RY, &CrobotCTDlg::OnBnClickedButtonRy)
	ON_BN_CLICKED(IDC_BUTTON_RZ, &CrobotCTDlg::OnBnClickedButtonRz)
	ON_BN_CLICKED(IDC_BUTTON_START, &CrobotCTDlg::OnBnClickedButtonStart)
	ON_MESSAGE(WM_ERROR, &CrobotCTDlg::OnError)
	ON_MESSAGE(WM_CONNECT, &CrobotCTDlg::OnConnect)
	ON_MESSAGE(WM_START, &CrobotCTDlg::OnStart)
	ON_MESSAGE(WM_REACH, &CrobotCTDlg::OnReach)
	ON_MESSAGE(WM_FINISH, &CrobotCTDlg::OnFinish)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CrobotCTDlg 消息处理程序

BOOL CrobotCTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	vec_expX.push_back(0);
	vec_expY.push_back(0);
	vec_expZ.push_back(0);
	vec_expRx.push_back(0);
	vec_expRy.push_back(0);
	vec_expRz.push_back(0);
	vec_poseR.assign(NUMBER_ANGLE, POSE_R(0.0, 0.0, 0.0, 0.0, 0.0, 0.0));

	b_connectSerial = false;
	b_connectRobot[0] = false;
	b_connectRobot[1] = false;
	b_connectDetector = false;
	i_progress = 0;
	
	m_hComm = INVALID_HANDLE_VALUE;
	for (int i = 0; i < 2; i++)
	{
		m_bSerialReached[i] = false;
		m_bSocketReached[i] = false;
	}
	m_cSerialSendData[0] = (BYTE)0xFF;
	m_cSerialSendData[1] = (BYTE)0xAA;
	m_cSerialSendData[2] = (BYTE)0x00;
	m_cSerialSendData[8] = (BYTE)0x00;
	//lastw = 0;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CrobotCTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CrobotCTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CrobotCTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CrobotCTDlg::OnBnClickedButtonX()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expX.begin(), vec_expX.end());
	expDlg.m_sExp = m_sExpX;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expX.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		for (unsigned int i = 0; i < vec_poseR.size(); i++)
		{
			vec_poseR[i].x = expDlg.vec_position[i];
		}
		m_sExpX = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonY()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expY.begin(), vec_expY.end());
	expDlg.m_sExp = m_sExpY;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expY.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		for (unsigned int i = 0; i < vec_poseR.size(); i++)
		{
			vec_poseR[i].y = expDlg.vec_position[i];
		}
		m_sExpY = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonZ()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expZ.begin(), vec_expZ.end());
	expDlg.m_sExp = m_sExpZ;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expZ.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		for (unsigned int i = 0; i < vec_poseR.size(); i++)
		{
			vec_poseR[i].z = expDlg.vec_position[i];
		}
		m_sExpZ = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonRx()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expRx.begin(), vec_expRx.end());
	expDlg.m_sExp = m_sExpRx;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expRx.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		for (unsigned int i = 0; i < vec_poseR.size(); i++)
		{
			vec_poseR[i].rx = expDlg.vec_position[i];
		}
		m_sExpRx = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonRy()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expRy.begin(), vec_expRy.end());
	expDlg.m_sExp = m_sExpRy;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expRy.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		for (unsigned int i = 0; i < vec_poseR.size(); i++)
		{
			vec_poseR[i].ry = expDlg.vec_position[i];
		}
		m_sExpRy = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonRz()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expRz.begin(), vec_expRz.end());
	expDlg.m_sExp = m_sExpRz;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expRz.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		for (unsigned int i = 0; i < vec_poseR.size(); i++)
		{
			vec_poseR[i].rz = expDlg.vec_position[i];
		}
		m_sExpRz = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	SetupPos();

	//th_workThread = (CWorkThread*)AfxBeginThread(RUNTIME_CLASS(CWorkThread));
	GetDlgItem(IDC_BUTTON_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Z)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

	// 打开串口
	m_hComm = CreateFile(
		L"COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		this->PostMessage(WM_ERROR, (WPARAM)DEVICE_SERIAL, (LPARAM)GetLastError());
		return;
	}

	// 配置串口
	SetupComm(m_hComm, 256, 256);
	DCB dcb;
	GetCommState(m_hComm, &dcb);
	dcb.BaudRate = CBR_9600;		// 波特率9600
	dcb.ByteSize = 8;				// 数据位8位
	dcb.Parity = NOPARITY;			// 无奇偶校验位
	dcb.StopBits = ONESTOPBIT;		// 停止位1位
	SetCommState(m_hComm, &dcb);
	COMMTIMEOUTS timeouts;			// 无超时设置
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(m_hComm, &timeouts);
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	this->PostMessage(WM_CONNECT, (WPARAM)DEVICE_SERIAL, NULL);

	// 打开服务器端网络并等待连接
	if (!AfxSocketInit())
	{
		this->PostMessage(WM_ERROR, (WPARAM)SOCKET_INITIAL, (LPARAM)GetLastError());
		return;
	}
	if (!m_socServer.Create(SERVER_PORT, SOCK_STREAM, FD_ACCEPT, SERVER_IP))
	{
		this->PostMessage(WM_ERROR, (WPARAM)DEVICE_SERVER, (LPARAM)GetLastError());
		m_socServer.Close();
		return;
	}
	m_socServer.Listen();
}


LRESULT CrobotCTDlg::OnError(WPARAM wParam, LPARAM lParam)
{
	CString msg;
	switch (wParam)
	{
	case DEVICE_SERIAL:
		msg.Format(_T("打开串口发生错误 %d"), (int)lParam);
		break;
	case SOCKET_INITIAL:
		msg.Format(_T("本机Socket初始化发生错误 %d"), (int)lParam);
		break;
	case DEVICE_SERVER:
		msg.Format(_T("本机服务器创建发生错误 %d"), (int)lParam);
		break;
	case DEVICE_ROBOT_0:
		msg.Format(_T("机械臂 0 连接发生错误 %d"), (int)lParam);
		break;
	case DEVICE_ROBOT_1:
		msg.Format(_T("机械臂 1 连接发生错误 %d"), (int)lParam);
		break;
	case DEVICE_DETECTOR:
		msg.Format(_T("探测器连接发生错误 %d"), (int)lParam);
		break;
	default:
		break;
	}
	MessageBox(msg, _T("Error"), MB_OK | MB_ICONERROR);
	GetDlgItem(IDC_BUTTON_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Y)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Z)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RX)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RZ)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

	return 0;
}


LRESULT CrobotCTDlg::OnConnect(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case DEVICE_SERIAL:
		b_connectSerial = true;
		break;
	case DEVICE_ROBOT_0:
		b_connectRobot[0] = true;
		break;
	case DEVICE_ROBOT_1:
		b_connectRobot[1] = true;
		break;
	case DEVICE_DETECTOR:
		b_connectDetector = true;
		break;
	default:
		break;
	}

	if (b_connectSerial && b_connectRobot[0] && b_connectRobot[1] && b_connectDetector)
	{
		SetTimer(TIMER_ID_COMM, TIMER_DOT_COMM, NULL);
		//th_workThread->PostThreadMessage(WM_START, (WPARAM)&vec_poseQ[0], NULL);
		this->PostMessage(WM_START, (WPARAM)i_progress, NULL);
	}

	return 0;
}


LRESULT CrobotCTDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	// 导轨运动通过串口发送
	double distance = wParam ? vec_poseQ[wParam].w - vec_poseQ[wParam - 1].w : vec_poseQ[wParam].w - BCS_Y;
	if (distance == 0)
	{
		m_bSerialReached[0] = true;
		m_bSerialReached[1] = true;
	}
	else
	{
		int pulse = int(abs(distance) * STEPPR_200 * DIVIDE_8 / LEAD);
		m_cSerialSendData[5] = (BYTE)pulse;
		m_cSerialSendData[6] = (BYTE)(pulse / 256);
		m_cSerialSendData[7] = (BYTE)(pulse / 65536);
		// 导轨 1
		m_cSerialSendData[3] = (BYTE)1;
		if (distance > 0)
		{
			m_cSerialSendData[4] = (BYTE)0x1F;
		}
		else
		{
			m_cSerialSendData[4] = (BYTE)0x2F;
		}
		m_cSerialSendData[9] = calcCheckBit(m_cSerialSendData);
		WriteFile(m_hComm, &m_cSerialSendData, BUFFER_SERIAL_SEND, &sendBytes, NULL);
		// 导轨 2
		m_cSerialSendData[3] = (BYTE)2;
		if (distance > 0)
		{
			m_cSerialSendData[4] = (BYTE)0x2F;
		}
		else
		{
			m_cSerialSendData[4] = (BYTE)0x1F;
		}
		m_cSerialSendData[9] = calcCheckBit(m_cSerialSendData);
		WriteFile(m_hComm, &m_cSerialSendData, BUFFER_SERIAL_SEND, &sendBytes, NULL);
	}

	// 机械臂运动通过网络发送
	int len = double2String(&vec_poseQ[wParam], m_cSocketSendData, PRECISION_6);
	m_socRobot[0].Send(PROTOCAL_ROBOT_MOVE, strlen(PROTOCAL_ROBOT_MOVE));
	m_socRobot[1].Send(PROTOCAL_ROBOT_MOVE, strlen(PROTOCAL_ROBOT_MOVE));
	m_socRobot[0].Send(m_cSocketSendData, len);
	m_socRobot[1].Send(m_cSocketSendData, len);

	return 0;
}


LRESULT CrobotCTDlg::OnReach(WPARAM wParam, LPARAM lParam)
{
	if (i_progress == vec_poseQ.size() - 1)
	{
		// 完成扫描, 清理工作线程
		KillTimer(TIMER_ID_COMM);
		//th_workThread->PostThreadMessage(WM_ALLCLEAR, NULL, NULL);
		m_socRobot[0].Send(PROTOCAL_ALL_CLEAR, strlen(PROTOCAL_ALL_CLEAR));
		m_socRobot[1].Send(PROTOCAL_ALL_CLEAR, strlen(PROTOCAL_ALL_CLEAR));
		m_socDetector.Send(PROTOCAL_ALL_CLEAR, strlen(PROTOCAL_ALL_CLEAR));
		CloseHandle(m_hComm);
		m_socRobot[0].Close();
		m_socRobot[1].Close();
		m_socDetector.Close();
		m_socServer.Close();
		MessageBox(_T("扫描完成！"), _T("Success"), MB_OK | MB_ICONINFORMATION);
		// 重置相关参数
		i_progress = 0;
		b_connectSerial = false;
		b_connectRobot[0] = false;
		b_connectRobot[1] = false;
		b_connectDetector = false;
		GetDlgItem(IDC_BUTTON_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RX)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	}
	else
	{
		// 未完成扫描
		if (vec_poseQ[i_progress].index == 0)
		{
			// 非成像位置, 直接运动到下一位置
			i_progress++;
			this->PostMessage(WM_START, (WPARAM)i_progress, NULL);
		}
		else
		{
			// 成像位置, 通知平板显示图片(模拟开始成像)
			//th_workThread->PostThreadMessage(WM_SHOW, (WPARAM)vec_poseQ[i_progress].index, NULL);
			char s[4];
			sprintf_s(s, sizeof(s), "%d", vec_poseQ[i_progress].index);
			m_socDetector.Send(PROTOCAL_DETECTOR_SHOW, strlen(PROTOCAL_DETECTOR_SHOW));
			m_socDetector.Send(s, strlen(s));
		}
	}

	return 0;
}


LRESULT CrobotCTDlg::OnFinish(WPARAM wParam, LPARAM lParam)
{
	i_progress++;
	//th_workThread->PostThreadMessage(WM_START, (WPARAM)&vec_poseQ[i_progress], NULL);
	this->PostMessage(WM_START, (WPARAM)i_progress, NULL);

	return 0;
}


void CrobotCTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER_ID_COMM:
		//th_workThread->PostThreadMessage(WM_THREADTIMER, NULL, NULL);
		ReadFile(m_hComm, &m_cSerialRecvData, BUFFER_SERIAL_RECEIVE, &recvBytes, NULL);
		if (m_cSerialRecvData[0] == (BYTE)0xFF && m_cSerialRecvData[1] == (BYTE)0xAA && m_cSerialRecvData[3] == (BYTE)0x3F)
		{
			m_bSerialReached[(int)m_cSerialRecvData[2] - 1] = true;
		}
		memset(m_cSerialRecvData, 0, BUFFER_SERIAL_RECEIVE);
		if (m_bSerialReached[0] && m_bSerialReached[1] && m_bSocketReached[0] && m_bSocketReached[1])
		{
			this->PostMessage(WM_REACH, NULL, NULL);
			m_bSerialReached[0] = false;
			m_bSerialReached[1] = false;
			m_bSocketReached[0] = false;
			m_bSocketReached[1] = false;
		}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


// CrobotCTDlg 计算程序

void CrobotCTDlg::SetupPos()
{
	std::vector<POSE_Q> poses_q;

	// 上轨道过渡点
	poses_q.push_back(POSE_Q(
		POSE_R(BCS_X, BCS_Y + vec_poseR.front().y, BCS_Z, BCS_RX, BCS_RY, BCS_RZ),
		1.1 * (BCS_Y + vec_poseR.front().y)));
	for (unsigned int i = 0; i < vec_poseR.size(); i++)
	{
		double guidepos = (vec_poseR[i].y >= 0 ? 1 : -1) * 
			ceil(min(abs(1.1 * vec_poseR[i].y), GUIDE_MAX_SIDE) / GUIDE_GAP);
		poses_q.push_back(POSE_Q(vec_poseR[i], guidepos * GUIDE_GAP, i + 1));
	}
	// 下轨道过渡点
	poses_q.push_back(POSE_Q(
		POSE_R(BCS_X, BCS_Y + vec_poseR.back().y, BCS_Z, BCS_RX, BCS_RY, (BCS_RZ + vec_poseR.back().rz) / 2), 
		1.1 * (BCS_Y + vec_poseR.back().y)));
	// 回到初始位置
	poses_q.push_back(POSE_Q(POSE_R(BCS_X, BCS_Y, BCS_Z, BCS_RX, BCS_RY, BCS_RZ)));

	vec_poseQ.assign(poses_q.begin(), poses_q.end());
}


int CrobotCTDlg::double2String(POSE_Q* pose, char* str, int precision)
{
	char temp[256];
	memset(str, 0, BUFFER_SOCKET_SEND);
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->x, precision);
	if (temp[strlen(temp) - 1] == '.')
	{
		strcat_s(temp, BUFFER_SOCKET_SEND, "0");
	}
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->y, precision);
	if (temp[strlen(temp) - 1] == '.')
	{
		strcat_s(temp, BUFFER_SOCKET_SEND, "0");
	}
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->z, precision);
	if (temp[strlen(temp) - 1] == '.')
	{
		strcat_s(temp, BUFFER_SOCKET_SEND, "0");
	}
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->q0, precision);
	if (temp[strlen(temp) - 1] == '.')
	{
		strcat_s(temp, BUFFER_SOCKET_SEND, "0");
	}
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->q1, precision);
	if (temp[strlen(temp) - 1] == '.')
	{
		strcat_s(temp, BUFFER_SOCKET_SEND, "0");
	}
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->q2, precision);
	if (temp[strlen(temp) - 1] == '.')
	{
		strcat_s(temp, BUFFER_SOCKET_SEND, "0");
	}
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, ",");
	_gcvt_s(temp, BUFFER_SOCKET_SEND, pose->q3, precision);
	if (temp[strlen(temp) - 1] == '.')
	{
		strcat_s(temp, BUFFER_SOCKET_SEND, "0");
	}
	strcat_s(str, BUFFER_SOCKET_SEND, temp);
	strcat_s(str, BUFFER_SOCKET_SEND, "\r");			// 结束位

	return strlen(str);
}


BYTE CrobotCTDlg::calcCheckBit(BYTE* data)
{
	BYTE sum = (BYTE)0;
	for (int i = 0; i < BUFFER_SERIAL_SEND - 1; i++)
	{
		sum += data[i];
	}
	return sum;
}