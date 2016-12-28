// PaperPlaneDlg.h : 头文件
//

#pragma once

#include "TPOpenGL.h"

// CPaperPlaneDlg 对话框
class CPaperPlaneDlg : public CDialog
{
// 构造
public:
	CPaperPlaneDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPaperPlaneDlg()
	{	
		delete pTPOpenGL;
	};
// 对话框数据
	enum { IDD = IDD_PAPERPLANE_DIALOG };
	bool FullScreenCapture();//全屏截图
	afx_msg void OnFullScreenCapture();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
/*-----------------------------------------------------------------------------------------------*///飞行姿态相关
	//FlyPosture flyposture; 
public:
	bool OpenGLInit(); 
protected:
	HDC hrenderDC;
	TPOpenGL *pTPOpenGL;
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);//响应定时器，主要用于测试
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
};
