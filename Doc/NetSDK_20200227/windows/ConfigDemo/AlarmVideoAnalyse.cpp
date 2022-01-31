// AlarmVideoAnalyse.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "AlarmVideoAnalyse.h"
//#include "UserDataManager.h"
#include "PtzLinkDlg.h"
#include "PeriodTimeSetDlg.h"

#include "AlarmInputDlg.h"

#include "VideoAVDRuleSetDlg.h"

// CAlarmVideoAnalyse �Ի���
extern long loginID;
IMPLEMENT_DYNAMIC(CAlarmVideoAnalyse, CDialog)

CAlarmVideoAnalyse::CAlarmVideoAnalyse(CWnd* pParent /*=NULL*/)
{
	m_bAnalyse = true;
	int i = 0; 
	for ( i = 0; i < NET_MAX_CHANNUM; i ++ )
		m_pBtnsRecord[i] = NULL;	//¼��ѡ��ť

	for ( i = 0; i < NET_MAX_CHANNUM; i ++ )
		m_pBtnsTour[i] = NULL;		//

	for ( i = 0; i < NET_MAX_CHANNUM; i ++ )
		m_pBtnsAlarmOut[i] = NULL;

	for (i = 0; i < NET_MAX_CHANNUM; i++)
		m_pBtnsSnap[i] = NULL;

	for ( i =0 ; i < NET_MAX_CHANNUM; i ++ )
	{
		m_bGet[i] = false;
	}
	memset(&m_analyse,0,sizeof(m_analyse));
	memset(&m_Oldanalyse,0,sizeof(m_Oldanalyse));
	memset( &m_coptData, 0, sizeof(m_coptData) );
	m_bCopy = false;
	memset(m_oldrect,0, sizeof(m_oldrect));
	memset(m_newrect,0,sizeof(m_newrect));

	memset(&m_analyseAbility,0,sizeof(m_analyseAbility));
	memset(&m_oldAnalyseAbility,0,sizeof(m_oldAnalyseAbility));
	m_bGetAnalyseAbility = false;
	
	m_LoginID = 0;
}

CAlarmVideoAnalyse::~CAlarmVideoAnalyse()
{
}

void CAlarmVideoAnalyse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlarmVideoAnalyse, CDialog)
	ON_BN_CLICKED(IDC_BTN_ALARM_CTRL_PERIOD, CAlarmVideoAnalyse::OnBnClickedBtnAlarmCtrlPeriod)
	ON_BN_CLICKED(IDC_BTN_RULE_SET, CAlarmVideoAnalyse::OnBnClickedBtnRuleSet)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, CAlarmVideoAnalyse::OnCbnSelchangeComboChannel)
	ON_BN_CLICKED(IDC_BTN_REFUSH, CAlarmVideoAnalyse::OnBnClickedBtnRefush)
	ON_BN_CLICKED(IDOK, CAlarmVideoAnalyse::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BTN_COPY, CAlarmVideoAnalyse::OnBnClickedCopy)
	//ON_BN_CLICKED(IDC_BTN_PLASTER, CAlarmVideoAnalyse::OnBnClickedPlaster)
	ON_WM_SHOWWINDOW()

	ON_EN_CHANGE(IDC_EDIT_INTERL_TIME, CAlarmVideoAnalyse::OnChangeIntelTime)
	ON_EN_CHANGE(IDC_EDIT_IGNORE_TIME, CAlarmVideoAnalyse::OnChangeIgnoreTime)
	ON_EN_CHANGE(IDC_EDIT_RECORD_IG_TIME, CAlarmVideoAnalyse::OnChangeRecIgTime)
	ON_EN_KILLFOCUS(IDC_EDIT_IGNORE_TIME, CAlarmVideoAnalyse::OnEnKillfocusEditIgnoreTime)
	ON_EN_KILLFOCUS(IDC_EDIT_RECORD_IG_TIME, CAlarmVideoAnalyse::OnEnKillfocusEditRecordIgTime)
	ON_BN_CLICKED(IDC_CHECK_ON_OFF, CAlarmVideoAnalyse::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_BTN_PTZLINK, CAlarmVideoAnalyse::OnBnClickedBtnPtzlink)
END_MESSAGE_MAP()


// CAlarmVideoAnalyse ��Ϣ�������
BOOL CAlarmVideoAnalyse::OnInitDialog()
{
	CDialog::OnInitDialog();
	_CWndCS(this);
	//	AfxSetResourceHandle(OldResourceHanlde);//�Ի����Ѵ��������û�ԭ����ģ��  
	GetPosition();
// 	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ARITHIMETIC);
// 	CString strText;
// 	strText.Format("%s", _CS("ConfigAlarm.PEA") );      
// 	int nInsert = pComboBox->AddString(strText);
// 	pComboBox->SetItemData(nInsert, 0);
// 
// 	strText.Format("%s", _CS("ConfigAlarm.OSC") );
// 	nInsert = pComboBox->AddString(strText);
// 	pComboBox->SetItemData(nInsert, 1);
// 
// 
// 	strText.Format("%s", _CS("ConfigAlarm.AVD") );
// 	nInsert = pComboBox->AddString(strText);
// 	pComboBox->SetItemData(nInsert, 2);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CAlarmVideoAnalyse::OnBnClickedBtnAlarmCtrlPeriod()
{
	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);
	//��ѡ��ȫ��ͨ��ʱ��Ĭ���޸ĵ��ǵ�һͨ��������������ʱ���ȫ����ֵ
	if ( nChannelNum == (deviceInfo.byChanNum + deviceInfo.iDigChannel))
	{
		nChannelNum = 0;
	}

	CPeriodTimeSetDlg dlg( &m_analyse.vAnalyzeAll[nChannelNum].hEvent.schedule );
	if ( IDOK ==  dlg.DoModal() )
	{
		memcpy( &m_analyse.vAnalyzeAll[nChannelNum].hEvent.schedule, 
			dlg.GetConfigWorkSheet(), sizeof( SDK_CONFIG_WORKSHEET ) );
	}
}

void CAlarmVideoAnalyse::OnBnClickedBtnRuleSet()
{
	// nLoginID = m_LoginID;
	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);
	//��ѡ��ȫ��ͨ��ʱ��Ĭ���޸ĵ��ǵ�һͨ��������������ʱ���ȫ����ֵ
	if ( nChannelNum == (deviceInfo.byChanNum + deviceInfo.iDigChannel))
	{
		nChannelNum = 0;
	}
	
	int nModuleType = GetComboBox(IDC_COMBO_ARITHIMETIC)->GetCurSel();
	switch (nModuleType)
	{
	case SDK_MODULE_TYPE_PEA:
		{
			//CVideoAnalyseSetDlg dlgVideoAnalyseSet(&m_analyse.vAnalyzeAll[nChannelNum].stRuleConfig,nChannelNum,nModuleType);
			dlgVideoAnalyseSet.SetValue(&m_analyse.vAnalyzeAll[nChannelNum].stRuleConfig,nChannelNum,nModuleType);
			if (IDOK==dlgVideoAnalyseSet.DoModal())
			{
				memcpy(&m_analyse.vAnalyzeAll[nChannelNum].stRuleConfig,&dlgVideoAnalyseSet.m_Rule,sizeof(SDK_RULECONFIG));
			}
		}	
		break;
 	case SDK_MODULE_TYPE_OSC:
		{
			// CVideoOSCRuleSetDlg dlgVideoOscRuleSet(&m_analyse.vAnalyzeAll[nChannelNum].stRuleConfig.stRuleOSC,nChannelNum);
			dlgVideoOscRuleSet.SetValue(&m_analyse.vAnalyzeAll[nChannelNum].stRuleConfig.stRuleOSC,nChannelNum);
			if (IDOK==dlgVideoOscRuleSet.DoModal())
			{
				memcpy(&m_analyse.vAnalyzeAll[nChannelNum].stRuleConfig.stRuleOSC,&dlgVideoOscRuleSet.m_Rule,sizeof(SDK_OSC_RULE_S));
			}
		}
 		break;
	case SDK_MODULE_TYPE_AVD:
		{
			CVideoAVDRuleSetDlg dlgVideoAvdRuleSet(&m_analyse.vAnalyzeAll[nChannelNum].stRuleConfig.stRuleAVD,nChannelNum);
			if (IDOK==dlgVideoAvdRuleSet.DoModal())
			{
				memcpy(&m_analyse.vAnalyzeAll[nChannelNum].stRuleConfig.stRuleAVD,&dlgVideoAvdRuleSet.m_rule,sizeof(SDK_AVD_RULE_S));
			}
		}	
		break;
	}
	
}

BOOL CAlarmVideoAnalyse::InitWndShow()
{
	Clear();
	
	#define HEIGHT_BTN  15   //��ʱ���ô����İ�ť�Ĵ�СΪ15������֮ǰ�ľ�̬�ı������㣬������Ҫ�����ı��߶�
	m_nLastChannelNum = -1;
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);

	//�����������������CHECK BOX
	CString strBtnTitle;
	int nVideoOutChannel = deviceInfo.byAlarmOutPortNum;
	ZeroMemory( m_pBtnsAlarmOut, sizeof(m_pBtnsAlarmOut) );

	int i = 0, j = 0;
	CRect rcOffSet;
	CRect rcAlarmOut;
	GetButton(IDC_STATIC_ALARM_OUT)->GetWindowRect(rcAlarmOut);
	ScreenToClient(rcAlarmOut);
	GetButton(IDC_BTN_COPY)->ShowWindow(SW_HIDE);
	GetButton(IDC_BTN_PLASTER)->ShowWindow(SW_HIDE);
	if (nVideoOutChannel > 0)
	{
		rcAlarmOut.left = rcAlarmOut.right;
		rcAlarmOut.right = rcAlarmOut.left + 20;
		rcAlarmOut.bottom = rcAlarmOut.top + HEIGHT_BTN;

		for ( i = 0; i < nVideoOutChannel; ++i )
		{
			strBtnTitle.Format("%d", i + 1);

			m_pBtnsAlarmOut[i] = new CButton();
			m_pBtnsAlarmOut[i]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE, rcAlarmOut, this, IDC_BTN_ALARMOUT + i);

			rcAlarmOut.left  += 20;
			rcAlarmOut.right += 20;
		}
		GetButton(IDC_STATIC_ALARM_OUT)->ShowWindow(SW_SHOW);
		GetButton(IDC_STATIC_ALARMOUT_LATCH)->ShowWindow(SW_SHOW);
		GetButton(IDC_EDIT_IGNORE_TIME)->ShowWindow(SW_SHOW);
		GetButton(IDC_STATIC_SEC2)->ShowWindow(SW_SHOW);

		GetButton(IDC_STATIC_TIMESECT)->GetWindowRect(rcOffSet);
		ScreenToClient(rcOffSet);
		rcOffSet.top = rcAlarmOut.bottom + 8;
	}
	else
	{
		GetButton(IDC_STATIC_ALARM_OUT)->ShowWindow(SW_HIDE);
		GetButton(IDC_STATIC_ALARMOUT_LATCH)->ShowWindow(SW_HIDE);
		GetButton(IDC_EDIT_IGNORE_TIME)->ShowWindow(SW_HIDE);
		GetButton(IDC_STATIC_SEC2)->ShowWindow(SW_HIDE);
		rcOffSet.bottom = rcAlarmOut.bottom;
		rcOffSet.top = rcAlarmOut.top;
	}
	//CUserDataManager *pData = CUserDataManager::instance();
	bool bSnap = false;
	
	//DWORD nReturn=0;
	SDK_SystemFunction sysFunc;
	H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_ABILITY_SYSFUNC,-1, (char *)&sysFunc,sizeof(sysFunc),&nReturn);
	//if ( pData )
	//{
		//SDK_SystemFunction* pFunc = pData->GetsysFunc();
		bSnap =  sysFunc.vEncodeFunction[SDK_ENCODE_FUNCTION_TYPE_SNAP_STREAM];
		if ( deviceInfo.byChanNum == 0 )
		{
			bSnap = false;
		}

		if (false == bSnap)
		{
			GetButton(IDC_STATIC_ANALYSE_SNAP)->ShowWindow(SW_HIDE);
		}
		else
		{
			GetButton(IDC_STATIC_ANALYSE_SNAP)->ShowWindow(SW_SHOW);
		}
	//}

	bool bFtp = false;
	//if ( pData )
	//{
		//SDK_SystemFunction* pFunc = pData->GetsysFunc();
		bFtp =  sysFunc.vNetServerFunction[SDK_NET_SERVER_TYPES_FTP];
		if (false == bFtp)
		{
			GetButton(IDC_CHECK_FTP)->ShowWindow(SW_HIDE);
			GetButton(IDC_STATIC_FTP)->ShowWindow(SW_HIDE);
		}
		else
		{
			GetButton(IDC_CHECK_FTP)->ShowWindow(SW_SHOW);
			GetButton(IDC_STATIC_FTP)->ShowWindow(SW_SHOW);
		}
	//}

	//��̨����
	bool bPtz = true;
	COleDateTime VesionTime0(2011, 11, 14, 13, 53, 32);
	COleDateTime VesionTime1(deviceInfo.tmBuildTime.year, deviceInfo.tmBuildTime.month, deviceInfo.tmBuildTime.day,
		deviceInfo.tmBuildTime.hour, deviceInfo.tmBuildTime.minute, deviceInfo.tmBuildTime.second);  //��ǰ�豸�İ汾ʱ��

	if ( /*pData &&*/ VesionTime1 >= VesionTime0 )
	{
		bPtz = sysFunc.vCommFunction[SDK_COMM_TYPES_RS485];
		bPtz &= (deviceInfo.byChanNum + deviceInfo.iDigChannel > 0);
		if ( bPtz )
		{
			//��ʾ
			GetDlgItem(IDC_BTN_PTZLINK)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_PTZ_LINK)->ShowWindow(SW_SHOW);
			//GetDlgItem(IDC_STATIC_DELAY_RECORDTIME)->ShowWindow(SW_SHOW);
			//GetDlgItem(IDC_EDIT_RECORD_IG_TIME)->ShowWindow(SW_SHOW);
			//GetDlgItem(IDC_STATIC_SEC3)->ShowWindow(SW_SHOW);
		}else
		{
			//����
			CRect rtTemp;
			GetDlgItem(IDC_STATIC_PTZ_LINK)->GetWindowRect(&rtTemp);
			ScreenToClient(&rtTemp);
			GetDlgItem(IDC_STATIC_DELAY_RECORDTIME)->MoveWindow(rtTemp);	
			rtTemp.left = rtTemp.right+5;		
			GetDlgItem(IDC_EDIT_RECORD_IG_TIME)->SetWindowPos(NULL,rtTemp.left,rtTemp.top,0,0,SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_SEC3)->SetWindowPos(NULL,rtTemp.right+45,rtTemp.top+3,0,0,SWP_NOSIZE);

			GetDlgItem(IDC_BTN_PTZLINK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_PTZ_LINK)->ShowWindow(SW_HIDE);
			
		}
	}
#define MAX_CHANNEL_PER_LINE 16
	//ͨ����Ŀ
	int nChanneCount = deviceInfo.byChanNum + deviceInfo.iDigChannel;
	int nBlank = (nChanneCount - 1) / MAX_CHANNEL_PER_LINE;
	int iHeight, iWidth;

	//����¼�����Ѳ������CHECK BOX
	ZeroMemory( m_pBtnsRecord, sizeof(m_pBtnsRecord) );
	ZeroMemory( m_pBtnsTour, sizeof(m_pBtnsRecord) );
	ZeroMemory(m_pBtnsSnap, sizeof(m_pBtnsSnap));

	CRect rcRecord, rcTour, rcSnap;
	GetButton(IDC_STATIC_RECORD_CHANNEL)->GetWindowRect(rcRecord);
	ScreenToClient(rcRecord);
	iHeight = HEIGHT_BTN;
	rcRecord.top = rcOffSet.top;
	rcRecord.bottom = rcRecord.top + iHeight;
	GetButton(IDC_STATIC_RECORD_CHANNEL)->MoveWindow(&rcRecord);

	rcTour.top = rcRecord.bottom + 8 + nBlank * iHeight;
	rcTour.bottom = rcTour.top + iHeight;
	rcTour.left = rcRecord.left;
	rcTour.right = rcRecord.right;
	GetButton(IDC_STATIC_TOUR)->MoveWindow(&rcTour);

	rcSnap.top = rcTour.bottom + 8 + nBlank * iHeight;
	rcSnap.bottom = rcSnap.top + iHeight;
	rcSnap.left = rcRecord.left;
	rcSnap.right = rcRecord.right;
	GetButton(IDC_STATIC_ANALYSE_SNAP)->MoveWindow(&rcSnap);

	rcRecord.left = rcRecord.right;
	rcRecord.right = rcRecord.left + 20;
	rcTour.left = rcTour.right;
	rcTour.right = rcTour.left + 20;
	rcSnap.left = rcSnap.right;
	rcSnap.right = rcSnap.left + 20;

	int nSect = nChanneCount / MAX_CHANNEL_PER_LINE;
	int nOffset = nChanneCount % MAX_CHANNEL_PER_LINE;
	for (j = 0; j < nSect; j++)
	{
		CRect orgrcRecord = rcRecord;
		CRect orgrcTour = rcTour;
		for ( i = 0; i < MAX_CHANNEL_PER_LINE; ++i )
		{
			strBtnTitle.Format("%d", i + 1 + j * MAX_CHANNEL_PER_LINE);

			m_pBtnsRecord[i + j * MAX_CHANNEL_PER_LINE] = new CButton();
			m_pBtnsTour[i + j * MAX_CHANNEL_PER_LINE] = new CButton();
			

			m_pBtnsRecord[i + j * MAX_CHANNEL_PER_LINE]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE,
				orgrcRecord, this, IDC_BTN_RECORD + i + j * MAX_CHANNEL_PER_LINE);
			m_pBtnsTour[i + j * MAX_CHANNEL_PER_LINE]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE,
				orgrcTour, this, IDC_BTN_RECORD + i  + j * MAX_CHANNEL_PER_LINE);

			orgrcRecord.left += 20;
			orgrcRecord.right+= 20;
			orgrcTour.left += 20;
			orgrcTour.right+= 20;	
		}
		//���һ��������
		if (j < nSect - 1)
		{
			rcRecord.top += 20;
			rcRecord.bottom += 20;
			rcTour.top += 20;
			rcTour.bottom+= 20;	
		}
	}
	if (nOffset)
	{
		if (nSect) //���ڶ���
		{
			//�����ƶ�һ��
			rcRecord.top += 20;
			rcRecord.bottom += 20;
			rcTour.top += 20;
			rcTour.bottom+= 20;	
		}

		for ( i = 0; i < nOffset; ++i )
		{
			strBtnTitle.Format("%d", i + 1 + nSect * MAX_CHANNEL_PER_LINE);

			m_pBtnsRecord[i + nSect * MAX_CHANNEL_PER_LINE] = new CButton();
			m_pBtnsTour[i + nSect * MAX_CHANNEL_PER_LINE] = new CButton();
			m_pBtnsRecord[i + nSect * MAX_CHANNEL_PER_LINE]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE, 
				rcRecord, this, IDC_BTN_RECORD + i + nSect * MAX_CHANNEL_PER_LINE);
			m_pBtnsTour[i + nSect * MAX_CHANNEL_PER_LINE]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE,
				rcTour, this, IDC_BTN_RECORD + i + nSect * MAX_CHANNEL_PER_LINE);

			rcRecord.left += 20;
			rcRecord.right+= 20;
			rcTour.left += 20;
			rcTour.right+= 20;	
		}
	}

	//��������ͨ�����ݲ�֧��ץͼ����
	int nScapChannel = deviceInfo.byChanNum;
	int nSectScap = nScapChannel / MAX_CHANNEL_PER_LINE;
	int nOffsetScap = nScapChannel % MAX_CHANNEL_PER_LINE;
	for (j = 0; j < nSectScap; j++)
	{
		CRect orgrcSnap = rcSnap;
		for ( i = 0; i < MAX_CHANNEL_PER_LINE; ++i )
		{
			strBtnTitle.Format("%d", i + 1 + j * MAX_CHANNEL_PER_LINE);

			m_pBtnsSnap[i + j * MAX_CHANNEL_PER_LINE] = new CButton();
			m_pBtnsSnap[i + j * MAX_CHANNEL_PER_LINE]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE,
				orgrcSnap, this, IDC_BTN_RECORD + i + j * MAX_CHANNEL_PER_LINE);
			m_pBtnsSnap[i + j * MAX_CHANNEL_PER_LINE]->ShowWindow(bSnap);

			orgrcSnap.left += 20;
			orgrcSnap.right+= 20;
		}
		//���һ��������
		if (j < nSectScap - 1)
		{
			rcSnap.top += 20;
			rcSnap.bottom+= 20;
		}
	}
	if (nOffsetScap)
	{
		if (nSect) //���ڶ���
		{
			//�����ƶ�һ��
			rcSnap.top += 20;
			rcSnap.bottom+= 20;
		}

		for ( i = 0; i < nOffsetScap; ++i )
		{
			strBtnTitle.Format("%d", i + 1 + nSectScap * MAX_CHANNEL_PER_LINE);

			m_pBtnsSnap[i + nSectScap * MAX_CHANNEL_PER_LINE] = new CButton();

			m_pBtnsSnap[i + nSectScap * MAX_CHANNEL_PER_LINE]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE,
				rcSnap, this, IDC_BTN_RECORD + i + nSectScap * MAX_CHANNEL_PER_LINE);
			m_pBtnsSnap[i + nSectScap * MAX_CHANNEL_PER_LINE]->ShowWindow(bSnap);

			rcSnap.left += 20;
			rcSnap.right+= 20;
		}
	}

	if (true == bSnap)
	{
		rcTour = rcSnap;
	}
	CRect rcPos, rcTmp, rcTmp2;
	GetButton(IDC_STATIC_PTZ_LINK)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTour.bottom + 12;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_STATIC_PTZ_LINK)->MoveWindow(&rcPos);
	
	GetButton(IDC_BTN_PTZLINK)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTour.bottom + 8;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_BTN_PTZLINK)->MoveWindow(&rcPos);
	
	GetButton(IDC_STATIC_DELAY_RECORDTIME)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTour.bottom + 12;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_STATIC_DELAY_RECORDTIME)->MoveWindow(&rcPos);

	GetButton(IDC_EDIT_RECORD_IG_TIME)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTour.bottom + 8;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_EDIT_RECORD_IG_TIME)->MoveWindow(&rcPos);

	GetButton(IDC_STATIC_SEC3)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTour.bottom + 12;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_STATIC_SEC3)->MoveWindow(&rcPos);
	rcTmp = rcPos;

	GetButton(IDC_CHECK_SCREEN_TIP)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTmp.bottom + 12;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_CHECK_SCREEN_TIP)->MoveWindow(&rcPos);
	m_newrect[0] = rcPos;

	GetButton(IDC_STATIC_SCREEN_TIP)->GetWindowRect(rcTmp2);
	ScreenToClient(rcTmp2);
	iHeight = rcTmp2.Height();
	iWidth = rcTmp2.Width();
	rcPos.left = rcPos.right + 4;
	rcPos.right = rcPos.left + iWidth;
	GetButton(IDC_STATIC_SCREEN_TIP)->MoveWindow(&rcPos);
	m_newrect[1] = rcPos;

	GetButton(IDC_CHECK_EMAIL)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTmp.bottom + 12;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_CHECK_EMAIL)->MoveWindow(&rcPos);
	m_newrect[2] = rcPos;

	GetButton(IDC_STATIC_EMAIL)->GetWindowRect(rcTmp2);
	ScreenToClient(rcTmp2);
	iHeight = rcTmp2.Height();
	iWidth = rcTmp2.Width();
	rcPos.left = rcPos.right + 4;
	rcPos.right = rcPos.left + iWidth;
	GetButton(IDC_STATIC_EMAIL)->MoveWindow(&rcPos);
	m_newrect[3] = rcPos;
	rcTmp = rcPos;

	GetButton(IDC_CHECK_BEEP)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTmp.bottom + 12;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_CHECK_BEEP)->MoveWindow(&rcPos);
	m_newrect[4] = rcPos;

	GetButton(IDC_STATIC_BEEP)->GetWindowRect(rcTmp2);
	ScreenToClient(rcTmp2);
	iHeight = rcTmp2.Height();
	iWidth = rcTmp2.Width();
	rcPos.left = rcPos.right + 4;
	rcPos.right = rcPos.left + iWidth;
	GetButton(IDC_STATIC_BEEP)->MoveWindow(&rcPos);
	m_newrect[5] = rcPos;

	GetButton(IDC_CHECK_FTP)->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	iHeight = rcPos.bottom - rcPos.top;
	rcPos.top = rcTmp.bottom + 12;
	rcPos.bottom = rcPos.top + iHeight;
	GetButton(IDC_CHECK_FTP)->MoveWindow(&rcPos);
	m_newrect[6] = rcPos;

	GetButton(IDC_STATIC_FTP)->GetWindowRect(rcTmp2);
	ScreenToClient(rcTmp2);
	iHeight = rcTmp2.Height();
	iWidth = rcTmp2.Width();
	rcPos.left = rcPos.right + 4;
	rcPos.right = rcPos.left + iWidth;
	GetButton(IDC_STATIC_FTP)->MoveWindow(&rcPos);
	m_newrect[7] = rcPos;
	//���ø��ơ�ճ����ˢ�¡�ȷ����ȡ����ť��λ��
	CRect btnRect;
	GetButton(IDC_BTN_COPY)->GetWindowRect(btnRect);
	ScreenToClient(btnRect);
	iHeight = btnRect.Height();
	btnRect.top = rcPos.bottom + 10;
	btnRect.bottom = btnRect.top + iHeight;
	GetButton(IDC_BTN_COPY)->MoveWindow(&btnRect);

	GetButton(IDC_BTN_PLASTER)->GetWindowRect(btnRect);
	ScreenToClient(btnRect);
	iHeight = btnRect.Height();
	btnRect.top = rcPos.bottom + 10;
	btnRect.bottom = btnRect.top + iHeight;
	GetButton(IDC_BTN_PLASTER)->MoveWindow(&btnRect);

	GetButton(IDC_BTN_REFUSH)->GetWindowRect(btnRect);
	ScreenToClient(btnRect);
	iHeight = btnRect.Height();
	btnRect.top = rcPos.bottom + 10;
	btnRect.bottom = btnRect.top + iHeight;
	GetButton(IDC_BTN_REFUSH)->MoveWindow(&btnRect);

	GetButton(IDOK)->GetWindowRect(btnRect);
	ScreenToClient(btnRect);
	iHeight = btnRect.Height();
	btnRect.top = rcPos.bottom + 10;
	btnRect.bottom = btnRect.top + iHeight;
	GetButton(IDOK)->MoveWindow(&btnRect);

	GetButton(IDCANCEL)->GetWindowRect(btnRect);
	ScreenToClient(btnRect);
	iHeight = btnRect.Height();
	btnRect.top = rcPos.bottom + 10;
	btnRect.bottom = btnRect.top + iHeight;
	GetButton(IDCANCEL)->MoveWindow(&btnRect);

	//����ͨ������ѡ���
	
	//��ȡ���ܷ�����������ΪInitWndShow���������õ�������Ҫ��֮ǰ��ȡ��
	if (!m_bGetAnalyseAbility)
	{
		DWORD nReturn=0;
		int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_ABILITY_ANALYZEABILITY,-1, (char *)&m_analyseAbility, sizeof(m_analyseAbility),&nReturn);
		//int nRet = UpdateParam(E_SDK_CONFIG_ABILITY_ANALYZEABILITY, (char *)&m_analyseAbility, sizeof(m_analyseAbility), -1, FALSE);
		if ( nRet > 0 )
		{
			m_bGetAnalyseAbility = true;
			
			//���»�ȡ���óɹ�
			memcpy( &m_oldAnalyseAbility, &m_analyseAbility, sizeof(SDK_ANALYZEABILITY) );
		}
		else
		{
			return 0;
		}
	}
	else
	{
		memcpy( &m_analyseAbility, &m_oldAnalyseAbility, sizeof(SDK_ANALYZEABILITY) );
	}

	/*DWORD dwAVD = m_analyseAbility.uIntelAVD;
	DWORD dwCPC = m_analyseAbility.uIntelCPC;
	DWORD dwOSC = m_analyseAbility.uIntelOSC;
	DWORD dwPEA = m_analyseAbility.uIntelPEA;*/

	int nInsert = 0;
	CComboBox *pComboBox = GetComboBox(IDC_COMBO_CHANNEL);
	pComboBox->ResetContent();
	CString strText;
	int iChannelFirst = -1;		//��¼��һ�����㷨�����ͨ��
	BOOL bFirst = TRUE;
	for (  i = 0; i < nChanneCount; ++i)
	{
		//��Ҫ��ͨ����һ�����㷨�������ʾ
		if ( (m_analyseAbility.uIntelAVD[i]) 
			/*|| (m_analyseAbility.uIntelCPC[i])*/ 
			|| (m_analyseAbility.uIntelOSC[i]) 
			|| (m_analyseAbility.uIntelPEA[i]) )
		{
			strText.Format("%d", i + 1);
			nInsert = pComboBox->AddString(strText);
			pComboBox->SetItemData(nInsert, i);
			if (bFirst)
			{
				iChannelFirst = i;
				bFirst = FALSE;
			}
		}
	}	

	if (iChannelFirst >= 0)
	{
		pComboBox->SetCurSel(0);
		m_nLastChannelNum = iChannelFirst;
	}
//	strText.Format("%s", _CS("OneClick.All"));
//	nInsert = pComboBox->AddString(strText);
//	pComboBox->SetItemData(nInsert, nChanneCount);

// 	if ( nChanneCount > 0 )
// 	{
// 		pComboBox->SetCurSel(0);
// 		m_nLastChannelNum = 0;
// 	}
//	pComboBox->EnableWindow(FALSE);
	return TRUE;
}

bool CAlarmVideoAnalyse::WinDataToStruct(int nChannelNum)
{
	if (nChannelNum < 0 || nChannelNum > NET_MAX_CHANNUM)
	{
		return false;
	}
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);
	int nChanneCount = deviceInfo.byChanNum + deviceInfo.iDigChannel;	//�õ���ǰ�豸��ͨ����Ŀ
	int nScapChannel = deviceInfo.byChanNum;

	if ( nChannelNum < 0 )
	{
		return FALSE;
	}

	m_analyse.vAnalyzeAll[nChannelNum].bEnable = GetButton(IDC_CHECK_ON_OFF)->GetCheck();
	m_analyse.vAnalyzeAll[nChannelNum].moduleType = GetComboBox(IDC_COMBO_ARITHIMETIC)->GetCurSel();

	SDK_EventHandler &eventHander = m_analyse.vAnalyzeAll[nChannelNum].hEvent;

	CAlarmInputDlg::GetButtonsCheck(m_pBtnsAlarmOut, eventHander.dwAlarmOut, deviceInfo.byAlarmOutPortNum);
	CAlarmInputDlg::GetButtonsCheck(m_pBtnsRecord, eventHander.dwRecord, nChanneCount);		// ¼������
	eventHander.iRecordLatch = GetEditValue(IDC_EDIT_RECORD_IG_TIME).Toint();		// ¼����ʱ��10��300 sec 
	CAlarmInputDlg::GetButtonsCheck(m_pBtnsTour, eventHander.dwTour, nChanneCount);			// ��Ѳ����
	eventHander.iEventLatch = GetEditValue(IDC_EDIT_INTERL_TIME).Toint();			// ��ʱʱ�䣬sΪ��λ 
	eventHander.iAOLatch = GetEditValue(IDC_EDIT_IGNORE_TIME).Toint();				// ���������ʱ��10��300 sec  
	CAlarmInputDlg::GetButtonsCheck(m_pBtnsSnap, eventHander.dwSnapShot, nScapChannel);

	//eventHander.bRecordEn = eventHander.dwRecord > 0 ? 1 : 0;	// ¼��ʹ�� 
	//eventHander.bTourEn = eventHander.dwTour > 0 ? 1 : 0;				// ��Ѳʹ�� 
	//eventHander.bAlarmOutEn = eventHander.dwAlarmOut > 0 ? 1 : 0;		// ����ʹ�� 
	//eventHander.bSnapEn = eventHander.dwSnapShot > 0 ? 1 : 0;  // ץͼʹ��
	eventHander.bRecordEn = false;
	eventHander.bTourEn = false;
	eventHander.bAlarmOutEn = false;
	eventHander.bSnapEn = false;
	for (int j = 0; j < nChanneCount; ++j)
	{
		if(eventHander.dwRecord[j])
			eventHander.bRecordEn = true;
		if(eventHander.dwTour[j])
			eventHander.bTourEn = true;
		if(eventHander.dwAlarmOut[j])
			eventHander.bAlarmOutEn = true;
		if(eventHander.dwSnapShot[j])
			eventHander.bSnapEn = true;			// ץͼʹ��	
	}

	eventHander.bTip = GetButton(IDC_CHECK_SCREEN_TIP)->GetCheck();			// ��Ļ��ʾʹ�� 
	eventHander.bMail = GetButton(IDC_CHECK_EMAIL)->GetCheck();				// �����ʼ� 	
	eventHander.bBeep = GetButton(IDC_CHECK_BEEP)->GetCheck();
	eventHander.bFTP =  GetButton(IDC_CHECK_FTP)->GetCheck();
	return true;
}

bool CAlarmVideoAnalyse::StructToWinData(int nChannelNum, bool bCopy/* = false */)
{
	if (nChannelNum < 0 || nChannelNum > NET_MAX_CHANNUM)
	{
		return false;
	}

	if ( !m_bGet[nChannelNum] )
	{
		DWORD nReturn=0;
		int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_VIDEO_ANALYZE, nChannelNum, (char *)&m_analyse.vAnalyzeAll[nChannelNum], sizeof(SDK_ANALYSECONFIG),&nReturn);
		//int nRet = UpdateParam(E_SDK_CONFIG_VIDEO_ANALYZE, (char *)&m_analyse.vAnalyzeAll[nChannelNum], sizeof(SDK_ANALYSECONFIG), nChannelNum, FALSE, FALSE);
		if ( nRet > 0 )
		{
			m_bGet[nChannelNum] = true;

			//���»�ȡ���óɹ�
			memcpy( &m_Oldanalyse.vAnalyzeAll[nChannelNum], &m_analyse.vAnalyzeAll[nChannelNum], sizeof(SDK_ANALYSECONFIG) );
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ( bCopy )
			memcpy( &m_analyse.vAnalyzeAll[nChannelNum], &m_Oldanalyse.vAnalyzeAll[nChannelNum], sizeof(SDK_ANALYSECONFIG) );
	}
	
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);

	//˵��������ͨ��������ͨ��û�������Ⱥ�����
	if ( nChannelNum >= deviceInfo.byChanNum )
	{
		GetDlgItem(IDC_BTN_RULE_SET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RULE)->ShowWindow(SW_HIDE);
	}
	else
	{	
		GetDlgItem(IDC_BTN_RULE_SET)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_RULE)->ShowWindow(SW_SHOW);
	}

//	CUserDataManager *pData = CUserDataManager::instance();
//	if ( pData )
	//{
		//SDK_SystemFunction *pSystemFunc = pData->GetsysFunc();

	/*	m_bAnalyse = pSystemFunc->vAlarmFunction[SDK_ALARM_FUNCTION_TYPE_VIDEOANALYSE];
		if ( m_bAnalyse )
		{
			GetButton(IDC_BTN_RULE_SET)->EnableWindow(FALSE);
			GetComboBox(IDC_COMBO_ARITHIMETIC)->EnableWindow(FALSE);
			GetButton(IDC_CHECK_ON_OFF)->EnableWindow(FALSE);
		}
		else
		{
			GetButton(IDC_BTN_RULE_SET)->EnableWindow(TRUE);
			GetButton(IDC_CHECK_ON_OFF)->EnableWindow(TRUE);
			GetComboBox(IDC_COMBO_ARITHIMETIC)->EnableWindow(TRUE);
		}*/
		
		int nChanneCount = deviceInfo.byChanNum + deviceInfo.iDigChannel;	//�õ���ǰ�豸��ͨ����Ŀ
		int nScapChannel = deviceInfo.byChanNum;
		
		ShowModuleType(nChannelNum);
		GetComboBox(IDC_COMBO_ARITHIMETIC)->SetCurSel(m_analyse.vAnalyzeAll	[nChannelNum].moduleType);
		GetButton(IDC_CHECK_ON_OFF)->SetCheck( m_analyse.vAnalyzeAll[nChannelNum].bEnable );

		SDK_EventHandler &eventHander = m_analyse.vAnalyzeAll[nChannelNum].hEvent;

		CAlarmInputDlg::SetButtonsCheck(m_pBtnsAlarmOut, eventHander.dwAlarmOut, deviceInfo.byAlarmOutPortNum );
		CAlarmInputDlg::SetButtonsCheck( m_pBtnsRecord, eventHander.dwRecord, nChanneCount );	// ¼������ 
		SetEditText( IDC_EDIT_RECORD_IG_TIME, eventHander.iRecordLatch);		// ¼����ʱ��10��300 sec  	
		CAlarmInputDlg::SetButtonsCheck( m_pBtnsTour, eventHander.dwTour, nChanneCount );		// ��Ѳ���� 	
		CAlarmInputDlg::SetButtonsCheck(m_pBtnsSnap, eventHander.dwSnapShot, nScapChannel);
		SetEditText(IDC_EDIT_INTERL_TIME, eventHander.iEventLatch);				// ��ʱʱ�䣬sΪ��λ 
		SetEditText(IDC_EDIT_IGNORE_TIME, eventHander.iAOLatch);				// ���������ʱ��10��300 sec  
		GetButton(IDC_CHECK_SCREEN_TIP)->SetCheck(eventHander.bTip);			// ��Ļ��ʾʹ�� 	
		GetButton(IDC_CHECK_EMAIL)->SetCheck(eventHander.bMail);				// �����ʼ� 	
		GetButton(IDC_CHECK_BEEP)->SetCheck(eventHander.bBeep);					// ������
		GetButton(IDC_CHECK_FTP)->SetCheck(eventHander.bFTP);                   // FTP

		int nflag = 2;
	//	SDK_SystemFunction* pFunc = pData->GetsysFunc();
		
		//DWORD nReturn=0;
		SDK_SystemFunction sysFunc;
		H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_ABILITY_SYSFUNC,-1, (char *)&sysFunc,sizeof(sysFunc),&nReturn);
	//	if ( pFunc )
		//{
			if ( sysFunc.vTipShowFunction[SDK_NO_EMAIL_TIP_SHOW])
			{
				GetButton(IDC_CHECK_EMAIL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_EMAIL)->ShowWindow(SW_HIDE);
				// �����ʼ� 		
			}
			else
			{
				switch (nflag)
				{
				case 1:
					{
						GetButton(IDC_CHECK_EMAIL)->MoveWindow(&m_newrect[0]);
						GetDlgItem(IDC_STATIC_EMAIL)->MoveWindow(&m_newrect[1]);
					}
					break;
				default:
					GetButton(IDC_CHECK_EMAIL)->MoveWindow(&m_newrect[2]);
					GetDlgItem(IDC_STATIC_EMAIL)->MoveWindow(&m_newrect[3]);
					break;
				}
				GetButton(IDC_CHECK_EMAIL)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_EMAIL)->ShowWindow(SW_SHOW);
				GetButton(IDC_CHECK_EMAIL)->SetCheck(eventHander.bMail);
				nflag++;
			}

			if ( sysFunc.vTipShowFunction[SDK_NO_BEEP_TIP_SHOW])
			{
				GetButton(IDC_CHECK_BEEP)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_BEEP)->ShowWindow(SW_HIDE);
				// ������
			}
			else
			{
				switch (nflag)
				{
				case 1:
					{
						GetButton(IDC_CHECK_BEEP)->MoveWindow(&m_newrect[0]);
						GetDlgItem(IDC_STATIC_BEEP)->MoveWindow(&m_newrect[1]);
					}
					break;
				case 2:
					{
						GetButton(IDC_CHECK_BEEP)->MoveWindow(&m_newrect[2]);
						GetDlgItem(IDC_STATIC_BEEP)->MoveWindow(&m_newrect[3]);

					}
					break;
				default:
					GetButton(IDC_CHECK_BEEP)->MoveWindow(&m_newrect[4]);
					GetDlgItem(IDC_STATIC_BEEP)->MoveWindow(&m_newrect[5]);
					break;
				}
				GetButton(IDC_CHECK_BEEP)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_BEEP)->ShowWindow(SW_SHOW);
				GetButton(IDC_CHECK_BEEP)->SetCheck(eventHander.bBeep);		
				nflag++;
			}



			if ( sysFunc.vTipShowFunction[SDK_NO_FTP_TIP_SHOW])
			{
				GetButton(IDC_CHECK_FTP)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_FTP)->ShowWindow(SW_HIDE);
				// FTP
			}
			else
			{
				switch (nflag)
				{
				case 1:
					{

						GetButton(IDC_CHECK_FTP)->MoveWindow(&m_newrect[0]);

						GetDlgItem(IDC_STATIC_FTP)->MoveWindow(&m_newrect[1]);
					}
					break;
				case 2:
					{

						GetButton(IDC_CHECK_FTP)->MoveWindow(&m_newrect[2]);

						GetDlgItem(IDC_STATIC_FTP)->MoveWindow(&m_newrect[3]);


					}
					break;
				case 3:
					{

						GetButton(IDC_CHECK_FTP)->MoveWindow(&m_newrect[4]);

						GetDlgItem(IDC_STATIC_FTP)->MoveWindow(&m_newrect[5]);

					}
					break;
				default:
					GetButton(IDC_CHECK_FTP)->MoveWindow(&m_newrect[6]);
					GetDlgItem(IDC_STATIC_FTP)->MoveWindow(&m_newrect[7]);
					break;
				}
				GetButton(IDC_CHECK_FTP)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_FTP)->ShowWindow(SW_SHOW);
				GetButton(IDC_CHECK_FTP)->SetCheck(eventHander.bFTP); 
			}
		//}
	//}


	OnBnClickedCheck();

	return true;
}

bool CAlarmVideoAnalyse::WinDataToStruct()
{
	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);	
	
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);
	//����������ͨ��ʱ�򣬴ӵ�һͨ������������
	if ( nChannelNum == (deviceInfo.byChanNum + deviceInfo.iDigChannel) )
	{
		//�ȱ����һͨ������
		WinDataToStruct(0);
		for ( int i = 1; i < nChannelNum; i ++ )
		{
			memcpy( &m_analyse.vAnalyzeAll[i], &m_analyse.vAnalyzeAll[0], sizeof(m_analyse.vAnalyzeAll[i]) );
		}

		return true;
	}
	else
	{
		return WinDataToStruct(nChannelNum);
	}
}

bool CAlarmVideoAnalyse::StructToWinData()
{
 	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);
	StructToWinData(nChannelNum);
	
	m_nLastChannelNum = nChannelNum;

	return true;
}


void CAlarmVideoAnalyse::Clear()
{
	int nChanneCount = NET_MAX_CHANNUM;

	int i = 0;
	for ( i = 0; i < nChanneCount; ++i )
	{
		if (m_pBtnsRecord[i])
		{
			delete m_pBtnsRecord[i];
			m_pBtnsRecord[i] = NULL;
		}

		if (m_pBtnsTour[i])
		{
			delete m_pBtnsTour[i];
			m_pBtnsTour[i] = NULL;
		}

		if (m_pBtnsSnap[i])
		{
			delete m_pBtnsSnap[i];
			m_pBtnsSnap[i] = NULL;
		}
	}

	int nVideoOutChannel = NET_MAX_CHANNUM;

	for ( i = 0; i < nVideoOutChannel; ++i )
	{
		if (m_pBtnsAlarmOut[i])
		{
			delete m_pBtnsAlarmOut[i];
			m_pBtnsAlarmOut[i] = NULL;
		}
	}
	ReSet();
}
void CAlarmVideoAnalyse::OnCbnSelchangeComboChannel()
{
	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);
	
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);
	int nTotalChannel = deviceInfo.byChanNum + deviceInfo.iDigChannel;
	//ѡ��ȫ��ͨ����Ĭ����ʾ��һͨ�����ݣ�������һͨ��������Ҳ����Ҫ����
	if ( nChannelNum == nTotalChannel )
	{
		StructToWinData(0);
		m_nLastChannelNum = nChannelNum;
	}
	//���ԭ����ȫ��ͨ�����л�ͨ������Ҫ���棬ֻҪ��ʾ��ǰͨ���Ϳ���
	else if ( m_nLastChannelNum == nTotalChannel )
	{
		StructToWinData(nChannelNum);
		m_nLastChannelNum = nChannelNum;
	}
	else
	{
		if (nChannelNum != m_nLastChannelNum)
		{
			WinDataToStruct(m_nLastChannelNum);
			StructToWinData(nChannelNum);

			m_nLastChannelNum = nChannelNum;
		}
	}
}

void CAlarmVideoAnalyse::OnBnClickedBtnRefush()
{
	for ( int i =0 ; i < NET_MAX_CHANNUM; i ++ )
	{
		m_bGet[i] = false;
	}
	
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);
	//ˢ��ʱ�򣬸��µľ��ǵ�ǰ��ͨ���ţ�û��Ҫ���´�0ͨ����ʼ��ʾ
	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);
	if ( nChannelNum == (deviceInfo.byChanNum + deviceInfo.iDigChannel) )
	{
		nChannelNum = 0;
	}
	//InitWndShow();
	StructToWinData(nChannelNum);

	UpdateData(FALSE);
}

void CAlarmVideoAnalyse::OnBnClickedOk()
{
	UpdateData();

	//��ҳ�����ݱ��浽�ṹ����
	WinDataToStruct();
	
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);
	int nRet = 1;
	int nChannelCount = deviceInfo.byChanNum + deviceInfo.iDigChannel; 
	//V2.62.R07֮��İ汾֧�ֵ�ͨ������ȫͨ������
	if (GetComboBoxData(IDC_COMBO_CHANNEL) == nChannelCount && strcmp(deviceInfo.sSoftWareVersion, "V2.62.R07") >= 0) 
	{
		
		//H264_DVR_DEVICEINFO deviceInfo;
		nRet = H264_DVR_SetDevConfig(loginID,E_SDK_CONFIG_VIDEO_ANALYZE,-1, (char *)&m_analyse.vAnalyzeAll[0],sizeof(SDK_ANALYSECONFIG));
		//nRet = UpdateParam(E_SDK_CONFIG_VIDEO_ANALYZE, (char *)&m_analyse.vAnalyzeAll[0], sizeof(SDK_ANALYSECONFIG), 0xff, TRUE);

	}
	else
	{
		for ( int i = 0; i < nChannelCount; i ++ )
		{
			//�Ƚ϶���ͨ�����ݸ��ľͱ������ͨ��������	
			if ( memcmp( &m_analyse.vAnalyzeAll[i], &m_Oldanalyse.vAnalyzeAll[i], sizeof(SDK_ANALYSECONFIG)) )
			{
				nRet = H264_DVR_SetDevConfig(loginID,E_SDK_CONFIG_VIDEO_ANALYZE,i, (char *)&m_analyse.vAnalyzeAll[i],sizeof(SDK_ANALYSECONFIG),10000);
				//nRet = UpdateParam(E_SDK_CONFIG_VIDEO_ANALYZE, (char *)&m_analyse.vAnalyzeAll[i], sizeof(SDK_ANALYSECONFIG), i, TRUE);

				//ֻҪ��һ��ͨ������ʧ�ܣ�����Ϊ��ʧ��
				if ( nRet <= 0 )
				{
					break;
				}
			}
		}
	}
	
	if (nRet == H264_DVR_OPT_REBOOT)
	{
		//CUserDataManager *pData = CUserDataManager::instance();
		memcpy( &m_Oldanalyse, &m_analyse, sizeof(m_Oldanalyse) );
		/*if (*/ MessageBox(_CS("ConfigNet.NeedReboot"), _CS("OneClick.Prompt"), MB_OK | MB_ICONQUESTION ) /*== IDOK)*/;
		{
			H264_DVR_SetDevConfig(loginID,E_SDK_REBOOT_DEV, -1, 0, 0);
			//pData->SaveConfig(E_SDK_REBOOT_DEV, -1, 0, 0);
		}
		CDialog::OnOK();//��Ҫ����֮��ֱ�ӷ��أ�ֻ��ʾ������������ʾ�����κ���Ϣ��
	}
	else if ( nRet > 0 )
	{
		memcpy( &m_Oldanalyse, &m_analyse, sizeof(m_Oldanalyse) );

		MessageBox(_CS("Error.SaveSuccess"), _CS("OneClick.Prompt") );  
		//CDialog::OnOK();
	}
}


/*void CAlarmVideoAnalyse::OnBnClickedCopy()
{
	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);
	
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);

	//ѡ��ȫ��ͨ����Ĭ����ʾ��һͨ�����ݣ�������һͨ��������Ҳ����Ҫ����
	if ( nChannelNum == (deviceInfo.byChanNum + deviceInfo.iDigChannel) )
	{
		nChannelNum = 0;
	}

	//�ȱ��浱ǰͨ������
	WinDataToStruct(nChannelNum);

	memcpy( &m_coptData, &m_analyse.vAnalyzeAll[nChannelNum], sizeof(m_coptData) );
	m_bCopy = true;
}*/
/*
void CAlarmVideoAnalyse::OnBnClickedPlaster()
{
	//û�а������ư�ť
	if ( !m_bCopy )
	{
		return;
	}

	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);

	//ѡ��ȫ��ͨ����Ĭ����ʾ��һͨ�����ݣ�������һͨ��������Ҳ����Ҫ����
	if ( nChannelNum == GetSysInfo()->byChanNum )
	{
	for ( int i = 0; i < nChannelNum; i ++ )
	{
	memcpy( &m_analyse.vAnalyzeAll[i], &m_coptData, sizeof(m_analyse.vAnalyzeAll[i]) );
	}

	nChannelNum = 0;
	}

	/*CUserDataManager *pData = CUserDataManager::instance();
	 SDK_SystemFunction *pSysFunc = pData->GetsysFunc();

	m_bAnalyse = pSysFunc->vAlarmFunction[SDK_ALARM_FUNCTION_TYPE_VIDEOANALYSE];
	if ( m_bAnalyse )
	{
		GetButton(IDC_BTN_RULE_SET)->EnableWindow(FALSE);
	}
	else
	{
		GetButton(IDC_BTN_RULE_SET)->EnableWindow(TRUE);
	}*/
/*
	int nChanneCount = GetSysInfo()->byChanNum + GetSysInfo()->iDigChannel;	//�õ���ǰ�豸��ͨ����Ŀ
	int nScapChannel =  GetSysInfo()->byChanNum;

	GetButton(IDC_CHECK_ON_OFF)->SetCheck( m_coptData.bEnable );
	//SetComBoxCurSel(IDC_COMBO_ARITHIMETIC,  m_coptData.moduleType);

	SDK_EventHandler &eventHander = m_coptData.hEvent;

	CAlarmInputDlg::SetButtonsCheck(m_pBtnsAlarmOut, eventHander.dwAlarmOut, GetSysInfo()->byAlarmOutPortNum );
	CAlarmInputDlg::SetButtonsCheck( m_pBtnsRecord, eventHander.dwRecord, nChanneCount );	// ¼������ 
	SetEditText( IDC_EDIT_RECORD_IG_TIME, eventHander.iRecordLatch);		// ¼����ʱ��10��300 sec  	
	CAlarmInputDlg::SetButtonsCheck( m_pBtnsTour, eventHander.dwTour, nChanneCount );		// ��Ѳ���� 	
	CAlarmInputDlg::SetButtonsCheck(m_pBtnsSnap, eventHander.dwSnapShot, nScapChannel);
	SetEditText(IDC_EDIT_INTERL_TIME, eventHander.iEventLatch);				// ��ʱʱ�䣬sΪ��λ 
	SetEditText(IDC_EDIT_IGNORE_TIME, eventHander.iAOLatch);				// ���������ʱ��10��300 sec  

	GetButton(IDC_CHECK_SCREEN_TIP)->SetCheck(eventHander.bTip);			// ��Ļ��ʾʹ�� 	
	GetButton(IDC_CHECK_EMAIL)->SetCheck(eventHander.bMail);				// �����ʼ� 	
	GetButton(IDC_CHECK_BEEP)->SetCheck(eventHander.bBeep);					// ������
	GetButton(IDC_CHECK_FTP)->SetCheck(eventHander.bFTP);             //FTP

	OnBnClickedCheck();
	UpdateData(FALSE);
}*/

void CAlarmVideoAnalyse::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	//��ȡ��������
	if ( bShow && m_bAnalyse)			
	{
				//
				OnBnClickedCheck();
		 		//int nRet = UpdateParam(E_SDK_CONFIG_VIDEO_ANALYZE, (char *)&m_analyse, sizeof(m_analyse), -1, FALSE);
		 		//if ( 1 == nRet || 2 == nRet )
		 		//{				
		 		//	//���»�ȡ���óɹ�
		 		//	if ( nRet == 1 )
		 		//	{
		 		//		memcpy( &m_Oldanalyse, &m_analyse, sizeof(m_Oldanalyse) );
		 		//	}
		 		//	else
		 		//		//ԭ�Ⱦͻ�ȡ��
		 		//	{
		 		//		memcpy( &m_analyse, &m_Oldanalyse, sizeof(m_analyse) );
		 		//	}
				
					for ( int i =0 ; i < NET_MAX_CHANNUM; i ++ )
					{
						m_bGet[i] = false;
					}

					if ( StructToWinData() )
					{
						UpdateData(FALSE);
					}
			/*	}*/
	}				
}

void CAlarmVideoAnalyse::ReInit()
{
//	CDlgSetBase::ReInit();

	for ( int i =0 ; i < NET_MAX_CHANNUM; i ++ )
	{
		m_bGet[i] = false;
	}
}	

void CAlarmVideoAnalyse::OnChangeIntelTime()
{
	UpdateData();

	if ( GetEditValue(IDC_EDIT_INTERL_TIME).Toint() > 600 )
	{
		GetDlgItem(IDC_EDIT_INTERL_TIME)->SetWindowText("600");
	}
}

void CAlarmVideoAnalyse::OnChangeIgnoreTime()
{
	UpdateData();

	if ( GetEditValue(IDC_EDIT_IGNORE_TIME).Toint() > 3000 )
	{
		GetDlgItem(IDC_EDIT_IGNORE_TIME)->SetWindowText("3000");
	}
	GetDlgItem(IDC_EDIT_IGNORE_TIME)->SetFocus();
}	

void CAlarmVideoAnalyse::OnChangeRecIgTime()
{
	UpdateData();

	if ( GetEditValue(IDC_EDIT_RECORD_IG_TIME).Toint() > 300 )
	{
		GetDlgItem(IDC_EDIT_RECORD_IG_TIME)->SetWindowText("300");
	}
	GetDlgItem(IDC_EDIT_RECORD_IG_TIME)->SetFocus();
}

void CAlarmVideoAnalyse::OnEnKillfocusEditIgnoreTime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if ( GetEditValue(IDC_EDIT_IGNORE_TIME).Toint() > 3000 )
	{
		GetDlgItem(IDC_EDIT_IGNORE_TIME)->SetWindowText("3000");
	}
	if ( GetEditValue(IDC_EDIT_IGNORE_TIME).Toint() < 10 )
	{
		GetDlgItem(IDC_EDIT_IGNORE_TIME)->SetWindowText("10");
	}
}

void CAlarmVideoAnalyse::OnEnKillfocusEditRecordIgTime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( GetEditValue(IDC_EDIT_RECORD_IG_TIME).Toint() > 300 )
	{
		GetDlgItem(IDC_EDIT_RECORD_IG_TIME)->SetWindowText("300");
	}
	if ( GetEditValue(IDC_EDIT_RECORD_IG_TIME).Toint() < 10 )
	{
		GetDlgItem(IDC_EDIT_RECORD_IG_TIME)->SetWindowText("10");
	}
}

void CAlarmVideoAnalyse::OnBnClickedCheck()
{
	UpdateData();
	BOOL bEnable = ((CButton *)GetDlgItem(IDC_CHECK_ON_OFF))->GetCheck();
	GetDlgItem(IDC_BTN_RULE_SET)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_ARITHIMETIC)->EnableWindow(bEnable); // ֻ֧��PEA
	GetDlgItem(IDC_BTN_ALARM_CTRL_PERIOD)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_INTERL_TIME)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_IGNORE_TIME)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_RECORD_IG_TIME)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_SCREEN_TIP)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_BEEP)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EMAIL)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FTP)->EnableWindow(bEnable);
	
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);

	for ( int i = 0; i < deviceInfo.byAlarmOutPortNum ; i++)
	{
		m_pBtnsAlarmOut[i]->EnableWindow(bEnable);
	}

	int nTotal =  deviceInfo.byChanNum +  deviceInfo.iDigChannel;
	for ( int  j = 0; j < nTotal; j++ )
	{
		m_pBtnsRecord[j]->EnableWindow(bEnable);
	}

	for ( int k = 0; k < nTotal; k++ )
	{
		m_pBtnsTour[k]->EnableWindow(bEnable);
	}

	int nScapChannel = deviceInfo.byChanNum;
	//setting Snap
	for ( int m = 0; m < nScapChannel; m++)
	{
		m_pBtnsSnap[m]->EnableWindow(bEnable);
	}
	UpdateData(FALSE);
}

void CAlarmVideoAnalyse::GetPosition()
{
	GetButton(IDC_CHECK_SCREEN_TIP)->GetWindowRect(m_oldrect[0]);
	ScreenToClient(m_oldrect[0]);
	GetButton(IDC_STATIC_SCREEN_TIP)->GetWindowRect(m_oldrect[1]);
	ScreenToClient(m_oldrect[1]);

	GetButton(IDC_CHECK_EMAIL)->GetWindowRect(m_oldrect[2]);
	ScreenToClient(m_oldrect[2]);
	GetButton(IDC_STATIC_EMAIL)->GetWindowRect(m_oldrect[3]);
	ScreenToClient(m_oldrect[3]);

	GetButton(IDC_CHECK_BEEP)->GetWindowRect(m_oldrect[4]);
	ScreenToClient(m_oldrect[4]);
	GetButton(IDC_STATIC_BEEP)->GetWindowRect(m_oldrect[5]);
	ScreenToClient(m_oldrect[5]);

	GetButton(IDC_CHECK_FTP)->GetWindowRect(m_oldrect[6]);
	ScreenToClient(m_oldrect[6]);
	GetButton(IDC_STATIC_FTP)->GetWindowRect(m_oldrect[7]);
	ScreenToClient(m_oldrect[7]);
}
void CAlarmVideoAnalyse::ReSet()
{
	GetButton(IDC_CHECK_SCREEN_TIP)->MoveWindow(&m_oldrect[0]);

	GetButton(IDC_STATIC_SCREEN_TIP)->MoveWindow(&m_oldrect[1]);


	GetButton(IDC_CHECK_EMAIL)->MoveWindow(&m_oldrect[2]);

	GetButton(IDC_STATIC_EMAIL)->MoveWindow(&m_oldrect[3]);


	GetButton(IDC_CHECK_BEEP)->MoveWindow(&m_oldrect[4]);

	GetButton(IDC_STATIC_BEEP)->MoveWindow(&m_oldrect[5]);


	GetButton(IDC_CHECK_FTP)->MoveWindow(&m_oldrect[6]);

	GetButton(IDC_STATIC_FTP)->MoveWindow(&m_oldrect[7]);
}
void CAlarmVideoAnalyse::OnBnClickedBtnPtzlink()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nChannelNum = GetComboBoxData(IDC_COMBO_CHANNEL);
	
	DWORD nReturn=0;
	H264_DVR_DEVICEINFO deviceInfo;
	int nRet = H264_DVR_GetDevConfig(loginID,E_SDK_CONFIG_SYSINFO,-1, (char *)&deviceInfo,sizeof(deviceInfo),&nReturn);

	//��ѡ��ȫ��ͨ��ʱ��Ĭ���޸ĵ��ǵ�һͨ��������������ʱ���ȫ����ֵ
	if ( nChannelNum == (deviceInfo.byChanNum + deviceInfo.iDigChannel))
	{
		nChannelNum = 0;
	}

	CPtzLinkDlg dlg( m_analyse.vAnalyzeAll[nChannelNum].hEvent.PtzLink);
	if ( IDOK == dlg.DoModal() )
	{
		memcpy(m_analyse.vAnalyzeAll[nChannelNum].hEvent.PtzLink, dlg.GetPtzConf(), sizeof(m_analyse.vAnalyzeAll[nChannelNum].hEvent.PtzLink));
	}
}

void CAlarmVideoAnalyse::ShowModuleType(int nChannelNum)
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ARITHIMETIC);
	pComboBox->ResetContent();
	CString strText;
	int nInsert = -1;
	if (m_analyseAbility.uIntelPEA[nChannelNum])
	{
		strText.Format("%s", _CS("ConfigAlarm.PEA") );      
		nInsert = pComboBox->AddString(strText);
		pComboBox->SetItemData(nInsert, 0);
	}
	if (m_analyseAbility.uIntelOSC[nChannelNum])
	{
		strText.Format("%s", _CS("ConfigAlarm.OSC") );
		nInsert = pComboBox->AddString(strText);
		pComboBox->SetItemData(nInsert, 1);
	}
	if (m_analyseAbility.uIntelAVD[nChannelNum])
	{
		strText.Format("%s", _CS("ConfigAlarm.AVD") );
		nInsert = pComboBox->AddString(strText);
		pComboBox->SetItemData(nInsert, 2);
	}
// 	if (m_analyseAbility.uIntelCPC && 1<< nChannelNum)
// 	{
// 		strText.Format("%s", _CS("ConfigAlarm.CPC") );
// 		nInsert = pComboBox->AddString(strText);
// 		pComboBox->SetItemData(nInsert, 3);
// 	}
}
