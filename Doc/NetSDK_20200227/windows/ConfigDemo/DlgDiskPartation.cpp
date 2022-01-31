// DlgDiskPartation.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "DlgDiskPartation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDiskPartation dialog


CDlgDiskPartation::CDlgDiskPartation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDiskPartation::IDD, pParent)
	, m_nRecord(0)
	, m_nSnap(0)
{
	//{{AFX_DATA_INIT(CDlgDiskPartation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDiskPartation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDiskPartation)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDIT_RECORD, m_nRecord);
	DDX_Text(pDX, IDC_EDIT_SNAP, m_nSnap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDiskPartation, CDialog)
	//{{AFX_MSG_MAP(CDlgDiskPartation)
	ON_EN_CHANGE(IDC_EDIT_RECORD, CDlgDiskPartation::OnEnChangeEditRecord)
	ON_EN_CHANGE(IDC_EDIT_SNAP, CDlgDiskPartation::OnEnChangeEditSnap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDiskPartation message handlers

BOOL CDlgDiskPartation::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	_CWndCS(this);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDiskPartation::OnEnChangeEditRecord()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_nRecord < 0)
	{
		m_nRecord = 0;
	}
	if (m_nRecord > m_nTotal)
	{
		m_nRecord = m_nTotal;
	}
	m_nSnap = m_nTotal - m_nRecord;
	UpdateData(FALSE);
}

void CDlgDiskPartation::OnEnChangeEditSnap()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_nSnap < 0)
	{
		m_nSnap = 0;
	}
	if (m_nSnap > m_nTotal)
	{
		m_nSnap = m_nTotal;
	}
	m_nRecord = m_nTotal - m_nSnap;
	UpdateData(FALSE);
}
