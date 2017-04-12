// Mode_Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_OpenCV_DLG.h"
#include "Mode_Dlg.h"
#include "afxdialogex.h"


// CMode_Dlg �Ի���

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


// CMode_Dlg ��Ϣ�������


BOOL CMode_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowTextA(m_szDlgName);	//��������
	m_bWindowSize = FALSE;
	m_bParam1 = FALSE;
	m_bParam2 = FALSE;
	//���ݴ�������ʾ�����༭��
	switch (m_nSelFunc)
	{
		case ADD_NOISE_INDEX:
		{
			m_ModeBox.InsertString(0, "��˹����");
			m_ModeBox.InsertString(1, "��������");
			m_bParam1 = TRUE;
			m_fParam1 = m_fNoisePSNR;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("PSNR(dB)��");
			//GetDlgItem(IDC_PARAM_EDIT)->EnableWindow(FALSE);
			break;
		}
		case GRAY_REV_INDEX:
		{
			m_ModeBox.InsertString(0, "��ͨ");
			m_ModeBox.InsertString(1, "cvAPI");
			break;
		}
		case REVLOG_INDEX:
		case LOG_INDEX:
		{
			m_ModeBox.InsertString(0, "���ٲ��");
			m_ModeBox.InsertString(1, "��ͨ");
			m_bParam1 = TRUE;
			m_fParam1 = m_fLogConst;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("Const��");
			break;
		}
		case GAMMA_INDEX:
		{
			m_ModeBox.InsertString(0, "���ٲ��");
			m_ModeBox.InsertString(1, "��ͨ");
			m_bParam1 = TRUE;
			m_fParam1 = m_fGamma;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("Gamma��");
			break;
		}
		case EQU_HIST_INDEX:
		{
			m_ModeBox.InsertString(0, "��ͨ");
			m_ModeBox.InsertString(1, "cvAPI");
			break;
		}
		case BLUR_INDEX:
		{
			m_ModeBox.InsertString(0, "���ٻ���ͼ");
			m_ModeBox.InsertString(1, "��ͨ");
			m_ModeBox.InsertString(2, "cvAPI");
			m_bWindowSize = TRUE;
			m_nWindowSize = m_nBlurSize;
			break;
		}
		case MEDIAN_INDEX:
		{
			m_ModeBox.InsertString(0, "����ֱ��ͼ");
			m_ModeBox.InsertString(1, "������ĭ����");
			m_ModeBox.InsertString(2, "����ѡ������");
			m_ModeBox.InsertString(3, "���ڲ�������");
			m_ModeBox.InsertString(4, "���ڿ�������");
			m_ModeBox.InsertString(5, "����Shell����");
			m_ModeBox.InsertString(6, "���ڻ�������");
			m_ModeBox.InsertString(7, "cvAPI");
			m_bWindowSize = TRUE;
			m_nWindowSize = m_nMedianSize;
			break;
		}
		case GAUSSIAN_INDEX:
		{
			m_ModeBox.InsertString(0, "�ɷ����˲���");
			m_ModeBox.InsertString(1, "��ͨ");
			m_ModeBox.InsertString(2, "cvAPI");
			m_bWindowSize = TRUE;
			m_bParam1 = TRUE;
			m_nWindowSize = m_nGaussianSize;
			m_fParam1 = m_fGaussianSigma;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("Sigma��");
			break;
		}
		case BILATERAL_INDEX:
		{
			m_ModeBox.InsertString(0, "��ͨ");
			m_ModeBox.InsertString(1, "cvAPI");
			m_bWindowSize = TRUE;
			m_bParam1 = TRUE;
			m_bParam2 = TRUE;
			m_nWindowSize = m_nBilateralSize;
			m_fParam1 = m_fBilateral_Sigma_Color;
			m_fParam2 = m_fBilateral_Sigma_Space;
			GetDlgItem(IDC_PARAM_SET)->SetWindowTextA("��ɫSigma:");
			GetDlgItem(IDC_PARAM_SET2)->SetWindowTextA("�ռ�Sigma��");
			break;
		}
		case SOBEL_INDEX:
		{
			m_ModeBox.InsertString(0, "��ͨ");
			m_ModeBox.InsertString(1, "cvAPI");
			break;
		}
		case MOTION_INDEX:
		{
			m_ModeBox.InsertString(0, "ȫ����");
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

	//���ڲ���
	CRect DlgRect;
	GetWindowRect(&DlgRect);
	DlgRect.bottom = DlgRect.top + 50;	//������С��С
	

	int currentCTLy = 15;	//��һ���ؼ�������

	DlgRect.bottom += 30;	//�Ի���Ϊ��һ���ؼ�����λ��
	GetDlgItem(IDC_MODE_BOX)->MoveWindow(85, currentCTLy, 115, 40);
	GetDlgItem(IDC_MODE_TEXT)->MoveWindow(20, currentCTLy, 60, 22);
	MoveWindow(DlgRect);

	if (m_bWindowSize)
	{
		DlgRect.bottom += 33;	//�Ի���Ϊÿ���ؼ�������һ���ռ�
		currentCTLy += 30;		//ÿ���ؼ����Ƶ�λ��
		MoveWindow(DlgRect);	//��չ����
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


	m_ModeBox.SetCurSel(*(m_pnModeSel + m_nSelFunc - 1));	//ѡ���㷨
	UpdateData(FALSE);	//��ǰ����ֵ���µ�Edit�༭��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//void CMode_Dlg::OnCbnSelchangeModeBox()
//{
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		m_szDlgName = szName + "����";
		return DoModal();
	}	
	return IDCANCEL;
}


void CMode_Dlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*(m_pnModeSel + m_nSelFunc - 1) = m_ModeBox.GetCurSel();
	CString stInput;
	if(m_bWindowSize)	//�жϿ�����
	{
		GetDlgItem(IDC_WINDOW_EDIT)->GetWindowTextA(stInput);
		if (stInput.IsEmpty())return;
	}
	if (m_bParam1)		//�жϿ�����
	{
		GetDlgItem(IDC_PARAM_EDIT)->GetWindowTextA(stInput);
		if (stInput.IsEmpty())return;
	}
	UpdateData(TRUE);			//�༭�����ݸ��µ�����

	//�ж�ģ���С����Ϸ���
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
		stTip.Format("�������,�ѽ�ģ���С��Ϊ%d", m_nWindowSize);
		AfxMessageBox(stTip, MB_ICONINFORMATION);
		UpdateData(FALSE);	//����ֵ���µ�Edit�༭��
		return;
	}

	//��������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString stInput;
	GetDlgItem(IDC_WINDOW_EDIT)->GetWindowTextA(stInput);
	if ( GAUSSIAN_INDEX == m_nSelFunc && !stInput.IsEmpty() )
	{
		UpdateData(TRUE);	//�༭�����ݸ��µ�����

		//���ݵ�ǰ����ģ���Сѡ��Ϻ�Sigmaֵ
		//ת����ʽ:
		//Sigma = (Size / 2 - 1)*0.3f + 0.8f;	
		//Size = 1 + 2 * ceil(3 * Sigma);
		//[-3*sigma,3*sigma] �������ݣ��Ḳ�Ǿ��󲿷��˲�ϵ�� 

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
		
		UpdateData(FALSE);	//����ֵ���µ�Edit�༭����������������С��Ҳ����������Ϊ����
	}
}



