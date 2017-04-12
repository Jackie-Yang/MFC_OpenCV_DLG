
// MFC_OpenCV_DLGDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_OpenCV_DLG.h"
#include "MFC_OpenCV_DLGDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFC_OpenCV_DLGDlg 对话框


CMFC_OpenCV_DLGDlg::CMFC_OpenCV_DLGDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_OpenCV_DLGDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImg_src = NULL;
	m_pImg_dst = NULL;
	m_pCapture = NULL;
	//直方图数据初始化
	memset(m_Hist.pGrayHist, 0, sizeof(float) * 256);
	memset(m_Hist.pRedHist, 0, sizeof(float) * 256);
	memset(m_Hist.pGreenHist, 0, sizeof(float) * 256);
	memset(m_Hist.pBlueHist, 0, sizeof(float) * 256);
	//创建一个非模态对话框用作直方图显示
	m_pDrawHist = new CDrawHist(); //分配内存用于创建非模态对话框
	m_pDrawHist->Create(IDD_DRAWHIST); //创建一个非模态对话框  

	m_nMinClientWidth = 0;		//客户区最小尺寸
	m_nMinClientHeight = 0;

	m_bInitDLG = FALSE;		//设置状态，窗口未初始化

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
	DelCtrlList();	//释放控件表数组
	delete m_pDrawHist;//释放直方图对话框内存

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


// CMFC_OpenCV_DLGDlg 消息处理程序

BOOL CMFC_OpenCV_DLGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标



	// TODO:  在此添加额外的初始化代码


	//获取窗口的初始大小
	CRect	rect;
	GetWindowRect(rect);
	//将初始值设为最小值
	m_ptMinTrackSize.x = (LONG)rect.Width();
	m_ptMinTrackSize.y = (LONG)rect.Height();


	//初始化动态控件调整
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

	//((CButton*)GetDlgItem(IDC_CHECK_GRAY))->SetCheck(TRUE);	//默认打开灰度图

	//初始化效果列表
	m_FuncBox.InsertString(0, "请选择处理效果");
	m_FuncBox.InsertString(ADD_NOISE_INDEX, "添加噪声");
	m_FuncBox.InsertString(GRAY_REV_INDEX, "灰度反转");
	m_FuncBox.InsertString(LOG_INDEX, "对数变换");
	m_FuncBox.InsertString(REVLOG_INDEX, "反对数变换");
	m_FuncBox.InsertString(GAMMA_INDEX, "冥次变换");
	m_FuncBox.InsertString(EQU_HIST_INDEX, "直方图均衡化");
	m_FuncBox.InsertString(BLUR_INDEX, "均值滤波");
	m_FuncBox.InsertString(MEDIAN_INDEX, "中值滤波");
	m_FuncBox.InsertString(GAUSSIAN_INDEX, "高斯滤波");
	m_FuncBox.InsertString(BILATERAL_INDEX, "双边滤波");
	m_FuncBox.InsertString(SOBEL_INDEX, "Sobel边缘检测");
	m_FuncBox.InsertString(MOTION_INDEX, "运动估计");
	m_FuncBox.InsertString(DIFF_INDEX, "图像误差");
	m_FuncBox.SetCurSel(0);

	EnableToolTips(TRUE);		//提示信息，鼠标放在控件上显示提示
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.SetMaxTipWidth(150);		//设定宽度才能换行
	m_ToolTip.SetDelayTime(TTDT_INITIAL, 800);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,30000);
	m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_SRC), "未载入原图");
	m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_DST), "未生成效果图");
	m_ToolTip.AddTool(GetDlgItem(IDC_CHECK_GRAY), "以灰度图方式打开");
	m_ToolTip.AddTool(GetDlgItem(IDC_IMG_PROCESS2), "效果图作为原图继续处理");
	m_ToolTip.AddTool(GetDlgItem(IDC_IMG_SAVE), "保存效果图");

	m_bInitDLG = TRUE;	//设置窗口状态，窗口初始化完成

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_OpenCV_DLGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//更新图像
		if (m_pImg_src)
		{
			PrintBMP(&m_Bmp_src, m_pImg_src);		//将图像显示到Picture Control
		}
		if (m_pImg_dst)
		{
			PrintBMP(&m_Bmp_dst, m_pImg_dst);		//将图像显示到Picture Control
		}
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_OpenCV_DLGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL CMFC_OpenCV_DLGDlg::PreTranslateMessage(MSG *pMsg)
{
	m_ToolTip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}


/*****************************************布局控制*************************************************/




//设置窗口最小尺寸
void CMFC_OpenCV_DLGDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize = m_ptMinTrackSize;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


/*窗口比例控制*/
/*

m_dDlgScale = double(rect.Width()) / double(rect.Height());	//设置窗口初始比例


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

	// TODO:  在此处添加消息处理程序代码


	int nWidth;
	int nHeight;
	double dScale = double(nWidth) / double(nHeight);


	CRect rect;				//原始窗口矩形
	GetWindowRect(&rect);

	if (fwSide == 1)		//Left: 1 
	{
		int nWidth = pRect->right - rect.left;

		int nMaxY = GetSystemMetrics(SM_CYSCREEN);

		int nHeightDifft = int(nWidth / m_dDlgScale) - rect.Height();	//高度增量
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


//窗口大小变化后的响应处理
void CMFC_OpenCV_DLGDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码

	//设置窗口客户区的初始值(客户区和窗口大小不同)
	if (!m_nMinClientWidth)
	{
		m_nMinClientWidth = cx;
	}
	if (!m_nMinClientHeight)
	{
		m_nMinClientHeight = cy;
	}

	m_dZoomX = (double)cx / (double)m_nMinClientWidth;		//窗口X轴变化比例
	m_dZoomY = (double)cy / (double)m_nMinClientHeight;		//窗口Y轴变化比例

	if (!m_bInitDLG)	//若窗口未初始化完成，不进对话框更新，否则会造成内存访问冲突
	//或IsWindow(GetDlgItem(IDD_MFC_OPENCV_DLG_DIALOG)->GetSafeHwnd())判断
	{
		return;
	}

	DlgUpdata();		//更新对话框
	
}

/**
*	function:		DlgUpdata
*	description:	更新对话框布局、图片
*	parameter:		void
*	return:			void
**/
void CMFC_OpenCV_DLGDlg::DlgUpdata()
{
	//方案一:清空图像->调整控件->刷新控件->显示图像 这样的顺序不会有图像残留
	//方案二:(当前测试)调整控件->窗口无效->窗口重绘触发OnPaint并在里面显示图像

	//清空图像，防止重绘过程图像残留在别的控件
	//if (m_pImg_src)
	//{
	//	PrintBMP(&m_Bmp_src, NULL);
	//}
	//if (m_pImg_dst)
	//{
	//	PrintBMP(&m_Bmp_dst, NULL);
	//}


	CtrlAutoFit(m_dZoomX, m_dZoomY);	//自动调节控件
	
	CRect rect;

	m_Bmp_src.GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_NAME1)->ShowWindow( !(rect.top <= 25) );		//小于这个高度不显示标签，要放在图像更新前，否则会残留在图像上

	m_Bmp_dst.GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_NAME2)->ShowWindow(!(rect.top <= 25));		//小于这个高度不显示标签，要放在图像更新前，否则会残留在图像上


	//重新绘制图像
	//if (m_pImg_src)	//清除旧图像重新绘制合适尺寸图像
	//{
	//	PrintBMP(&m_Bmp_src, m_pImg_src);		//将图像显示到Picture Control
	//}
	//if (m_pImg_dst)	//清除旧图像重新绘制合适尺寸图像
	//{
	//	PrintBMP(&m_Bmp_dst, m_pImg_dst);		//将图像显示到Picture Control
	//}

	//CtrlRefresh();						//控件重绘，防止控件上有图像残留,触发OnPaint更新图像
	Invalidate(FALSE);
}


/**
*	function:		CtrlInfo_Init
*	description:	将需要自动适应窗口的控件进行初始化
*	parameter:		nID：控件ID
*					b_Zoom:是否对控件进行缩放
*	return:			void
*	notice:			该函数只能在控件所属窗体的类中使用
**/
ControlInfo* CMFC_OpenCV_DLGDlg::CtrlInfo_Init(int nID, bool b_Zoom, double dScaleLock /* = 0.0*/)
{
	ControlInfo* newCtrl = new ControlInfo;

	newCtrl->m_pWnd = GetDlgItem(nID);	//获取控件指针

	newCtrl->m_bZoom = b_Zoom;			//控件是否缩放
	newCtrl->m_dScaleLock = dScaleLock;	//控件比例控制
	newCtrl->m_pWnd->GetWindowRect(newCtrl->m_rectWnd);		//获取控件原始位置信息
	ScreenToClient(&newCtrl->m_rectWnd);
	newCtrl->m_nHeight = newCtrl->m_rectWnd.Height();
	newCtrl->m_nWidth = newCtrl->m_rectWnd.Width();
	newCtrl->m_x = newCtrl->m_rectWnd.left;
	newCtrl->m_y = newCtrl->m_rectWnd.top;

	m_CtrlList.Add(newCtrl);			//将控件信息加入动态数组
	return newCtrl;
}


/**
*	function:		CtrlAutoFit
*	description:	使控件表中的控件自动适应窗口大小变化，进行适当移动、缩放
*	parameter:		f_ZoomX,f_ZoomX：缩放比例
*	return:			void
*	notice:			该函数只能在控件所属窗体的类中使用
**/
void CMFC_OpenCV_DLGDlg::CtrlAutoFit(double fZoomX, double fZoomY)
{
	unsigned long ListSize = (unsigned long)m_CtrlList.GetCount();	//数组大小
	for (unsigned long i = 0; i < ListSize; i++)		//逐个控件进行调整
	{
		ControlInfo* CtrlInfo = m_CtrlList.ElementAt((INT_PTR)i);	//控件信息
		if (CtrlInfo->m_pWnd)	//判断NULL
		{
			int nDstX, nDstY, nDstWidth, nDstHeight;	//转换后控件尺寸
			if (CtrlInfo->m_bZoom)	//进行缩放的控件
			{
				nDstX = int(fZoomX * CtrlInfo->m_x);		//移位
				nDstY = int(fZoomY * CtrlInfo->m_y);
				nDstWidth = int(fZoomX * CtrlInfo->m_nWidth);		//缩放
				nDstHeight = int(fZoomY * CtrlInfo->m_nHeight);

				if (abs(CtrlInfo->m_dScaleLock) > 0.00000000000000000)	//比例调整
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
			{			//不进行缩放的控件，仅保持控件中心在窗口的相对位置
				nDstX = int(fZoomX * CtrlInfo->m_x + (fZoomX * CtrlInfo->m_nWidth - CtrlInfo->m_nWidth) / 2);
				nDstY = int(fZoomY * CtrlInfo->m_y + (fZoomY * CtrlInfo->m_nHeight - CtrlInfo->m_nHeight) / 2);
				nDstWidth = int(CtrlInfo->m_nWidth);
				nDstHeight = int(CtrlInfo->m_nHeight);
			}			//更新控件
			CtrlInfo->m_pWnd->MoveWindow(nDstX, nDstY, nDstWidth, nDstHeight);
		}
	}

}

/**
*	function:		CtrlRefresh
*	description:	重画控件表中的控件
*	parameter:		void
*	return:			void
*	notice:			该函数只能在控件所属窗体的类中使用
**/
void CMFC_OpenCV_DLGDlg::CtrlRefresh( )
{
	unsigned long ListSize = (unsigned long)m_CtrlList.GetCount();	//数组大小
	for (unsigned long i = 0; i < ListSize; i++)		//逐个控件进行调整
	{
		ControlInfo* CtrlInfo = m_CtrlList.ElementAt((INT_PTR)i);	//获取控件信息
		if (CtrlInfo->m_pWnd)	//判断NULL
		{
			CtrlInfo->m_pWnd->Invalidate();		//使控件无效、强制重画控件
		}
	}
}

/**
*	function:		DelCtrlList
*	description:	释放控件表数组
*	parameter:		void
*	return:			void
**/
void CMFC_OpenCV_DLGDlg::DelCtrlList()
{
	//数组大小
	unsigned long size = (unsigned long)m_CtrlList.GetCount();

	//遍历数组释放空间
	for (unsigned long i = 0; i < size; i++)
	{
		ControlInfo* CtrlInfo = m_CtrlList.ElementAt((INT_PTR)i);
		delete CtrlInfo;
	}

	//清空数组
	m_CtrlList.RemoveAll();
}







/***********************************控件操作******************************************/


/**
*	function:		PrintBMP
*	description:	将图片显示到Picture Control控件
*	parameter:		pPictureCtl:图像控件指针(需要先对控件添加控件变量)
*                   pImg：IplImage指针
*	return:			void
**/
void CMFC_OpenCV_DLGDlg::PrintBMP(CStatic *pPictureCtl, IplImage * pImg)
{
	if (pImg == NULL)
	{
		pPictureCtl->SetBitmap(NULL);		//清空位图
	}
	else
	{
		HBITMAP pHbitmap;
		CRect rect;
		pPictureCtl->GetWindowRect(&rect);		//获取图像控件矩形
		pHbitmap = m_ImgProcessing.IplImage2hBitmap(pImg, rect.Width(), rect.Height());//转换成控件尺寸的位图
		pPictureCtl->SetBitmap(pHbitmap);	//显示位图
		DeleteObject(pHbitmap);			//释放内存
		pHbitmap = NULL;				//清零指针
	}
}



/****************************事件响应************************************/


//按下载入新图像
void CMFC_OpenCV_DLGDlg::OnBnClickedImgOpen()
{
	// TODO:  在此添加控件通知处理程序代码

	// 设置过滤器，过滤出支持OpenCV文件
	CString szOpenFilter = "JPEG (*.jpg;*.jpeg;*.jpe)|*.jpg;*.jpeg;*.jpe|PNG (*.png)|*.png|TIFF (*.tif;*.tiff)|*.tif;*.tiff|JPEG 2000 (*.jp2)|*.jp2|Sun rasters (*.sr;*.ras)|*.sr;*.ras|OpenEXR HDR (*.exr)|*.exr|位图文件 (*.bmp;*.dib)|*.bmp;*.dib|便携式图像格式 (*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm|AVI (*.avi)|*.avi|所有支持格式|*.jpg;*.jpeg;*.jpe;*.png;*.tif;*.tiff;*.jp2;*.sr;*.ras;*.exr;*.bmp;*.dib;*.pbm;*.pgm;*.ppm;*.avi||";
	// 构造打开文件对话框 
	CFileDialog OpenDlgScr(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szOpenFilter); 
	OpenDlgScr.m_pOFN->nFilterIndex = 10;	//默认选项为过滤出所有支持的图像格式
	if (OpenDlgScr.DoModal() == IDOK)
	{
		CString stFilePath_open = OpenDlgScr.GetPathName();
		CString stFileExt_open = OpenDlgScr.GetFileExt();
		stFileExt_open.MakeLower();

		//临时存放图像
		IplImage *pTemp_Img = NULL;
		CvCapture *pTemp_Capture = NULL;
		
		//载入图像文件,根据CheckBox以黑白或彩色打开
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
			pTemp_Img = cvLoadImage(stFilePath_open, CV_LOAD_IMAGE_GRAYSCALE);	//单通道打开
		}
		else
		{
			pTemp_Img = cvLoadImage(stFilePath_open, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR); //CV_LOAD_IMAGE_UNCHANGED
		}
		
		if (pTemp_Img)	//打开成功
		{
			if (m_pCapture)//清除旧视频
			{
				cvReleaseCapture(&m_pCapture);
			}
			m_pCapture = pTemp_Capture;

			if (m_pImg_src)//清除旧图像
			{
				if (cvGetWindowHandle("原图 " + m_stFilePath))
				{
					cvDestroyWindow("原图 " + m_stFilePath);
				}
				cvReleaseImage(&m_pImg_src);
			}
			m_pImg_src = pTemp_Img;
			pTemp_Img = NULL;

			m_Bmp_src_Info->m_dScaleLock = double(m_pImg_src->width) / double(m_pImg_src->height);	//更新图像比例

			DlgUpdata();		//更新对话框


			m_stFilePath = stFilePath_open;		//记录文件路径
			m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_SRC), "单击显示直方图\n双击浏览原图\n" + m_stFilePath);	//气泡更新路径

			/*窗口比例控制*/
			/*
			m_dImgScale = double(m_pImg_src->width) / double(m_pImg_src->height);	//获取图像比例

			CRect rect;
			int nWidth;
			int nHeight;
			double dScale;

			m_Bmp_src.GetWindowRect(&rect);
			dScale = double( rect.Width() ) / double( rect.Height() );		
			
			GetWindowRect(&rect);
			nWidth = rect.Width();
			nHeight = rect.Height();

			//窗口比例调整(优先使用缩小，除非缩小后小于最小值)
			if (dScale > m_dImgScale)		//显示区域宽高比太大(太宽)，缩小宽度
			{
				nWidth = int( nWidth * (m_dImgScale / dScale) );
				if (nWidth < m_ptMinTrackSize.x)	//缩小后小于最小宽度，改为放大高度
				{
					nWidth = rect.Width();
					nHeight = int(nHeight * (dScale / m_dImgScale));
				}
			}
			else							//显示区域宽高比太小(太高),缩小高度
			{
				nHeight = int( nHeight * (dScale / m_dImgScale) );
				if (nHeight < m_ptMinTrackSize.y)	//缩小后小于最小高度，改为放大宽度
				{
					nHeight = rect.Height();
					nWidth = int(nWidth * (m_dImgScale / dScale));
				}
			}

			m_dDlgScale = double(nWidth) / double(nHeight);		//记录调整好的对话框比例
			MoveWindow(rect.left, rect.top, nWidth, nHeight);	//调整窗口
			*/
			

		}
		else
		{
			AfxMessageBox(_T("文件打开失败,请检查文件名及后缀名"), MB_ICONINFORMATION);
			return;
		}
	
	}
	else
		return;

}



//文件拖入打开(拖入窗口即能打开，能否拖入指定控件)
void CMFC_OpenCV_DLGDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	char szFile[MAX_PATH];
	UINT n = DragQueryFile(hDropInfo, 0, szFile, MAX_PATH);
	DragFinish(hDropInfo);
	CString stFilePath_open = szFile;
	CString stFileExt_open = stFilePath_open.Mid(stFilePath_open.ReverseFind('.') + 1);
	stFileExt_open.MakeLower();

	//临时存放图像
	IplImage *pTemp_Img = NULL;
	CvCapture *pTemp_Capture = NULL;

	//载入图像文件,根据CheckBox以黑白或彩色打开
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
		pTemp_Img = cvLoadImage(stFilePath_open, CV_LOAD_IMAGE_GRAYSCALE);	//单通道打开
	}
	else
	{
		pTemp_Img = cvLoadImage(stFilePath_open, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR); //CV_LOAD_IMAGE_UNCHANGED
	}

	if (pTemp_Img)	//打开成功
	{
		if (m_pCapture)//清除旧视频
		{
			cvReleaseCapture(&m_pCapture);
		}
		m_pCapture = pTemp_Capture;

		if (m_pImg_src)//清除旧图像
		{
			if (cvGetWindowHandle("原图 " + m_stFilePath))
			{
				cvDestroyWindow("原图 " + m_stFilePath);
			}
			cvReleaseImage(&m_pImg_src);
		}
		m_pImg_src = pTemp_Img;
		pTemp_Img = NULL;

		m_Bmp_src_Info->m_dScaleLock = double(m_pImg_src->width) / double(m_pImg_src->height);	//获取图像比例

		DlgUpdata();		//更新对话框

		m_stFilePath = stFilePath_open;		//记录文件路径
		m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_SRC), "单击显示直方图\n双击浏览原图\n" + m_stFilePath);	//气泡更新路径
	}
	else
	{
		AfxMessageBox(_T("文件打开失败,请检查是否支持该文件类型"), MB_ICONINFORMATION);
		return;
	}


	CDialogEx::OnDropFiles(hDropInfo);
}



//图片保存
void CMFC_OpenCV_DLGDlg::OnBnClickedImgSave()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pImg_dst)
	{
		CString stFilePath_save = m_stFileToSavePath;		//获取即将保存的路径
		int nDot_index = stFilePath_save.ReverseFind('.');	//获取后缀'.'位置
		//获取后缀名
		CString szFileType = stFilePath_save.Right(stFilePath_save.GetLength() - nDot_index);
		szFileType.MakeLower();	//统一后缀小写
		//保存文件重命名
		stFilePath_save.Insert(nDot_index, "_processed");
		// 设置过滤器，过滤出支持OpenCV文件
		CString szOpenFilter = "JPEG (*.jpg;*.jpeg;*.jpe)|*.jpg;*.jpeg;*.jpe|PNG (*.png)|*.png|TIFF (*.tif;*.tiff)|*.tif;*.tiff|JPEG 2000 (*.jp2)|*.jp2|Sun rasters (*.sr;*.ras)|*.sr;*.ras|OpenEXR HDR (*.exr)|*.exr|位图文件 (*.bmp;*.dib)|*.bmp;*.dib|便携式图像格式 (*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||";
		// 构造保存文件对话框 
		CFileDialog SaveDlgDst(FALSE, szFileType, stFilePath_save, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szOpenFilter);
	
		
		//选择默认文件类型
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
				AfxMessageBox(_T("文件保存失败,请检查文件名及后缀名"), MB_ICONINFORMATION);
				return;
			}
			m_stFileSavePath = stFilePath_save;		//记录文件保存路径
			m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_DST), "单击显示直方图\n双击浏览效果图\n" + m_stFileSavePath);	//气泡更新路径
		}
		else
			return;
	}
	else
	{
		AfxMessageBox(_T("未生成效果图"), MB_ICONINFORMATION);
	}
}



//将处理好的图作为原图，可继续处理
void CMFC_OpenCV_DLGDlg::OnBnClickedImgProcess2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pImg_dst)
	{
		if (m_stFileSavePath == "")
		{
			if (IDYES == AfxMessageBox(_T("效果图未保存,是否保存?"), MB_YESNO))
			{
				OnBnClickedImgSave();
			}
		}
		if (m_pCapture)//清除旧视频
		{
			cvReleaseCapture(&m_pCapture);
		}
		if (m_pImg_src)//清除原图
		{
			if (cvGetWindowHandle("原图 " + m_stFilePath))
			{
				cvDestroyWindow("原图 " + m_stFilePath);
			}
			cvReleaseImage(&m_pImg_src);
		}
		m_pImg_src = m_pImg_dst;		//将处理好的图作为原图
		m_pImg_dst = NULL;

		m_Bmp_src_Info->m_dScaleLock = m_Bmp_dst_Info->m_dScaleLock;	//比例
		
		DlgUpdata();		//更新对话框

		m_stFilePath = m_stFileSavePath;		//文件路径
		m_stFileSavePath = "";
		m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_SRC), "单击显示直方图\n双击浏览原图\n" + m_stFilePath);	//气泡更新路径
		m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_DST), "未生成效果图");	//气泡更新路径
	}
	else
	{
		AfxMessageBox(_T("未生成效果图"), MB_ICONINFORMATION);
	}
}


void OnTrackBar(int pos, void *pDlg)
{
	//对话框对象指针
	CMFC_OpenCV_DLGDlg *pCurDlg = (CMFC_OpenCV_DLGDlg *)pDlg;
	//临时存放图像
	IplImage *pTemp_Img = NULL;
	if (pCurDlg->m_pCapture)
	{
		cvSetCaptureProperty(pCurDlg->m_pCapture, CV_CAP_PROP_POS_FRAMES, (double)pos);
		pTemp_Img = cvCloneImage(cvQueryFrame(pCurDlg->m_pCapture));

		//若图像是以灰度图打开，则将前一帧图像转成灰度图
		if (pCurDlg->m_pImg_src->nChannels == 1 && pTemp_Img->nChannels != 1)
		{
			IplImage *pGray_Img = cvCreateImage(cvGetSize(pTemp_Img), pTemp_Img->depth, 1);
			cvCvtColor(pTemp_Img, pGray_Img, CV_BGR2GRAY);
			cvReleaseImage(&pTemp_Img);
			pTemp_Img = pGray_Img;
		}

		if (pTemp_Img)	//打开成功
		{
			if (pCurDlg->m_pImg_src)//清除旧图像
			{
				cvReleaseImage(&pCurDlg->m_pImg_src);
			}
			pCurDlg->m_pImg_src = pTemp_Img;
			pTemp_Img = NULL;

			pCurDlg->m_Bmp_src_Info->m_dScaleLock = double(pCurDlg->m_pImg_src->width) / double(pCurDlg->m_pImg_src->height);	//更新图像比例

			pCurDlg->DlgUpdata();		//更新对话框

			cvShowImage("原图 " + pCurDlg->m_stFilePath, pCurDlg->m_pImg_src);
		}
		else
		{
			AfxMessageBox(_T("帧截取失败"), MB_ICONINFORMATION);
		}
	}
	else
	{
		AfxMessageBox(_T("视频已关闭"), MB_ICONINFORMATION);
		return;
	}
}

//双击显示原图
void CMFC_OpenCV_DLGDlg::OnStnDblclickPictureSrc()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pImg_src)
	{
		cvNamedWindow("原图 " + m_stFilePath, CV_WINDOW_NORMAL);
		cvShowImage("原图 " + m_stFilePath, m_pImg_src);
		//cvNamedWindow("进度条", CV_WINDOW_NORMAL);
		//cvShowImage("进度条", pTemp_Img);
		if (m_pCapture)
		{
			cvCreateTrackbar2("TrackBar", "原图 " + m_stFilePath, NULL, (int)cvGetCaptureProperty(m_pCapture, CV_CAP_PROP_FRAME_COUNT) - 1, OnTrackBar, this);
			cvSetTrackbarPos("TrackBar", "原图 " + m_stFilePath, (int)cvGetCaptureProperty(m_pCapture, CV_CAP_PROP_POS_FRAMES) - 1);
		}
	}
	else
	{
		//AfxMessageBox(_T("未加载原图"), MB_ICONINFORMATION);
		OnBnClickedImgOpen();	//调用打开按钮
	}
}


//双击显示效果图
void CMFC_OpenCV_DLGDlg::OnStnDblclickPictureDst()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pImg_dst)
	{
		cvNamedWindow("效果图 " + m_stFileSavePath, CV_WINDOW_NORMAL);
		cvShowImage("效果图 " + m_stFileSavePath, m_pImg_dst);
	}
	else
	{
		AfxMessageBox(_T("未进行图像处理"), MB_ICONINFORMATION);
	}
}



//单击显示原图直方图
void CMFC_OpenCV_DLGDlg::OnStnClickedPictureSrc()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pImg_src)	//检测是否有灰度图
	{
		if (m_pImg_src->nChannels == 1)	//单通道直接计算，绘制灰度直方图
		{
			if (m_ImgProcessing.GetHistogram(m_pImg_src, m_Hist.pGrayHist, &m_Hist.fGrayMax))
			{
				m_pDrawHist->ShowGrayHist(&m_Hist, "原图直方图");	//绘制灰度直方图
			}
			else
			{
				AfxMessageBox(_T("直方图绘制失败"), MB_ICONINFORMATION);
			}
		}
		else
		{
			//生成灰度图，分离RGB，各自进行直方图计算
			IplImage *pGray_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
			IplImage *pBlue_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
			IplImage *pGreen_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
			IplImage *pRed_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);
			bool bValidGray, bValidR, bValidG, bValidB;
			//计算灰度直方图
			cvCvtColor(m_pImg_src, pGray_Img, CV_BGR2GRAY);
			bValidGray = m_ImgProcessing.GetHistogram(pGray_Img, m_Hist.pGrayHist, &m_Hist.fGrayMax);
			//计算RGB直方图
			cvSplit(m_pImg_src, pBlue_Img, pGreen_Img, pRed_Img, NULL);
			bValidR = m_ImgProcessing.GetHistogram(pRed_Img, m_Hist.pRedHist, &m_Hist.fRedMax);
			bValidG = m_ImgProcessing.GetHistogram(pBlue_Img, m_Hist.pBlueHist, &m_Hist.fBlueMax);
			bValidB = m_ImgProcessing.GetHistogram(pGreen_Img, m_Hist.pGreenHist, &m_Hist.fGreenMax);
			if (bValidGray && bValidR && bValidG && bValidB)
			{
				m_pDrawHist->ShowRGBHist(&m_Hist, "原图直方图");	//绘制灰度、RGB直方图
			}
			else
			{
				AfxMessageBox(_T("直方图绘制失败"), MB_ICONINFORMATION);
			}
			cvReleaseImage(&pGray_Img);
			cvReleaseImage(&pRed_Img);
			cvReleaseImage(&pGreen_Img);
			cvReleaseImage(&pBlue_Img);
		}
		
	}
	else	//未载入原图，调用打开
	{
		OnBnClickedImgOpen();	//调用打开按钮
	//	  AfxMessageBox(_T("无原图灰度图像"), MB_ICONINFORMATION);
	}
}

//单击显示效果图直方图
void CMFC_OpenCV_DLGDlg::OnStnClickedPictureDst()
{
	// TODO:  在此添加控件通知处理程序代码

	if (m_pImg_dst)	//检测是否有图
	{
		if (m_pImg_dst->nChannels == 1)	//单通道直接计算，绘制灰度直方图
		{
			if (m_ImgProcessing.GetHistogram(m_pImg_dst, m_Hist.pGrayHist, &m_Hist.fGrayMax))
			{
				m_pDrawHist->ShowGrayHist(&m_Hist, "效果图直方图");	//绘制灰度直方图
			}
			else
			{
				AfxMessageBox(_T("直方图绘制失败"), MB_ICONINFORMATION);
			}
		}
		else
		{
			//生成灰度图，分离RGB，各自进行直方图计算
			IplImage *pGray_Img = cvCreateImage(cvGetSize(m_pImg_dst), m_pImg_dst->depth, 1);
			IplImage *pBlue_Img = cvCreateImage(cvGetSize(m_pImg_dst), m_pImg_dst->depth, 1);
			IplImage *pGreen_Img = cvCreateImage(cvGetSize(m_pImg_dst), m_pImg_dst->depth, 1);
			IplImage *pRed_Img = cvCreateImage(cvGetSize(m_pImg_dst), m_pImg_dst->depth, 1);
			bool bValidGray,bValidR, bValidG, bValidB;
			//计算灰度直方图
			cvCvtColor(m_pImg_dst,pGray_Img,CV_BGR2GRAY);
			bValidGray = m_ImgProcessing.GetHistogram(pGray_Img, m_Hist.pGrayHist, &m_Hist.fGrayMax);
			//计算RGB直方图
			cvSplit(m_pImg_dst, pBlue_Img, pGreen_Img, pRed_Img, NULL);
			bValidR = m_ImgProcessing.GetHistogram(pRed_Img, m_Hist.pRedHist, &m_Hist.fRedMax);
			bValidG = m_ImgProcessing.GetHistogram(pBlue_Img, m_Hist.pBlueHist, &m_Hist.fBlueMax);
			bValidB = m_ImgProcessing.GetHistogram(pGreen_Img, m_Hist.pGreenHist, &m_Hist.fGreenMax);
			if (bValidGray && bValidR && bValidG && bValidB)
			{
				m_pDrawHist->ShowRGBHist(&m_Hist, "效果图直方图");	//绘制灰度、RGB直方图
			}
			else
			{
				AfxMessageBox(_T("直方图绘制失败"), MB_ICONINFORMATION);
			}
			cvReleaseImage(&pGray_Img);
			cvReleaseImage(&pRed_Img);
			cvReleaseImage(&pGreen_Img);
			cvReleaseImage(&pBlue_Img);
		}
	}
	//else
	//{
	//	AfxMessageBox(_T("无效果图灰度图像"), MB_ICONINFORMATION);
	//}

}




//按下进行图像处理
void CMFC_OpenCV_DLGDlg::OnBnClickedImgProcess()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pImg_src && m_FuncBox.GetCurSel())	//判断是否打开原图以及选中处理效果
	{
		bool bResult = FALSE;		//是否处理成功
		bool bInput = TRUE;		//是否有输入

		//临时存放处理完图像
		IplImage *pTemp_Img = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, m_pImg_src->nChannels);

		LARGE_INTEGER nTime1, nTime2, nFrequency;		//CPU计数及频率，用于高精度计时
		CString szTime;		//显示计时

		
		int nSel = m_FuncBox.GetCurSel();	//选中处理函数索引
		CString szSel;						
		m_FuncBox.GetLBText(nSel, szSel);	//获取该处理函数名称作为设置窗口标题

		if (bInput = IDOK == m_Mode_Dlg.ShowModeDLG(nSel, szSel))		//弹出处理模式设置对话框
		{
			int nSelMode = *(m_Mode_Dlg.m_pnModeSel + nSel - 1);		//处理模式
			//图像处理
			switch (nSel)
			{
				case ADD_NOISE_INDEX:	//灰度反转
				{
					QueryPerformanceCounter(&nTime1); //起始计数器
					//获得参数
					float fNoisePSNR = m_Mode_Dlg.m_fNoisePSNR;
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.AddGuassianNoise(m_pImg_src, pTemp_Img, fNoisePSNR);
					}
					else if(1 == nSelMode)
					{
						bResult = m_ImgProcessing.AddPepperSaltNoise(m_pImg_src, pTemp_Img, fNoisePSNR);
					}
					QueryPerformanceCounter(&nTime2); //结束计数器  
					QueryPerformanceFrequency(&nFrequency);//计数器频率  
					break;
				}
				case GRAY_REV_INDEX:	//灰度反转
				{
					QueryPerformanceCounter(&nTime1); //起始计数器
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.GrayReversion(m_pImg_src, pTemp_Img);
					}
					else
					{
						cvNot(m_pImg_src, pTemp_Img); bResult = TRUE;
					}
					QueryPerformanceCounter(&nTime2); //结束计数器  
					QueryPerformanceFrequency(&nFrequency);//计数器频率  
					break;
				}


				case LOG_INDEX:	//对数变换
				{
					//获得参数
					float fConst = m_Mode_Dlg.m_fLogConst;
					QueryPerformanceCounter(&nTime1); //起始计数器 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.LogCorrection_Table(m_pImg_src, pTemp_Img, fConst);
					}
					else
					{
						bResult = m_ImgProcessing.LogCorrection(m_pImg_src, pTemp_Img, fConst);
					}
					QueryPerformanceCounter(&nTime2); //结束计数器
					QueryPerformanceFrequency(&nFrequency);//计数器频率  		
					break;
				}


				case REVLOG_INDEX:	//反对数变换
				{
					//获得参数
					float fConst = m_Mode_Dlg.m_fLogConst;
					QueryPerformanceCounter(&nTime1); //起始计数器 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.RevLogCorrection_Table(m_pImg_src, pTemp_Img, fConst);
					}
					else
					{
						bResult = m_ImgProcessing.RevLogCorrection(m_pImg_src, pTemp_Img, fConst);
					}
					QueryPerformanceCounter(&nTime2); //结束计数器
					QueryPerformanceFrequency(&nFrequency);//计数器频率  		
					break;
				}

				case GAMMA_INDEX:	//冥次变换
				{
					//获得参数
					float fGamma = m_Mode_Dlg.m_fGamma;
					QueryPerformanceCounter(&nTime1); //起始计数器 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.GammaCorrection_Table(m_pImg_src, pTemp_Img, fGamma);
					}
					else
					{
						bResult = m_ImgProcessing.GammaCorrection(m_pImg_src, pTemp_Img, fGamma);
					}
					QueryPerformanceCounter(&nTime2); //结束计数器
					QueryPerformanceFrequency(&nFrequency);//计数器频率  
					break;
				}

				case EQU_HIST_INDEX://直方图均衡化
				{
					//直方图均衡化函数仅适合处理灰度图
					
					QueryPerformanceCounter(&nTime1); //起始计数器 
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
							IplImage *pResult = cvCreateImage(cvGetSize(m_pImg_src), m_pImg_src->depth, 1);	//临时存储输出结果
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

					QueryPerformanceCounter(&nTime2); //结束计数器  
					QueryPerformanceFrequency(&nFrequency);//计数器频率  
						
					if (!bResult)
					{
						AfxMessageBox(_T("处理失败,请以灰度图打开并重试"), MB_ICONINFORMATION);
					}

					break;
				}

				case BLUR_INDEX:
				{
					//获得参数
					int nSize = m_Mode_Dlg.m_nBlurSize;

					QueryPerformanceCounter(&nTime1); //起始计数器 
					if (0 == nSelMode)//积分图计算
					{
						bResult = m_ImgProcessing.Smooth_Blur_Table(m_pImg_src, pTemp_Img, nSize);
					}
					else if (1 == nSelMode)
					{
						bResult = m_ImgProcessing.Smooth_Blur(m_pImg_src, pTemp_Img, nSize);
					}
					else//openCV函数
					{
						cvSmooth(m_pImg_src, pTemp_Img, CV_BLUR, nSize);
						bResult = TRUE;
					}
					QueryPerformanceCounter(&nTime2); //结束计数器
					QueryPerformanceFrequency(&nFrequency);//计数器频率  
					break;
				}

				case MEDIAN_INDEX:
				{
					//获得参数
					int nSize = m_Mode_Dlg.m_nMedianSize;

					if (nSelMode > 0 && nSelMode < 7 && nSize > 10)
					{
						if (IDOK != AfxMessageBox(_T("模板较大时,基于排序的中值滤波效率很低,是否继续?"), MB_ICONINFORMATION || MB_OKCANCEL))
						{
							bInput = FALSE;
							break;
						}
					}
					QueryPerformanceCounter(&nTime1); //起始计数器 

					if (0 == nSelMode)	//基于直方图
					{
						bResult = m_ImgProcessing.Median_Blur_Hist(m_pImg_src, pTemp_Img, nSize);
					}
					else if (1 == nSelMode)		//基于泡沫排序
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Bubble_Sort));
					}
					else if (2 == nSelMode)		//基于选择排序
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Selection_Sort));
					}
					else if (3 == nSelMode)		//基于插入排序
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Insertion_Sort));
					}
					else if (4 == nSelMode)		//基于快速排序
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Quick_Sort));
					}
					else if (5 == nSelMode)		//基于Shell排序
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::Shell_Sort));
					}
					else if (6 == nSelMode)		//基于MSD基数排序
					{
						bResult = m_ImgProcessing.Median_Blur_Sort(m_pImg_src, pTemp_Img, nSize, &(CImgProcessing::RadixMSD_Sort));
					}
					else//openCV函数
					{
						cvSmooth(m_pImg_src, pTemp_Img, CV_MEDIAN, nSize);
						bResult = TRUE;
					}

					QueryPerformanceCounter(&nTime2); //结束计数器
					QueryPerformanceFrequency(&nFrequency);//计数器频率  
					break;
				}

				case GAUSSIAN_INDEX://高斯滤波
				{
					//获得参数
					int nSize = m_Mode_Dlg.m_nGaussianSize;
					float fSigma = m_Mode_Dlg.m_fGaussianSigma;
					QueryPerformanceCounter(&nTime1); //起始计数器 

					if (0 == nSelMode)//可分离滤波器
					{
						bResult = m_ImgProcessing.Smooth_Gaussian_SepFilter(m_pImg_src, pTemp_Img, nSize, fSigma);
					}
					else if (1 == nSelMode)//普通高斯滤波
					{
						bResult = m_ImgProcessing.Smooth_Gaussian(m_pImg_src, pTemp_Img, nSize, fSigma);
					}
					else//openCV函数
					{
						cvSmooth(m_pImg_src, pTemp_Img, CV_GAUSSIAN, nSize, nSize, fSigma, fSigma);
						bResult = TRUE;
					}

					QueryPerformanceCounter(&nTime2); //结束计数器
					QueryPerformanceFrequency(&nFrequency);//计数器频率

					break;
				}

				case BILATERAL_INDEX:
				{
					//获得参数
					int nSize = m_Mode_Dlg.m_nBilateralSize;
					float fSigma_Color = m_Mode_Dlg.m_fBilateral_Sigma_Color;
					float fSigma_Spcae = m_Mode_Dlg.m_fBilateral_Sigma_Space;

					QueryPerformanceCounter(&nTime1); //起始计数器 

					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.BilateralFilter(m_pImg_src, pTemp_Img, nSize, fSigma_Color, fSigma_Spcae);
					}
					else//openCV函数
					{
						cvSmooth(m_pImg_src, pTemp_Img, CV_BILATERAL, nSize, nSize, fSigma_Color, fSigma_Spcae);
						bResult = TRUE;
					}

					QueryPerformanceCounter(&nTime2); //结束计数器
					QueryPerformanceFrequency(&nFrequency);//计数器频率
					break;
				}


				case SOBEL_INDEX://Sobel边缘检测
				{
					QueryPerformanceCounter(&nTime1); //起始计数器 

					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.Sobel(m_pImg_src, pTemp_Img);
					}
					else//openCV函数
					{
						IplImage *p16S_Img_X = cvCreateImage(cvSize(m_pImg_src->width, m_pImg_src->height), IPL_DEPTH_16S, m_pImg_src->nChannels);
						IplImage *p16S_Img_Y = cvCreateImage(cvSize(m_pImg_src->width, m_pImg_src->height), IPL_DEPTH_16S, m_pImg_src->nChannels);
						cvSobel(m_pImg_src, p16S_Img_X, 1, 0, 3);	//x方向求导
						cvSobel(m_pImg_src, p16S_Img_Y, 0, 1, 3);	//y方向求导
						//两方向导数结合,也可 sqrt(abs(nSum_X) * abs(nSum_X) + abs(nSum_Y) * abs(nSum_Y))
						//但abs(nSum_X) + abs(nSum_Y)能求出近似值，且更快
						cvAbs(p16S_Img_X, p16S_Img_X);
						cvAbs(p16S_Img_Y, p16S_Img_Y);
						cvAdd(p16S_Img_X, p16S_Img_Y, p16S_Img_X);
						cvConvertScaleAbs(p16S_Img_X, pTemp_Img);
						cvReleaseImage(&p16S_Img_X);
						cvReleaseImage(&p16S_Img_Y);
						bResult = TRUE;
					}
				
					QueryPerformanceCounter(&nTime2); //结束计数器  
					QueryPerformanceFrequency(&nFrequency);//计数器频率  
					break;
				}

				case MOTION_INDEX:
				{
					if (!m_pCapture)
					{
						AfxMessageBox(_T("未载入视频"), MB_ICONINFORMATION);
						OnBnClickedImgOpen();	//调用打开按钮
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

					//获取前一帧图像
					cvSetCaptureProperty(m_pCapture, CV_CAP_PROP_POS_FRAMES, (double)PreFrame);
					pPreImage = cvQueryFrame(m_pCapture);
					cvSetCaptureProperty(m_pCapture, CV_CAP_PROP_POS_FRAMES, (double)(CurFrame + 1));
					if (!pPreImage)
					{
						break;
					}
					//若图像是以灰度图打开，则将前一帧图像转成灰度图
					if (m_pImg_src->nChannels == 1 && pPreImage->nChannels != 1)
					{
						bNewImage = TRUE;
						IplImage *pGray_Img = cvCreateImage(cvGetSize(pPreImage), pPreImage->depth, 1);
						cvCvtColor(pPreImage, pGray_Img, CV_BGR2GRAY);
						pPreImage = pGray_Img;
					}
					
					QueryPerformanceCounter(&nTime1); //起始计数器 
					if (0 == nSelMode)
					{
						bResult = m_ImgProcessing.Motion_Estimation(pPreImage, m_pImg_src, pTemp_Img, m_Mode_Dlg.m_nSchRange);
					}
					QueryPerformanceCounter(&nTime2); //结束计数器  
					QueryPerformanceFrequency(&nFrequency);//计数器频率  

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
						AfxMessageBox(_T("未生成效果图"), MB_ICONINFORMATION);
						break;
					}
					if (m_pImg_src->width != m_pImg_dst->width || m_pImg_src->height != m_pImg_dst->height || m_pImg_src->nChannels != m_pImg_dst->nChannels || m_pImg_src->depth != m_pImg_dst->depth)
					{
						AfxMessageBox(_T("原图与效果图参数不一致！"), MB_ICONINFORMATION);
						break;
					}
					QueryPerformanceCounter(&nTime1); //起始计数器 
					if (0 == nSelMode)
					{
						cvAbsDiff(m_pImg_src, m_pImg_dst, pTemp_Img);
						bResult = true;
					}
					QueryPerformanceCounter(&nTime2); //结束计数器  
					QueryPerformanceFrequency(&nFrequency);//计数器频率  
					break;
				}

				default:break;
			}
		}
		

		if (!bResult)	//处理失败，释放空间
		{
			cvReleaseImage(&pTemp_Img);
			return;
		}

		//处理成功

		//计算处理时间
		LARGE_INTEGER run_count;
		run_count.QuadPart = nTime2.QuadPart - nTime1.QuadPart;//运行时间计数器  
		double run_time = (double)run_count.QuadPart / (double)nFrequency.QuadPart * 1000; //运行时间(ms)
		szTime.Format(_T("处理时间:%5.2fms"), run_time);
		GetDlgItem(IDC_TIME)->ShowWindow(TRUE);
		SetDlgItemText(IDC_TIME, szTime);

		//询问是否保存，并清除旧图像
		if (m_pImg_dst)
		{
			if (m_stFileSavePath == "")
			{
				if (IDYES == AfxMessageBox(_T("效果图未保存,是否保存?"), MB_YESNO))
				{
					OnBnClickedImgSave();
				}
			}
			cvReleaseImage(&m_pImg_dst);
		}
		m_pImg_dst = pTemp_Img;		//指针指向处理好的图像
		pTemp_Img = NULL;

		//调整控件,显示图片
		m_Bmp_dst_Info->m_dScaleLock = m_Bmp_src_Info->m_dScaleLock;	//与原图比例相同
		DlgUpdata();		//更新对话框


		m_stFileSavePath = "";					//效果图未保存，清空路径
		m_ToolTip.AddTool(GetDlgItem(IDC_PICTURE_DST), "单击显示直方图\n双击浏览效果图\n" + m_stFileSavePath);	//气泡更新路径
		if (m_stFilePath != "")					//若原图无路径，即是处理完的图作为原图再处理的，而且该图未保存
		{
			m_stFileToSavePath = m_stFilePath;		//即将保存的路径
		}
	}
	else
	{	//错误提示
		if (!m_pImg_src)
		{
			AfxMessageBox(_T("未载入原图"), MB_ICONINFORMATION);
			OnBnClickedImgOpen();	//调用打开按钮
		}
		else
		{
			AfxMessageBox(_T("请选择处理效果"), MB_ICONINFORMATION);
			m_FuncBox.ShowDropDown(TRUE);		//弹出处理效果列表，提示选择
		}

	}
}




