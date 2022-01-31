// UpgradeDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "UpgradeDemoDlg.h"
#include "ConfigDemoDlg.h"


CUpgradeDemoDlg *g_pRECUpgrade = NULL;
// CUpgradeDemoDlg �Ի���

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

	m_progress1.SetRange(0, 100);	//�������Ĵ�С

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

	DDX_Control(pDX, IDC_PROGRESS_EXPORT, m_progress1);	//������
	DDX_Control(pDX, IDC_BTN_UPGRADE, m_Upgrade);
	DDX_Control(pDX, IDC_BTN_BROWSE, m_Browse);
	DDX_Text(pDX, IDC_EDIT_UPGRADEFILE, m_UpgradeFile);
	DDX_Text(pDX, IDC_STATIC_PROCESS, m_strProcess);
	DDX_Text(pDX, IDC_STATIC_UpgardeFile, m_strUpgradeFile);	//����ϵͳ�ļ�
}


BEGIN_MESSAGE_MAP(CUpgradeDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CUpgradeDemoDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_UPGRADE, &CUpgradeDemoDlg::OnBnClickedBtnUpgrade)
	ON_BN_CLICKED(IDC_BTN_Update, &CUpgradeDemoDlg::OnBnClickedBtnUpdate)
END_MESSAGE_MAP()


// CUpgradeDemoDlg ��Ϣ�������

void CUpgradeDemoDlg::OnBnClickedBtnBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//˵���������
	if ( nSendSize == -1 )
	{	
		pDataChnl->m_lUpgradeHandle = lUpgradechannel;
		CWnd * pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PROCESS);
		pWnd->SetWindowText("100%");
		pDataChnl->MessageBox(_CS("Upgrade.UpgradeSuccess"));
		pDataChnl->m_progress1.SetPos(0);
		pWnd->SetWindowText("");
	}
	//��������
	else if ( nSendSize == -2 )
	{
			pDataChnl->MessageBox(_CS("Upgrade.UpgradeFail"));
	}
	else if( nSendSize < -2)
	{
		CString strTemp;
		strTemp.Format(_T("Upgrade reason��%s"), g_pRECUpgrade->GetUpgradeErrorMsg(nSendSize));
	}
	//������������
	else if ( nTotalSize == -1 )
	{	
		pDataChnl->m_strProcess.Format("%d%c",nSendSize,'%');
		CWnd * pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PRO1);
		pWnd->SetWindowText(_CS("Upgrade.Progress"));
		pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PROCESS);
		pWnd->SetWindowText(pDataChnl->m_strProcess.GetBuffer(0));
		pDataChnl->m_progress1.SetPos(nSendSize);	//������
	}//���������ؽ���
	else if( nTotalSize == -2)
	{
		pDataChnl->m_strProcess.Format("%d%c",nSendSize,'%');
		CWnd * pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PRO1);
		pWnd->SetWindowText(_CS("Upgrade.DownloadProgress"));
		pWnd = pDataChnl->GetDlgItem(IDC_STATIC_PROCESS);
		pWnd->SetWindowText(pDataChnl->m_strProcess.GetBuffer(0));
		pDataChnl->m_progress1.SetPos(nSendSize);	//������
	}
	//�������ݽ���
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
		strError += _CS(_T("No enough memory")); //�ڴ治��
		break;
	case H264_DVR_INVALID_FORMAT:
		strError += _CS(_T("Invalid upgrade packet"));//�����ļ���ʽ����
		break;
	case H264_DVR_UPDATE_PART_FAIL:
		strError += _CS(_T("Some part upgrade fail"));//ĳ����������ʧ��
		break;
	case H264_DVR_INVALID_HARDWARE:
		strError += _CS(_T("Invalid hardware")); //Ӳ���ͺŲ�ƥ��
		break;
	case H264_DVR_INVALID_VENDOR:
		strError += _CS(_T("Invalid vendor")); //�ͻ���Ϣ��ƥ��
		break;
	case H264_DVR_INVALID_COMPALIBLE:
		strError += _CS(_T("Invalid compalible"));//��������ļ��ݰ汾�ű��豸���е�С���������豸�������ϳ���
		break;
	case H264_DVR_INVALID_VERSION:
		strError += _CS(_T("Invalid version"));//�汾��Ч
		break;
	case H264_DVR_INVALID_WIFI_DRIVE:
		strError += _CS(_T("Invalid wifi drive")); //����������wifi�������豸��ǰ��ʹ�õ�wifi������ƥ��
		break;
	default:
		strError += _CS(_T("Unknow error")); //δ֪����
		break;
	}	
	return strError;
}
void CUpgradeDemoDlg::OnBnClickedBtnUpgrade()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			MessageBox("��������ȷ�ļ�!");
		}
	}
	else
	{
		MessageBox(_CS("DigitalChannel.NoLogin"));
	}
}
//���߸���ϵͳ�ļ�
void CUpgradeDemoDlg::OnBnClickedBtnUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CClientDemo5Dlg* pParent=((CClientDemo5Dlg*)AfxGetMainWnd());
	if(loginID > 0)
	{
		if(pParent->m_sysFunc.vOtherFunction[SDK_OTHER_SUPPORT_CLOUD_UPGRADE] == TRUE)
		{
			if(!m_UpgradeList.versions->length)
			{
				MessageBox("�Ҳ��������ļ���");
				return;
			}
			long handID = H264_DVR_Upgrade_Cloud(loginID, m_UpgradeList.versions, 0, cbUpgrade, (long)this);
			if(handID)
			{
				
			}
		}
		else
		{
			MessageBox("���豸��֧����������");
		}
	}
	else
	{
		MessageBox(_CS("NotFoundTheLatestSystem"));
	}
}