// PaperPlaneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PaperPlane.h"
#include "PaperPlaneDlg.h"
#include "math.h"
//#include "ComOperating.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPaperPlaneDlg 对话框




CPaperPlaneDlg::CPaperPlaneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaperPlaneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPaperPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPaperPlaneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_COMMAND(ID_32771, &CPaperPlaneDlg::OnFullScreenCapture)
END_MESSAGE_MAP()


// CPaperPlaneDlg 消息处理程序

BOOL CPaperPlaneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CMenu* pMenu = new CMenu;
	pMenu -> LoadMenu(IDR_MENU);
	pTPOpenGL = new TPOpenGL(0);//实例化OpenGL类,后面一定要释放内存否则内存泄露
	if(!OpenGLInit())//初始化OpenGL
	{
		MessageBox("OpenGl初始化不成功","ERROR",MB_OK | MB_ICONINFORMATION);
		return false;
	}
	SetTimer(1,10,NULL);//启动定时器用于测试
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPaperPlaneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPaperPlaneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CPaperPlaneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPaperPlaneDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
#ifndef YMOPENGL
	pTPOpenGL->RenderScene(hrenderDC);
#endif
	CDialog::OnTimer(nIDEvent);
}

bool CPaperPlaneDlg::FullScreenCapture()
{
	CDC *pDC=GetDesktopWindow()->GetDC();//屏幕DC
    int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
    int Width = pDC->GetDeviceCaps(HORZRES);
    int Height = pDC->GetDeviceCaps(VERTRES);
    
    CDC memDC;//内存DC
    memDC.CreateCompatibleDC(pDC);
    
    CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
    memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

    oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
    memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

    //以下代码保存memDC中的位图到文件
    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);//获得位图信息
    
    FILE *fp = fopen("d:\\PaperPlane.jpg", "w+b");
	if(fp == NULL)
	{
		MessageBox("OpenGl初始化不成功","ERROR",MB_OK | MB_ICONINFORMATION);
		return 0;
	}
    BITMAPINFOHEADER bih = {0};//位图信息头
    bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;//高度
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
    bih.biWidth = bmp.bmWidth;//宽度
    
    BITMAPFILEHEADER bfh = {0};//位图文件头
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
    bfh.bfType = (WORD)0x4d42;
    
    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头
    
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头
    
    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据

    GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据

    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据

    delete [] p;

    fclose(fp);

    memDC.SelectObject(oldmemBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
	return 1;
}

void CPaperPlaneDlg::OnFullScreenCapture()
{
	// TODO: Add your command handler code here
	if(FullScreenCapture())
	MessageBox("截图成功，保存在D盘","成功",MB_OK | MB_ICONINFORMATION);
}

bool CPaperPlaneDlg::OpenGLInit()
{
	CWnd *wnd=GetDlgItem(IDC_FLY);
	hrenderDC=::GetDC(wnd->m_hWnd);
	if(pTPOpenGL->SetWindowPixelFormat(hrenderDC)==FALSE)
	return 0;
	if(pTPOpenGL->CreateViewGLContext(hrenderDC)==FALSE)
	return 0;
	pTPOpenGL->InitGL();
	return 1;
}
