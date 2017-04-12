#pragma once
#include "afxwin.h"

//处理函数索引
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

// CMode_Dlg 对话框

class CMode_Dlg : public CDialog
{
	DECLARE_DYNAMIC(CMode_Dlg)

public:
	CMode_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMode_Dlg();

	int m_nSelFunc;			//使用那种函数进行处理
	CString m_szDlgName;	//对话框名称

	int *m_pnModeSel;		//指向存储各种函数使用的算法

	int m_nBlurSize;		//均值滤波模板大小
	int m_nGaussianSize;	//高斯滤波模板大小
	int m_nMedianSize;		//中值滤波窗口大小
	int m_nBilateralSize;	//双边滤波窗口大小
	int m_nSchRange;		//运动估计搜索范围

	float m_fNoisePSNR;
	float m_fLogConst;	//对数变换常数
	float m_fGamma;		//冥次变换参数Gamma
	float m_fGaussianSigma;	//高斯滤波Sigma
	float m_fBilateral_Sigma_Color;	//双边滤波参数
	float m_fBilateral_Sigma_Space;
	//双边滤波参数
	bool m_bWindowSize;		//是否显示输入的模板大小
	bool m_bParam1;			//是否显示输入的参数1
	bool m_bParam2;			//是否显示输入的参数2

	/**
	*	function:		Sobel
	*	description:	弹出设置参数对话框
	*	parameter:		nSelFunc:处理效果索引
						szName:对话框标题
	*	return:			bool值:是否处理成功
	**/
	INT_PTR ShowModeDLG(int nSelFunc, CString szName = "");

// 对话框数据
	enum { IDD = IDD_MODE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
