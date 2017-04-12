#pragma once


// CInput_DLG 对话框

class CInput_DLG : public CDialog
{
	DECLARE_DYNAMIC(CInput_DLG)

public:
	CInput_DLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInput_DLG();

// 对话框数据
	enum { IDD = IDD_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float fInput_Num;
	CString szInput_Tip;
};
