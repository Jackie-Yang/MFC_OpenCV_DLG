
// MFC_OpenCV_DLG.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_OpenCV_DLGApp: 
// �йش����ʵ�֣������ MFC_OpenCV_DLG.cpp
//

class CMFC_OpenCV_DLGApp : public CWinApp
{
public:
	CMFC_OpenCV_DLGApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_OpenCV_DLGApp theApp;