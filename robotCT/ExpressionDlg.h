#pragma once

#include <array>
#include <stack>
#include <vector>

#include "define.h"


// CExpressionDlg 对话框

class CExpressionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExpressionDlg)

public:
	CExpressionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExpressionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EXP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	std::array<double, NUMBER_ANGLE> ary_the;		// 各角度的θ值
	std::array<double, NUMBER_ANGLE> ary_sin;		// 各角度的sin(θ)值
	std::array<double, NUMBER_ANGLE> ary_cos;		// 各角度的cos(θ)值
	std::array<double, NUMBER_ANGLE> ary_tan;		// 各角度的tan(θ)值
	std::array<double, NUMBER_ANGLE> ary_value;		// 表达式计算结果
	std::vector<int> vec_expression;				// 存储输入的内容
	std::stack<std::array<double, NUMBER_ANGLE>> stk_value;		// 计算表达式的数值栈
	std::stack<int> stk_operator;					// 计算表达式的运算符栈
	void inputNumber(int number);
	bool calcExpression();							// 检查表达式合法性并计算表达式
	void calculate();								// 运行栈顶的一次运算

public:
	virtual BOOL OnInitDialog();
	CString m_sExp;									// 用于显示的字符串
	HACCEL m_hAccel;								// 快捷键句柄
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
