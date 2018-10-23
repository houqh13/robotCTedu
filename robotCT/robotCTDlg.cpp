
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
	, s_expX(_T("0"))
	, s_expY(_T("0"))
	, s_expZ(_T("0"))
	, s_expRx(_T("0"))
	, s_expRy(_T("0"))
	, s_expRz(_T("0"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrobotCTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, s_expX);
	DDX_Text(pDX, IDC_EDIT_Y, s_expY);
	DDX_Text(pDX, IDC_EDIT_Z, s_expZ);
	DDX_Text(pDX, IDC_EDIT_RX, s_expRx);
	DDX_Text(pDX, IDC_EDIT_RY, s_expRy);
	DDX_Text(pDX, IDC_EDIT_RZ, s_expRz);
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
	expDlg.m_sExp = s_expX;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expX.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		s_expX = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonY()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expY.begin(), vec_expY.end());
	expDlg.m_sExp = s_expY;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expY.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		s_expY = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonZ()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expZ.begin(), vec_expZ.end());
	expDlg.m_sExp = s_expZ;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expZ.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		s_expZ = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonRx()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expRx.begin(), vec_expRx.end());
	expDlg.m_sExp = s_expRx;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expRx.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		s_expRx = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonRy()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expRy.begin(), vec_expRy.end());
	expDlg.m_sExp = s_expRy;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expRy.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		s_expRy = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}


void CrobotCTDlg::OnBnClickedButtonRz()
{
	// TODO: 在此添加控件通知处理程序代码
	CExpressionDlg expDlg;
	expDlg.vec_expression.assign(vec_expRz.begin(), vec_expRz.end());
	expDlg.m_sExp = s_expRz;

	if (expDlg.DoModal() == IDOK)
	{
		vec_expRz.assign(expDlg.vec_expression.begin(), expDlg.vec_expression.end());
		s_expRz = expDlg.m_sExp;
	}
	UpdateData(FALSE);
}
