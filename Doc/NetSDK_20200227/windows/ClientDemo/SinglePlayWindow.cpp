// SinglePlayWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "SinglePlayWindow.h"


// CSinglePlayWindow 对话框

IMPLEMENT_DYNAMIC(CSinglePlayWindow, CDialog)

CSinglePlayWindow::CSinglePlayWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CSinglePlayWindow::IDD, pParent)
{
	m_index=0;
}

CSinglePlayWindow::~CSinglePlayWindow()
{
}

void CSinglePlayWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSinglePlayWindow, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CSinglePlayWindow::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CDialog::OnShowWindow(bShow, nStatus);
}


// CSinglePlayWindow 消息处理程序
