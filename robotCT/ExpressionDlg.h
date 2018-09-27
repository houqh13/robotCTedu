#pragma once

#include <vector>

#include "define.h"


// CExpressionDlg 对话框

class expUnit;

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
	std::vector<expUnit> vec_expression;		// 存储输入的内容
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
	int type;			// 详见define.h定义
	double value;
};
