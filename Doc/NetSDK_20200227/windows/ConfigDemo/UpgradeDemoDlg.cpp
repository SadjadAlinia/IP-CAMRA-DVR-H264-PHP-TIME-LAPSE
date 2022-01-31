// UpgradeDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "UpgradeDemoDlg.h"
#include "ConfigDemoDlg.h"


CUpgradeDemoDlg *g_pRECUpgrade = NULL;
// CUpgradeDemoDlg 对话框

IMPLEMENT_DYNAMIC(CUpgradeDemoDlg, CDialog)

CUpgradeDemoDlg::CUpgradeDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpgradeDemoDlg::IDD, pParent)
{
	
}

CUpgradeDemoDlg::~CUpgradeDemoDlg()
{
}
BOOL CUpgradeDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATIC1)->SetWindowText(_CS("Upgrade.File"));
	GetDlgItem(IDC_BTN_BROWSE)->SetWindowText(_CS("Upgrade.SelectFile"));
	GetDlgItem(IDC_BTN_UPGRADE)->SetWindowText(_CS("Upgrade.ManualUpgrade"));
	GetDlgItem(IDC_BTN_Update)->SetWindowText(_CS("Upgrade.OnlineUpgrade"));
	GetDlgItem(IDC_STATIC_PRO1)->SetWindowText(_CS("Upgrade.progress"));

	m_progress1.SetRange(0, 100);	//进度条的大小

	return TRUE;
}
void CUpgradeDemoDlg::CleanAll()
{
	SetDlgItemText(IDC_STATIC_UpgardeFile, "");
	SetDlgItemText(IDC_EDIT_UPGRADEFILE, "");
	SetDlgItemText(IDC_STATIC_PROCESS, "");
	m_progress1.SetPos(0);
}
void CUpgradeDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS_EXPORT, m_progress1);	//进度条
	DDX_Control(pDX, IDC_BTN_UPGRADE, m_Upgrade);
	DDX_Control(pDX, IDC_BTN_BROWSE, m_Browse);
	DDX_Text(pDX, IDC_EDIT_UPGRADEFILE, m_UpgradeFile);
	DDX_Text(pDX, IDC_STATIC_PROCESS, m_strProcess);
	DDX_Text(pDX, IDC_STATIC_UpgardeFile, m_strUpgradeFile);	//在线系统文件
}


BEGIN_MESSAGE_MAP(CUpgradeDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CUpgradeDemoDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_UPGRADE, &CUpgradeDemoDlg::OnBnClickedBtnUpgrade)
	ON_BN_CLICKED(IDC_BTN_Update, &CUpgradeDemoDlg::OnBnClickedBtnUpdate)
END_MESSAGE_MAP()


// CUpgradeDemoDlg 消息处理程序

void CUpgradeDemoDlg::OnBnClickedBtnBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!loginID)
	{
		MessageBox(_CS("Upgrade.Please connect device"));
		return ;
	}
	CString str("");
	char szFilter[] = _T("System files (*.bin)|*.*||");	
	
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,szFilter, this);
	if ( dlg.DoModal() == IDOK)
	{
		m_UpgradeFile = dlg.GetPathName();
	}
	UpdateData(FALSE);
}
void  __stdcall CUpgradeDemoDlg::cbUpgrade( long lLoginID, long lUpgradechannel,int nTotalSize, int nSendSize, long dwUser)
{	
	CUpgradeDemoDlg *pDataChnl = (CUpgradeDemoDlg*)dwUser;
	//说明升级完成
	if ( nSendSize == -1 )
	{	
		pDataChnl->m_lUpgradeHandle = lUpgradechannel;
		CWnd * pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PROCESS);
		pWnd->SetWindowText("100%");
		pDataChnl->MessageBox(_CS("Upgrade.UpgradeSuccess"));
		pDataChnl->m_progress1.SetPos(0);
		pWnd->SetWindowText("");
	}
	//升级出错
	else if ( nSendSize == -2 )
	{
			pDataChnl->MessageBox(_CS("Upgrade.UpgradeFail"));
	}
	else if( nSendSize < -2)
	{
		CString strTemp;
		strTemp.Format(_T("Upgrade reason：%s"), g_pRECUpgrade->GetUpgradeErrorMsg(nSendSize));
	}
	//返回升级进度
	else if ( nTotalSize == -1 )
	{	
		pDataChnl->m_strProcess.Format("%d%c",nSendSize,'%');
		CWnd * pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PRO1);
		pWnd->SetWindowText(_CS("Upgrade.Progress"));
		pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PROCESS);
		pWnd->SetWindowText(pDataChnl->m_strProcess.GetBuffer(0));
		pDataChnl->m_progress1.SetPos(nSendSize);	//进度条
	}//云升级下载进度
	else if( nTotalSize == -2)
	{
		pDataChnl->m_strProcess.Format("%d%c",nSendSize,'%');
		CWnd * pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PRO1);
		pWnd->SetWindowText(_CS("Upgrade.DownloadProgress"));
		pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PROCESS);
		pWnd->SetWindowText(pDataChnl->m_strProcess.GetBuffer(0));
		pDataChnl->m_progress1.SetPos(nSendSize);	//进度条
	}
	//发送数据进度
	else
	{	
		pDataChnl->m_strProcess.Format("%d%c",nSendSize*100/nTotalSize,'%');
		CWnd * pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PRO1);
		pWnd->SetWindowText(_CS("Upgrade.Progress"));
		pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PROCESS);
		pWnd->SetWindowText(pDataChnl->m_strProcess.GetBuffer(0));
	}
}
CString CUpgradeDemoDlg::GetUpgradeErrorMsg(int nError)
{
	CString strError("");
	switch(nError)
	{
	case H264_DVR_NOENOUGH_MEMORY:
		strError += _CS(_T("No enough memory")); //内存不足
		break;
	case H264_DVR_INVALID_FORMAT:
		strError += _CS(_T("Invalid upgrade packet"));//升级文件格式不对
		break;
	case H264_DVR_UPDATE_PART_FAIL:
		strError += _CS(_T("Some part upgrade fail"));//某个分区升级失败
		break;
	case H264_DVR_INVALID_HARDWARE:
		strError += _CS(_T("Invalid hardware")); //硬件型号不匹配
		break;
	case H264_DVR_INVALID_VENDOR:
		strError += _CS(_T("Invalid vendor")); //客户信息不匹配
		break;
	case H264_DVR_INVALID_COMPALIBLE:
		strError += _CS(_T("Invalid compalible"));//升级程序的兼容版本号比设备现有的小，不允许设备升级回老程序
		break;
	case H264_DVR_INVALID_VERSION:
		strError += _CS(_T("Invalid version"));//版本无效
		break;
	case H264_DVR_INVALID_WIFI_DRIVE:
		strError += _CS(_T("Invalid wifi drive")); //升级程序里wifi驱动和设备当前在使用的wifi网卡不匹配
		break;
	default:
		strError += _CS(_T("Unknow error")); //未知错误
		break;
	}	
	return strError;
}
void CUpgradeDemoDlg::OnBnClickedBtnUpgrade()
{
	// TODO: 在此添加控件通知处理程序代码
	char *FileName = (LPSTR)(LPCTSTR)m_UpgradeFile;
	if(loginID > 0)
	{
		if(FileName)
		{
			long handlID = H264_DVR_Upgrade(loginID,FileName,0,cbUpgrade,(long)this);
			if(handlID <= 0)
			{
				MessageBox(_CS("Upgrade.UpgradeFail"));
				return;
			}
			else
			{
				MessageBox(_CS("Upgrade.UpgradeSuccess"));
			}
		}
		else
		{
			MessageBox("请输入正确文件!");
		}
	}
	else
	{
		MessageBox(_CS("DigitalChannel.NoLogin"));
	}
}
//在线更新系统文件
void CUpgradeDemoDlg::OnBnClickedBtnUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	CClientDemo5Dlg* pParent=((CClientDemo5Dlg*)AfxGetMainWnd());
	if(loginID > 0)
	{
		if(pParent->m_sysFunc.vOtherFunction[SDK_OTHER_SUPPORT_CLOUD_UPGRADE] == TRUE)
		{
			if(!m_UpgradeList.versions->length)
			{
				MessageBox("找不到升级文件！");
				return;
			}
			long handID = H264_DVR_Upgrade_Cloud(loginID, m_UpgradeList.versions, 0, cbUpgrade, (long)this);
			if(handID)
			{
				
			}
		}
		else
		{
			MessageBox("该设备不支持云升级！");
		}
	}
	else
	{
		MessageBox(_CS("NotFoundTheLatestSystem"));
	}
}