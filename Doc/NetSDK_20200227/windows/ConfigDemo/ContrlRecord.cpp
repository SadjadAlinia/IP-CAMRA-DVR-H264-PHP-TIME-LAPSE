// ContrlRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "ContrlRecord.h"
#include "ConfigDemoDlg.h"


// CContrlRecord 对话框

IMPLEMENT_DYNAMIC(CContrlRecord, CDialog)

CContrlRecord::CContrlRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CContrlRecord::IDD, pParent)
	, m_All1(0), m_Channel1(0), m_Channel2(0), m_Channel3(0)
	, m_Channel4(0), m_Channel5(0), m_Channel6(0), m_Channel7(0)
	, m_Channel8(0), m_Channel9(0), m_Channel10(0)
	, m_Channel11(0), m_Channel12(0), m_Channel13(0)
	, m_Channel14(0), m_Channel15(0), m_Channel16(0)
{
}

CContrlRecord::~CContrlRecord()
{
}

void CContrlRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ALL1, m_All1);
	DDX_Radio(pDX, IDC_11, m_Channel1);
	DDX_Radio(pDX, IDC_12, m_Channel2);
	DDX_Radio(pDX, IDC_13, m_Channel3);
	DDX_Radio(pDX, IDC_14, m_Channel4);
	DDX_Radio(pDX, IDC_15, m_Channel5);
	DDX_Radio(pDX, IDC_16, m_Channel6);
	DDX_Radio(pDX, IDC_17, m_Channel7);
	DDX_Radio(pDX, IDC_18, m_Channel8);
	DDX_Radio(pDX, IDC_19, m_Channel9);
	DDX_Radio(pDX, IDC_110, m_Channel10);
	DDX_Radio(pDX, IDC_111, m_Channel11);
	DDX_Radio(pDX, IDC_112, m_Channel12);
	DDX_Radio(pDX, IDC_113, m_Channel13);
	DDX_Radio(pDX, IDC_114, m_Channel14);
	DDX_Radio(pDX, IDC_115, m_Channel15);
	DDX_Radio(pDX, IDC_116, m_Channel16);
}


BEGIN_MESSAGE_MAP(CContrlRecord, CDialog)
	ON_BN_CLICKED(IDC_BTNCONCTRLOK, OnBnClickedBtnconctrlok)
	ON_BN_CLICKED(IDC_BTNCONCTRLCNACEL, OnBnClickedBtnconctrlcnacel)
	ON_BN_CLICKED(IDC_ALL1, &CContrlRecord::OnBnClickedAll1)
	ON_BN_CLICKED(IDC_ALL2, &CContrlRecord::OnBnClickedAll2)
	ON_BN_CLICKED(IDC_ALL3, &CContrlRecord::OnBnClickedAll3)
	ON_BN_CLICKED(IDC_ALL4, &CContrlRecord::OnBnClickedAll4)
	ON_BN_CLICKED(IDC_ALL5, &CContrlRecord::OnBnClickedAll5)
END_MESSAGE_MAP()


// CContrlRecord 消息处理程序
BOOL CContrlRecord::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_RECONFIG)->SetWindowText(_CS("RecordConfig.Mode"));
	GetDlgItem(IDC_REAUTO)->SetWindowText(_CS("Record.Auto"));
	GetDlgItem(IDC_REMANUAL)->SetWindowText(_CS("Record.Manual"));
	GetDlgItem(IDC_RESTREAM)->SetWindowText(_CS("Record.ExtStream"));
	GetDlgItem(IDC_REAUTOST)->SetWindowText(_CS("Record.Auto"));
	GetDlgItem(IDC_REMANUALST)->SetWindowText(_CS("Record.Manual"));
	GetDlgItem(IDC_RECLOSED)->SetWindowText(_CS("Record.Closed"));
	GetDlgItem(IDC_BTNCONCTRLOK)->SetWindowText(_CS("Record.SaveConfig"));
	GetDlgItem(IDC_BTNCONCTRLCNACEL)->SetWindowText(_CS("Record.Update"));

	return TRUE;
}
void CContrlRecord::CleanAll()
{
	SetAll(FALSE);
	SetChannel1(FALSE);
	SetChannel2(FALSE);
	SetChannel3(FALSE);
	SetChannel4(FALSE);
	SetChannel5(FALSE);
}
void CContrlRecord::OnBnClickedBtnconctrlok()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BOOL ContrlFailflag = FALSE;	//设置成功的标志
	CClientDemo5Dlg* pParent = ((CClientDemo5Dlg*)AfxGetMainWnd());

	if(0 != pParent->m_LoginID)
	{
		return;
	}
	//判断哪个通道是支持辅码流录像设置
	SDK_AbilityMask m_mask;
	SDK_RECORDCONFIG_ALL RecordConfig;
	unsigned long lRelen;
	H264_DVR_GetDevConfig(pParent->m_LoginID, E_SDK_CFG_EXT_RECORD, -1,
		(char*)&RecordConfig, sizeof(SDK_RECORDCONFIG_ALL), &lRelen);
	RecordConfig.vRecordConfigAll[0].iRecordMode = 4-m_Channel1;
	RecordConfig.vRecordConfigAll[1].iRecordMode = 4-m_Channel2;
	RecordConfig.vRecordConfigAll[2].iRecordMode = 4-m_Channel3;
	RecordConfig.vRecordConfigAll[3].iRecordMode = 4-m_Channel4;
	RecordConfig.vRecordConfigAll[4].iRecordMode = 4-m_Channel5;
	RecordConfig.vRecordConfigAll[5].iRecordMode = 4-m_Channel6;
	RecordConfig.vRecordConfigAll[6].iRecordMode = 4-m_Channel7;
	RecordConfig.vRecordConfigAll[7].iRecordMode = 4-m_Channel8;
	RecordConfig.vRecordConfigAll[8].iRecordMode = 4-m_Channel9;
	RecordConfig.vRecordConfigAll[9].iRecordMode = 4-m_Channel10;
	RecordConfig.vRecordConfigAll[10].iRecordMode = 4-m_Channel11;
	RecordConfig.vRecordConfigAll[11].iRecordMode = 4-m_Channel12;
	RecordConfig.vRecordConfigAll[12].iRecordMode = 4-m_Channel13;
	RecordConfig.vRecordConfigAll[13].iRecordMode = 4-m_Channel14;
	RecordConfig.vRecordConfigAll[14].iRecordMode = 4-m_Channel15;
	RecordConfig.vRecordConfigAll[15].iRecordMode = 4-m_Channel16;
	int nRet = H264_DVR_GetDevConfig(pParent->m_LoginID, E_SDK_ABILITY_SUPPORT_EXTSTREAM, -1,
		(char*)&m_mask, sizeof(SDK_AbilityMask), &lRelen, 10000);
	for(int Channel = 0; Channel < 16; ++Channel)
	{
		if(pParent->m_RecordChannel[Channel])
		{
			//找到支持辅码流设备的通道
			nRet = H264_DVR_SetDevConfig(pParent->m_LoginID, E_SDK_CFG_EXT_RECORD, Channel,
				(char*)&RecordConfig.vRecordConfigAll[Channel], sizeof(SDK_RECORDCONFIG), 10000);
			if(nRet > 0)
			{
				MessageBox(_CS("Error.SaveSuccess"));
			}
			else
			{
				ContrlFailflag = TRUE;
			}
		}
	}
	if(ContrlFailflag)
	{
		MessageBox(_CS("Error.SetConfigFail"));
	}
	else
	{
		return;
	}
	
}

void CContrlRecord::OnBnClickedBtnconctrlcnacel()
{
	// TODO: 在此添加控件通知处理程序代码
	CClientDemo5Dlg* pParent = ((CClientDemo5Dlg*)AfxGetMainWnd());
	for(int i=0; i<sizeof(pParent->m_RecordChannel); ++i)
	{
		if(!pParent->m_RecordChannel[i])
		{
			MessageBox(_CS("Desktop.UnsupportRecord"));
			return;
		}
	}
}
void CContrlRecord::OnBnClickedAll1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_All1 = 1;
	RadioAll(m_All1);
}

void CContrlRecord::OnBnClickedAll2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_All1 = 2;
	RadioAll(m_All1);
}

void CContrlRecord::OnBnClickedAll3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_All1 = 3;
	RadioAll(m_All1);
}

void CContrlRecord::OnBnClickedAll4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_All1 = 4;
	RadioAll(m_All1);
}

void CContrlRecord::OnBnClickedAll5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_All1 = 5;
	RadioAll(m_All1);
}
void CContrlRecord::RadioAll(int nRet)
{
	switch(nRet)
	{
		case 1:
			SetChannel1(TRUE);SetChannel2(FALSE);SetChannel3(FALSE);
			SetChannel4(FALSE);SetChannel5(FALSE);
			break;
		case 2:
			SetChannel1(FALSE);SetChannel2(TRUE);SetChannel3(FALSE);
			SetChannel4(FALSE);SetChannel5(FALSE);
			break;
		case 3:
			SetChannel1(FALSE);SetChannel2(FALSE);SetChannel3(TRUE);
			SetChannel4(FALSE);SetChannel5(FALSE);
			break;
		case 4:
			SetChannel1(FALSE);SetChannel2(FALSE);SetChannel3(FALSE);
			SetChannel4(TRUE);SetChannel5(FALSE);
			break;
		case 5:
			SetChannel1(FALSE);SetChannel2(FALSE);SetChannel3(FALSE);
			SetChannel4(FALSE);SetChannel5(TRUE);
			break;
		default:
			break;
	}
}
void CContrlRecord::SetAll(BOOL nRet)
{
	((CButton *)GetDlgItem(IDC_ALL1))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_ALL2))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_ALL3))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_ALL4))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_ALL5))->SetCheck(nRet);
}
void CContrlRecord::SetChannel1(BOOL nRet)
{
	 ((CButton *)GetDlgItem(IDC_11))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_12))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_13))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_14))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_15))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_16))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_17))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_18))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_19))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_110))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_111))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_112))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_113))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_114))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_115))->SetCheck(nRet);
	 ((CButton *)GetDlgItem(IDC_116))->SetCheck(nRet);
}
void CContrlRecord::SetChannel2(BOOL nRet)
{
	((CButton *)GetDlgItem(IDC_21))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_22))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_23))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_24))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_25))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_26))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_26))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_27))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_28))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_29))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_210))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_211))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_212))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_213))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_214))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_215))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_216))->SetCheck(nRet);
}
void CContrlRecord::SetChannel3(BOOL nRet)
{
	((CButton *)GetDlgItem(IDC_31))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_32))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_33))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_34))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_35))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_36))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_37))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_38))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_39))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_310))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_311))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_312))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_313))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_314))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_315))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_316))->SetCheck(nRet);
}
void CContrlRecord::SetChannel4(BOOL nRet)
{
	((CButton *)GetDlgItem(IDC_41))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_42))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_43))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_44))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_45))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_46))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_47))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_48))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_49))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_410))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_411))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_412))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_413))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_414))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_415))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_416))->SetCheck(nRet);
}
void CContrlRecord::SetChannel5(BOOL nRet)
{
	((CButton *)GetDlgItem(IDC_51))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_52))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_53))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_54))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_55))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_56))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_57))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_58))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_59))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_510))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_511))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_512))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_513))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_514))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_515))->SetCheck(nRet);
	((CButton *)GetDlgItem(IDC_516))->SetCheck(nRet);
}
BOOL CContrlRecord::GetChannel1()
{
	if(!((CButton *)GetDlgItem(IDC_11))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_12))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_13))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_14))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_15))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_16))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_17))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_18))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_19))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_110))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_111))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_112))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_113))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_114))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_115))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_116))->GetCheck()){return FALSE;}
}
BOOL CContrlRecord::GetChannel2()
{
	if(!((CButton *)GetDlgItem(IDC_21))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_22))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_23))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_24))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_25))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_26))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_27))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_28))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_29))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_210))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_211))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_212))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_213))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_214))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_215))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_216))->GetCheck()){return FALSE;}
}
BOOL CContrlRecord::GetChannel3()
{
	if(!((CButton *)GetDlgItem(IDC_31))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_32))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_33))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_34))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_35))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_36))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_37))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_38))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_39))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_310))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_311))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_312))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_313))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_314))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_315))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_316))->GetCheck()){return FALSE;}
}
BOOL CContrlRecord::GetChannel4()
{
	if(!((CButton *)GetDlgItem(IDC_41))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_42))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_43))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_44))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_45))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_46))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_47))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_48))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_49))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_410))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_411))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_412))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_413))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_414))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_415))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_416))->GetCheck()){return FALSE;}
}
BOOL CContrlRecord::GetChannel5()
{
	if(!((CButton *)GetDlgItem(IDC_51))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_52))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_53))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_54))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_55))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_56))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_57))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_58))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_59))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_510))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_511))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_512))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_513))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_514))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_515))->GetCheck()){return FALSE;}
	if(!((CButton *)GetDlgItem(IDC_516))->GetCheck()){return FALSE;}
}