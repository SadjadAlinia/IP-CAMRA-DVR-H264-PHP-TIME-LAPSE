// AutoRboot.cpp : 实现文件  


//自动维护文件

#include "stdafx.h"
#include "ConfigDemo.h"
#include "AutoRboot.h"
#include "ConfigDemoDlg.h"


// CAutoRboot 对话框

IMPLEMENT_DYNAMIC(CAutoRboot, CDialog)

CAutoRboot::CAutoRboot(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoRboot::IDD, pParent)
{
	nIndex = 0;
}

CAutoRboot::~CAutoRboot()
{
}

void CAutoRboot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_WEEK, m_cmdWeek);
	DDX_Control(pDX, IDC_COMBO_DATE, m_cmdDate);
	DDX_Control(pDX, IDC_COMBO_FILE, m_cmdClearFile);
}


BEGIN_MESSAGE_MAP(CAutoRboot, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CAutoRboot 消息处理程序
BOOL CAutoRboot::OnInitDialog()
{
	CDialog::OnInitDialog();

	_CWndCS(this);

	GetDlgItem(IDC_STATIC_REBOOT)->SetWindowText(_CS("AutoMain.AutoReboot"));
	GetDlgItem(IDC_STATIC_AT)->SetWindowText(_CS("AutoMain.At"));
	GetDlgItem(IDC_STATIC_DELETE)->SetWindowText(_CS("AutoMain.AutoDelete"));
	GetDlgItem(IDOK)->SetWindowText(_CS("OneClick.OK"));
	GetDlgItem(IDCANCEL)->SetWindowText(_CS("OneClick.Cancel"));

	InitDlgAuto();
	return TRUE;
}

void CAutoRboot::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SDK_AutoMaintainConfig m_pAutoConfig;
	m_pAutoConfig.iAutoDeleteFilesDays = m_cmdClearFile.GetCurSel();
	m_pAutoConfig.iAutoRebootDay = m_cmdWeek.GetCurSel();
	m_pAutoConfig.iAutoRebootHour = m_cmdDate.GetCurSel();
	int nRet = H264_DVR_SetDevConfig(loginID, E_SDK_CONFIG_AUTO, -1,
		(char*)&m_pAutoConfig, sizeof(SDK_AutoMaintainConfig));
	if(nRet > 0)
	{
		MessageBox(_CS("Error.SaveSuccess"));
		OnCancel();
	}
}

void CAutoRboot::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
void CAutoRboot::InitDlgAuto()
{
	long lLoginID = ((CClientDemo5Dlg*)AfxGetMainWnd())->m_LoginID;
	if (lLoginID <= 0)
	{
		return;
	}
	DWORD dwRetLen = 0;
	int nWaitTime = 10000;
	SDK_AutoMaintainConfig pAutoConfig;
	BOOL bSuccess = H264_DVR_GetDevConfig(lLoginID,E_SDK_CONFIG_AUTO,-1,
		(char *)&pAutoConfig,sizeof(SDK_AutoMaintainConfig),&dwRetLen,nWaitTime);
	if (bSuccess && dwRetLen == sizeof(SDK_AutoMaintainConfig))
	{
		CString str;
		SDK_AutoMaintainConfig* pAuto = &pAutoConfig;

		if (pAuto)
		{
			m_cmdWeek.SetCurSel(pAuto->iAutoRebootDay);
			m_cmdDate.SetCurSel(pAuto->iAutoRebootHour);
			m_cmdClearFile.SetCurSel(pAuto->iAutoDeleteFilesDays);
		}
	}
}