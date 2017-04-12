
// MFC_OpenCV_DLGDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "Mode_Dlg.h"
#include "DrawHist.h"
#include"ImgProcessing.h"


using namespace cv;
using namespace std;


typedef struct CTL_INFO					//储存控件信息
{
	CWnd*   m_pWnd;             //指向控件的指针
	CRect	m_rectWnd;			//控件矩形信息
	int     m_x, m_y, m_nWidth, m_nHeight; //位置信息
	bool	m_bZoom;			//是否缩放控件
	double	m_dScaleLock;		//控件锁定比例
}ControlInfo;






// CMFC_OpenCV_DLGDlg 对话框
class CMFC_OpenCV_DLGDlg : public CDialogEx
{
// 构造
public:
	~CMFC_OpenCV_DLGDlg();
	CMFC_OpenCV_DLGDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_OPENCV_DLG_DIALOG };


//对话框元素、参数
	IplImage *m_pImg_src,		//原图指针
			 *m_pImg_dst;		//效果图指针
	CvCapture* m_pCapture;

	double				m_dZoomX,			//对话框X轴相对于最小值缩放比例
	    				m_dZoomY;			//对话框Y轴相对于最小值缩放比例

	/*窗口比例控制*/
	//double				m_dImgScale;			//图像宽高比
	//double				m_dDlgScale;			//对话框宽高比


	int					m_nMinClientWidth,		//客户区最小宽度
						m_nMinClientHeight;		//客户区最小高度

	POINT				m_ptMinTrackSize;		//窗口的最小大小

	bool				m_bInitDLG;				//窗口是否初始化完成

	CArray<ControlInfo*> m_CtrlList;		//参与动态调整的控件表
	ControlInfo			*m_Bmp_src_Info,		//原图显示控件的控件信息
						*m_Bmp_dst_Info;		//效果图显示控件的控件信息


	CString m_stFilePath;		//打开文件路径
	CString m_stFileToSavePath;	//文件即将保存的路径名
	CString m_stFileSavePath;	//保存文件路径

	CImgProcessing m_ImgProcessing;	//图像处理相关对象
	CMode_Dlg m_Mode_Dlg;			//模式参数设置对话框
	CDrawHist *m_pDrawHist;			//直方图对话框指针
	CToolTipCtrl m_ToolTip;			//气泡提示

	Hist_Info m_Hist;	//直方图数据结构体

//函数
	/**
	*	function:		CtrlInfo_Init
	*	description:	将需要自动适应窗口的控件进行初始化
	*	parameter:		nID：控件ID 
	*					b_Zoom:是否对控件进行缩放
	*	return:			ControlInfo:适配信息的结构体指针
	*	notice:			该函数只能在控件所属窗体的类中使用
	**/
	ControlInfo* CtrlInfo_Init(int nID, bool b_Zoom, double dScaleLock = 0.0);


	/**
	*	function:		CtrlAutoFit
	*	description:	使控件表中的控件自动适应窗口大小变化，进行适当移动、缩放
	*	parameter:		f_ZoomX,f_ZoomX：缩放比例
	*	return:			void
	*	notice:			该函数只能在控件所属窗体的类中使用
	**/
	void CtrlAutoFit(double f_ZoomX, double f_ZoomY);

	/**
	*	function:		CtrlRefresh
	*	description:	重画控件表中的控件
	*	parameter:		void
	*	return:			void
	*	notice:			该函数只能在控件所属窗体的类中使用
	**/
	void CtrlRefresh();

	/**
	*	function:		DelCtrlList
	*	description:	释放控件表数组
	*	parameter:		void
	*	return:			void
	**/
	void DelCtrlList();

	/**
	*	function:		PrintBMP
	*	description:	将图片显示到Picture Control控件
	*	parameter:		pPictureCtl:图像控件指针(需要先对控件添加控件变量)
	*					pImg：IplImage指针
	*	return:			void
	**/
	void PrintBMP(CStatic *pPictureCtl, IplImage * pImg);

	/**
	*	function:		DlgUpdata
	*	description:	更新对话框布局、图片
	*	parameter:		void
	*	return:			void
	**/
	void DlgUpdata();


	BOOL PreTranslateMessage(MSG *pMsg);


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Bmp_src;
	CStatic m_Bmp_dst;
	afx_msg void OnBnClickedImgOpen();
	afx_msg void OnBnClickedImgProcess();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnStnDblclickPictureSrc();
	afx_msg void OnStnDblclickPictureDst();
	CComboBox m_FuncBox;
	afx_msg void OnBnClickedImgProcess2();
	afx_msg void OnStnClickedPictureSrc();
	afx_msg void OnStnClickedPictureDst();
	afx_msg void OnBnClickedImgSave();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
