// PaperPlaneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PaperPlane.h"
#include "PaperPlaneDlg.h"
#include "math.h"
//#include "ComOperating.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPaperPlaneDlg �Ի���




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


// CPaperPlaneDlg ��Ϣ�������

BOOL CPaperPlaneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CMenu* pMenu = new CMenu;
	pMenu -> LoadMenu(IDR_MENU);
	pTPOpenGL = new TPOpenGL(0);//ʵ����OpenGL��,����һ��Ҫ�ͷ��ڴ�����ڴ�й¶
	if(!OpenGLInit())//��ʼ��OpenGL
	{
		MessageBox("OpenGl��ʼ�����ɹ�","ERROR",MB_OK | MB_ICONINFORMATION);
		return false;
	}
	SetTimer(1,10,NULL);//������ʱ�����ڲ���
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPaperPlaneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
	CDC *pDC=GetDesktopWindow()->GetDC();//��ĻDC
    int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ
    int Width = pDC->GetDeviceCaps(HORZRES);
    int Height = pDC->GetDeviceCaps(VERTRES);
    
    CDC memDC;//�ڴ�DC
    memDC.CreateCompatibleDC(pDC);
    
    CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
    memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

    oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
    memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

    //���´��뱣��memDC�е�λͼ���ļ�
    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ
    
    FILE *fp = fopen("d:\\PaperPlane.jpg", "w+b");
	if(fp == NULL)
	{
		MessageBox("OpenGl��ʼ�����ɹ�","ERROR",MB_OK | MB_ICONINFORMATION);
		return 0;
	}
    BITMAPINFOHEADER bih = {0};//λͼ��Ϣͷ
    bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;//�߶�
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
    bih.biWidth = bmp.bmWidth;//���
    
    BITMAPFILEHEADER bfh = {0};//λͼ�ļ�ͷ
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
    bfh.bfType = (WORD)0x4d42;
    
    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//д��λͼ�ļ�ͷ
    
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//д��λͼ��Ϣͷ
    
    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//�����ڴ汣��λͼ����

    GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);//��ȡλͼ����

    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//д��λͼ����

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
	MessageBox("��ͼ�ɹ���������D��","�ɹ�",MB_OK | MB_ICONINFORMATION);
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
