// ExpressionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "robotCT.h"
#include "afxdialogex.h"

#include <math.h>

#include "ExpressionDlg.h"


// CExpressionDlg �Ի���

IMPLEMENT_DYNAMIC(CExpressionDlg, CDialogEx)

CExpressionDlg::CExpressionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExpressionDlg::IDD, pParent)
	, m_sExp(_T(""))
{
	for (int i = 0; i < NUMBER_ANGLE; i++)
	{
		ary_the[i] = i * DELTA_ANGLE * PI / 180;
		ary_sin[i] = sin(ary_the[i]);
		ary_cos[i] = cos(ary_the[i]);
		ary_tan[i] = tan(ary_the[i]);
	}
}

CExpressionDlg::~CExpressionDlg()
{
}

void CExpressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EXP, m_sExp);
}


BEGIN_MESSAGE_MAP(CExpressionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_1, &CExpressionDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CExpressionDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CExpressionDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CExpressionDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CExpressionDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CExpressionDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CExpressionDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CExpressionDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CExpressionDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CExpressionDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_P, &CExpressionDlg::OnBnClickedButtonP)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CExpressionDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CExpressionDlg::OnBnClickedButtonSub)
	ON_BN_CLICKED(IDC_BUTTON_MUL, &CExpressionDlg::OnBnClickedButtonMul)
	ON_BN_CLICKED(IDC_BUTTON_DIV, &CExpressionDlg::OnBnClickedButtonDiv)
	ON_BN_CLICKED(IDC_BUTTON_LBR, &CExpressionDlg::OnBnClickedButtonLbr)
	ON_BN_CLICKED(IDC_BUTTON_RBR, &CExpressionDlg::OnBnClickedButtonRbr)
	ON_BN_CLICKED(IDC_BUTTON_SIN, &CExpressionDlg::OnBnClickedButtonSin)
	ON_BN_CLICKED(IDC_BUTTON_COS, &CExpressionDlg::OnBnClickedButtonCos)
	ON_BN_CLICKED(IDC_BUTTON_TAN, &CExpressionDlg::OnBnClickedButtonTan)
	ON_BN_CLICKED(IDC_BUTTON_THETA, &CExpressionDlg::OnBnClickedButtonTheta)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CExpressionDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CExpressionDlg::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CExpressionDlg::OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CExpressionDlg ��Ϣ�������

BOOL CExpressionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_hAccel = LoadAccelerators(
		AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_EXP));
	UpdateData(FALSE);
	return TRUE;
}


BOOL CExpressionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CExpressionDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(1);
}


void CExpressionDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(2);
}


void CExpressionDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(3);
}


void CExpressionDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(4);
}


void CExpressionDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(5);
}


void CExpressionDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(6);
}


void CExpressionDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(7);
}


void CExpressionDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(8);
}


void CExpressionDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(9);
}


void CExpressionDlg::OnBnClickedButton0()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	inputNumber(0);
}


void CExpressionDlg::OnBnClickedButtonP()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_P);
	m_sExp += ".";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_ADD);
	m_sExp += "+";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonSub()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_SUB);
	m_sExp += "-";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonMul()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_MUL);
	m_sExp += "��";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonDiv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_DIV);
	m_sExp += "��";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonLbr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_LBR);
	m_sExp += "(";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonRbr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_RBR);
	m_sExp += ")";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonSin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_SIN);
	m_sExp += "sin(��)";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonCos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_COS);
	m_sExp += "cos(��)";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonTan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_TAN);
	m_sExp += "tan(��)";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonTheta()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.push_back(EXP_THE);
	m_sExp += "��";
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vec_expression.clear();
	m_sExp.Empty();
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonBack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!vec_expression.empty())
	{
		if (vec_expression.back() > EXP_THE)
		{
			m_sExp = m_sExp.Left(m_sExp.GetLength() - 6);
		}
		else
		{
			m_sExp = m_sExp.Left(m_sExp.GetLength() - 1);
		}
		vec_expression.pop_back();
	}
	UpdateData(FALSE);
}


void CExpressionDlg::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool flag = calcExpression();

	if (flag)
	{
		vec_position.assign(stk_value.top().begin(), stk_value.top().end());
	}
	while (!stk_operator.empty())
	{
		stk_operator.pop();
	}
	while (!stk_value.empty())
	{
		stk_value.pop();
	}

	if (flag)
	{
		OnOK();
	}
	else
	{
		MessageBox(_T("���ʽ�����﷨������������ȷ�ı��ʽ��"), 
			_T("Error"), MB_OK | MB_ICONERROR);
	}
}


// CExpressionDlg �������

void CExpressionDlg::inputNumber(int number)
{
	CString s;
	s.Format(_T("%d"), number);
	vec_expression.push_back(number);
	m_sExp += s;
	UpdateData(FALSE);
}


bool CExpressionDlg::calcExpression()
{
	std::array<double, NUMBER_ANGLE> temp;
	temp.fill(0);
	stk_value.push(temp);					// ���ʽ��ʼֵΪ0
	bool flag = true;						// ���ʽ��λ
	
	// ���ַ�ʶ����ʽ
	int length = vec_expression.size();
	if (length == 0)
	{
		return false;
	}
	for (int i = 0; i < length; i++)
	{
		if (flag)
		{
			// ��λ��ǰһλΪ������
			flag = false;
			if (vec_expression[0] < EXP_P)					// ����
			{
				stk_value.pop();
				temp.fill(vec_expression[0]);
				stk_value.push(temp);
			}
			else if (vec_expression[0] == EXP_THE)			// theta
			{
				stk_value.pop();
				stk_value.push(ary_the);
			}
			else if (vec_expression[0] == EXP_SIN)			// sin
			{
				stk_value.pop();
				stk_value.push(ary_sin);
			}
			else if (vec_expression[0] == EXP_COS)			// cos
			{
				stk_value.pop();
				stk_value.push(ary_cos);
			}
			else if (vec_expression[0] == EXP_TAN)			// tan
			{
				stk_value.pop();
				stk_value.push(ary_tan);
			}
			else if (vec_expression[0] <= EXP_SUB)			// �Ӻ�/����/С����
			{
				stk_operator.push(vec_expression[0]);
			}
			else if (vec_expression[0] == EXP_LBR)			// ������
			{
				stk_value.pop();
				stk_operator.push(EXP_LBR);
				temp.fill(0);
				stk_value.push(temp);
				flag = true;			// �����ź�Ϊ��Ч��λ
			}
			else											// �˺�/����/������
			{
				return false;
			}
		}
		else
		{
			// ����λ
			if (vec_expression[i] < 10)						// ����
			{
				if (vec_expression[i - 1] < 10)
				{
					temp.fill(10 * stk_value.top()[i] + vec_expression[i]);
					stk_value.pop();
					stk_value.push(temp);
				}
				else if (vec_expression[i - 1] <= EXP_DIV)
				{
					temp.fill(vec_expression[i]);
					stk_value.push(temp);
				}
				else
				{
					return false;
				}
			}
			else if (vec_expression[i] == EXP_P)			// С����
			{
				if (stk_operator.top() == EXP_P)
				{
					return false;
				}
				if (vec_expression[i - 1] < 10)
				{
					stk_operator.push(EXP_P);
				}
				else if (vec_expression[i - 1] >= EXP_ADD && 
					vec_expression[i - 1] <= EXP_DIV)
				{
					temp.fill(0);
					stk_value.push(temp);
					stk_operator.push(EXP_P);
				}
				else
				{
					return false;
				}
			}
			else if (vec_expression[i] == EXP_ADD || 
				vec_expression[i] == EXP_SUB)				// �Ӻ�/����
			{
				if (vec_expression[i - 1] >= EXP_P && vec_expression[i - 1] <= EXP_DIV)
				{
					return false;
				}
				while (!(stk_operator.empty() || stk_operator.top() == EXP_LBR))
				{
					calculate();
				}
				stk_operator.push(vec_expression[i]);
			}
			else if (vec_expression[i] == EXP_MUL || 
				vec_expression[i] == EXP_DIV)				// �˺�/����
			{
				if (vec_expression[i - 1] >= EXP_P && vec_expression[i - 1] <= EXP_DIV)
				{
					return false;
				}
				if (!(stk_operator.empty() || stk_operator.top() == EXP_LBR || 
					stk_operator.top() == EXP_ADD || stk_operator.top() == EXP_SUB))
				{
					calculate();
				}
				stk_operator.push(vec_expression[i]);
			}
			else if (vec_expression[i] == EXP_LBR)			// ������
			{
				if (vec_expression[i - 1] >= EXP_ADD && vec_expression[i - 1] <= EXP_DIV)
				{
					stk_operator.push(EXP_LBR);
					temp.fill(0);
					stk_value.push(temp);
					flag = true;			// �����ź�Ϊ��Ч��λ
				}
				else
				{
					return false;
				}
			}
			else if (vec_expression[i] == EXP_RBR)			// ������
			{
				if (vec_expression[i - 1] >= EXP_P && vec_expression[i - 1] <= EXP_LBR)
				{
					return false;
				}
				else
				{
					while (stk_operator.top() != EXP_LBR)
					{
						calculate();
						if (stk_operator.empty())
						{
							return false;
						}
					}
					stk_operator.pop();
				}
			}
			else											// sin/cos/tan/theta
			{
				if (vec_expression[i - 1] >= EXP_ADD && vec_expression[i - 1] <= EXP_DIV)
				{
					switch (vec_expression[i])
					{
					case EXP_THE:
						stk_value.push(ary_the);
						break;
					case EXP_SIN:
						stk_value.push(ary_sin);
						break;
					case EXP_COS:
						stk_value.push(ary_cos);
						break;
					case EXP_TAN:
						stk_value.push(ary_tan);
						break;
					default:
						break;
					}
				}
				else
				{
					return false;
				}
			}
		}
	}

	// ʶ�����, ���������
	if (vec_expression[length - 1] >= EXP_P && vec_expression[length - 1] <= EXP_LBR)
	{
		return false;
	}
	while (!stk_operator.empty())
	{
		if (stk_operator.top() == EXP_LBR)
		{
			return false;
		}
		calculate();
	}

	return true;
}


void CExpressionDlg::calculate()
{
	std::array<double, NUMBER_ANGLE> first = stk_value.top();
	stk_value.pop();
	std::array<double, NUMBER_ANGLE> second = stk_value.top();
	stk_value.pop();
	int opr = stk_operator.top();
	stk_operator.pop();
	std::array<double, NUMBER_ANGLE> temp;
	if (opr == EXP_P)
	{
		double integer = first[0];
		double decimal = second[0];
		while (decimal >= 1)
		{
			decimal /= 10;
		}
		temp.fill(integer + decimal);
	}
	else
	{
		for (int i = 0; i < NUMBER_ANGLE; i++)
		{
			switch (opr)
			{
			case EXP_ADD:
				temp[i] = second[i] + first[i];
				break;
			case EXP_SUB:
				temp[i] = second[i] - first[i];
				break;
			case EXP_MUL:
				temp[i] = second[i] * first[i];
				break;
			case EXP_DIV:
				temp[i] = second[i] / first[i];
				break;
			default:
				break;
			}
		}
	}
	stk_value.push(temp);
}
