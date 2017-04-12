#pragma once
#include "afxwin.h"

//����������
typedef enum
{
	ADD_NOISE_INDEX = 1,
	GRAY_REV_INDEX,
	LOG_INDEX,
	REVLOG_INDEX,
	GAMMA_INDEX,
	EQU_HIST_INDEX,
	BLUR_INDEX,
	MEDIAN_INDEX,
	GAUSSIAN_INDEX,
	BILATERAL_INDEX,
	SOBEL_INDEX,
	MOTION_INDEX,
	DIFF_INDEX,
	END_INDEX
}FuncIndex;

// CMode_Dlg �Ի���

class CMode_Dlg : public CDialog
{
	DECLARE_DYNAMIC(CMode_Dlg)

public:
	CMode_Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMode_Dlg();

	int m_nSelFunc;			//ʹ�����ֺ������д���
	CString m_szDlgName;	//�Ի�������

	int *m_pnModeSel;		//ָ��洢���ֺ���ʹ�õ��㷨

	int m_nBlurSize;		//��ֵ�˲�ģ���С
	int m_nGaussianSize;	//��˹�˲�ģ���С
	int m_nMedianSize;		//��ֵ�˲����ڴ�С
	int m_nBilateralSize;	//˫���˲����ڴ�С
	int m_nSchRange;		//�˶�����������Χ

	float m_fNoisePSNR;
	float m_fLogConst;	//�����任����
	float m_fGamma;		//ڤ�α任����Gamma
	float m_fGaussianSigma;	//��˹�˲�Sigma
	float m_fBilateral_Sigma_Color;	//˫���˲�����
	float m_fBilateral_Sigma_Space;
	//˫���˲�����
	bool m_bWindowSize;		//�Ƿ���ʾ�����ģ���С
	bool m_bParam1;			//�Ƿ���ʾ����Ĳ���1
	bool m_bParam2;			//�Ƿ���ʾ����Ĳ���2

	/**
	*	function:		Sobel
	*	description:	�������ò����Ի���
	*	parameter:		nSelFunc:����Ч������
						szName:�Ի������
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	INT_PTR ShowModeDLG(int nSelFunc, CString szName = "");

// �Ի�������
	enum { IDD = IDD_MODE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ModeBox;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	UINT m_nWindowSize;
	float m_fParam1;
	afx_msg void OnEnChangeWindowEdit();
	float m_fParam2;
//	afx_msg void OnCbnSelchangeModeBox();
};
