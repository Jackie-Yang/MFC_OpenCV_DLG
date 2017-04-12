// Mode_Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_OpenCV_DLG.h"
#include "Mode_Dlg.h"
#include "afxdialogex.h"


// CMode_Dlg 对话框

IMPLEMENT_DYNAMIC(CMode_Dlg, CDialog)

CMode_Dlg::CMode_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMode_Dlg::IDD, pParent)
	, m_nWindowSize(0)
	, m_fParam1(0)
	, m_fParam2(0)
{
	m_nSelFunc = 0;

	m_pnModeSel = new int[END_INDEX - 1];
	memset(m_pnModeSel, 0, sizeof(int) * (END_INDEX - 1));
	m_nWindowSize = 3;

	m_fNoisePSNR = 20.0f;
	m_nBlurSize = 3;
	m_nGaussianSize = 3;
	m_nMedianSize = 3;
	m_nBilateralSize = 3;
	m_nSchRange = 15;

	m_fLogConst = 46.0f;
	m_fGamma = 0.6f;
	m_fGaussianSigma = 0.8f;
	m_fBilateral_Sigma_Color = 0.0f;
	m_fBilateral_Sigma_Space = 0.0f;
}

CMode_Dlg::~CMode_Dlg()
{
	delete[] m_pnModeSel;
}

void CMode_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODE_BOX, m_ModeBox);
	DDX_Text(pDX, IDC_WINDOW_EDIT, m_nWindowSize);
	DDX_Text(pDX, IDC_PARAM_EDIT, m_fParam1);
	DDX_Text(pDX, IDC_PARAM_EDIT2, m_fParam2);
}


BEGIN_MESSAGE_MAP(CMode_Dlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMode_Dlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_WINDOW_EDIT, &CMode_Dlg::OnEnChangeWindowEdit)
//	ON_CBN_SELCHANGE(IDC_MODE_BOX, &CMode_Dlg::OnCbnSelchangeModeBox)
END_MESSAGE_MAP()


// CMode_Dlg 消息处理程序


BOOL CMode_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowTextA(m_szDlgName);	//窗口命名
	m_bWindowSize = FALSE;
	m_bParam1 = FALSE;
	m_bParam2 = FALSE;
	//根据处理函数显示参数编辑框
	switch (m_nSelFunc)
	{
		case ADD_NOISE_INDEX:
		{
			m_ModeBox.InsertString(0, "高斯噪声");
			m_ModeBox.InsertString(1, "椒盐噪声");
			m_bParam1 = TRUE;
			m_fParam1 = m_fNoisePSNR;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("PSNR(dB)：");
			//GetDlgItem(IDC_PARAM_EDIT)->EnableWindow(FALSE);
			break;
		}
		case GRAY_REV_INDEX:
		{
			m_ModeBox.InsertString(0, "普通");
			m_ModeBox.InsertString(1, "cvAPI");
			break;
		}
		case REVLOG_INDEX:
		case LOG_INDEX:
		{
			m_ModeBox.InsertString(0, "快速查表");
			m_ModeBox.InsertString(1, "普通");
			m_bParam1 = TRUE;
			m_fParam1 = m_fLogConst;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("Const：");
			break;
		}
		case GAMMA_INDEX:
		{
			m_ModeBox.InsertString(0, "快速查表");
			m_ModeBox.InsertString(1, "普通");
			m_bParam1 = TRUE;
			m_fParam1 = m_fGamma;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("Gamma：");
			break;
		}
		case EQU_HIST_INDEX:
		{
			m_ModeBox.InsertString(0, "普通");
			m_ModeBox.InsertString(1, "cvAPI");
			break;
		}
		case BLUR_INDEX:
		{
			m_ModeBox.InsertString(0, "快速积分图");
			m_ModeBox.InsertString(1, "普通");
			m_ModeBox.InsertString(2, "cvAPI");
			m_bWindowSize = TRUE;
			m_nWindowSize = m_nBlurSize;
			break;
		}
		case MEDIAN_INDEX:
		{
			m_ModeBox.InsertString(0, "基于直方图");
			m_ModeBox.InsertString(1, "基于泡沫排序");
			m_ModeBox.InsertString(2, "基于选择排序");
			m_ModeBox.InsertString(3, "基于插入排序");
			m_ModeBox.InsertString(4, "基于快速排序");
			m_ModeBox.InsertString(5, "基于Shell排序");
			m_ModeBox.InsertString(6, "基于基数排序");
			m_ModeBox.InsertString(7, "cvAPI");
			m_bWindowSize = TRUE;
			m_nWindowSize = m_nMedianSize;
			break;
		}
		case GAUSSIAN_INDEX:
		{
			m_ModeBox.InsertString(0, "可分离滤波器");
			m_ModeBox.InsertString(1, "普通");
			m_ModeBox.InsertString(2, "cvAPI");
			m_bWindowSize = TRUE;
			m_bParam1 = TRUE;
			m_nWindowSize = m_nGaussianSize;
			m_fParam1 = m_fGaussianSigma;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("Sigma：");
			break;
		}
		case BILATERAL_INDEX:
		{
			m_ModeBox.InsertString(0, "普通");
			m_ModeBox.InsertString(1, "cvAPI");
			m_bWindowSize = TRUE;
			m_bParam1 = TRUE;
			m_bParam2 = TRUE;
			m_nWindowSize = m_nBilateralSize;
			m_fParam1 = m_fBilateral_Sigma_Color;
			m_fParam2 = m_fBilateral_Sigma_Space;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("颜色Sigma:");
			GetDlgItem(IDC_PARAM_SET2)->SetWindowTextA("空间Sigma：");
			break;
		}
		case SOBEL_INDEX:
		{
			m_ModeBox.InsertString(0, "普通");
			m_ModeBox.InsertString(1, "cvAPI");
			break;
		}
		case MOTION_INDEX:
		{
			m_ModeBox.InsertString(0, "全搜索");
			m_bWindowSize = TRUE;
			m_nWindowSize = m_nSchRange;
			break;
		}
		case DIFF_INDEX:
		{
			m_ModeBox.InsertString(0, "cvAPI");
			break;
		}
		default:break;
	}


	GetDlgItem(IDC_WINDOW_SET)->ShowWindow(FALSE);
	GetDlgItem(IDC_WINDOW_EDIT)->ShowWindow(FALSE);
	GetDlgItem(IDC_PARAM_SET)->ShowWindow(FALSE);
	GetDlgItem(IDC_PARAM_EDIT)->ShowWindow(FALSE);
	GetDlgItem(IDC_PARAM_SET2)->ShowWindow(FALSE);
	GetDlgItem(IDC_PARAM_EDIT2)->ShowWindow(FALSE);

	//窗口布局
	CRect DlgRect;
	GetWindowRect(&DlgRect);
	DlgRect.bottom = DlgRect.top + 50;	//窗口最小大小
	

	int currentCTLy = 15;	//第一个控件行坐标

	DlgRect.bottom += 30;	//对话框为第一个控件开辟位置
	GetDlgItem(IDC_MODE_BOX)->MoveWindow(85, currentCTLy, 115, 40);
	GetDlgItem(IDC_MODE_TEXT)->MoveWindow(20, currentCTLy, 60, 22);
	MoveWindow(DlgRect);

	if (m_bWindowSize)
	{
		DlgRect.bottom += 33;	//对话框为每个控件都开辟一定空间
		currentCTLy += 30;		//每个控件下移的位置
		MoveWindow(DlgRect);	//拓展窗口
		GetDlgItem(IDC_WINDOW_EDIT)->MoveWindow(85, currentCTLy, 115, 22);
		GetDlgItem(IDC_WINDOW_SET)->MoveWindow(20, currentCTLy, 60, 20);
		GetDlgItem(IDC_WINDOW_SET)->ShowWindow(TRUE);
		GetDlgItem(IDC_WINDOW_EDIT)->ShowWindow(TRUE);
	}

	if (m_bParam1)
	{
		DlgRect.bottom += 33;
		currentCTLy += 30;
		MoveWindow(DlgRect);
		GetDlgItem(IDC_PARAM_EDIT)->MoveWindow(85, currentCTLy, 115, 22);
		GetDlgItem(IDC_PARAM_SET)->MoveWindow(20, currentCTLy, 60, 20);
		GetDlgItem(IDC_PARAM_SET)->ShowWindow(TRUE);
		GetDlgItem(IDC_PARAM_EDIT)->ShowWindow(TRUE);
	}

	if (m_bParam2)
	{
		DlgRect.bottom += 33;
		currentCTLy += 30;
		MoveWindow(DlgRect);
		GetDlgItem(IDC_PARAM_EDIT2)->MoveWindow(85, currentCTLy, 115, 22);
		GetDlgItem(IDC_PARAM_SET2)->MoveWindow(20, currentCTLy, 60, 20);
		GetDlgItem(IDC_PARAM_SET2)->ShowWindow(TRUE);
		GetDlgItem(IDC_PARAM_EDIT2)->ShowWindow(TRUE);
	}
	DlgRect.bottom += 33;
	currentCTLy += 30;
	MoveWindow(DlgRect);
	GetDlgItem(IDOK)->MoveWindow(90, currentCTLy, 60, 25);


	m_ModeBox.SetCurSel(*(m_pnModeSel + m_nSelFunc - 1));	//选中算法
	UpdateData(FALSE);	//当前变量值更新到Edit编辑框

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//void CMode_Dlg::OnCbnSelchangeModeBox()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	switch (m_nSelFunc)
//	{
//		case ADD_NOISE:
//		{
//			int nCurSel = m_ModeBox.GetCurSel();
//			if (nCurSel == 0)
//			{
//				GetDlgItem(IDC_PARAM_EDIT)->EnableWindow(FALSE);
//			}
//			else if (nCurSel == 1)
//			{
//				GetDlgItem(IDC_PARAM_EDIT)->EnableWindow(TRUE);
//			}
//			break;
//		}
//		default:break;
//	}
//}


INT_PTR CMode_Dlg::ShowModeDLG(int nSelFunc, CString szName /*= ""*/)
{
	if (nSelFunc)
	{
		m_nSelFunc = nSelFunc;
		m_szDlgName = szName + "设置";
		return DoModal();
	}	
	return IDCANCEL;
}


void CMode_Dlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	*(m_pnModeSel + m_nSelFunc - 1) = m_ModeBox.GetCurSel();
	CString stInput;
	if(m_bWindowSize)	//判断空输入
	{
		GetDlgItem(IDC_WINDOW_EDIT)->GetWindowTextA(stInput);
		if (stInput.IsEmpty())return;
	}
	if (m_bParam1)		//判断空输入
	{
		GetDlgItem(IDC_PARAM_EDIT)->GetWindowTextA(stInput);
		if (stInput.IsEmpty())return;
	}
	UpdateData(TRUE);			//编辑框数据更新到变量

	//判断模板大小输入合法性
	if (m_bWindowSize && (m_nWindowSize % 2 != 1 || m_nWindowSize < 3) )
	{
		if (m_nWindowSize < 3)
		{
			m_nWindowSize = 3;
		}
		else //if (nSize % 2 != 1)
		{
			m_nWindowSize--;
		}
		CString stTip;
		stTip.Format("输入错误,已将模板大小改为%d", m_nWindowSize);
		AfxMessageBox(stTip, MB_ICONINFORMATION);
		UpdateData(FALSE);	//变量值更新到Edit编辑框
		return;
	}

	//参数更新
	switch (m_nSelFunc)
	{
		case ADD_NOISE_INDEX:
		{
			m_fNoisePSNR = m_fParam1;
			break;
		}
		case REVLOG_INDEX:
		case LOG_INDEX:
		{
			m_fLogConst = m_fParam1;
			break;
		}
		case GAMMA_INDEX:
		{
			m_fGamma = m_fParam1;
			break;
		}
		case BLUR_INDEX:
		{
			m_nBlurSize = m_nWindowSize;
			break;
		}
		case MEDIAN_INDEX:
		{
			m_nMedianSize = m_nWindowSize;
			break;
		}
		case GAUSSIAN_INDEX:
		{
			m_nGaussianSize = m_nWindowSize;
			m_fGaussianSigma = m_fParam1;
			break;
		}
		case BILATERAL_INDEX:
		{
			m_nBilateralSize = m_nWindowSize;
			m_fBilateral_Sigma_Color = m_fParam1;
			m_fBilateral_Sigma_Space = m_fParam2;
			break;
		}
		case MOTION_INDEX:
		{
			m_nSchRange = m_nWindowSize;
			break;
		}
	default:break;
	}
	CDialog::OnOK();
}



void CMode_Dlg::OnEnChangeWindowEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString stInput;
	GetDlgItem(IDC_WINDOW_EDIT)->GetWindowTextA(stInput);
	if ( GAUSSIAN_INDEX == m_nSelFunc && !stInput.IsEmpty() )
	{
		UpdateData(TRUE);	//编辑框数据更新到变量

		//根据当前输入模板大小选择较好Sigma值
		//转换公式:
		//Sigma = (Size / 2 - 1)*0.3f + 0.8f;	
		//Size = 1 + 2 * ceil(3 * Sigma);
		//[-3*sigma,3*sigma] 以内数据，会覆盖绝大部分滤波系数 

		if ((m_nWindowSize % 2 != 1) || (m_nWindowSize < 3))
		{
			if (m_nWindowSize < 3)
			{
				m_fParam1 = 0.8f;
			}
			else //if (nSize % 2 != 1)
			{
				m_fParam1 = ((m_nWindowSize - 1) / 2 - 1)*0.3f + 0.8f;
			}
		}
		else
		{
			m_fParam1 = ((m_nWindowSize - 1) / 2 - 1)*0.3f + 0.8f;
		}
		
		UpdateData(FALSE);	//变量值更新到Edit编辑框，若在整数框输入小数也能立即更新为整数
	}
}



