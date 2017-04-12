// Input_DLG.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_OpenCV_DLG.h"
#include "Input_DLG.h"
#include "afxdialogex.h"


// CInput_DLG 对话框

IMPLEMENT_DYNAMIC(CInput_DLG, CDialog)

CInput_DLG::CInput_DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CInput_DLG::IDD, pParent)
	, fInput_Num(0)
	, szInput_Tip(_T(""))
{
	//GetDlgItem(IDC_EDIT1)->SetFocus();
}

CInput_DLG::~CInput_DLG()
{
}

void CInput_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, fInput_Num);
	DDX_Text(pDX, IDC_INPUT_TIP, szInput_Tip);
}


BEGIN_MESSAGE_MAP(CInput_DLG, CDialog)
END_MESSAGE_MAP()


// CInput_DLG 消息处理程序
