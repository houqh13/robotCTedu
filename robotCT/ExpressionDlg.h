#pragma once

#include <array>
#include <stack>
#include <vector>

#include "define.h"


// CExpressionDlg �Ի���

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
	std::array<double, NUMBER_ANGLE> ary_the;		// ���ǶȵĦ�ֵ
	std::array<double, NUMBER_ANGLE> ary_sin;		// ���Ƕȵ�sin(��)ֵ
	std::array<double, NUMBER_ANGLE> ary_cos;		// ���Ƕȵ�cos(��)ֵ
	std::array<double, NUMBER_ANGLE> ary_tan;		// ���Ƕȵ�tan(��)ֵ
	std::array<double, NUMBER_ANGLE> ary_value;		// ���ʽ������
	std::vector<int> vec_expression;				// �洢���������
	std::stack<std::array<double, NUMBER_ANGLE>> stk_value;		// ������ʽ����ֵջ
	std::stack<int> stk_operator;					// ������ʽ�������ջ
	void inputNumber(int number);
	bool calcExpression();							// �����ʽ�Ϸ��Բ�������ʽ
	void calculate();								// ����ջ����һ������

public:
	virtual BOOL OnInitDialog();
	CString m_sExp;									// ������ʾ���ַ���
	HACCEL m_hAccel;								// ��ݼ����
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButtonP();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonSub();
	afx_msg void OnBnClickedButtonMul();
	afx_msg void OnBnClickedButtonDiv();
	afx_msg void OnBnClickedButtonLbr();
	afx_msg void OnBnClickedButtonRbr();
	afx_msg void OnBnClickedButtonSin();
	afx_msg void OnBnClickedButtonCos();
	afx_msg void OnBnClickedButtonTan();
	afx_msg void OnBnClickedButtonTheta();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButtonOk();
};
