
// robotCT.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CrobotCTApp:
// �йش����ʵ�֣������ robotCT.cpp
//

class CrobotCTApp : public CWinApp
{
public:
	CrobotCTApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CrobotCTApp theApp;