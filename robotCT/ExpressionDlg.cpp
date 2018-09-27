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
{

}

CExpressionDlg::~CExpressionDlg()
{
}

void CExpressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExpressionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_1, &CExpressionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

// CExpressionDlg �������

void CExpressionDlg::inputNumber(int number)
{
	expUnit lastunit = vec_expression.back();
	switch (lastunit.type)
	{
	case EXP_NUM:
		if (lastunit.value - int(lastunit.value))
		{
			int digit = 1;
			int remainder = 10 * (lastunit.value - int(lastunit.value));
			while (remainder)
			{
				digit ++;
				remainder = 10 * (remainder - int(remainder));
			}
			lastunit.value += number * pow(10, -digit);
		}
		else
		{
			lastunit.value = lastunit.value * 10 + number;
		}
		vec_expression.pop_back();
		vec_expression.push_back(lastunit);
		break;
	case EXP_OPR:
		vec_expression.push_back(expUnit(EXP_NUM, double(number)));
		// �ַ���
		break;
	case EXP_TRI:
	default:
		break;
	}
}


// CExpressionDlg ��Ϣ�������

void CExpressionDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
