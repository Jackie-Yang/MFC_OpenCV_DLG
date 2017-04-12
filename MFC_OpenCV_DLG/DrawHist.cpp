// DrawHist.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_OpenCV_DLG.h"
#include "DrawHist.h"
#include "afxdialogex.h"


// CDrawHist �Ի���

IMPLEMENT_DYNAMIC(CDrawHist, CDialog)

CDrawHist::CDrawHist(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawHist::IDD, pParent)
{
	m_pHist = NULL;		//������
	m_fMaxY = 1;		//Ĭ��ֱ��ͼ���ֵ
	m_nFocus = -1;	//����޽���
	m_nType = GRAY;		//Ĭ����ʾ�Ҷ�ֱ��ͼ
}

CDrawHist::~CDrawHist()
{
}

void CDrawHist::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtl);
}


BEGIN_MESSAGE_MAP(CDrawHist, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDrawHist::OnTcnSelchangeTab1)
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CDrawHist ��Ϣ�������

//�����ػ�ʱ�ػ�ֱ��ͼ,�������ʧ
void CDrawHist::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	DrawPic();	//���ڻ���ֱ��ͼ
}



//�������ֱ��ͼλ���ú�ɫͻ����ʾ
void CDrawHist::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd * pWnd = GetDlgItem(IDC_HIST);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	float width = (float)rect.Width();
	ScreenToClient(&rect);

	//�����λ��ת��������ϵ����
	m_nFocus = int( (float(point.x - rect.left) - width * 0.12 ) * 255.0f / 0.83 / width );
	DrawPic();

	CDialog::OnLButtonDown(nFlags, point);
}




/**
*	function:		ShowGrayHist
*	description:	��ʾ�Ҷ�ͼֱ��ͼ
*	parameter:		pHist:ֱ��ͼ����ָ��
szName:ֱ��ͼ���ƣ�Ĭ��Ϊ��ֱ��ͼ��
*	return:			void
**/
void CDrawHist::ShowGrayHist(Hist_Info *pHist, CString szName/* = "ֱ��ͼ"*/)
{
	m_pHistInfo = pHist;	//����ֱ��ͼ����ָ��
	m_pHist = m_pHistInfo->pGrayHist;	//������ֱ��ͼΪ�Ҷ�ͼ
	m_fMaxY = m_pHistInfo->fGrayMax;
	m_nFocus = -1;	//ֱ��ͼ��꽹��,����0~256,�޽���
	//ֱ��ͼѡ���ǩ
	m_TabCtl.DeleteAllItems();
	m_TabCtl.InsertItem(GRAY, "�Ҷ�");
	m_nType = GRAY;	//���лҶ�ֱ��ͼѡ��

	DrawPic();			//����ֱ��ͼ
	ShowWindow(SW_SHOWNORMAL); //��ʾΪ��ģֱ̬��ͼ�Ի���,�ɼ�������
	SetWindowTextA(szName);
	SetActiveWindow();		//����ֱ��ͼ����
}

/**
*	function:		ShowRGBHist
*	description:	��ʾRGBͼֱ��ͼ
*	parameter:		pHist:ֱ��ͼ����ָ��
szName:ֱ��ͼ���ƣ�Ĭ��Ϊ��ֱ��ͼ��
*	return:			void
**/
void CDrawHist::ShowRGBHist(Hist_Info *pHist, CString szName/* = "ֱ��ͼ"*/)
{
	m_pHistInfo = pHist;	//����ֱ��ͼ����ָ��
	m_nFocus = -1;	//ֱ��ͼ��꽹��,����0~256,�޽���

	//ֱ��ͼ����ѡ���ǩ
	m_TabCtl.DeleteAllItems();
	m_TabCtl.InsertItem(GRAY, "�Ҷ�");
	m_TabCtl.InsertItem(RED, "��ɫ");
	m_TabCtl.InsertItem(GREEN, "��ɫ");
	m_TabCtl.InsertItem(BLUE, "��ɫ");
	//Ϊ����Ա�ѡ��֮ǰѡ��ı�ǩ����û�У����캯���г�ʼ��ΪGray
	m_TabCtl.SetCurSel(m_nType);
	//���ݱ�ǩѡ��ֱ��ͼ
	if (GRAY == m_nType)
	{
		m_pHist = m_pHistInfo->pGrayHist;	//����ֱ��ͼ����ָ��
		m_fMaxY = m_pHistInfo->fGrayMax;
	}
	else if (RED == m_nType)
	{
		m_pHist = m_pHistInfo->pRedHist;	//����ֱ��ͼ����ָ��
		m_fMaxY = m_pHistInfo->fRedMax;
	}
	else if (GREEN == m_nType)
	{
		m_pHist = m_pHistInfo->pGreenHist;	//����ֱ��ͼ����ָ��
		m_fMaxY = m_pHistInfo->fGreenMax;
	}
	else if (BLUE == m_nType)
	{
		m_pHist = m_pHistInfo->pBlueHist;	//����ֱ��ͼ����ָ��
		m_fMaxY = m_pHistInfo->fBlueMax;
	}

	DrawPic();			//����ֱ��ͼ
	ShowWindow(SW_SHOWNORMAL); //��ʾΪ��ģֱ̬��ͼ�Ի���,�ɼ�������
	SetWindowTextA(szName);
	SetActiveWindow();		//����ֱ��ͼ����
}





/**
*	function:		DrawHist
*	description:	����ֱ��ͼ
*	parameter:		void
*	return:			void
**/
void CDrawHist::DrawPic()
{
	CPaintDC dc(this); // device context for painting 

	CString str;	//�����ַ���ʱʹ�õ���ʱ�ַ���
	CWnd * pWnd = GetDlgItem(IDC_HIST);
	CDC *pDC = pWnd->GetDC(); //��ȡ�ؼ���CDCָ��

	CPen Pen_White(PS_SOLID, 1, RGB(255, 255, 255));	//������ɫ����,���ƿؼ�����Ϊ��ɫ
	CPen Pen_Black(PS_SOLID, 1, RGB(0, 0, 0));			//������ɫ���ʻ��Ʊ���
	CPen Pen_Gray(PS_SOLID, 1, RGB(230, 230, 230));		//������ɫ���ʻ��ƿ̶�������
	//��̬��������,��ͬ��ֱ��ͼ�ò�ͬ����ɫ����
	CPen *pPen_Hist = new CPen(); //����ֱ��ͼ���ƻ���
	CPen *pPen_Focus = new CPen(); //������꽹�㻭��

	//���ݲ�ͬ��ֱ��ͼ�����ƶ���ͬ����ɫ
	if (GRAY == m_nType)
	{
		pPen_Hist->CreatePen(PS_SOLID, 1, RGB(130, 130, 130)); //��ɫ
		pPen_Focus->CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //��ɫ
	}
	else if (RED == m_nType)
	{
		pPen_Hist->CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //��ɫ
		pPen_Focus->CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); //��ɫ
	}
	else if (GREEN == m_nType)
	{
		pPen_Hist->CreatePen(PS_SOLID, 1, RGB(0, 210, 0)); //��ɫ
		pPen_Focus->CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); //��ɫ
	}
	else if (BLUE == m_nType)
	{
		pPen_Hist->CreatePen(PS_SOLID, 1, RGB(100, 100, 255)); //����ɫ
		pPen_Focus->CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //��ɫ
	}

	//��ʼ����
	//ǿ�Ƹ��»�ͼ, ������, �����ͼ�����
	pWnd->Invalidate(); //ʹstatic�ؼ���������Ч
	pWnd->UpdateWindow();//���´���, ��ʱ��������ϵͳ�����ػ���Ϣ

	
	CRect rect;
	pWnd->GetWindowRect(&rect);	//��ȡ�ؼ�����
	CGdiObject *pOldPen = pDC->SelectObject(&Pen_White);	//ѡ�а�ɫ���ʻ��ƿؼ���������,��������ǰ�Ļ���
	pDC->Rectangle(0, rect.Height(), rect.Width(), 0);		//�ڿؼ�����һ�����ο�
	pDC->SetBkMode(TRANSPARENT);	//���ֱ���ɫ͸��

	int axis_X0 = axis_X(0);	//��¼�������꣬����ظ�����
	int axis_Y0 = axis_Y(0.00f);
	int axis_Xmax = axis_X(260);
	int axis_Ymax = axis_Y(1.04f);
	int axis_Xi, axis_Yi;		//��ʱ��¼�̶�ֵ

	//����Y�����ű���
	int nZoom = 0;	//�������ֵ������̶�ֵ�Ŵ󣬿�ѧ������
	while (m_fMaxY * pow((float)10, nZoom) < 0.1f)
	{
		nZoom++;
	}
	if (nZoom)//��������̶Ƚ��зŴ󣬻��Ƴ��Ŵ���
	{
		str.Format(_T("x10^(-%d)"), nZoom);
		pDC->TextOut(axis_X(5), axis_Y(1.14f), str);
	}

	//����X��̶�ֵ,�̶���
	for (int i = 0; i <= 255; i += 50)
	{
		pDC->SelectObject(Pen_Black);	//ѡ�кڱʻ��ƿ̶�ֵ
		str.Format(_T("%d"), i);
		pDC->TextOut(axis_X(i - 5), axis_Y(-0.01f), str);
		pDC->SelectObject(Pen_Gray);	//ѡ�лұʻ��ƿ̶�������
		axis_Xi = axis_X(i);
		pDC->MoveTo(axis_Xi, axis_Y0);
		pDC->LineTo(axis_Xi, axis_Ymax);
	}

	//����Y��̶�ֵ,�̶���
	for (float i = 0.1f; i <= 1.01f; i += 0.1f)
	{
		pDC->SelectObject(Pen_Black);	//ѡ�кڱʻ��ƿ̶�ֵ
		str.Format(_T("%.2f"), (i * m_fMaxY) *  pow((float)10, nZoom));
		pDC->TextOut(axis_X(-22), axis_Y(i + 0.043f), str);
		pDC->SelectObject(Pen_Gray);	//ѡ�лұʻ��ƿ̶�������
		axis_Yi = axis_Y(i);
		pDC->MoveTo(axis_X0, axis_Yi);
		pDC->LineTo(axis_Xmax, axis_Yi);
	}


	pDC->SelectObject(pPen_Hist);	//ѡ��ֱ��ͼ���ƻ���
	//����ֱ��ͼ
	if (m_pHist)
	{
		for (int i = 0; i < 256; i++)
		{
			axis_Xi = axis_X(i);
			if (m_nFocus == i)	//��鵱ǰ�̶��Ƿ�Ϊ��꽹��
			{
				pDC->SelectObject(pPen_Focus);	//�����λ��������ɫ
				pDC->MoveTo(axis_Xi, axis_Y0);
				pDC->LineTo(axis_Xi, axis_Y(*(m_pHist + i) * 1 / m_fMaxY));	//�������ֵ�Ŵ�ֱ��ͼ
				pDC->SelectObject(Pen_Black);	//�ú�ɫ����д����꽹�㴦����ֵ
				str.Format(_T("��%3d,%.3f��"), i, *(m_pHist + i) * pow((float)10, nZoom));
				pDC->TextOut(axis_X(190), axis_Y(1.14f), str);
				pDC->SelectObject(pPen_Hist);
			}
			else
			{
				pDC->MoveTo(axis_Xi, axis_Y0);
				pDC->LineTo(axis_Xi, axis_Y(*(m_pHist + i) * 1 / m_fMaxY));	//�������ֵ�Ŵ�ֱ��ͼ
			}

		}
	}

	

	//��������ϵ(���һ��ʼ���ƣ��ᱻֱ��ͼ�ڸǵ�һЩ)
	pDC->SelectObject(Pen_Black);	//ѡ�кڱʻ�������ϵ

	//ѡ��ջ�ˢ�������ɫ����ֹ�������ݣ�����ɻ�ˢ
	CBrush *pOldBrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(axis_X0, axis_Y0 + 1, axis_Xmax, axis_Ymax); //��һ�����ο���Ϊ����ϵ
	//pDC->MoveTo(axis_X0, axis_Ymax); //����������
	//pDC->LineTo(axis_X0, axis_Y0); //������
	//pDC->LineTo(axis_Xmax, axis_Y0); //ˮƽ��


	//�������������
	CFont font;	//������������
	font.CreateFontA(
		13,          //nHeight  ָ���߼���λ���ַ������ַ�Ԫ�߶�
		6,           //nWidth   ָ���߼���λ�������ַ���ƽ�����
		0,           //nEscapement ָ��ÿ���ı����ʱ������豸x��ĽǶȣ��䵥λΪ1/10�ȡ�
		0,           //nOrientation  ָ���ַ�����������豸x��ĽǶȣ��䵥λΪ1/10�ȡ���ֵ��Win9X�к�lfEscapement������ͬ��ֵ������WinNT����ʱ����ܲ�ͬ��
		FW_NORMAL,   //nWeight  ָ�������������Windows������������ʾ����Ĵ�ϸ�̶ȣ��䷶Χ��0��1000֮�䣬����Ϊ400������Ϊ700������ֵΪ�գ���ʹ��Ĭ�ϵ�����������
		FALSE,       //bItalic  ��ֵΪTRUEʱ������Ϊб�塣
		FALSE,       //bUnderline  ��ֵΪTRUEʱ��������»��ߡ�
		0,           //cStrikeOut  ��ֵΪTRUEʱ�������ɾ���ߡ�
		ANSI_CHARSET,                       //nCharSet  ָ����ʹ�õ��ַ�������GB2312_CHARSET,CHINESEBIG5_CHARSET�ȡ�
		OUT_DEFAULT_PRECIS,                 //nOutPrecision  ָ��������ȣ����������������Ҫ�������߶ȡ���ȡ��ַ������������͵���ӽ��ĳ̶ȡ�
		CLIP_DEFAULT_PRECIS,                //nClipPrecision  ָ���������ȣ��������˵��ַ���һ���ֳ�����������ʱ���ַ��ļ�����ʽ��
		CLEARTYPE_NATURAL_QUALITY,          //nQuality  ָ�������������������GDI��ƥ���߼��������Ե�ʵ�ʵ���������ʱ��ʹ�õķ�ʽ��
		DEFAULT_PITCH | FF_SWISS,			//nPitchAndFamily  ָ��������ַ������塣
		_T("����"));						//lpszFaceName  ָ��NULL��β���ַ�����ָ�룬���ַ�����Ϊ��ʹ�õ��������ƣ��䳤�Ȳ��ܳ���32���ַ������Ϊ�գ���ʹ��ϵͳĬ�ϵ����塣
	CFont *pOldFont = pDC->SelectObject(&font);	
	//X�����
	if (m_nType == GRAY)
	{
		pDC->TextOut(axis_X(115), axis_Y(-0.095f), "�Ҷȼ�");
	}
	else
	{
		pDC->TextOut(axis_X(120), axis_Y(-0.095f), "ɫ��");
	}
	//Y�����
	int textX = axis_X(-35);	//����λ��
	int textY = axis_Y(0.8f);
	pDC->TextOut(textX, textY, "��");
	textY += 15;
	pDC->TextOut(textX, textY, "��");
	textY += 15;
	pDC->TextOut(textX, textY, "��");
	textY += 15;
	pDC->TextOut(textX, textY, "��"); 
	textY += 15;
	pDC->TextOut(textX, textY, "��"); 
	textY += 15;
	pDC->TextOut(textX, textY, "��"); 
	textY += 15;

	pDC->SelectObject(pOldPen);		//�ָ���ǰ�Ļ���
	pDC->SelectObject(pOldBrush);	//�ָ���ǰ��ˢ
	pDC->SelectObject(&pOldFont);	//�ָ���ǰ������
	delete pPen_Focus, pPen_Hist;	//ɾ������
	ReleaseDC(pDC);
}

/**
*	function:		axis_X
*	description:	��ֱ��ͼ��X���껻�ɿͻ�������
*	parameter:		ֱ��ͼx����
*	return:			�ͻ�������
**/

int CDrawHist::axis_X(int x)
{
	CWnd * pWnd = GetDlgItem(IDC_HIST);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	float width = (float)rect.Width();
	return int(width * 0.83 / 255.0f * x + width * 0.12);	//ֱ��ͼˮƽ����ռ83%����߼��ռ12%
}


/**
*	function:		axis_Y
*	description:	��ֱ��ͼ��Y���껻�ɿͻ�������
*	parameter:		ֱ��ͼy����
*	return:			�ͻ�������
**/
int CDrawHist::axis_Y(float y)
{
	CWnd * pWnd = GetDlgItem(IDC_HIST);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	float height = (float)rect.Height();
	return  int(height * 0.76 * (1 - y) + height * 0.11);	//ֱ��ͼ��ֱ����ռ75%���ϱ߼��ռ11%
}

//��ǩ�����ģ����»��Ƶ�ֱ��ͼ����(�Ҷȡ��졢�̡���)
void CDrawHist::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_nType = m_TabCtl.GetCurSel();
	if (GRAY == m_nType)
	{
		m_pHist = m_pHistInfo->pGrayHist;	//����ֱ��ͼ����ָ��
		m_fMaxY = m_pHistInfo->fGrayMax;
	}
	else if (RED == m_nType)
	{
		m_pHist = m_pHistInfo->pRedHist;	//����ֱ��ͼ����ָ��
		m_fMaxY = m_pHistInfo->fRedMax;
	}
	else if (GREEN == m_nType)
	{
		m_pHist = m_pHistInfo->pGreenHist;	//����ֱ��ͼ����ָ��
		m_fMaxY = m_pHistInfo->fGreenMax;
	}
	else if (BLUE == m_nType)
	{
		m_pHist = m_pHistInfo->pBlueHist;	//����ֱ��ͼ����ָ��
		m_fMaxY = m_pHistInfo->fBlueMax;
	}
	m_nFocus = -1;	//ֱ��ͼ��꽹��,����0~256,�޽���
	DrawPic();			//����ֱ��ͼ

	*pResult = 0;
}


//void CDrawHist::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	switch (nChar)
//	{
//		case VK_LEFT:
//		{
//			if (++m_nFocus >= 256)
//			{
//				m_nFocus = -1;
//			}
//			DrawPic();
//			break;
//		}
//		case VK_RIGHT:
//		{
//			if (--m_nFocus < -1)
//			{
//				m_nFocus = -1;
//			}
//			DrawPic();
//			break;
//		}
//		default:break;
//	}
//	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
//}


BOOL CDrawHist::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_RIGHT:
			{
				if (++m_nFocus >= 256)
				{
					m_nFocus = -1;
				}
				DrawPic();
				break;
			}
			case VK_LEFT:
			{
				if (--m_nFocus < -1)
				{
					m_nFocus = -1;
				}
				DrawPic();
				break;
			}
			default:break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
