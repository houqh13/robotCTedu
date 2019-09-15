
// robotCTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "robotCT.h"
#include "robotCTDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CrobotCTDlg �Ի���



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


// CrobotCTDlg ��Ϣ�������

BOOL CrobotCTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CrobotCTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CrobotCTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CrobotCTDlg::OnBnClickedButtonX()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		msg.Format(_T("�򿪴��ڷ������� %d"), (int)lParam);
		break;
	case SOCKET_INITIAL:
		msg.Format(_T("����Socket��ʼ���������� %d"), (int)lParam);
		break;
	case DEVICE_SERVER:
		msg.Format(_T("���������������������� %d"), (int)lParam);
		break;
	case DEVICE_ROBOT_0:
		msg.Format(_T("��е�� 0 ���ӷ������� %d"), (int)lParam);
		break;
	case DEVICE_ROBOT_1:
		msg.Format(_T("��е�� 1 ���ӷ������� %d"), (int)lParam);
		break;
	case DEVICE_DETECTOR:
		msg.Format(_T("̽�������ӷ������� %d"), (int)lParam);
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
		// ���ɨ��, �������߳�
		KillTimer(1);
		th_workThread->PostThreadMessage(WM_QUIT, NULL, NULL);
		MessageBox(_T("ɨ����ɣ�"), _T("Success"), MB_OK | MB_ICONINFORMATION);
		// ������ز���
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
		// δ���ɨ��
		if (vec_poseQ[i_progress].index == -1)
		{
			// �ǳ���λ��, ֱ���˶�����һλ��
			this->PostMessage(WM_FINISH, NULL, NULL);
		}
		else
		{
			// ����λ��, ֪ͨƽ����ʾͼƬ(ģ�⿪ʼ����)
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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


// CrobotCTDlg �������

void CrobotCTDlg::SetupPos()
{
	std::vector<POSE_Q> poses_q;

	// �Ϲ�����ɵ�
	poses_q.push_back(POSE_Q(
		POSE_R(BCS_X, BCS_Y + vec_poseR.front().y, BCS_Z, BCS_RX, BCS_RY, BCS_RZ),
		1.1 * (BCS_Y + vec_poseR.front().y)));
	for (unsigned int i = 0; i < vec_poseR.size(); i++)
	{
		double guidepos = (vec_poseQ[i].y >= 0 ? 1 : -1) * 
			ceil(min(abs(1.1 * vec_poseR[i].y), GUIDE_MAX_SIDE) / GUIDE_GAP);
		poses_q.push_back(POSE_Q(vec_poseR[i], guidepos * GUIDE_GAP, i));
	}
	// �¹�����ɵ�
	poses_q.push_back(POSE_Q(
		POSE_R(BCS_X, BCS_Y + vec_poseR.back().y, BCS_Z, BCS_RX, BCS_RY, (BCS_RZ + vec_poseR.back().rz) / 2), 
		1.1 * (BCS_Y + vec_poseR.back().y)));
	// �ص���ʼλ��
	poses_q.push_back(POSE_Q(POSE_R(BCS_X, BCS_Y, BCS_Z, BCS_RX, BCS_RY, BCS_RZ)));

	vec_poseQ.assign(poses_q.begin(), poses_q.end());
}
