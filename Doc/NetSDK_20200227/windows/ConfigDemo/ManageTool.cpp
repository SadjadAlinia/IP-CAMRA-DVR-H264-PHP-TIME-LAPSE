// ManageTool.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "ManageTool.h"
#include "ConfigDemoDlg.h"


// CManageTool �Ի���

IMPLEMENT_DYNAMIC(CManageTool, CDialog)

CManageTool::CManageTool(CWnd* pParent /*=NULL*/)
	: CDialog(CManageTool::IDD, pParent)
{

}

CManageTool::~CManageTool()
{
}
BOOL CManageTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_BTN_AutoManage)->SetWindowText(_CS("Manage.AutoManagement"));
	GetDlgItem(IDC_BTN_RD)->SetWindowText(_CS("Manage.RestoreDefault"));
	GetDlgItem(IDC_BTN_IAE)->SetWindowText(_CS("Manage.ImportAExport"));
	GetDlgItem(IDC_BTN_RESTART)->SetWindowText(_CS("Manage.Restart"));

	return TRUE;
}
void CManageTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_AutoManage, m_AutoManagement);
	DDX_Control(pDX, IDC_BTN_RD, m_RestoreDefault);
	DDX_Control(pDX, IDC_BTN_IAE, m_ImportAExport);
	DDX_Control(pDX, IDC_BTN_RESTART, m_Restart);
}


BEGIN_MESSAGE_MAP(CManageTool, CDialog)
	ON_BN_CLICKED(IDC_BTN_AutoManage, &CManageTool::OnBnClickedBtnAutomanage)
	ON_BN_CLICKED(IDC_BTN_RD, &CManageTool::OnBnClickedBtnRd)
	ON_BN_CLICKED(IDC_BTN_IAE, &CManageTool::OnBnClickedBtnIae)
	ON_BN_CLICKED(IDC_BTN_RESTART, &CManageTool::OnBnClickedBtnRestart)
END_MESSAGE_MAP()


// CManageTool ��Ϣ�������
//�Զ�ά��
void CManageTool::OnBnClickedBtnAutomanage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_AutoRoot.DoModal();
}
//�ָ�Ĭ��
void CManageTool::OnBnClickedBtnRd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Default.DoModal();
}
//���뵼��
void CManageTool::OnBnClickedBtnIae()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Import.DoModal();
}
//����
void CManageTool::OnBnClickedBtnRestart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Ret.DoModal();
}
void CManageTool::Control()
{
	/*m_AutoRoot.InitDlgAuto();*/
	/*m_Default.InitDlgDefault();*/
}