// DeviceSearch.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DeviceSearch.h"
//#include "Markup.h"
#include "ConfigDemoDlg.h"


// CDeviceSearch �Ի���

IMPLEMENT_DYNAMIC(CDeviceSearch, CDialog)

CDeviceSearch::CDeviceSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceSearch::IDD, pParent)
	,m_nDevNum(0)
	,m_lUpgradeHandle(0)
{
	m_strDevName = _T("Test");
	m_nPort = 34567;
	m_strPsw = _T("");
	m_nChannel = 16;
	m_strUserName = _T("admin");
	m_strIP = _T("");
	m_DevSerialID = _T("");
	m_HostPort = 0;
	m_strHostIP = _T("");
	m_bSerialID = FALSE;

	m_nType = 0;	//"0" means to add device
	memset(&m_devInfo, 0, sizeof(m_devInfo));
	m_info = NULL;
}

CDeviceSearch::~CDeviceSearch()
{
	if (m_info)
	{
		delete m_info;
		m_info = NULL;
	}
}

void CDeviceSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMB_LOGINTYPE, m_typeCombo);
	DDX_Control(pDX, IDC_LIST_Upg, m_listDevice);
	DDX_Control(pDX, IDC_EDT_SVR, m_editIP);
	DDX_Control(pDX, IDC_EDT_USERNAME, m_EditUserName);
	DDX_Control(pDX, IDC_EDT_TOTALCHANNEL, m_EditTotal);
	DDX_Control(pDX, IDC_EDT_PSW, m_EditPsw);
	DDX_Control(pDX, IDC_EDT_PORT, m_EditPort);
	DDX_Control(pDX, IDC_EDT_DEVICE_NAME, m_EditDevName);
	DDX_Text(pDX, IDC_EDT_DEVICE_NAME, m_strDevName);
	DDX_Text(pDX, IDC_EDT_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDT_PSW, m_strPsw);
	DDX_Text(pDX, IDC_EDT_TOTALCHANNEL, m_nChannel);
	DDX_Text(pDX, IDC_EDT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDT_SVR, m_strIP);
	DDX_Text(pDX, IDC_EDT_SERIALID, m_DevSerialID);
	DDX_Text(pDX, IDC_EDT_HOSTPORT, m_HostPort);
	DDX_Text(pDX, IDC_EDT_HOSTIP, m_strHostIP);

	DDX_Check(pDX, IDC_CK_SerialID, m_bSerialID);
}


BEGIN_MESSAGE_MAP(CDeviceSearch, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CK_SerialID, OnCHECKSerialID)
	ON_CBN_SELCHANGE(IDC_CMB_LOGINTYPE, OnSelchangeComboLogintype)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, CDeviceSearch::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, CDeviceSearch::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_Searsh, CDeviceSearch::OnBnClickedBtnSearch)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Upg, CDeviceSearch::OnNMDblclkListUpgrade)
	ON_NOTIFY(NM_CLICK, IDC_LIST_Upg, CDeviceSearch::OnNMClickListUpgrade)
	ON_WM_QUERYDRAGICON()
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDC_BTN_GETINFO, &CDeviceEdit::OnBnClickedBtnGetinfo)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_Upg, CDeviceSearch::OnLvnColumnclickListUpgrade)

END_MESSAGE_MAP()

BOOL CDeviceSearch::OnInitDialog()
{
	CDialog::OnInitDialog();

	_CWndCS(this);

	m_EditUserName.SetLimitText(8);
	m_EditTotal.SetLimitText(2);
	m_EditPsw.SetLimitText(8);
	m_EditPort.SetLimitText(5);
	m_EditDevName.SetLimitText(60);

	//modify the device,show the former device information
	if ( 1 == m_nType )
	{
		m_nPort = m_devInfo.nPort;
		m_strDevName = m_devInfo.szDevName;
		m_strUserName = m_devInfo.szUserName;
		m_strPsw = m_devInfo.szPsw;
		m_nChannel = m_devInfo.nTotalChannel;

		UpdateData(FALSE);
	}

	AfxGetApp()->DoWaitCursor(0);

	SetIcon(m_hIcon,TRUE);
	SetIcon(m_hIcon,FALSE);

	//m_PromptDlg.Create(IDD_DLG_PROMPT, this);

	//setting list item 
	m_listDevice.InsertColumn(0, _CS(_T("Upgrade.NO")), LVCFMT_LEFT, 80);
	m_listDevice.InsertColumn(1, _CS(_T("Upgrade.IPAdress")), LVCFMT_LEFT, 200);
	m_listDevice.InsertColumn(2, _CS(_T("Upgrade.Port")), LVCFMT_LEFT, 200);


	m_listDevice.SetExtendedStyle( m_listDevice.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	//DDNS EditCtrl
	m_strHostIP=_T("10.6.1.35");
	m_HostPort=15000;
	//m_DevSerialID=_T("0001020102af");
	m_DevSerialID=_T("1");
	UpdateData(FALSE);
	//DDNS List
	GetDlgItem(IDC_EDT_HOSTPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDT_HOSTIP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDT_SERIALID)->EnableWindow(FALSE);
	UpdateData(FALSE);// 


	m_typeCombo.AddString("TCP");
	m_typeCombo.AddString("UDP");	
	m_typeCombo.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDeviceSearch::OnOK()
{
//	UpdateData();
//	BOOL bIPExists=FALSE;
//
//	
//	if(m_bSerialID)
//	{
//		int maxDeviceNum = 1000;  //���֧���豸����100
//		DDNS_INFO *pDDNSInfo = new DDNS_INFO[maxDeviceNum];
//		SearchMode searchmode;
//		int nReNum = 0;  //ʵ�ʻ�õ��豸����
//		
//		searchmode.nType = DDNS_SERIAL;
//		
//		strcpy(searchmode.szSerIP,m_strHostIP.GetBuffer(0));
//		searchmode.nSerPort = m_HostPort;
//		strcpy(searchmode.szSerialInfo, m_DevSerialID.GetBuffer(0));
//		CString strTemp;
//		bool bret = H264_DVR_GetDDNSInfo(searchmode, pDDNSInfo, maxDeviceNum, nReNum);
//		if ( !bret )
//		{
//			delete pDDNSInfo;
//			pDDNSInfo = NULL;
//			return;
//		}
//		m_strIP=pDDNSInfo[0].IP;
//		m_nPort=pDDNSInfo[0].MediaPort;
//		UpdateData(FALSE);
//	}
//	
//	//*******************************************************************************************
//
//	
//	if ( m_nPort <= 0 )
//	{
//		MessageBox(_CS("Error.PortInvalid"));
//		return;
//	}
//	if ( m_strDevName == "" )
//	{
//		MessageBox(_CS("DeviceManage.NameInvalid"));
//		return;
//	}
//	if ( m_strUserName == "" )
//	{
//		MessageBox(_CS("Error.UserNameInvalid"));
//		return;
//	}
//	/*CMarkup xml;
//	xml.Load(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml");
//	while(xml.FindChildElem("ip"))
//	{
//		xml.IntoElem();
//		CString IP="";
//		xml.FindChildElem("ip2");
//		IP=xml.GetChildData();
//		xml.OutOfElem();
//		if (IP==m_strIP)
//		{
//			MessageBox(_CS("Error.IPExits"));
//			bIPExists=TRUE;
//			OnBnClickedCancel();
//			break;
//		}
//	}*/
//
//	if(bIPExists==FALSE)
//	{
//		H264_DVR_DEVICEINFO OutDev;
//		int nError = 0;
//		//���ó��������豸�����͵ȴ�ʱ��
//		H264_DVR_SetConnectTime(3000, 1);//���ó�������1�Σ��ȴ�ʱ��3s	
////		H264_DVR_SetLocalBindAddress("10.2.55.25");
//		long lLogin=0;
//     	int type=m_typeCombo.GetCurSel();
//		lLogin = H264_DVR_Login(m_strIP.GetBuffer(0), m_nPort, m_strUserName.GetBuffer(0), 
//			m_strPsw.GetBuffer(0), &OutDev, &nError,(SocketStyle)type );
//	
//		if ( lLogin <= 0 )
//		{
//			string strErr;
//			switch (nError)
//			{
//			case H264_DVR_PASSWORD_NOT_VALID:
//				strErr = _CS("Error.PwdErr");
//				break;
//			case H264_DVR_NOPOWER:
//				strErr = _CS("Error.NoPower");
//				break;
//			case H264_DVR_LOGIN_USER_NOEXIST:
//				strErr = _CS("Error.UserNotExisted");
//				break;
//			case H264_DVR_USER_LOCKED:
//				strErr = _CS("Login.Locked");
//				break;
//			case H264_DVR_USER_IN_BLACKLIST:
//				strErr = _CS("Error.InBlackList");
//				break;
//			case H264_DVR_USER_HAS_USED:
//				strErr = _CS("Error.HasLogined");
//				break;
//			case H264_DVR_CONNECT_DEVICE_ERROR:
//				strErr = _CS("Error.NotFound");
//				break;
//			default:
//				{
//					char ch[10];
//					itoa(nError, ch, 10);
//					strErr = _CS("Log.Error") + ch;
//				}
//
//				break;
//			}
//			MessageBox(strErr.c_str());
//			return;
//		}
//		
//		m_devInfo.nTotalChannel = OutDev.byChanNum+OutDev.iDigChannel;
//		m_devInfo.lLoginID = lLogin;
//		m_devInfo.nPort = m_nPort;
//		m_devInfo.devctype=type;
//		strcpy(m_devInfo.szDevName, m_strDevName);
//		strcpy(m_devInfo.szUserName, m_strUserName);
//		strcpy(m_devInfo.szPsw, m_strPsw);
//		strcpy(m_devInfo.szIpaddress, m_strIP.GetBuffer(0));
//
//		if(m_bSerialID)
//		{
//			m_devInfo.bSerialID=m_bSerialID;		
//			m_devInfo.nSerPort=m_HostPort;
//			strcpy(m_devInfo.szSerIP,m_strHostIP.GetBuffer(0));		
//			strcpy(m_devInfo.szSerialInfo, m_DevSerialID.GetBuffer(0));
//		}
//	
//
//		//WriteXML();
//		H264_DVR_SetupAlarmChan(lLogin);
//	}
	CDialog::OnOK();
}

void CDeviceSearch::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CDeviceSearch::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CDeviceSearch::OnBnClickedOk()
{
	OnOK();
}

void CDeviceSearch::OnBnClickedCancel()
{
	OnCancel();
}

//BOOL CDeviceSearch::WriteXML()
//{
//	CMarkup xml;
//	if(xml.Load(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml"))
//	{
//		xml.FindChildElem("ip");
//		if (m_devInfo.szIpaddress!=xml.GetChildData())   // ��ͬ��IP��ַ д�û���������//
//		{
//			xml.AddChildElem( "ip",m_devInfo.szIpaddress);
//			xml.IntoElem();
//			xml.AddChildElem("ip2",m_devInfo.szIpaddress);
//			xml.AddChildElem("DEVICENAME",m_devInfo.szDevName);
//			xml.AddChildElem("nTotalChannel",m_devInfo.nTotalChannel);
//			xml.AddChildElem("username",m_devInfo.szUserName);
//			xml.AddChildElem("port",m_devInfo.nPort);
//			xml.AddChildElem("pwd",m_devInfo.szPsw);
//			xml.AddChildElem("byChanNum",m_devInfo.nTotalChannel);
//			xml.AddChildElem("lID",m_devInfo.lID);
//
//			xml.AddChildElem("bSerialID",m_devInfo.bSerialID);
//			xml.AddChildElem("szSerIP",m_devInfo.szSerIP);
//			xml.AddChildElem("nSerPort",m_devInfo.nSerPort);
//			xml.AddChildElem("szSerialInfo",m_devInfo.szSerialInfo);
//
//			xml.AddChildElem("LoginType",m_devInfo.devctype);
//
//		}
//		xml.Save(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml");
//	}
//	else
//	{
//		xml.AddElem( "INFO" );
//		xml.AddChildElem( "ip",m_devInfo.szIpaddress);
//		xml.IntoElem();
//		xml.AddChildElem("ip2",m_devInfo.szIpaddress);
//		xml.AddChildElem("DEVICENAME",m_devInfo.szDevName);
//		xml.AddChildElem("nTotalChannel",m_devInfo.nTotalChannel);
//		xml.AddChildElem("username",m_devInfo.szUserName);
//		xml.AddChildElem("port",m_devInfo.nPort);
//		xml.AddChildElem("pwd",m_devInfo.szPsw);
//		xml.AddChildElem("byChanNum",m_devInfo.nTotalChannel);
//		xml.AddChildElem("lID",m_devInfo.lID);
//
//		xml.AddChildElem("bSerialID",m_devInfo.bSerialID);
//		xml.AddChildElem("szSerIP",m_devInfo.szSerIP);
//		xml.AddChildElem("nSerPort",m_devInfo.nSerPort);
//		xml.AddChildElem("szSerialInfo",m_devInfo.szSerialInfo);
//		xml.Save(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml");
//	}
//	return TRUE;
//}

UINT CDeviceSearch::SearchDevice( LPVOID parm )
{
	CDeviceSearch *pThis = ( CDeviceSearch* ) parm;

	int nRetLength = 0;
	bool bRet = H264_DVR_SearchDevice( (char *)&pThis->m_Device, sizeof(pThis->m_Device), &nRetLength, 5000 );

	if ( bRet && nRetLength > 0 )
	{
		pThis->m_nDevNum = nRetLength / sizeof(SDK_CONFIG_NET_COMMON_V2);	
	}
	CloseHandle(pThis->m_hSearch);
	pThis->m_hSearch = NULL;

	pThis->SetTimer(2, 0, NULL);
	return 0;
}

void CDeviceSearch::OnBnClickedBtnSearch()
{
	m_nDevNum = 0;
	int count = m_listDevice.GetItemCount(); 
	if (m_info)
	{
		delete []m_info;
		m_info = NULL;
	}
	m_listDevice.DeleteAllItems();

	//���Map
	m_mapDev.clear();
	memset( &m_mapDev[0],0,sizeof(DEV_INFO));


	GetDlgItem(IDC_BTN_Searsh)->EnableWindow(FALSE);

	// 	BeginWaitCursor();
	// 	Sleep(5000);
	// 	EndWaitCursor();

	//m_PromptDlg.ShowMsg(true);

	DWORD dwThreadID;
	m_hSearch = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)SearchDevice, this , 0, &dwThreadID );

	//HCURSOR hCursor;
	//hCursor = LoadCursorFromFile(GET_MODULE_FILE_INFO.strPath + _T("Wait.ani"));//  AfxGetApp()->LoadCursor(IDC_CURSOR1);
	//hCursor=LoadCursorFromFile(_T("res/Wait.ani"));
	//SetCursor(hCursor);
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	CWnd::SetCapture();   //���۽�����ǰ����
}

void CDeviceSearch::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent==1 )
	{
		KillTimer(1);

		//m_PromptDlg.ShowMsg(true);

		//HCURSOR hCursor;
		//hCursor = LoadCursorFromFile(GET_MODULE_FILE_INFO.strPath+ _T("Wait.ani"));//  AfxGetApp()->LoadCursor(IDC_CURSOR1);
		//hCursor=LoadCursorFromFile(_T("res/Wait.ani"));
		//SetCursor(hCursor);
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		//CWnd::SetCapture();
	}
	else if ( nIDEvent == 2 )
	{
		GetDlgItem(IDC_BTN_Searsh)->EnableWindow(TRUE);
		KillTimer(2);

		//m_PromptDlg.ShowMsg(false);

		int count = m_listDevice.GetItemCount();
		int i = 0; 
		//check
		for ( i = 0; i < count; i ++ )
		{
			CString strIp;
			CString strIp2;
			int nPort;
			int nPort2;
			strIp.Format(_T("%d.%d.%d.%d"), m_Device[i].HostIP.c[0], m_Device[i].HostIP.c[1],m_Device[i].HostIP.c[2], m_Device[i].HostIP.c[3]);
			nPort=m_Device[i].TCPPort;

			for ( int j = i + 1; j < count; j ++ )
			{
				strIp2.Format(_T("%d.%d.%d.%d"), m_Device[j].HostIP.c[0], m_Device[j].HostIP.c[1],
					m_Device[j].HostIP.c[2], m_Device[j].HostIP.c[3]);
				nPort2 = m_Device[j].TCPPort;

				if ( strIp2 == strIp && nPort2 == nPort )
				{
					memset( &m_Device[j], 0, sizeof(SDK_CONFIG_NET_COMMON_V2) );
				}
			}
		}

		int nIndex = 0;
		for (i = 0 ; i < m_nDevNum; i ++ )
		{
			if ( m_Device[i].TCPPort == 0 || m_Device[i].HostIP.l == 0 )
			{
				continue;
			}
			//			CString strTemp(_T(""));
			//			strTemp.Format(_T("%d"), count + nIndex + 1);
			//			m_listDevice.InsertItem(count + nIndex, strTemp);
			//			
			//			strTemp.Format(_T("%d.%d.%d.%d"), m_Device[i].HostIP.c[0], m_Device[i].HostIP.c[1],m_Device[i].HostIP.c[2], m_Device[i].HostIP.c[3]);
			//			m_listDevice.SetItemText(count + nIndex, 1, strTemp);
			//			strTemp.Format(_T("%ld"), m_Device[i].TCPPort );
			//			m_listDevice.SetItemText(count + nIndex, 2, strTemp );

			//Edit Device Information

			DEV_INFO *pData = new DEV_INFO;
			memset( pData, 0, sizeof(DEV_INFO));

			pData->nPort = m_Device[i].TCPPort;
			sprintf(pData->szIpaddress, "%d.%d.%d.%d", m_Device[i].HostIP.c[0], m_Device[i].HostIP.c[1],
				m_Device[i].HostIP.c[2], m_Device[i].HostIP.c[3]);
			//sprintf(pData->szUserName, "%s", "admin");
			strcpy(pData->szUserName,"admin");
			//sprintf(pData->szPsw, "%s", "");
			strcpy(pData->szPsw,"");
			memcpy(&pData->NetComm, &m_Device[i], sizeof(SDK_CONFIG_NET_COMMON_V2));

			//�����Ѿ����ڵ�IP��mac��ͬ���豸
			int nFlag = 0;
			if ( strcmp(m_mapDev[0].szIpaddress, "") )
			{
				int j = 0;
				std::map<int, DEV_INFO>::iterator iter = m_mapDev.begin();
				for ( ; iter != m_mapDev.end(); iter ++)
				{
					DEV_INFO pDevInfo;
					memset( &pDevInfo, 0 ,sizeof(DEV_INFO));
					memcpy( &pDevInfo ,&m_mapDev[j],sizeof(DEV_INFO));
					if ( !strcmp( pDevInfo.szIpaddress, pData->szIpaddress) 
						&& !strcmp( pDevInfo.NetComm.sMac , pData->NetComm.sMac))
					{
						nFlag = 1; //������ͬ��IP����Mac
						break;
					}
					j ++;
				}
			}

			//			pData->nListNum = count + nIndex;
			//			
			//			m_listDevice.SetItemData( count + nIndex,(long)pData );
			//			
			//			nIndex ++;
			if ( nFlag )
			{
				delete pData;
				pData = NULL;
				continue;
			}else
			{
				memcpy( &m_mapDev[ nIndex ], pData, sizeof(DEV_INFO));
				m_Device_Sort[nIndex++] = m_Device[i];
			}
		}
		//����
		for(int i=0; i<nIndex; i++)
		{
			CString strTemp1(_T("")),strTemp2(_T("")),strTemp(_T(""));
			SDK_CONFIG_NET_COMMON_V2 tempComm;
			for(int j=0;j < nIndex-1 ; j++)
			{
				strTemp1.Format(_T("%d.%d.%d.%d"), m_Device_Sort[j].HostIP.c[0], m_Device_Sort[j].HostIP.c[1],m_Device_Sort[j].HostIP.c[2], m_Device_Sort[j].HostIP.c[3]);
				strTemp2.Format(_T("%d.%d.%d.%d"), m_Device_Sort[j+1].HostIP.c[0], m_Device_Sort[j+1].HostIP.c[1],m_Device_Sort[j+1].HostIP.c[2], m_Device_Sort[j+1].HostIP.c[3]);
				if (strTemp1>strTemp2)
				{
					tempComm = m_Device_Sort[j];
					m_Device_Sort[j] = m_Device_Sort[j+1];
					m_Device_Sort[j+1] = tempComm;
				}
			}
		}
		if (m_info)
		{
			delete []m_info;
			m_info = NULL;
		}
		m_info = new DEV_INFO[nIndex];
		memset(m_info,0,sizeof(DEV_INFO));
		for(int i=0; i<nIndex; i++)
		{

			CString strTemp(_T(""));
			strTemp.Format(_T("%d"), i + 1);
			m_listDevice.InsertItem(i, strTemp);

			strTemp.Format(_T("%d.%d.%d.%d"), m_Device_Sort[i].HostIP.c[0], m_Device_Sort[i].HostIP.c[1],m_Device_Sort[i].HostIP.c[2], m_Device_Sort[i].HostIP.c[3]);
			OutputDebugString(strTemp);
			m_listDevice.SetItemText(i, 1, strTemp);
			strTemp.Format(_T("%ld"), m_Device_Sort[i].TCPPort );
			m_listDevice.SetItemText(i, 2, strTemp );



			m_info[i].nPort = m_Device_Sort[i].TCPPort;
			sprintf(m_info[i].szIpaddress, "%d.%d.%d.%d", m_Device_Sort[i].HostIP.c[0], m_Device_Sort[i].HostIP.c[1],
				m_Device_Sort[i].HostIP.c[2], m_Device_Sort[i].HostIP.c[3]);

			strcpy(m_info[i].szUserName,"admin");

			strcpy(m_info[i].szPsw,"");
			memcpy(&m_info[i].NetComm, &m_Device_Sort[i], sizeof(SDK_CONFIG_NET_COMMON_V2));

			char ch[100];
			sprintf(ch,"i:%d,%x\n",i,&m_info[i]);
			OutputDebugString(ch);
			m_listDevice.SetItemData(i,(long)&m_info[i] );


		}
		ReleaseCapture();  //�ͷ����
	}
	//close
	else if ( nIDEvent == 3 )
	{
		KillTimer( 3 );
		H264_DVR_CloseUpgradeHandle( m_lUpgradeHandle );
	}
	CDialog::OnTimer(nIDEvent);
}

void CDeviceSearch::DelArpEntry( DWORD dwAddr )
{

}

void CDeviceSearch::OnNMDblclkListUpgrade( NMHDR *pNMHDR, LRESULT *pResult )
{
	*pResult = 0;
}

void CDeviceSearch::OnNMClickListUpgrade( NMHDR *pNMHDR, LRESULT *pResult )
{
	int nIndex = m_listDevice.GetSelectionMark();
	if ( nIndex >= 0 )
	{	
		DEV_INFO *pData = (DEV_INFO *)m_listDevice.GetItemData( nIndex );
		if ( pData )
		{
			CDeviceSearch dlg;
			//dlg.m_bModify = true;
			memcpy( &dlg.m_devInfo, pData, sizeof(DEV_INFO) );
			m_nPort=pData->nPort;
			CString str_IP;
			str_IP.Format(_T("%d.%d.%d.%d"),pData->NetComm.HostIP.c[0],pData->NetComm.HostIP.c[1],pData->NetComm.HostIP.c[2],pData->NetComm.HostIP.c[3]);
			m_strDevName = str_IP;
			m_strIP=str_IP;
			m_strUserName=pData->szUserName;
			m_strPsw=pData->szPsw;
			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}

void CDeviceSearch::OnDestroy()
{
	CDialog::OnDestroy();
}

void CDeviceSearch::OnCHECKSerialID()
{
	UpdateData();
	if(m_bSerialID)
	{
		GetDlgItem(IDC_EDIT_HOSTPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_HOSTIP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_SERIALID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SVR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_LOGINTYPE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_HOSTPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_HOSTIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_SERIALID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SVR)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_LOGINTYPE)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDeviceSearch::OnSelchangeComboLogintype()
{
	
}

DWORD sort_colum = 0; //��¼�������
//int  method = 1; //��¼�Ƚϵķ���
bool metod = false;

static int _stdcall MyCompareProc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort) 
{
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	LVFINDINFO findinfo;
	findinfo.flags = LVFI_PARAM;
	findinfo.lParam = lParam1;
	int iltem1 = pListCtrl->FindItem(&findinfo,-1);
	findinfo.lParam = lParam2;
	int iltem2 = pListCtrl->FindItem(&findinfo,-1);

	CString strltem1,strltem2;
	strltem1 = pListCtrl->GetItemText(iltem1,sort_colum);
	strltem2 = pListCtrl->GetItemText(iltem2,sort_colum);
	if (sort_colum < 1)
	{
		if (metod)
		{
			return atoi(strltem1) - atoi(strltem2);
		}
		return atoi(strltem2) - atoi(strltem1);
	}
	else
	{
		if (metod)
		{
			return strcmp(strltem1,strltem2);
		}
		return strcmp(strltem2,strltem1);
	}
}
void CDeviceSearch::OnLvnColumnclickListUpgrade( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (sort_colum != pNMLV->iSubItem)
	{
		sort_colum = pNMLV->iSubItem;
		metod = true;
	}
	else
	{
		metod = !metod;
	}
	m_listDevice.SortItems(MyCompareProc,(LPARAM)(&m_listDevice));
	*pResult = 0;
}

// CDeviceSearch ��Ϣ�������
