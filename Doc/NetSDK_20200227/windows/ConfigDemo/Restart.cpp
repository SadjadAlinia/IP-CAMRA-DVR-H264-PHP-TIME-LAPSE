// Restart.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "Restart.h"
#include "ConfigDemoDlg.h"

// CRestart �Ի���

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
// CRestart ��Ϣ�������
void CRestart::OnBnClickedRes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	long lLoginID = ((CClientDemo5Dlg*)AfxGetMainWnd())->m_LoginID;
	if (lLoginID <= 0)
	{
		return;
	}
	BOOL ret = H264_DVR_ControlDVR(lLoginID, 0, 10000);
	if(ret)
	{
		MessageBox(_CS("Error.RebootSuccess"));	//�����ɹ�
		OnCancel();	//�رմ���
	}
	else
	{
		MessageBox(_CS("Error.RebootFail"));	//����ʧ��
		
	}
}

void CRestart::OnBnClickedRestart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}