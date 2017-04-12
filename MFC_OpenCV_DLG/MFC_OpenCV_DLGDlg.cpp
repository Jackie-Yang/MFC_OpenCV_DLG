
// MFC_OpenCV_DLGDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_OpenCV_DLG.h"
#include "MFC_OpenCV_DLGDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_OpenCV_DLGDlg �Ի���


CMFC_OpenCV_DLGDlg::CMFC_OpenCV_DLGDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_OpenCV_DLGDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImg_src = NULL;
	m_pImg_dst = NULL;
	m_pCapture = NULL;
	//ֱ��ͼ���ݳ�ʼ��
	memset(m_Hist.pGrayHist, 0, sizeof(float) * 256);
	memset(m_Hist.pRedHist, 0, sizeof(float) * 256);
	memset(m_Hist.pGreenHist, 0, sizeof(float) * 256);
	memset(m_Hist.pBlueHist, 0, sizeof(float) * 256);
	//����һ����ģ̬�Ի�������ֱ��ͼ��ʾ
	m_pDrawHist = new CDrawHist(); //�����ڴ����ڴ�����ģ̬�Ի���
	m_pDrawHist->Create(IDD_DRAWHIST); //����һ����ģ̬�Ի���  

	m_nMinClientWidth = 0;		//�ͻ�����С�ߴ�
	m_nMinClientHeight = 0;

	m_bInitDLG = FALSE;		//����״̬������δ��ʼ��

}


CMFC_OpenCV_DLGDlg::~CMFC_OpenCV_DLGDlg( )
{
	cvDestroyAllWindows();

	if (m_pImg_src)
	{
		cvReleaseImage(&m_pImg_src);
	}
	if (m_pImg_dst)
	{
		cvReleaseImage(&m_pImg_dst);
	}

	if (m_pCapture)
	{
		cvReleaseCapture(&m_pCapture);
	}
	DelCtrlList();	//�ͷſؼ�������
	delete m_pDrawHist;//�ͷ�ֱ��ͼ�Ի����ڴ�

}

void CMFC_OpenCV_DLGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_SRC, m_Bmp_src);
	DDX_Control(pDX, IDC_PICTURE_DST, m_Bmp_dst);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_FuncBox);
}

BEGIN_MESSAGE_MAP(CMFC_OpenCV_DLGDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_IMG_OPEN, &CMFC_OpenCV_DLGDlg::OnBnClickedImgOpen)
	ON_BN_CLICKED(IDC_IMG_PROCESS, &CMFC_OpenCV_DLGDlg::OnBnClickedImgProcess)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_STN_DBLCLK(IDC_PICTURE_SRC, &CMFC_OpenCV_DLGDlg::OnStnDblclickPictureSrc)
	ON_STN_DBLCLK(IDC_PICTURE_DST, &CMFC_OpenCV_DLGDlg::OnStnDblclickPictureDst)
	ON_BN_CLICKED(IDC_IMG_PROCESS2, &CMFC_OpenCV_DLGDlg::OnBnClickedImgProcess2)
	ON_STN_CLICKED(IDC_PICTURE_SRC, &CMFC_OpenCV_DLGDlg::OnStnClickedPictureSrc)
	ON_STN_CLICKED(IDC_PICTURE_DST, &CMFC_OpenCV_DLGDlg::OnStnClickedPictureDst)
	ON_BN_CLICKED(IDC_IMG_SAVE, &CMFC_OpenCV_DLGDlg::OnBnClickedImgSave)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMFC_OpenCV_DLGDlg ��Ϣ�������

BOOL CMFC_OpenCV_DLGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��



	// TODO:  �ڴ���Ӷ���ĳ�ʼ������


	//��ȡ���ڵĳ�ʼ��С
	CRect	rect;
	GetWindowRect(rect);
	//����ʼֵ��Ϊ��Сֵ
	m_ptMinTrackSize.x = (LONG)rect.Width();
	m_ptMinTrackSize.y = (LONG)rect.Height();


	//��ʼ����̬�ؼ�����
	m_Bmp_src_Info = CtrlInfo_Init(IDC_PICTURE_SRC, TRUE);
	m_Bmp_dst_Info = CtrlInfo_Init(IDC_PICTURE_DST, TRUE);
	CtrlInfo_Init(IDC_IMG_OPEN,FALSE);
	CtrlInfo_Init(IDC_IMG_PROCESS, FALSE);
	CtrlInfo_Init(IDC_COMBO_FUNC, FALSE);
	CtrlInfo_Init(IDC_CHECK_GRAY, FALSE);
	CtrlInfo_Init(IDC_IMG_PROCESS2, FALSE);
	CtrlInfo_Init(IDC_TIME, FALSE);
	CtrlInfo_Init(IDC_NAME1,FALSE);
	CtrlInfo_Init(IDC_NAME2, FALSE);
	CtrlInfo_Init(IDC_IMG_SAVE, FALSE);

	//((CButton*)GetDlgItem(IDC_CHECK_GRAY))->SetCheck(TRUE);	//Ĭ�ϴ򿪻Ҷ�ͼ

	//��ʼ��Ч���б�
	m_FuncBox.InsertString(0, "��ѡ����Ч��");
	m_FuncBox.InsertString(ADD_NOISE_INDEX, "�������");
	m_FuncBox.InsertString(GRAY_REV_INDEX, "�Ҷȷ�ת");
	m_FuncBox.InsertString(LOG_INDEX, "�����任");
	m_FuncBox.InsertString(REVLOG_INDEX, "�������任");
	m_FuncBox.InsertString(GAMMA_INDEX, "ڤ�α任");
	m_FuncBox.InsertString(EQU_HIST_INDEX, "ֱ��ͼ���⻯");
	m_FuncBox.InsertString(BLUR_INDEX, "��ֵ�˲�");
	m_FuncBox.InsertString(MEDIAN_INDEX, "��ֵ�˲�");
	m_FuncBox.InsertString(GAUSSIAN_INDEX, "��˹�˲�");
	m_FuncBox.InsertString(BILATERAL_INDEX, "˫���˲�");
	m_FuncBox.InsertString(SOBEL_INDEX, "Sobel��Ե���");
	m_FuncBox.InsertString(MOTION_INDEX, "�˶�����");
	m_FuncBox.InsertString(DIFF_INDEX, "ͼ�����");
	m_FuncBox.SetCurSel(0);

	EnableToolTips(TRUE);		//��ʾ��Ϣ�������ڿؼ�����ʾ��ʾ
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.SetMaxTipWidth(150);		//�趨��Ȳ��ܻ���
	m_ToolTip.SetDelayTime(TTDT_INITIAL, 800);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,30000);
	m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_SRC), "δ����ԭͼ");
	m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_DST), "δ����Ч��ͼ");
	m_ToolTip.AddTool(GetDlgItem(IDC_CHECK_GRAY), "�ԻҶ�ͼ��ʽ��");
	m_ToolTip.AddTool(GetDlgItem(IDC_IMG_PROCESS2), "Ч��ͼ��Ϊԭͼ��������");
	m_ToolTip.AddTool(GetDlgItem(IDC_IMG_SAVE), "����Ч��ͼ");

	m_bInitDLG = TRUE;	//���ô���״̬�����ڳ�ʼ�����

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFC_OpenCV_DLGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_OpenCV_DLGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//����ͼ��
		if (m_pImg_src)
		{
			PrintBMP(&m_Bmp_src, m_pImg_src);		//��ͼ����ʾ��Picture Control
		}
		if (m_pImg_dst)
		{
			PrintBMP(&m_Bmp_dst, m_pImg_dst);		//��ͼ����ʾ��Picture Control
		}
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_OpenCV_DLGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL CMFC_OpenCV_DLGDlg::PreTranslateMessage(MSG *pMsg)
{
	m_ToolTip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}


/*****************************************���ֿ���*************************************************/




//���ô�����С�ߴ�
void CMFC_OpenCV_DLGDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	lpMMI->ptMinTrackSize = m_ptMinTrackSize;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


/*���ڱ�������*/
/*

m_dDlgScale = double(rect.Width()) / double(rect.Height());	//���ô��ڳ�ʼ����


void CMFC_OpenCV_DLGDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

     
	//Right : 2 
	//Top : 3     
	//Bottom : 6     
	//Left + Top :  4
	//Right + Top : 5      
	//Left + Bottom : 7     
	//Right + Bottom : 8

	// TODO:  �ڴ˴������Ϣ����������


	int nWidth;
	int nHeight;
	double dScale = double(nWidth) / double(nHeight);


	CRect rect;				//ԭʼ���ھ���
	GetWindowRect(&rect);

	if (fwSide == 1)		//Left: 1 
	{
		int nWidth = pRect->right - rect.left;

		int nMaxY = GetSystemMetrics(SM_CYSCREEN);

		int nHeightDifft = int(nWidth / m_dDlgScale) - rect.Height();	//�߶�����
		int nTopDiff = nHeightDifft / 2;
		int nBottomDiff = nHeightDifft - nTopDiff;

		pRect->top -= nTopDiff;
		if (pRect->top < 0)
		{
			nTopDiff += pRect->top;
			pRect->top = 0;
		}

		pRect->bottom += (nHeightDifft - nTopDiff);
		if (pRect->bottom > nMaxY)
		{
			pRect->top -= (pRect->bottom - nMaxY);
			pRect->bottom = nMaxY;
			if (pRect->top < 0)
			{
				pRect->top = 0;
				pRect->bottom = nMaxY;
			}
		}
		
	}
}
*/


//���ڴ�С�仯�����Ӧ����
void CMFC_OpenCV_DLGDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������

	//���ô��ڿͻ����ĳ�ʼֵ(�ͻ����ʹ��ڴ�С��ͬ)
	if (!m_nMinClientWidth)
	{
		m_nMinClientWidth = cx;
	}
	if (!m_nMinClientHeight)
	{
		m_nMinClientHeight = cy;
	}

	m_dZoomX = (double)cx / (double)m_nMinClientWidth;		//����X��仯����
	m_dZoomY = (double)cy / (double)m_nMinClientHeight;		//����Y��仯����

	if (!m_bInitDLG)	//������δ��ʼ����ɣ������Ի�����£����������ڴ���ʳ�ͻ
	//��IsWindow(GetDlgItem(IDD_MFC_OPENCV_DLG_DIALOG)->GetSafeHwnd())�ж�
	{
		return;
	}

	DlgUpdata();		//���¶Ի���
	
}

/**
*	function:		DlgUpdata
*	description:	���¶Ի��򲼾֡�ͼƬ
*	parameter:		void
*	return:			void
**/
void CMFC_OpenCV_DLGDlg::DlgUpdata()
{
	//����һ:���ͼ��->�����ؼ�->ˢ�¿ؼ�->��ʾͼ�� ������˳�򲻻���ͼ�����
	//������:(��ǰ����)�����ؼ�->������Ч->�����ػ津��OnPaint����������ʾͼ��

	//���ͼ�񣬷�ֹ�ػ����ͼ������ڱ�Ŀؼ�
	//if (m_pImg_src)
	//{
	//	PrintBMP(&m_Bmp_src, NULL);
	//}
	//if (m_pImg_dst)
	//{
	//	PrintBMP(&m_Bmp_dst, NULL);
	//}


	CtrlAutoFit(m_dZoomX, m_dZoomY);	//�Զ����ڿؼ�
	
	CRect rect;

	m_Bmp_src.GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_NAME1)->ShowWindow( !(rect.top <= 25) );		//С������߶Ȳ���ʾ��ǩ��Ҫ����ͼ�����ǰ������������ͼ����

	m_Bmp_dst.GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_NAME2)->ShowWindow(!(rect.top <= 25));		//С������߶Ȳ���ʾ��ǩ��Ҫ����ͼ�����ǰ������������ͼ����


	//���»���ͼ��
	//if (m_pImg_src)	//�����ͼ�����»��ƺ��ʳߴ�ͼ��
	//{
	//	PrintBMP(&m_Bmp_src, m_pImg_src);		//��ͼ����ʾ��Picture Control
	//}
	//if (m_pImg_dst)	//�����ͼ�����»��ƺ��ʳߴ�ͼ��
	//{
	//	PrintBMP(&m_Bmp_dst, m_pImg_dst);		//��ͼ����ʾ��Picture Control
	//}

	//CtrlRefresh();						//�ؼ��ػ棬��ֹ�ؼ�����ͼ�����,����OnPaint����ͼ��
	Invalidate(FALSE);
}


/**
*	function:		CtrlInfo_Init
*	description:	����Ҫ�Զ���Ӧ���ڵĿؼ����г�ʼ��
*	parameter:		nID���ؼ�ID
*					b_Zoom:�Ƿ�Կؼ���������
*	return:			void
*	notice:			�ú���ֻ���ڿؼ��������������ʹ��
**/
ControlInfo* CMFC_OpenCV_DLGDlg::CtrlInfo_Init(int nID, bool b_Zoom, double dScaleLock /* = 0.0*/)
{
	ControlInfo* newCtrl = new ControlInfo;

	newCtrl->m_pWnd = GetDlgItem(nID);	//��ȡ�ؼ�ָ��

	newCtrl->m_bZoom = b_Zoom;			//�ؼ��Ƿ�����
	newCtrl->m_dScaleLock = dScaleLock;	//�ؼ���������
	newCtrl->m_pWnd->GetWindowRect(newCtrl->m_rectWnd);		//��ȡ�ؼ�ԭʼλ����Ϣ
	ScreenToClient(&newCtrl->m_rectWnd);
	newCtrl->m_nHeight = newCtrl->m_rectWnd.Height();
	newCtrl->m_nWidth = newCtrl->m_rectWnd.Width();
	newCtrl->m_x = newCtrl->m_rectWnd.left;
	newCtrl->m_y = newCtrl->m_rectWnd.top;

	m_CtrlList.Add(newCtrl);			//���ؼ���Ϣ���붯̬����
	return newCtrl;
}


/**
*	function:		CtrlAutoFit
*	description:	ʹ�ؼ����еĿؼ��Զ���Ӧ���ڴ�С�仯�������ʵ��ƶ�������
*	parameter:		f_ZoomX,f_ZoomX�����ű���
*	return:			void
*	notice:			�ú���ֻ���ڿؼ��������������ʹ��
**/
void CMFC_OpenCV_DLGDlg::CtrlAutoFit(double fZoomX, double fZoomY)
{
	unsigned long ListSize = (unsigned long)m_CtrlList.GetCount();	//�����С
	for (unsigned long i = 0; i < ListSize; i++)		//����ؼ����е���
	{
		ControlInfo* CtrlInfo = m_CtrlList.ElementAt((INT_PTR)i);	//�ؼ���Ϣ
		if (CtrlInfo->m_pWnd)	//�ж�NULL
		{
			int nDstX, nDstY, nDstWidth, nDstHeight;	//ת����ؼ��ߴ�
			if (CtrlInfo->m_bZoom)	//�������ŵĿؼ�
			{
				nDstX = int(fZoomX * CtrlInfo->m_x);		//��λ
				nDstY = int(fZoomY * CtrlInfo->m_y);
				nDstWidth = int(fZoomX * CtrlInfo->m_nWidth);		//����
				nDstHeight = int(fZoomY * CtrlInfo->m_nHeight);

				if (abs(CtrlInfo->m_dScaleLock) > 0.00000000000000000)	//��������
				{
					double dCurScale = double(nDstWidth) / double(nDstHeight);
					if (CtrlInfo->m_dScaleLock > dCurScale)
					{
						nDstHeight = int(nDstHeight * dCurScale / CtrlInfo->m_dScaleLock);
						nDstY = int(fZoomY * CtrlInfo->m_y + (fZoomY * CtrlInfo->m_nHeight - nDstHeight) / 2);
					}
					else
					{
						nDstWidth = int(nDstWidth * CtrlInfo->m_dScaleLock / dCurScale);
						nDstX = int(fZoomX * CtrlInfo->m_x + (fZoomX * CtrlInfo->m_nWidth - nDstWidth) / 2);
					}
				}
			}
			else
			{			//���������ŵĿؼ��������ֿؼ������ڴ��ڵ����λ��
				nDstX = int(fZoomX * CtrlInfo->m_x + (fZoomX * CtrlInfo->m_nWidth - CtrlInfo->m_nWidth) / 2);
				nDstY = int(fZoomY * CtrlInfo->m_y + (fZoomY * CtrlInfo->m_nHeight - CtrlInfo->m_nHeight) / 2);
				nDstWidth = int(CtrlInfo->m_nWidth);
				nDstHeight = int(CtrlInfo->m_nHeight);
			}			//���¿ؼ�
			CtrlInfo->m_pWnd->MoveWindow(nDstX, nDstY, nDstWidth, nDstHeight);
		}
	}

}

/**
*	function:		CtrlRefresh
*	description:	�ػ��ؼ����еĿؼ�
*	parameter:		void
*	return:			void
*	notice:			�ú���ֻ���ڿؼ��������������ʹ��
**/
void CMFC_OpenCV_DLGDlg::CtrlRefresh( )
{
	unsigned long ListSize = (unsigned long)m_CtrlList.GetCount();	//�����С
	for (unsigned long i = 0; i < ListSize; i++)		//����ؼ����е���
	{
		ControlInfo* CtrlInfo = m_CtrlList.ElementAt((INT_PTR)i);	//��ȡ�ؼ���Ϣ
		if (CtrlInfo->m_pWnd)	//�ж�NULL
		{
			CtrlInfo->m_pWnd->Invalidate();		//ʹ�ؼ���Ч��ǿ���ػ��ؼ�
		}
	}
}

/**
*	function:		DelCtrlList
*	description:	�ͷſؼ�������
*	parameter:		void
*	return:			void
**/
void CMFC_OpenCV_DLGDlg::DelCtrlList()
{
	//�����С
	unsigned long size = (unsigned long)m_CtrlList.GetCount();

	//���������ͷſռ�
	for (unsigned long i = 0; i < size; i++)
	{
		ControlInfo* CtrlInfo = m_CtrlList.ElementAt((INT_PTR)i);
		delete CtrlInfo;
	}

	//�������
	m_CtrlList.RemoveAll();
}







/***********************************�ؼ�����******************************************/


/**
*	function:		PrintBMP
*	description:	��ͼƬ��ʾ��Picture Control�ؼ�
*	parameter:		pPictureCtl:ͼ��ؼ�ָ��(��Ҫ�ȶԿؼ���ӿؼ�����)
*                   pImg��IplImageָ��
*	return:			void
**/
void CMFC_OpenCV_DLGDlg::PrintBMP(CStatic *pPictureCtl, IplImage * pImg)
{
	if (pImg == NULL)
	{
		pPictureCtl->SetBitmap(NULL);		//���λͼ
	}
	else
	{
		HBITMAP pHbitmap;
		CRect rect;
		pPictureCtl->GetWindowRect(&rect);		//��ȡͼ��ؼ�����
		pHbitmap = m_ImgProcessing.IplImage2hBitmap(pImg, rect.Width(), rect.Height());//ת���ɿؼ��ߴ��λͼ
		pPictureCtl->SetBitmap(pHbitmap);	//��ʾλͼ
		DeleteObject(pHbitmap);			//�ͷ��ڴ�
		pHbitmap = NULL;				//����ָ��
	}
}



/****************************�¼���Ӧ************************************/


//����������ͼ��
void CMFC_OpenCV_DLGDlg::OnBnClickedImgOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	// ���ù����������˳�֧��OpenCV�ļ�
	CString szOpenFilter = "JPEG (*.jpg;*.jpeg;*.jpe)|*.jpg;*.jpeg;*.jpe|PNG (*.png)|*.png|TIFF (*.tif;*.tiff)|*.tif;*.tiff|JPEG 2000 (*.jp2)|*.jp2|Sun rasters (*.sr;*.ras)|*.sr;*.ras|OpenEXR HDR (*.exr)|*.exr|λͼ�ļ� (*.bmp;*.dib)|*.bmp;*.dib|��Яʽͼ���ʽ (*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm|AVI (*.avi)|*.avi|����֧�ָ�ʽ|*.jpg;*.jpeg;*.jpe;*.png;*.tif;*.tiff;*.jp2;*.sr;*.ras;*.exr;*.bmp;*.dib;*.pbm;*.pgm;*.ppm;*.avi||";
	// ������ļ��Ի��� 
	CFileDialog OpenDlgScr(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szOpenFilter); 
	OpenDlgScr.m_pOFN->nFilterIndex = 10;	//Ĭ��ѡ��Ϊ���˳�����֧�ֵ�ͼ���ʽ
	if (OpenDlgScr.DoModal() == IDOK)
	{
		CString stFilePath_open = OpenDlgScr.GetPathName();
		CString stFileExt_open = OpenDlgScr.GetFileExt();
		stFileExt_open.MakeLower();

		//��ʱ���ͼ��
		IplImage *pTemp_Img = NULL;
		CvCapture *pTemp_Capture = NULL;
		
		//����ͼ���ļ�,����CheckBox�Ժڰ׻��ɫ��
		if (stFileExt_open == "avi")
		{
			pTemp_Capture = cvCreateFileCapture(stFilePath_open);
			if (pTemp_Capture)
			{
				pTemp_Img = cvCloneImage(cvQueryFrame(pTemp_Capture));
				if (IsDlgButtonChecked(IDC_CHECK_GRAY) && pTemp_Img)
				{
					IplImage *pGray_Img = cvCreateImage(cvGetSize(pTemp_Img), pTemp_Img->depth, 1);
					cvCvtColor(pTemp_Img, pGray_Img, CV_BGR2GRAY);
					cvReleaseImage(&pTemp_Img);
					pTemp_Img = pGray_Img;
				}
			}
		}
		else if (IsDlgButtonChecked(IDC_CHECK_GRAY))
		{
			pTemp_Img = cvLoadImage(stFilePath_open, CV_LOAD_IMAGE_GRAYSCALE);	//��ͨ����
		}
		else
		{
			pTemp_Img = cvLoadImage(stFilePath_open, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR); //CV_LOAD_IMAGE_UNCHANGED
		}
		
		if (pTemp_Img)	//�򿪳ɹ�
		{
			if (m_pCapture)//�������Ƶ
			{
				cvReleaseCapture(&m_pCapture);
			}
			m_pCapture = pTemp_Capture;

			if (m_pImg_src)//�����ͼ��
			{
				if (cvGetWindowHandle("ԭͼ " + m_stFilePath))
				{
					cvDestroyWindow("ԭͼ " + m_stFilePath);
				}
				cvReleaseImage(&m_pImg_src);
			}
			m_pImg_src = pTemp_Img;
			pTemp_Img = NULL;

			m_Bmp_src_Info->m_dScaleLock = double(m_pImg_src->width) / double(m_pImg_src->height);	//����ͼ�����

			DlgUpdata();		//���¶Ի���


			m_stFilePath = stFilePath_open;		//��¼�ļ�·��
			m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_SRC), "������ʾֱ��ͼ\n˫�����ԭͼ\n" + m_stFilePath);	//���ݸ���·��

			/*���ڱ�������*/
			/*
			m_dImgScale = double(m_pImg_src->width) / double(m_pImg_src->height);	//��ȡͼ�����

			CRect rect;
			int nWidth;
			int nHeight;
			double dScale;

			m_Bmp_src.GetWindowRect(&rect);
			dScale = double( rect.Width() ) / double( rect.Height() );		
			
			GetWindowRect(&rect);
			nWidth = rect.Width();
			nHeight = rect.Height();

			//���ڱ�������(����ʹ����С��������С��С����Сֵ)
			if (dScale > m_dImgScale)		//��ʾ�����߱�̫��(̫��)����С���
			{
				nWidth = int( nWidth * (m_dImgScale / dScale) );
				if (nWidth < m_ptMinTrackSize.x)	//��С��С����С��ȣ���Ϊ�Ŵ�߶�
				{
					nWidth = rect.Width();
					nHeight = int(nHeight * (dScale / m_dImgScale));
				}
			}
			else							//��ʾ�����߱�̫С(̫��),��С�߶�
			{
				nHeight = int( nHeight * (dScale / m_dImgScale) );
				if (nHeight < m_ptMinTrackSize.y)	//��С��С����С�߶ȣ���Ϊ�Ŵ���
				{
					nHeight = rect.Height();
					nWidth = int(nWidth * (m_dImgScale / dScale));
				}
			}

			m_dDlgScale = double(nWidth) / double(nHeight);		//��¼�����õĶԻ������
			MoveWindow(rect.left, rect.top, nWidth, nHeight);	//��������
			*/
			

		}
		else
		{
			AfxMessageBox(_T("�ļ���ʧ��,�����ļ�������׺��"), MB_ICONINFORMATION);
			return;
		}
	
	}
	else
		return;

}



//�ļ������(���봰�ڼ��ܴ򿪣��ܷ�����ָ���ؼ�)
void CMFC_OpenCV_DLGDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	char szFile[MAX_PATH];
	UINT n = DragQueryFile(hDropInfo, 0, szFile, MAX_PATH);
	DragFinish(hDropInfo);
	CString stFilePath_open = szFile;
	CString stFileExt_open = stFilePath_open.Mid(stFilePath_open.ReverseFind('.') + 1);
	stFileExt_open.MakeLower();

	//��ʱ���ͼ��
	IplImage *pTemp_Img = NULL;
	CvCapture *pTemp_Capture = NULL;

	//����ͼ���ļ�,����CheckBox�Ժڰ׻��ɫ��
	if (stFileExt_open == "avi")
	{
		pTemp_Capture = cvCreateFileCapture(stFilePath_open);
		if (pTemp_Capture)
		{
			pTemp_Img = cvCloneImage(cvQueryFrame(pTemp_Capture));
			if (IsDlgButtonChecked(IDC_CHECK_GRAY) && pTemp_Img)
			{
				IplImage *pGray_Img = cvCreateImage(cvGetSize(pTemp_Img), pTemp_Img->depth, 1);
				cvCvtColor(pTemp_Img, pGray_Img, CV_BGR2GRAY);
				cvReleaseImage(&pTemp_Img);
				pTemp_Img = pGray_Img;
			}
		}
	}
	else if (IsDlgButtonChecked(IDC_CHECK_GRAY))
	{
		pTemp_Img = cvLoadImage(stFilePath_open, CV_LOAD_IMAGE_GRAYSCALE);	//��ͨ����
	}
	else
	{
		pTemp_Img = cvLoadImage(stFilePath_open, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR); //CV_LOAD_IMAGE_UNCHANGED
	}

	if (pTemp_Img)	//�򿪳ɹ�
	{
		if (m_pCapture)//�������Ƶ
		{
			cvReleaseCapture(&m_pCapture);
		}
		m_pCapture = pTemp_Capture;

		if (m_pImg_src)//�����ͼ��
		{
			if (cvGetWindowHandle("ԭͼ " + m_stFilePath))
			{
				cvDestroyWindow("ԭͼ " + m_stFilePath);
			}
			cvReleaseImage(&m_pImg_src);
		}
		m_pImg_src = pTemp_Img;
		pTemp_Img = NULL;

		m_Bmp_src_Info->m_dScaleLock = double(m_pImg_src->width) / double(m_pImg_src->height);	//��ȡͼ�����

		DlgUpdata();		//���¶Ի���

		m_stFilePath = stFilePath_open;		//��¼�ļ�·��
		m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_SRC), "������ʾֱ��ͼ\n˫�����ԭͼ\n" + m_stFilePath);	//���ݸ���·��
	}
	else
	{
		AfxMessageBox(_T("�ļ���ʧ��,�����Ƿ�֧�ָ��ļ�����"), MB_ICONINFORMATION);
		return;
	}


	CDialogEx::OnDropFiles(hDropInfo);
}



//ͼƬ����
void CMFC_OpenCV_DLGDlg::OnBnClickedImgSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pImg_dst)
	{
		CString stFilePath_save = m_stFileToSavePath;		//��ȡ���������·��
		int nDot_index = stFilePath_save.ReverseFind('.');	//��ȡ��׺'.'λ��
		//��ȡ��׺��
		CString szFileType = stFilePath_save.Right(stFilePath_save.GetLength() - nDot_index);
		szFileType.MakeLower();	//ͳһ��׺Сд
		//�����ļ�������
		stFilePath_save.Insert(nDot_index, "_processed");
		// ���ù����������˳�֧��OpenCV�ļ�
		CString szOpenFilter = "JPEG (*.jpg;*.jpeg;*.jpe)|*.jpg;*.jpeg;*.jpe|PNG (*.png)|*.png|TIFF (*.tif;*.tiff)|*.tif;*.tiff|JPEG 2000 (*.jp2)|*.jp2|Sun rasters (*.sr;*.ras)|*.sr;*.ras|OpenEXR HDR (*.exr)|*.exr|λͼ�ļ� (*.bmp;*.dib)|*.bmp;*.dib|��Яʽͼ���ʽ (*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||";
		// ���챣���ļ��Ի��� 
		CFileDialog SaveDlgDst(FALSE, szFileType, stFilePath_save, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szOpenFilter);
	
		
		//ѡ��Ĭ���ļ�����
		if (szFileType == ".jpg" || szFileType == ".jpeg" || szFileType == ".jpe")
		{
			SaveDlgDst.m_pOFN->nFilterIndex = 1;
		}
		else if (szFileType == ".png")
		{
			SaveDlgDst.m_pOFN->nFilterIndex = 2;
		}
		else if (szFileType == ".tiff" || szFileType == ".tif")
		{
			SaveDlgDst.m_pOFN->nFilterIndex = 3;
		}
		else if (szFileType == ".jp2")
		{
			SaveDlgDst.m_pOFN->nFilterIndex = 4;
		}
		else if (szFileType == ".sr" || szFileType == ".ras")
		{
			SaveDlgDst.m_pOFN->nFilterIndex = 5;
		}
		else if (szFileType == ".exr")
		{
			SaveDlgDst.m_pOFN->nFilterIndex = 6;
		}
		else if (szFileType == ".bmp" || szFileType == ".dib")
		{
			SaveDlgDst.m_pOFN->nFilterIndex = 7;
		}
		else if (szFileType == ".pbm" || szFileType == ".ppm" || szFileType == ".pgm")
		{
			SaveDlgDst.m_pOFN->nFilterIndex = 8;
		}


		if (SaveDlgDst.DoModal() == IDOK)
		{
			stFilePath_save = SaveDlgDst.GetPathName();
			try
			{
				cvSaveImage(stFilePath_save, m_pImg_dst);
			}
			catch (cv::Exception e)
			{
				AfxMessageBox(_T("�ļ�����ʧ��,�����ļ�������׺��"), MB_ICONINFORMATION);
				return;
			}
			m_stFileSavePath = stFilePath_save;		//��¼�ļ�����·��
			m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_DST), "������ʾֱ��ͼ\n˫�����Ч��ͼ\n" + m_stFileSavePath);	//���ݸ���·��
		}
		else
			return;
	}
	else
	{
		AfxMessageBox(_T("δ����Ч��ͼ"), MB_ICONINFORMATION);
	}
}



//������õ�ͼ��Ϊԭͼ���ɼ�������
void CMFC_OpenCV_DLGDlg::OnBnClickedImgProcess2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pImg_dst)
	{
		if (m_stFileSavePath == "")
		{
			if (IDYES == AfxMessageBox(_T("Ч��ͼδ����,�Ƿ񱣴�?"), MB_YESNO))
			{
				OnBnClickedImgSave();
			}
		}
		if (m_pCapture)//�������Ƶ
		{
			cvReleaseCapture(&m_pCapture);
		}
		if (m_pImg_src)//���ԭͼ
		{
			if (cvGetWindowHandle("ԭͼ " + m_stFilePath))
			{
				cvDestroyWindow("ԭͼ " + m_stFilePath);
			}
			cvReleaseImage(&m_pImg_src);
		}
		m_pImg_src = m_pImg_dst;		//������õ�ͼ��Ϊԭͼ
		m_pImg_dst = NULL;

		m_Bmp_src_Info->m_dScaleLock = m_Bmp_dst_Info->m_dScaleLock;	//����
		
		DlgUpdata();		//���¶Ի���

		m_stFilePath = m_stFileSavePath;		//�ļ�·��
		m_stFileSavePath = "";
		m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_SRC), "������ʾֱ��ͼ\n˫�����ԭͼ\n" + m_stFilePath);	//���ݸ���·��
		m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_DST), "δ����Ч��ͼ");	//���ݸ���·��
	}
	else
	{
		AfxMessageBox(_T("δ����Ч��ͼ"), MB_ICONINFORMATION);
	}
}


void OnTrackBar(int pos, void *pDlg)
{
	//�Ի������ָ��
	CMFC_OpenCV_DLGDlg *pCurDlg = (CMFC_OpenCV_DLGDlg *)pDlg;
	//��ʱ���ͼ��
	IplImage *pTemp_Img = NULL;
	if (pCurDlg->m_pCapture)
	{
		cvSetCaptureProperty(pCurDlg->m_pCapture, CV_CAP_PROP_POS_FRAMES, (double)pos);
		pTemp_Img = cvCloneImage(cvQueryFrame(pCurDlg->m_pCapture));

		//��ͼ�����ԻҶ�ͼ�򿪣���ǰһ֡ͼ��ת�ɻҶ�ͼ
		if (pCurDlg->m_pImg_src->nChannels == 1 && pTemp_Img->nChannels != 1)
		{
			IplImage *pGray_Img = cvCreateImage(cvGetSize(pTemp_Img), pTemp_Img->depth, 1);
			cvCvtColor(pTemp_Img, pGray_Img, CV_BGR2GRAY);
			cvReleaseImage(&pTemp_Img);
			pTemp_Img = pGray_Img;
		}

		if (pTemp_Img)	//�򿪳ɹ�
		{
			if (pCurDlg->m_pImg_src)//�����ͼ��
			{
				cvReleaseImage(&pCurDlg->m_pImg_src);
			}
			pCurDlg->m_pImg_src = pTemp_Img;
			pTemp_Img = NULL;

			pCurDlg->m_Bmp_src_Info->m_dScaleLock = double(pCurDlg->m_pImg_src->width) / double(pCurDlg->m_pImg_src->height);	//����ͼ�����

			pCurDlg->DlgUpdata();		//���¶Ի���

			cvShowImage("ԭͼ " + pCurDlg->m_stFilePath, pCurDlg->m_pImg_src);
		}
		else
		{
			AfxMessageBox(_T("֡��ȡʧ��"), MB_ICONINFORMATION);
		}
	}
	else
	{
		AfxMessageBox(_T("��Ƶ�ѹر�"), MB_ICONINFORMATION);
		return;
	}
}

//˫����ʾԭͼ
void CMFC_OpenCV_DLGDlg::OnStnDblclickPictureSrc()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pImg_src)
	{
		cvNamedWindow("ԭͼ " + m_stFilePath, CV_WINDOW_NORMAL);
		cvShowImage("ԭͼ " + m_stFilePath, m_pImg_src);
		//cvNamedWindow("������", CV_WINDOW_NORMAL);
		//cvShowImage("������", pTemp_Img);
		if (m_pCapture)
		{
			cvCreateTrackbar2("TrackBar", "ԭͼ " + m_stFilePath, NULL, (int)cvGetCaptureProperty(m_pCapture, CV_CAP_PROP_FRAME_COUNT) - 1, OnTrackBar, this);
			cvSetTrackbarPos("TrackBar", "ԭͼ " + m_stFilePath, (int)cvGetCaptureProperty(m_pCapture, CV_CAP_PROP_POS_FRAMES) - 1);
		}
	}
	else
	{
		//AfxMessageBox(_T("δ����ԭͼ"), MB_ICONINFORMATION);
		OnBnClickedImgOpen();	//���ô򿪰�ť
	}
}


//˫����ʾЧ��ͼ
void CMFC_OpenCV_DLGDlg::OnStnDblclickPictureDst()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pImg_dst)
	{
		cvNamedWindow("Ч��ͼ " + m_stFileSavePath, CV_WINDOW_NORMAL);
		cvShowImage("Ч��ͼ " + m_stFileSavePath, m_pImg_dst);
	}
	else
	{
		AfxMessageBox(_T("δ����ͼ����"), MB_ICONINFORMATION);
	}
}



//������ʾԭͼֱ��ͼ
void CMFC_OpenCV_DLGDlg::OnStnClickedPictureSrc()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pImg_src)	//����Ƿ��лҶ�ͼ
	{
		if (m_pImg_src->nChannels == 1)	//��ͨ��ֱ�Ӽ��㣬���ƻҶ�ֱ��ͼ
		{
			if (m_ImgProcessing.GetHistogram(m_pImg_src, m_Hist.pGrayHist, &m_Hist.fGrayMax))
			{
				m_pDrawHist->ShowGrayHist(&m_Hist, "ԭͼֱ��ͼ");	//���ƻҶ�ֱ��ͼ
			}
			else
			{
				AfxMessageBox(_T("ֱ��ͼ����ʧ��"), MB_ICONINFORMATION);
			}
		}
		else
		{
			//���ɻҶ�ͼ������RGB�����Խ���ֱ��ͼ����
			IplImage *pGray_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
			IplImage *pBlue_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
			IplImage *pGreen_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
			IplImage *pRed_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
			bool bValidGray, bValidR, bValidG, bValidB;
			//����Ҷ�ֱ��ͼ
			cvCvtColor(m_pImg_src, pGray_Img, CV_BGR2GRAY);
			bValidGray = m_ImgProcessing.GetHistogram(pGray_Img, m_Hist.pGrayHist, &m_Hist.fGrayMax);
			//����RGBֱ��ͼ
			cvSplit(m_pImg_src, pBlue_Img, pGreen_Img, pRed_Img, NULL);
			bValidR = m_ImgProcessing.GetHistogram(pRed_Img, m_Hist.pRedHist, &m_Hist.fRedMax);
			bValidG = m_ImgProcessing.GetHistogram(pBlue_Img, m_Hist.pBlueHist, &m_Hist.fBlueMax);
			bValidB = m_ImgProcessing.GetHistogram(pGreen_Img, m_Hist.pGreenHist, &m_Hist.fGreenMax);
			if (bValidGray && bValidR && bValidG && bValidB)
			{
				m_pDrawHist->ShowRGBHist(&m_Hist, "ԭͼֱ��ͼ");	//���ƻҶȡ�RGBֱ��ͼ
			}
			else
			{
				AfxMessageBox(_T("ֱ��ͼ����ʧ��"), MB_ICONINFORMATION);
			}
			cvReleaseImage(&pGray_Img);
			cvReleaseImage(&pRed_Img);
			cvReleaseImage(&pGreen_Img);
			cvReleaseImage(&pBlue_Img);
		}
		
	}
	else	//δ����ԭͼ�����ô�
	{
		OnBnClickedImgOpen();	//���ô򿪰�ť
	//	  AfxMessageBox(_T("��ԭͼ�Ҷ�ͼ��"), MB_ICONINFORMATION);
	}
}

//������ʾЧ��ͼֱ��ͼ
void CMFC_OpenCV_DLGDlg::OnStnClickedPictureDst()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (m_pImg_dst)	//����Ƿ���ͼ
	{
		if (m_pImg_dst->nChannels == 1)	//��ͨ��ֱ�Ӽ��㣬���ƻҶ�ֱ��ͼ
		{
			if (m_ImgProcessing.GetHistogram(m_pImg_dst, m_Hist.pGrayHist, &m_Hist.fGrayMax))
			{
				m_pDrawHist->ShowGrayHist(&m_Hist, "Ч��ͼֱ��ͼ");	//���ƻҶ�ֱ��ͼ
			}
			else
			{
				AfxMessageBox(_T("ֱ��ͼ����ʧ��"), MB_ICONINFORMATION);
			}
		}
		else
		{
			//���ɻҶ�ͼ������RGB�����Խ���ֱ��ͼ����
			IplImage *pGray_Img = cvCreateImage(cvGetSize(m_pImg_dst), m_pImg_dst->depth, 1);
			IplImage *pBlue_Img = cvCreateImage(cvGetSize(m_pImg_dst), m_pImg_dst->depth, 1);
			IplImage *pGreen_Img = cvCreateImage(cvGetSize(m_pImg_dst), m_pImg_dst->depth, 1);
			IplImage *pRed_Img = cvCreateImage(cvGetSize(m_pImg_dst), m_pImg_dst->depth, 1);
			bool bValidGray,bValidR, bValidG, bValidB;
			//����Ҷ�ֱ��ͼ
			cvCvtColor(m_pImg_dst,pGray_Img,CV_BGR2GRAY);
			bValidGray = m_ImgProcessing.GetHistogram(pGray_Img, m_Hist.pGrayHist, &m_Hist.fGrayMax);
			//����RGBֱ��ͼ
			cvSplit(m_pImg_dst, pBlue_Img, pGreen_Img, pRed_Img, NULL);
			bValidR = m_ImgProcessing.GetHistogram(pRed_Img, m_Hist.pRedHist, &m_Hist.fRedMax);
			bValidG = m_ImgProcessing.GetHistogram(pBlue_Img, m_Hist.pBlueHist, &m_Hist.fBlueMax);
			bValidB = m_ImgProcessing.GetHistogram(pGreen_Img, m_Hist.pGreenHist, &m_Hist.fGreenMax);
			if (bValidGray && bValidR && bValidG && bValidB)
			{
				m_pDrawHist->ShowRGBHist(&m_Hist, "Ч��ͼֱ��ͼ");	//���ƻҶȡ�RGBֱ��ͼ
			}
			else
			{
				AfxMessageBox(_T("ֱ��ͼ����ʧ��"), MB_ICONINFORMATION);
			}
			cvReleaseImage(&pGray_Img);
			cvReleaseImage(&pRed_Img);
			cvReleaseImage(&pGreen_Img);
			cvReleaseImage(&pBlue_Img);
		}
	}
	//else
	//{
	//	AfxMessageBox(_T("��Ч��ͼ�Ҷ�ͼ��"), MB_ICONINFORMATION);
	//}

}




//���½���ͼ����
void CMFC_OpenCV_DLGDlg::OnBnClickedImgProcess()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pImg_src && m_FuncBox.GetCurSel())	//�ж��Ƿ��ԭͼ�Լ�ѡ�д���Ч��
	{
		bool bResult = FALSE;		//�Ƿ���ɹ�
		bool bInput = TRUE;		//�Ƿ�������

		//��ʱ��Ŵ�����ͼ��
		IplImage *pTemp_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, m_pImg_src->nChannels);

		LARGE_INTEGER nTime1, nTime2, nFrequency;		//CPU������Ƶ�ʣ����ڸ߾��ȼ�ʱ
		CString szTime;		//��ʾ��ʱ

		
		int nSel = m_FuncBox.GetCurSel();	//ѡ�д���������
		CString szSel;						
		m_FuncBox.GetLBText(nSel, szSel);	//��ȡ�ô�����������Ϊ���ô��ڱ���

		if (bInput = IDOK == m_Mode_Dlg.ShowModeDLG(nSel, szSel))		//��������ģʽ���öԻ���
		{
			int nSelMode = *(m_Mode_Dlg.m_pnModeSel + nSel - 1);		//����ģʽ
			//ͼ����
			switch (nSel)
			{
				case ADD_NOISE_INDEX:	//�Ҷȷ�ת
				{
					QueryPerformanceCounter(&nTime1); //��ʼ������
					//��ò���
					float fNoisePSNR = m_Mode_Dlg.m_fNoisePSNR;
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.AddGuassianNoise(m_pImg_src, pTemp_Img, fNoisePSNR);
					}
					else if(1 == nSelMode)
					{
						bResult = m_ImgProcessing.AddPepperSaltNoise(m_pImg_src, pTemp_Img, fNoisePSNR);
					}
					QueryPerformanceCounter(&nTime2); //����������  
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  
					break;
				}
				case GRAY_REV_INDEX:	//�Ҷȷ�ת
				{
					QueryPerformanceCounter(&nTime1); //��ʼ������
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.GrayReversion(m_pImg_src, pTemp_Img);
					}
					else
					{
						cvNot(m_pImg_src, pTemp_Img); bResult = TRUE;
					}
					QueryPerformanceCounter(&nTime2); //����������  
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  
					break;
				}


				case LOG_INDEX:	//�����任
				{
					//��ò���
					float fConst = m_Mode_Dlg.m_fLogConst;
					QueryPerformanceCounter(&nTime1); //��ʼ������ 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.LogCorrection_Table(m_pImg_src, pTemp_Img, fConst);
					}
					else
					{
						bResult = m_ImgProcessing.LogCorrection(m_pImg_src, pTemp_Img, fConst);
					}
					QueryPerformanceCounter(&nTime2); //����������
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  		
					break;
				}


				case REVLOG_INDEX:	//�������任
				{
					//��ò���
					float fConst = m_Mode_Dlg.m_fLogConst;
					QueryPerformanceCounter(&nTime1); //��ʼ������ 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.RevLogCorrection_Table(m_pImg_src, pTemp_Img, fConst);
					}
					else
					{
						bResult = m_ImgProcessing.RevLogCorrection(m_pImg_src, pTemp_Img, fConst);
					}
					QueryPerformanceCounter(&nTime2); //����������
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  		
					break;
				}

				case GAMMA_INDEX:	//ڤ�α任
				{
					//��ò���
					float fGamma = m_Mode_Dlg.m_fGamma;
					QueryPerformanceCounter(&nTime1); //��ʼ������ 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.GammaCorrection_Table(m_pImg_src, pTemp_Img, fGamma);
					}
					else
					{
						bResult = m_ImgProcessing.GammaCorrection(m_pImg_src, pTemp_Img, fGamma);
					}
					QueryPerformanceCounter(&nTime2); //����������
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  
					break;
				}

				case EQU_HIST_INDEX://ֱ��ͼ���⻯
				{
					//ֱ��ͼ���⻯�������ʺϴ���Ҷ�ͼ
					
					QueryPerformanceCounter(&nTime1); //��ʼ������ 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.EqualizeHist(m_pImg_src, pTemp_Img);
					}
					else
					{
						if (m_pImg_src->nChannels == 1)
						{
							cvEqualizeHist(m_pImg_src, pTemp_Img);
							bResult = TRUE;
						}
						else if (m_pImg_src->nChannels == 3)
						{
							IplImage *pImgYCrCb = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 3);
							IplImage *pChannelY = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
							IplImage *pChannelCr = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
							IplImage *pChannelCb = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
							IplImage *pResult = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);	//��ʱ�洢������
							cvCvtColor(m_pImg_src, pImgYCrCb, CV_BGR2YCrCb);
							cvSplit(pImgYCrCb, pChannelY, pChannelCr, pChannelCb, NULL);
							cvEqualizeHist(pChannelY, pResult);
							cvMerge(pResult, pChannelCr, pChannelCb, NULL, pImgYCrCb);
							cvCvtColor(pImgYCrCb, pTemp_Img, CV_YCrCb2BGR);

							cvReleaseImage(&pImgYCrCb);
							cvReleaseImage(&pChannelY);
							cvReleaseImage(&pChannelCr);
							cvReleaseImage(&pChannelCb);
							cvReleaseImage(&pResult);
							bResult = TRUE;
						}

					}

					QueryPerformanceCounter(&nTime2); //����������  
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  
						
					if (!bResult)
					{
						AfxMessageBox(_T("����ʧ��,���ԻҶ�ͼ�򿪲�����"), MB_ICONINFORMATION);
					}

					break;
				}

				case BLUR_INDEX:
				{
					//��ò���
					int nSize = m_Mode_Dlg.m_nBlurSize;

					QueryPerformanceCounter(&nTime1); //��ʼ������ 
					if (0 == nSelMode)//����ͼ����
					{
						bResult = m_ImgProcessing.Smooth_Blur_Table(m_pImg_src, pTemp_Img, nSize);
					}
					else if (1 == nSelMode)
					{
						bResult = m_ImgProcessing.Smooth_Blur(m_pImg_src, pTemp_Img, nSize);
					}
					else//openCV����
					{
						cvSmooth(m_pImg_src, pTemp_Img, CV_BLUR, nSize);
						bResult = TRUE;
					}
					QueryPerformanceCounter(&nTime2); //����������
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  
					break;
				}

				case MEDIAN_INDEX:
				{
					//��ò���
					int nSize = m_Mode_Dlg.m_nMedianSize;

					if (nSelMode > 0 && nSelMode < 7 && nSize > 10)
					{
						if (IDOK != AfxMessageBox(_T("ģ��ϴ�ʱ,�����������ֵ�˲�Ч�ʺܵ�,�Ƿ����?"), MB_ICONINFORMATION || MB_OKCANCEL))
						{
							bInput = FALSE;
							break;
						}
					}
					QueryPerformanceCounter(&nTime1); //��ʼ������ 

					if (0 == nSelMode)	//����ֱ��ͼ
					{
						bResult = m_ImgProcessing.Median_Blur_Hist(m_pImg_src, pTemp_Img, nSize);
					}
					else if (1 == nSelMode)		//������ĭ����
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Bubble_Sort));
					}
					else if (2 == nSelMode)		//����ѡ������
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Selection_Sort));
					}
					else if (3 == nSelMode)		//���ڲ�������
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Insertion_Sort));
					}
					else if (4 == nSelMode)		//���ڿ�������
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Quick_Sort));
					}
					else if (5 == nSelMode)		//����Shell����
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Shell_Sort));
					}
					else if (6 == nSelMode)		//����MSD��������
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::RadixMSD_Sort));
					}
					else//openCV����
					{
						cvSmooth(m_pImg_src, pTemp_Img, CV_MEDIAN, nSize);
						bResult = TRUE;
					}

					QueryPerformanceCounter(&nTime2); //����������
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  
					break;
				}

				case GAUSSIAN_INDEX://��˹�˲�
				{
					//��ò���
					int nSize = m_Mode_Dlg.m_nGaussianSize;
					float fSigma = m_Mode_Dlg.m_fGaussianSigma;
					QueryPerformanceCounter(&nTime1); //��ʼ������ 

					if (0 == nSelMode)//�ɷ����˲���
					{
						bResult = m_ImgProcessing.Smooth_Gaussian_SepFilter(m_pImg_src, pTemp_Img, nSize, fSigma);
					}
					else if (1 == nSelMode)//��ͨ��˹�˲�
					{
						bResult = m_ImgProcessing.Smooth_Gaussian(m_pImg_src, pTemp_Img, nSize, fSigma);
					}
					else//openCV����
					{
						cvSmooth(m_pImg_src, pTemp_Img, CV_GAUSSIAN, nSize, nSize, fSigma, fSigma);
						bResult = TRUE;
					}

					QueryPerformanceCounter(&nTime2); //����������
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��

					break;
				}

				case BILATERAL_INDEX:
				{
					//��ò���
					int nSize = m_Mode_Dlg.m_nBilateralSize;
					float fSigma_Color = m_Mode_Dlg.m_fBilateral_Sigma_Color;
					float fSigma_Spcae = m_Mode_Dlg.m_fBilateral_Sigma_Space;

					QueryPerformanceCounter(&nTime1); //��ʼ������ 

					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.BilateralFilter(m_pImg_src, pTemp_Img, nSize, fSigma_Color, fSigma_Spcae);
					}
					else//openCV����
					{
						cvSmooth(m_pImg_src, pTemp_Img, CV_BILATERAL, nSize, nSize, fSigma_Color, fSigma_Spcae);
						bResult = TRUE;
					}

					QueryPerformanceCounter(&nTime2); //����������
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��
					break;
				}


				case SOBEL_INDEX://Sobel��Ե���
				{
					QueryPerformanceCounter(&nTime1); //��ʼ������ 

					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.Sobel(m_pImg_src, pTemp_Img);
					}
					else//openCV����
					{
						IplImage *p16S_Img_X = cvCreateImage(cvSize(m_pImg_src->width, m_pImg_src->height), IPL_DEPTH_16S, m_pImg_src->nChannels);
						IplImage *p16S_Img_Y = cvCreateImage(cvSize(m_pImg_src->width, m_pImg_src->height), IPL_DEPTH_16S, m_pImg_src->nChannels);
						cvSobel(m_pImg_src, p16S_Img_X, 1, 0, 3);	//x������
						cvSobel(m_pImg_src, p16S_Img_Y, 0, 1, 3);	//y������
						//�����������,Ҳ�� sqrt(abs(nSum_X) * abs(nSum_X) + abs(nSum_Y) * abs(nSum_Y))
						//��abs(nSum_X) + abs(nSum_Y)���������ֵ���Ҹ���
						cvAbs(p16S_Img_X, p16S_Img_X);
						cvAbs(p16S_Img_Y, p16S_Img_Y);
						cvAdd(p16S_Img_X, p16S_Img_Y, p16S_Img_X);
						cvConvertScaleAbs(p16S_Img_X, pTemp_Img);
						cvReleaseImage(&p16S_Img_X);
						cvReleaseImage(&p16S_Img_Y);
						bResult = TRUE;
					}
				
					QueryPerformanceCounter(&nTime2); //����������  
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  
					break;
				}

				case MOTION_INDEX:
				{
					if (!m_pCapture)
					{
						AfxMessageBox(_T("δ������Ƶ"), MB_ICONINFORMATION);
						OnBnClickedImgOpen();	//���ô򿪰�ť
						break;
					}
					int CurFrame = (int)cvGetCaptureProperty(m_pCapture, CV_CAP_PROP_POS_FRAMES) - 1;
					int PreFrame = CurFrame - 1;
					bool bNewImage = FALSE;
					IplImage *pPreImage = NULL;
					if (PreFrame < 0)
					{
						PreFrame = 0;
					}

					//��ȡǰһ֡ͼ��
					cvSetCaptureProperty(m_pCapture, CV_CAP_PROP_POS_FRAMES, (double)PreFrame);
					pPreImage = cvQueryFrame(m_pCapture);
					cvSetCaptureProperty(m_pCapture, CV_CAP_PROP_POS_FRAMES, (double)(CurFrame + 1));
					if (!pPreImage)
					{
						break;
					}
					//��ͼ�����ԻҶ�ͼ�򿪣���ǰһ֡ͼ��ת�ɻҶ�ͼ
					if (m_pImg_src->nChannels == 1 && pPreImage->nChannels != 1)
					{
						bNewImage = TRUE;
						IplImage *pGray_Img = cvCreateImage(cvGetSize(pPreImage), pPreImage->depth, 1);
						cvCvtColor(pPreImage, pGray_Img, CV_BGR2GRAY);
						pPreImage = pGray_Img;
					}
					
					QueryPerformanceCounter(&nTime1); //��ʼ������ 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.Motion_Estimation(pPreImage, m_pImg_src, pTemp_Img, m_Mode_Dlg.m_nSchRange);
					}
					QueryPerformanceCounter(&nTime2); //����������  
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  

					if (bNewImage)
					{
						cvReleaseImage(&pPreImage);
					}
					break;
				}

				case DIFF_INDEX:
				{
					if (!m_pImg_dst)
					{
						AfxMessageBox(_T("δ����Ч��ͼ"), MB_ICONINFORMATION);
						break;
					}
					if (m_pImg_src->width != m_pImg_dst->width || m_pImg_src->height != m_pImg_dst->height || m_pImg_src->nChannels != m_pImg_dst->nChannels || m_pImg_src->depth != m_pImg_dst->depth)
					{
						AfxMessageBox(_T("ԭͼ��Ч��ͼ������һ�£�"), MB_ICONINFORMATION);
						break;
					}
					QueryPerformanceCounter(&nTime1); //��ʼ������ 
					if (0 == nSelMode)
					{
						cvAbsDiff(m_pImg_src, m_pImg_dst, pTemp_Img);
						bResult = true;
					}
					QueryPerformanceCounter(&nTime2); //����������  
					QueryPerformanceFrequency(&nFrequency);//������Ƶ��  
					break;
				}

				default:break;
			}
		}
		

		if (!bResult)	//����ʧ�ܣ��ͷſռ�
		{
			cvReleaseImage(&pTemp_Img);
			return;
		}

		//����ɹ�

		//���㴦��ʱ��
		LARGE_INTEGER run_count;
		run_count.QuadPart = nTime2.QuadPart - nTime1.QuadPart;//����ʱ�������  
		double run_time = (double)run_count.QuadPart / (double)nFrequency.QuadPart * 1000; //����ʱ��(ms)
		szTime.Format(_T("����ʱ��:%5.2fms"), run_time);
		GetDlgItem(IDC_TIME)->ShowWindow(TRUE);
		SetDlgItemText(IDC_TIME, szTime);

		//ѯ���Ƿ񱣴棬�������ͼ��
		if (m_pImg_dst)
		{
			if (m_stFileSavePath == "")
			{
				if (IDYES == AfxMessageBox(_T("Ч��ͼδ����,�Ƿ񱣴�?"), MB_YESNO))
				{
					OnBnClickedImgSave();
				}
			}
			cvReleaseImage(&m_pImg_dst);
		}
		m_pImg_dst = pTemp_Img;		//ָ��ָ����õ�ͼ��
		pTemp_Img = NULL;

		//�����ؼ�,��ʾͼƬ
		m_Bmp_dst_Info->m_dScaleLock = m_Bmp_src_Info->m_dScaleLock;	//��ԭͼ������ͬ
		DlgUpdata();		//���¶Ի���


		m_stFileSavePath = "";					//Ч��ͼδ���棬���·��
		m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_DST), "������ʾֱ��ͼ\n˫�����Ч��ͼ\n" + m_stFileSavePath);	//���ݸ���·��
		if (m_stFilePath != "")					//��ԭͼ��·�������Ǵ������ͼ��Ϊԭͼ�ٴ���ģ����Ҹ�ͼδ����
		{
			m_stFileToSavePath = m_stFilePath;		//���������·��
		}
	}
	else
	{	//������ʾ
		if (!m_pImg_src)
		{
			AfxMessageBox(_T("δ����ԭͼ"), MB_ICONINFORMATION);
			OnBnClickedImgOpen();	//���ô򿪰�ť
		}
		else
		{
			AfxMessageBox(_T("��ѡ����Ч��"), MB_ICONINFORMATION);
			m_FuncBox.ShowDropDown(TRUE);		//��������Ч���б���ʾѡ��
		}

	}
}




