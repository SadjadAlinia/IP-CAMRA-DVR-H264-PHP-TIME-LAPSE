// Restart.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "Restart.h"
#include "ConfigDemoDlg.h"

// CRestart 对话框

IMPLEMENT_DYNAMIC(CRestart, CDialog)

CRestart::CRestart(CWnd* pParent /*=NULL*/)
	: CDialog(CRestart::IDD, pParent)
{

}
CRestart::~CRestart()
{

}
void CRestart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRestart, CDialog)
	ON_BN_CLICKED(IDOK_RES, &CRestart::OnBnClickedRes)
	ON_BN_CLICKED(IDCAN_RESTART, &CRestart::OnBnClickedRestart)
END_MESSAGE_MAP()

BOOL CRestart::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDOK_RES)->SetWindowText(_CS("OneClick.OK"));
	GetDlgItem(IDCAN_RESTART)->SetWindowText(_CS("OneClick.Cancel"));
	return TRUE;
}
// CRestart 消息处理程序
void CRestart::OnBnClickedRes()
{
	// TODO: 在此添加控件通知处理程序代码
	long lLoginID = ((CClientDemo5Dlg*)AfxGetMainWnd())->m_LoginID;
	if (lLoginID <= 0)
	{
		return;
	}
	BOOL ret = H264_DVR_ControlDVR(lLoginID, 0, 10000);
	if(ret)
	{
		MessageBox(_CS("Error.RebootSuccess"));	//重启成功
		OnCancel();	//关闭窗口
	}
	else
	{
		MessageBox(_CS("Error.RebootFail"));	//重启失败
		
	}
}

void CRestart::OnBnClickedRestart()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}