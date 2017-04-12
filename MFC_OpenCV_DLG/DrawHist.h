#pragma once
#include "afxcmn.h"


// CDrawHist 对话框
typedef struct HIST_INFO					//储存直方图信息
{
	float pGrayHist[256];	//灰度直方图数据
	float fGrayMax;			//灰度直方图最大值
	float pRedHist[256];	//红色直方图数据
	float fRedMax;			//红色直方图最大值
	float pGreenHist[256];	//绿色直方图数据
	float fGreenMax;		//绿色直方图最大值
	float pBlueHist[256];	//蓝色直方图数据
	float fBlueMax;			//蓝色直方图最大值
}Hist_Info;

enum HistType
{
	GRAY = 0,
	RED,
	GREEN,
	BLUE,
};

class CDrawHist : public CDialog
{
	DECLARE_DYNAMIC(CDrawHist)

public:
	CDrawHist(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDrawHist();

	

	/**
	*	function:		ShowGrayHist
	*	description:	显示灰度图直方图
	*	parameter:		pHist:直方图数据指针
						szName:直方图名称，默认为“直方图”
	*	return:			void
	**/
	void ShowGrayHist(Hist_Info *pHist, CString szName = "直方图");

	/**
	*	function:		ShowRGBHist
	*	description:	显示RGB图直方图
	*	parameter:		pHist:直方图数据指针
						szName:直方图名称，默认为“直方图”
	*	return:			void
	**/
	void ShowRGBHist(Hist_Info *pHist, CString szName = "直方图");

private:

	Hist_Info * m_pHistInfo;	//传递直方图数据
	float *m_pHist;	//待绘制直方图数据
	float m_fMaxY;	//待绘制直方图Y轴最大值
	int m_nType;	//待绘制直方图类型
	int m_nFocus;	//直方图焦点

	//绘制直方图
	void DrawPic();
	//将直方图的X坐标换成客户区坐标
	int axis_X(int x);
	//将直方图的Y坐标换成客户区坐标
	int axis_Y(float y);

// 对话框数据
	enum { IDD = IDD_DRAWHIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CTabCtrl m_TabCtl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
