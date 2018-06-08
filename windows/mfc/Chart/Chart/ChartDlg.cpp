
// ChartDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Chart.h"
#include "ChartDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChartDlg 对话框



CChartDlg::CChartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHART_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChartDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CChartDlg 消息处理程序

BOOL CChartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChartDlg::OnPaint()
{
		CPaintDC dc(this); // device context for painting

		LOGFONT log;
		CFont font;
		log.lfHeight = 0;
		log.lfEscapement = 0;
		log.lfItalic = 0;
		log.lfUnderline = 0;
		log.lfStrikeOut = 0;

		//    dc.SelectObject(&font); 
		font.CreateFontIndirect(&log);
		//画笔
		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		dc.SelectObject(&pen);
		//    *OldPen = dc.SelectObject(&pen);//旧画笔


		//逻辑坐标
		SetViewportOrgEx(dc, 50, 500, NULL);
		dc.SetMapMode(MM_LOMETRIC);//0.1毫米

								   //纵坐标
		dc.MoveTo(0, 0);
		dc.LineTo(0, 1000);//高1000
		dc.LineTo(-10, 980);
		dc.MoveTo(0, 1000);
		dc.LineTo(10, 980);
		//纵坐标循环
		CString str;
		int i;
		for (i = 1; i<10; i++)
		{
			str.Format(L"%d", i * 10);

			dc.TextOut(-30, i * 100, str);
		}
		dc.TextOut(-50, 1050, L"%");
		//横坐标
		dc.MoveTo(0, 0);
		dc.LineTo(850, 0);
		dc.LineTo(830, 10);
		dc.MoveTo(850, 0);
		dc.LineTo(830, -10);

		int x1, y1, x2, y2, y3, x3;
		float zhuan = 0.4, ben = 0.3, shuo = 0.2, bo = 0.1;
		int h = 1000, r = 400;



		//画柱子
		dc.SelectObject(&font);     //修改字体

		CRect r1(100, zhuan*h, 200, 0);
		CBrush br1(RGB(253, 51, 67));
		dc.FillRect(r1, &br1);//矩形，颜色
		dc.TextOut(130, -10, L"专科");//横坐标的说明文字

		CRect r2(300, ben*h, 400, 0);
		CBrush br2(RGB(250, 164, 51));
		dc.FillRect(r2, &br2);
		dc.TextOut(330, -10, L"本科");

		CRect r3(500, shuo*h, 600, 0);
		CBrush br3(RGB(139, 197, 51));
		dc.FillRect(r3, &br3);
		dc.TextOut(530, -10, L"硕士");

		CRect r4(700, bo*h, 800, 0);
		CBrush br4(RGB(3, 141, 216));
		dc.FillRect(r4, &br4);
		dc.TextOut(730, -10, L"博士");


		//    dc.SetBkColor(RGB(0,0,0));
		//    dc.Rectangle(100,zhuan*h,200,0);
		//    dc.Rectangle(300,ben*h,400,0);
		//    dc.Rectangle(500,shuo*h,600,0);
		//    dc.Rectangle(700,bo*h,800,0);

		//画饼图，利用cos，sin函数计算顶点值，颜色沿用之前已经定义了的brush
		dc.SelectObject(&br1);
		x1 = int(r*cos(zhuan*3.14 * 2)) + 1400;
		y1 = int(r*sin(zhuan*3.14 * 2) + 400);
		dc.Pie(1000, 800, 1800, 0, 1800, 400, x1, y1);


		dc.SelectObject(&br2);
		x2 = int(r*cos((ben + zhuan)*3.14 * 2)) + 1400;
		y2 = int(r*sin((ben + zhuan)*3.14 * 2)) + 400;
		dc.Pie(1000, 800, 1800, 0, x1, y1, x2, y2);

		dc.SelectObject(&br3);
		x3 = int(r*cos((ben + zhuan + shuo)*3.14 * 2)) + 1400;
		y3 = int(r*sin((ben + zhuan + shuo)*3.14 * 2)) + 400;
		dc.Pie(1000, 800, 1800, 0, x2, y2, x3, y3);
		CBrush   br9(RGB(125, 30, 227));
		dc.SelectObject(&br9);

		dc.SelectObject(&br4);
		dc.Pie(1000, 800, 1800, 0, x3, y3, 1800, 400);

		//在园外右上角画图例，加上说明文字
		CRect r5(2000, 1000, 2080, 1060);
		dc.FillRect(r5, &br1);
		dc.TextOut(2100, 1040, L"专科");

		CRect r6(2000, 900, 2080, 960);
		dc.FillRect(r6, &br2);
		dc.TextOut(2100, 940, L"本科");

		CRect r7(2000, 800, 2080, 860);
		dc.FillRect(r7, &br3);
		dc.TextOut(2100, 840, L"硕士");

		CRect r8(2000, 700, 2080, 760);
		dc.FillRect(r8, &br4);
		dc.TextOut(2100, 740, L"博士");
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

