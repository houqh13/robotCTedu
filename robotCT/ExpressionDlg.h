#pragma once

#include <vector>

#include "define.h"


// CExpressionDlg �Ի���

class expUnit;

class CExpressionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExpressionDlg)

public:
	CExpressionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExpressionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_EXP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	std::vector<expUnit> vec_expression;		// �洢���������
	void inputNumber(int number);

public:
	afx_msg void OnBnClickedButton1();
};

class expUnit
{
public:
	expUnit(int type = EXP_NUM, double value = 0.0){
		this->type = type;
		this->value = value;
	}
	int type;			// ���define.h����
	double value;
};
