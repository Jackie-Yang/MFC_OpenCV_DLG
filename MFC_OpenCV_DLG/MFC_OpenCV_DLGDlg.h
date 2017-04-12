
// MFC_OpenCV_DLGDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "Mode_Dlg.h"
#include "DrawHist.h"
#include"ImgProcessing.h"


using namespace cv;
using namespace std;


typedef struct CTL_INFO					//����ؼ���Ϣ
{
	CWnd*   m_pWnd;             //ָ��ؼ���ָ��
	CRect	m_rectWnd;			//�ؼ�������Ϣ
	int     m_x, m_y, m_nWidth, m_nHeight; //λ����Ϣ
	bool	m_bZoom;			//�Ƿ����ſؼ�
	double	m_dScaleLock;		//�ؼ���������
}ControlInfo;






// CMFC_OpenCV_DLGDlg �Ի���
class CMFC_OpenCV_DLGDlg : public CDialogEx
{
// ����
public:
	~CMFC_OpenCV_DLGDlg();
	CMFC_OpenCV_DLGDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_OPENCV_DLG_DIALOG };


//�Ի���Ԫ�ء�����
	IplImage *m_pImg_src,		//ԭͼָ��
			 *m_pImg_dst;		//Ч��ͼָ��
	CvCapture* m_pCapture;

	double				m_dZoomX,			//�Ի���X���������Сֵ���ű���
	    				m_dZoomY;			//�Ի���Y���������Сֵ���ű���

	/*���ڱ�������*/
	//double				m_dImgScale;			//ͼ���߱�
	//double				m_dDlgScale;			//�Ի����߱�


	int					m_nMinClientWidth,		//�ͻ�����С���
						m_nMinClientHeight;		//�ͻ�����С�߶�

	POINT				m_ptMinTrackSize;		//���ڵ���С��С

	bool				m_bInitDLG;				//�����Ƿ��ʼ�����

	CArray<ControlInfo*> m_CtrlList;		//���붯̬�����Ŀؼ���
	ControlInfo			*m_Bmp_src_Info,		//ԭͼ��ʾ�ؼ��Ŀؼ���Ϣ
						*m_Bmp_dst_Info;		//Ч��ͼ��ʾ�ؼ��Ŀؼ���Ϣ


	CString m_stFilePath;		//���ļ�·��
	CString m_stFileToSavePath;	//�ļ����������·����
	CString m_stFileSavePath;	//�����ļ�·��

	CImgProcessing m_ImgProcessing;	//ͼ������ض���
	CMode_Dlg m_Mode_Dlg;			//ģʽ�������öԻ���
	CDrawHist *m_pDrawHist;			//ֱ��ͼ�Ի���ָ��
	CToolTipCtrl m_ToolTip;			//������ʾ

	Hist_Info m_Hist;	//ֱ��ͼ���ݽṹ��

//����
	/**
	*	function:		CtrlInfo_Init
	*	description:	����Ҫ�Զ���Ӧ���ڵĿؼ����г�ʼ��
	*	parameter:		nID���ؼ�ID 
	*					b_Zoom:�Ƿ�Կؼ���������
	*	return:			ControlInfo:������Ϣ�Ľṹ��ָ��
	*	notice:			�ú���ֻ���ڿؼ��������������ʹ��
	**/
	ControlInfo* CtrlInfo_Init(int nID, bool b_Zoom, double dScaleLock = 0.0);


	/**
	*	function:		CtrlAutoFit
	*	description:	ʹ�ؼ����еĿؼ��Զ���Ӧ���ڴ�С�仯�������ʵ��ƶ�������
	*	parameter:		f_ZoomX,f_ZoomX�����ű���
	*	return:			void
	*	notice:			�ú���ֻ���ڿؼ��������������ʹ��
	**/
	void CtrlAutoFit(double f_ZoomX, double f_ZoomY);

	/**
	*	function:		CtrlRefresh
	*	description:	�ػ��ؼ����еĿؼ�
	*	parameter:		void
	*	return:			void
	*	notice:			�ú���ֻ���ڿؼ��������������ʹ��
	**/
	void CtrlRefresh();

	/**
	*	function:		DelCtrlList
	*	description:	�ͷſؼ�������
	*	parameter:		void
	*	return:			void
	**/
	void DelCtrlList();

	/**
	*	function:		PrintBMP
	*	description:	��ͼƬ��ʾ��Picture Control�ؼ�
	*	parameter:		pPictureCtl:ͼ��ؼ�ָ��(��Ҫ�ȶԿؼ���ӿؼ�����)
	*					pImg��IplImageָ��
	*	return:			void
	**/
	void PrintBMP(CStatic *pPictureCtl, IplImage * pImg);

	/**
	*	function:		DlgUpdata
	*	description:	���¶Ի��򲼾֡�ͼƬ
	*	parameter:		void
	*	return:			void
	**/
	void DlgUpdata();


	BOOL PreTranslateMessage(MSG *pMsg);


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
