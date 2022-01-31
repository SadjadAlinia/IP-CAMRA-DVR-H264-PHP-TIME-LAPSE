// RestoreDefault.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "RestoreDefault.h"
#include "ConfigDemoDlg.h"


// CRestoreDefault �Ի���

IMPLEMENT_DYNAMIC(CRestoreDefault, CDialog)

CRestoreDefault::CRestoreDefault(CWnd* pParent /*=NULL*/)
	: CDialog(CRestoreDefault::IDD, pParent)
{
	n_update = FALSE;
	for(int i = 0; i < 10; ++i)
		m_DefaultConfig.vSetDefaultKinds[i] = FALSE;
}

CRestoreDefault::~CRestoreDefault()
{
}

void CRestoreDefault::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHK_ALL, m_ctrlCheckAll);
	DDX_Control(pDX, IDC_CHK_GENERAL, m_ctrlCheckGeneral);
	DDX_Control(pDX, IDC_CHK_ENCODE, m_ctrlCheckEncode);
	DDX_Control(pDX, IDC_CHK_RECORD, m_ctrlCheckRecord);
	DDX_Control(pDX, IDC_CHK_NET_SERVICE, m_ctrlCheckService);
	DDX_Control(pDX, IDC_CHK_NET_COMM, m_ctrlCheckNet);
	DDX_Control(pDX, IDC_CHK_ALARM, m_ctrlCheckAlarm);
	DDX_Control(pDX, IDC_CHK_COMM, m_ctrlCheckComm);
	DDX_Control(pDX, IDC_CHK_USER, m_ctrlCheckUser);
	DDX_Control(pDX, IDC_CHK_PREVIEW, m_ctrlCheckPreview);
	DDX_Control(pDX, IDC_CHK_WEBCAMERA, m_ctrlCheckWebCamera);
}


BEGIN_MESSAGE_MAP(CRestoreDefault, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK_SURE, OnBnClickedSure)
	ON_BN_CLICKED(IDCANNEL, OnBnClickedCannel)
	ON_BN_CLICKED(IDC_CHK_ALL, OnBnClickedChkAll)
	ON_BN_CLICKED(IDC_CHK_GENERAL, OnBnClickedChkGeneral)
	ON_BN_CLICKED(IDC_CHK_ENCODE, OnBnClickedChkEncode)
	ON_BN_CLICKED(IDC_CHK_RECORD, OnBnClickedChkRecord)
	ON_BN_CLICKED(IDC_CHK_NET_SERVICE, OnBnClickedChkNetService)
	ON_BN_CLICKED(IDC_CHK_NET_COMM, OnBnClickedChkNet)
	ON_BN_CLICKED(IDC_CHK_COMM, OnBnClickedChkComm)
	ON_BN_CLICKED(IDC_CHK_ALARM, OnBnClickedChkAlarm)
	ON_BN_CLICKED(IDC_CHK_USER, OnBnClickedChkUser)
	ON_BN_CLICKED(IDC_CHK_PREVIEW, OnBnClickedChkPreview)
	ON_BN_CLICKED(IDC_CHK_WEBCAMERA, OnBnClickedChkWebcamera)
END_MESSAGE_MAP()

BOOL CRestoreDefault::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATIC_SURE)->SetWindowText(_CS("ConfigDefault.Sure"));
	GetDlgItem(IDC_STATIC_ALL)->SetWindowText(_CS("ConfigDefault.AllDefault"));
	GetDlgItem(IDC_STATIC_GCONFIG)->SetWindowText(_CS("Title.GeneralConfig"));
	GetDlgItem(IDC_STATIC_ENCODE)->SetWindowText(_CS("Title.EncodeConfig"));
	GetDlgItem(IDC_STATIC_RCONFIG)->SetWindowText(_CS("Title.RecordConfig"));
	GetDlgItem(IDC_STATIC_NETC)->SetWindowText(_CS("Title.NetCommonConfig"));
	GetDlgItem(IDC_STATIC_DCONFIG)->SetWindowText(_CS("Title.DisplayConfig"));
	GetDlgItem(IDC_STATIC_CCONFIG)->SetWindowText(_CS("Title.CommConfig"));
	GetDlgItem(IDC_STATIC_ACONFIG)->SetWindowText(_CS("Title.AlarmConfig"));
	GetDlgItem(IDC_STATIC_NETSERVICE)->SetWindowText(_CS("Title.NetService"));
	GetDlgItem(IDC_STATIC_UMANAGE)->SetWindowText(_CS("Title.UserManager"));
	GetDlgItem(IDC_STATIC_CAMERA_PARAM)->SetWindowText(_CS("Title.WebCameraConfig"));
	GetDlgItem(IDOK_SURE)->SetWindowText(_CS("OneClick.OK"));
	GetDlgItem(IDCANNEL)->SetWindowText(_CS("OneClick.Cancel"));

	return TRUE;
}
void CRestoreDefault::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

}
// CRestoreDefault ��Ϣ�������
void CRestoreDefault::OnBnClickedSure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	long lLoginID = ((CClientDemo5Dlg*)AfxGetMainWnd())->m_LoginID;
	if (lLoginID <= 0)
	{
		return;
	}
	//�ָ�ѡ�����Ĭ����Ϣ
	//������ѡ��Ϣ����һ������
	int nRet = H264_DVR_SetDevConfig(loginID, E_SDK_CONFIG_DEFAULT, -1, (char*)m_DefaultConfig.vSetDefaultKinds, 10, 4000);
	if(nRet > 0)
	{
		//�ָ�Ĭ�ϳɹ�
		MessageBox(_CS("Error.DefaultSuccess"));
	}
	else
	{
		//�ָ�Ĭ��ʧ��
		MessageBox(_CS("Error.DefaultFail"));
	}
}

void CRestoreDefault::OnBnClickedCannel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CRestoreDefault::OnBnClickedChkAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ctrlCheckAll.GetCheck())
	{
		m_ctrlCheckGeneral.SetCheck(TRUE);
		m_ctrlCheckEncode.SetCheck(TRUE);
		m_ctrlCheckRecord.SetCheck(TRUE);
		m_ctrlCheckNet.SetCheck(TRUE);
		m_ctrlCheckService.SetCheck(TRUE);
		m_ctrlCheckComm.SetCheck(TRUE);
		m_ctrlCheckAlarm.SetCheck(TRUE);
		m_ctrlCheckUser.SetCheck(TRUE);
		m_ctrlCheckPreview.SetCheck(TRUE);
		m_ctrlCheckWebCamera.SetCheck(TRUE);
		for(int i=0; i<10; ++i)
			m_DefaultConfig.vSetDefaultKinds[i] = TRUE;
	}
	else
	{
		m_ctrlCheckGeneral.SetCheck(FALSE);
		m_ctrlCheckEncode.SetCheck(FALSE);
		m_ctrlCheckRecord.SetCheck(FALSE);
		m_ctrlCheckNet.SetCheck(FALSE);
		m_ctrlCheckService.SetCheck(FALSE);
		m_ctrlCheckComm.SetCheck(FALSE);
		m_ctrlCheckAlarm.SetCheck(FALSE);
		m_ctrlCheckUser.SetCheck(FALSE);
		m_ctrlCheckPreview.SetCheck(FALSE);
		m_ctrlCheckWebCamera.SetCheck(FALSE);
		for(int i=0; i<10; ++i)
			m_DefaultConfig.vSetDefaultKinds[i] = FALSE;
	}
	
}

void CRestoreDefault::OnBnClickedChkGeneral()	//��ͨ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[0] = m_ctrlCheckGeneral.GetCheck();
}

void CRestoreDefault::OnBnClickedChkEncode()	//��������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[1] = m_ctrlCheckEncode.GetCheck();
}

void CRestoreDefault::OnBnClickedChkRecord()	//¼������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[2] = m_ctrlCheckRecord.GetCheck();
}
void CRestoreDefault::OnBnClickedChkNetService()//�������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[3] = m_ctrlCheckService.GetCheck();
}
void CRestoreDefault::OnBnClickedChkNet()		//��������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[4] = m_ctrlCheckNet.GetCheck();
}

void CRestoreDefault::OnBnClickedChkAlarm()		//����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[5] = m_ctrlCheckAlarm.GetCheck();
}

void CRestoreDefault::OnBnClickedChkComm()		//��������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[6] = m_ctrlCheckComm.GetCheck();
}

void CRestoreDefault::OnBnClickedChkUser()		//�û�����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[7] = m_ctrlCheckUser.GetCheck();
}

void CRestoreDefault::OnBnClickedChkPreview()	//Ԥ������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[8] = m_ctrlCheckPreview.GetCheck();
}

void CRestoreDefault::OnBnClickedChkWebcamera()	//��������ͷ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DefaultConfig.vSetDefaultKinds[9] = m_ctrlCheckWebCamera.GetCheck();
}

void CRestoreDefault::InitDlgDefault()
{
	long lLoginID = ((CClientDemo5Dlg*)AfxGetMainWnd())->m_LoginID;
	if (lLoginID <= 0)
	{
		return;
	}
	//DWORD dwRetLen = 0;
	//int nWaitTime = 10000;
	//SDK_SetDefaultConfigTypes pDefault;
	//BOOL bSuccess = H264_DVR_GetDevConfig(lLoginID,E_SDK_CONFIG_DEFAULT,-1,
	//	(char *)&pDefault,sizeof(SDK_SetDefaultConfigTypes),&dwRetLen,nWaitTime);
	//if (bSuccess && dwRetLen == sizeof(SDK_SetDefaultConfigTypes))
	//{
	//	
	//}
}