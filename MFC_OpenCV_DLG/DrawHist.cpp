// DrawHist.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_OpenCV_DLG.h"
#include "DrawHist.h"
#include "afxdialogex.h"


// CDrawHist 对话框

IMPLEMENT_DYNAMIC(CDrawHist, CDialog)

CDrawHist::CDrawHist(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawHist::IDD, pParent)
{
	m_pHist = NULL;		//无数据
	m_fMaxY = 1;		//默认直方图最大值
	m_nFocus = -1;	//鼠标无焦点
	m_nType = GRAY;		//默认显示灰度直方图
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


// CDrawHist 消息处理程序

//窗口重画时重画直方图,否则会消失
void CDrawHist::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	DrawPic();	//窗口绘制直方图
}



//鼠标点击的直方图位置用红色突出显示
void CDrawHist::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CWnd * pWnd = GetDlgItem(IDC_HIST);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	float width = (float)rect.Width();
	ScreenToClient(&rect);

	//鼠标点击位置转换成坐标系坐标
	m_nFocus = int( (float(point.x - rect.left) - width * 0.12 ) * 255.0f / 0.83 / width );
	DrawPic();

	CDialog::OnLButtonDown(nFlags, point);
}




/**
*	function:		ShowGrayHist
*	description:	显示灰度图直方图
*	parameter:		pHist:直方图数据指针
szName:直方图名称，默认为“直方图”
*	return:			void
**/
void CDrawHist::ShowGrayHist(Hist_Info *pHist, CString szName/* = "直方图"*/)
{
	m_pHistInfo = pHist;	//传递直方图数据指针
	m_pHist = m_pHistInfo->pGrayHist;	//待绘制直方图为灰度图
	m_fMaxY = m_pHistInfo->fGrayMax;
	m_nFocus = -1;	//直方图鼠标焦点,不在0~256,无焦点
	//直方图选择标签
	m_TabCtl.DeleteAllItems();
	m_TabCtl.InsertItem(GRAY, "灰度");
	m_nType = GRAY;	//仅有灰度直方图选择

	DrawPic();			//绘制直方图
	ShowWindow(SW_SHOWNORMAL); //显示为非模态直方图对话框,可继续操作
	SetWindowTextA(szName);
	SetActiveWindow();		//激活直方图窗口
}

/**
*	function:		ShowRGBHist
*	description:	显示RGB图直方图
*	parameter:		pHist:直方图数据指针
szName:直方图名称，默认为“直方图”
*	return:			void
**/
void CDrawHist::ShowRGBHist(Hist_Info *pHist, CString szName/* = "直方图"*/)
{
	m_pHistInfo = pHist;	//传递直方图数据指针
	m_nFocus = -1;	//直方图鼠标焦点,不在0~256,无焦点

	//直方图类型选择标签
	m_TabCtl.DeleteAllItems();
	m_TabCtl.InsertItem(GRAY, "灰度");
	m_TabCtl.InsertItem(RED, "红色");
	m_TabCtl.InsertItem(GREEN, "绿色");
	m_TabCtl.InsertItem(BLUE, "蓝色");
	//为方便对比选中之前选择的标签，若没有，构造函数中初始化为Gray
	m_TabCtl.SetCurSel(m_nType);
	//根据标签选择直方图
	if (GRAY == m_nType)
	{
		m_pHist = m_pHistInfo->pGrayHist;	//传递直方图数据指针
		m_fMaxY = m_pHistInfo->fGrayMax;
	}
	else if (RED == m_nType)
	{
		m_pHist = m_pHistInfo->pRedHist;	//传递直方图数据指针
		m_fMaxY = m_pHistInfo->fRedMax;
	}
	else if (GREEN == m_nType)
	{
		m_pHist = m_pHistInfo->pGreenHist;	//传递直方图数据指针
		m_fMaxY = m_pHistInfo->fGreenMax;
	}
	else if (BLUE == m_nType)
	{
		m_pHist = m_pHistInfo->pBlueHist;	//传递直方图数据指针
		m_fMaxY = m_pHistInfo->fBlueMax;
	}

	DrawPic();			//绘制直方图
	ShowWindow(SW_SHOWNORMAL); //显示为非模态直方图对话框,可继续操作
	SetWindowTextA(szName);
	SetActiveWindow();		//激活直方图窗口
}





/**
*	function:		DrawHist
*	description:	绘制直方图
*	parameter:		void
*	return:			void
**/
void CDrawHist::DrawPic()
{
	CPaintDC dc(this); // device context for painting 

	CString str;	//绘制字符串时使用的临时字符串
	CWnd * pWnd = GetDlgItem(IDC_HIST);
	CDC *pDC = pWnd->GetDC(); //获取控件的CDC指针

	CPen Pen_White(PS_SOLID, 1, RGB(255, 255, 255));	//创建白色画笔,绘制控件区域为白色
	CPen Pen_Black(PS_SOLID, 1, RGB(0, 0, 0));			//创建黑色画笔绘制背景
	CPen Pen_Gray(PS_SOLID, 1, RGB(230, 230, 230));		//创建灰色画笔绘制刻度网格线
	//动态创建画笔,不同的直方图用不同的配色方案
	CPen *pPen_Hist = new CPen(); //创建直方图绘制画笔
	CPen *pPen_Focus = new CPen(); //创建鼠标焦点画笔

	//根据不同的直方图类型制定不同的配色
	if (GRAY == m_nType)
	{
		pPen_Hist->CreatePen(PS_SOLID, 1, RGB(130, 130, 130)); //灰色
		pPen_Focus->CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //红色
	}
	else if (RED == m_nType)
	{
		pPen_Hist->CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //红色
		pPen_Focus->CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); //黑色
	}
	else if (GREEN == m_nType)
	{
		pPen_Hist->CreatePen(PS_SOLID, 1, RGB(0, 210, 0)); //绿色
		pPen_Focus->CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); //黑色
	}
	else if (BLUE == m_nType)
	{
		pPen_Hist->CreatePen(PS_SOLID, 1, RGB(100, 100, 255)); //淡蓝色
		pPen_Focus->CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //红色
	}

	//开始绘制
	//强制更新绘图, 不可少, 否则绘图会出错
	pWnd->Invalidate(); //使static控件的区域无效
	pWnd->UpdateWindow();//更新窗口, 此时才真正向系统发送重绘消息

	
	CRect rect;
	pWnd->GetWindowRect(&rect);	//获取控件区域
	CGdiObject *pOldPen = pDC->SelectObject(&Pen_White);	//选中白色画笔绘制控件矩形区域,并保存以前的画笔
	pDC->Rectangle(0, rect.Height(), rect.Width(), 0);		//在控件区域画一个矩形框
	pDC->SetBkMode(TRANSPARENT);	//文字背景色透明

	int axis_X0 = axis_X(0);	//记录常用坐标，免得重复计算
	int axis_Y0 = axis_Y(0.00f);
	int axis_Xmax = axis_X(260);
	int axis_Ymax = axis_Y(1.04f);
	int axis_Xi, axis_Yi;		//临时记录刻度值

	//计算Y轴缩放倍数
	int nZoom = 0;	//根据最大值对坐标刻度值放大，科学记数法
	while (m_fMaxY * pow((float)10, nZoom) < 0.1f)
	{
		nZoom++;
	}
	if (nZoom)//若对坐标刻度进行放大，绘制出放大倍数
	{
		str.Format(_T("x10^(-%d)"), nZoom);
		pDC->TextOut(axis_X(5), axis_Y(1.14f), str);
	}

	//绘制X轴刻度值,刻度线
	for (int i = 0; i <= 255; i += 50)
	{
		pDC->SelectObject(Pen_Black);	//选中黑笔绘制刻度值
		str.Format(_T("%d"), i);
		pDC->TextOut(axis_X(i - 5), axis_Y(-0.01f), str);
		pDC->SelectObject(Pen_Gray);	//选中灰笔绘制刻度网格线
		axis_Xi = axis_X(i);
		pDC->MoveTo(axis_Xi, axis_Y0);
		pDC->LineTo(axis_Xi, axis_Ymax);
	}

	//绘制Y轴刻度值,刻度线
	for (float i = 0.1f; i <= 1.01f; i += 0.1f)
	{
		pDC->SelectObject(Pen_Black);	//选中黑笔绘制刻度值
		str.Format(_T("%.2f"), (i * m_fMaxY) *  pow((float)10, nZoom));
		pDC->TextOut(axis_X(-22), axis_Y(i + 0.043f), str);
		pDC->SelectObject(Pen_Gray);	//选中灰笔绘制刻度网格线
		axis_Yi = axis_Y(i);
		pDC->MoveTo(axis_X0, axis_Yi);
		pDC->LineTo(axis_Xmax, axis_Yi);
	}


	pDC->SelectObject(pPen_Hist);	//选中直方图绘制画笔
	//绘制直方图
	if (m_pHist)
	{
		for (int i = 0; i < 256; i++)
		{
			axis_Xi = axis_X(i);
			if (m_nFocus == i)	//检查当前刻度是否为鼠标焦点
			{
				pDC->SelectObject(pPen_Focus);	//鼠标点击位置特殊颜色
				pDC->MoveTo(axis_Xi, axis_Y0);
				pDC->LineTo(axis_Xi, axis_Y(*(m_pHist + i) * 1 / m_fMaxY));	//根据最大值放大直方图
				pDC->SelectObject(Pen_Black);	//用黑色画笔写出鼠标焦点处坐标值
				str.Format(_T("（%3d,%.3f）"), i, *(m_pHist + i) * pow((float)10, nZoom));
				pDC->TextOut(axis_X(190), axis_Y(1.14f), str);
				pDC->SelectObject(pPen_Hist);
			}
			else
			{
				pDC->MoveTo(axis_Xi, axis_Y0);
				pDC->LineTo(axis_Xi, axis_Y(*(m_pHist + i) * 1 / m_fMaxY));	//根据最大值放大直方图
			}

		}
	}

	

	//绘制坐标系(如果一开始绘制，会被直方图遮盖掉一些)
	pDC->SelectObject(Pen_Black);	//选中黑笔绘制坐标系

	//选择空画刷，无填充色，防止覆盖内容，保存旧画刷
	CBrush *pOldBrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(axis_X0, axis_Y0 + 1, axis_Xmax, axis_Ymax); //画一个矩形框作为坐标系
	//pDC->MoveTo(axis_X0, axis_Ymax); //绘制坐标轴
	//pDC->LineTo(axis_X0, axis_Y0); //竖起轴
	//pDC->LineTo(axis_Xmax, axis_Y0); //水平轴


	//绘制坐标轴标题
	CFont font;	//设置文字字体
	font.CreateFontA(
		13,          //nHeight  指定逻辑单位的字符或者字符元高度
		6,           //nWidth   指定逻辑单位的字体字符的平均宽度
		0,           //nEscapement 指定每行文本输出时相对于设备x轴的角度，其单位为1/10度。
		0,           //nOrientation  指定字符基线相对于设备x轴的角度，其单位为1/10度。此值在Win9X中和lfEscapement具有相同的值，而在WinNT下有时候可能不同。
		FW_NORMAL,   //nWeight  指定字体的重量，Windows中字体重量表示字体的粗细程度，其范围在0～1000之间，正常为400，粗体为700，若此值为空，则使用默认的字体重量。
		FALSE,       //bItalic  此值为TRUE时，字体为斜体。
		FALSE,       //bUnderline  此值为TRUE时，字体带下划线。
		0,           //cStrikeOut  此值为TRUE时，字体带删除线。
		ANSI_CHARSET,                       //nCharSet  指定所使用的字符集，如GB2312_CHARSET,CHINESEBIG5_CHARSET等。
		OUT_DEFAULT_PRECIS,                 //nOutPrecision  指定输出精度，它定义了输出与所要求的字体高度、宽度、字符方向及字体类型等相接近的程度。
		CLIP_DEFAULT_PRECIS,                //nClipPrecision  指定剪辑精度，它定义了当字符的一部分超过剪辑区域时对字符的剪辑方式。
		CLEARTYPE_NATURAL_QUALITY,          //nQuality  指定输出质量，它定义了GDI在匹配逻辑字体属性到实际的物理字体时所使用的方式。
		DEFAULT_PITCH | FF_SWISS,			//nPitchAndFamily  指定字体的字符间距和族。
		_T("宋体"));						//lpszFaceName  指向NULL结尾的字符串的指针，此字符串即为所使用的字体名称，其长度不能超过32个字符，如果为空，则使用系统默认的字体。
	CFont *pOldFont = pDC->SelectObject(&font);	
	//X轴标题
	if (m_nType == GRAY)
	{
		pDC->TextOut(axis_X(115), axis_Y(-0.095f), "灰度级");
	}
	else
	{
		pDC->TextOut(axis_X(120), axis_Y(-0.095f), "色阶");
	}
	//Y轴标题
	int textX = axis_X(-35);	//标题位置
	int textY = axis_Y(0.8f);
	pDC->TextOut(textX, textY, "概");
	textY += 15;
	pDC->TextOut(textX, textY, "率");
	textY += 15;
	pDC->TextOut(textX, textY, "密");
	textY += 15;
	pDC->TextOut(textX, textY, "度"); 
	textY += 15;
	pDC->TextOut(textX, textY, "函"); 
	textY += 15;
	pDC->TextOut(textX, textY, "数"); 
	textY += 15;

	pDC->SelectObject(pOldPen);		//恢复以前的画笔
	pDC->SelectObject(pOldBrush);	//恢复以前画刷
	pDC->SelectObject(&pOldFont);	//恢复以前的字体
	delete pPen_Focus, pPen_Hist;	//删除画笔
	ReleaseDC(pDC);
}

/**
*	function:		axis_X
*	description:	将直方图的X坐标换成客户区坐标
*	parameter:		直方图x坐标
*	return:			客户区坐标
**/

int CDrawHist::axis_X(int x)
{
	CWnd * pWnd = GetDlgItem(IDC_HIST);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	float width = (float)rect.Width();
	return int(width * 0.83 / 255.0f * x + width * 0.12);	//直方图水平区域占83%，左边间隔占12%
}


/**
*	function:		axis_Y
*	description:	将直方图的Y坐标换成客户区坐标
*	parameter:		直方图y坐标
*	return:			客户区坐标
**/
int CDrawHist::axis_Y(float y)
{
	CWnd * pWnd = GetDlgItem(IDC_HIST);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	float height = (float)rect.Height();
	return  int(height * 0.76 * (1 - y) + height * 0.11);	//直方图竖直区域占75%，上边间隔占11%
}

//标签栏更改，更新绘制的直方图类型(灰度、红、绿、蓝)
void CDrawHist::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	m_nType = m_TabCtl.GetCurSel();
	if (GRAY == m_nType)
	{
		m_pHist = m_pHistInfo->pGrayHist;	//传递直方图数据指针
		m_fMaxY = m_pHistInfo->fGrayMax;
	}
	else if (RED == m_nType)
	{
		m_pHist = m_pHistInfo->pRedHist;	//传递直方图数据指针
		m_fMaxY = m_pHistInfo->fRedMax;
	}
	else if (GREEN == m_nType)
	{
		m_pHist = m_pHistInfo->pGreenHist;	//传递直方图数据指针
		m_fMaxY = m_pHistInfo->fGreenMax;
	}
	else if (BLUE == m_nType)
	{
		m_pHist = m_pHistInfo->pBlueHist;	//传递直方图数据指针
		m_fMaxY = m_pHistInfo->fBlueMax;
	}
	m_nFocus = -1;	//直方图鼠标焦点,不在0~256,无焦点
	DrawPic();			//绘制直方图

	*pResult = 0;
}


//void CDrawHist::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	// TODO:  在此添加专用代码和/或调用基类
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
