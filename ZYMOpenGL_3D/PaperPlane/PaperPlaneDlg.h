// PaperPlaneDlg.h : ͷ�ļ�
//

#pragma once

#include "TPOpenGL.h"

// CPaperPlaneDlg �Ի���
class CPaperPlaneDlg : public CDialog
{
// ����
public:
	CPaperPlaneDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CPaperPlaneDlg()
	{	
		delete pTPOpenGL;
	};
// �Ի�������
	enum { IDD = IDD_PAPERPLANE_DIALOG };
	bool FullScreenCapture();//ȫ����ͼ
	afx_msg void OnFullScreenCapture();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
/*-----------------------------------------------------------------------------------------------*///������̬���
	//FlyPosture flyposture; 
public:
	bool OpenGLInit(); 
protected:
	HDC hrenderDC;
	TPOpenGL *pTPOpenGL;
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);//��Ӧ��ʱ������Ҫ���ڲ���
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
};
