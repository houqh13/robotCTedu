
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

	th_workThread = (CWorkThread*)AfxBeginThread(RUNTIME_CLASS(CWorkThread));
	GetDlgItem(IDC_BUTTON_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Z)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
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
		SetTimer(1, 20, NULL);
		th_workThread->PostThreadMessage(WM_START, (WPARAM)&vec_poseQ[0], NULL);
	}

	return 0;
}


LRESULT CrobotCTDlg::OnReach(WPARAM wParam, LPARAM lParam)
{
	if (i_progress == vec_poseQ.size() - 1)
	{
		// 完成扫描, 清理工作线程
		KillTimer(1);
		th_workThread->PostThreadMessage(WM_QUIT, NULL, NULL);
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
		if (vec_poseQ[i_progress].index == -1)
		{
			// 非成像位置, 直接运动到下一位置
			this->PostMessage(WM_FINISH, NULL, NULL);
		}
		else
		{
			// 成像位置, 通知平板显示图片(模拟开始成像)
			th_workThread->PostThreadMessage(WM_SHOW, (WPARAM)vec_poseQ[i_progress].index, NULL);
		}
	}

	return 0;
}


LRESULT CrobotCTDlg::OnFinish(WPARAM wParam, LPARAM lParam)
{
	i_progress++;
	th_workThread->PostThreadMessage(WM_START, (WPARAM)&vec_poseQ[i_progress], NULL);

	return 0;
}


void CrobotCTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		th_workThread->PostThreadMessage(WM_THREADTIMER, NULL, NULL);
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
		double guidepos = (vec_poseQ[i].y >= 0 ? 1 : -1) * 
			ceil(min(abs(1.1 * vec_poseR[i].y), GUIDE_MAX_SIDE) / GUIDE_GAP);
		poses_q.push_back(POSE_Q(vec_poseR[i], guidepos * GUIDE_GAP, i));
	}
	// 下轨道过渡点
	poses_q.push_back(POSE_Q(
		POSE_R(BCS_X, BCS_Y + vec_poseR.back().y, BCS_Z, BCS_RX, BCS_RY, (BCS_RZ + vec_poseR.back().rz) / 2), 
		1.1 * (BCS_Y + vec_poseR.back().y)));
	// 回到初始位置
	poses_q.push_back(POSE_Q(POSE_R(BCS_X, BCS_Y, BCS_Z, BCS_RX, BCS_RY, BCS_RZ)));

	vec_poseQ.assign(poses_q.begin(), poses_q.end());
}
