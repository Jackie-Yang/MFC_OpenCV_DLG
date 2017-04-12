#pragma once
#include "afxcmn.h"


// CDrawHist �Ի���
typedef struct HIST_INFO					//����ֱ��ͼ��Ϣ
{
	float pGrayHist[256];	//�Ҷ�ֱ��ͼ����
	float fGrayMax;			//�Ҷ�ֱ��ͼ���ֵ
	float pRedHist[256];	//��ɫֱ��ͼ����
	float fRedMax;			//��ɫֱ��ͼ���ֵ
	float pGreenHist[256];	//��ɫֱ��ͼ����
	float fGreenMax;		//��ɫֱ��ͼ���ֵ
	float pBlueHist[256];	//��ɫֱ��ͼ����
	float fBlueMax;			//��ɫֱ��ͼ���ֵ
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
	CDrawHist(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDrawHist();

	

	/**
	*	function:		ShowGrayHist
	*	description:	��ʾ�Ҷ�ͼֱ��ͼ
	*	parameter:		pHist:ֱ��ͼ����ָ��
						szName:ֱ��ͼ���ƣ�Ĭ��Ϊ��ֱ��ͼ��
	*	return:			void
	**/
	void ShowGrayHist(Hist_Info *pHist, CString szName = "ֱ��ͼ");

	/**
	*	function:		ShowRGBHist
	*	description:	��ʾRGBͼֱ��ͼ
	*	parameter:		pHist:ֱ��ͼ����ָ��
						szName:ֱ��ͼ���ƣ�Ĭ��Ϊ��ֱ��ͼ��
	*	return:			void
	**/
	void ShowRGBHist(Hist_Info *pHist, CString szName = "ֱ��ͼ");

private:

	Hist_Info * m_pHistInfo;	//����ֱ��ͼ����
	float *m_pHist;	//������ֱ��ͼ����
	float m_fMaxY;	//������ֱ��ͼY�����ֵ
	int m_nType;	//������ֱ��ͼ����
	int m_nFocus;	//ֱ��ͼ����

	//����ֱ��ͼ
	void DrawPic();
	//��ֱ��ͼ��X���껻�ɿͻ�������
	int axis_X(int x);
	//��ֱ��ͼ��Y���껻�ɿͻ�������
	int axis_Y(float y);

// �Ի�������
	enum { IDD = IDD_DRAWHIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CTabCtrl m_TabCtl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
