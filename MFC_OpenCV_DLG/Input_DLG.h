#pragma once


// CInput_DLG �Ի���

class CInput_DLG : public CDialog
{
	DECLARE_DYNAMIC(CInput_DLG)

public:
	CInput_DLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInput_DLG();

// �Ի�������
	enum { IDD = IDD_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float fInput_Num;
	CString szInput_Tip;
};
