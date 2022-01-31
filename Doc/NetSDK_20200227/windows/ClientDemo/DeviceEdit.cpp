// DeviceEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "ClientDemoDlg.h"
#include "DeviceEdit.h"
#include "Markup.h"
#include "Define.h"
#include <vector>
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib,"ws2_32.lib"


#include "IPEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceEdit dialog


CDeviceEdit::CDeviceEdit(CWnd* pParent /*=NULL*/)
: CDialog(CDeviceEdit::IDD, pParent)
, m_nDevNum(0)
, m_lUpgradeHandle(0)
, m_bCloud(FALSE)
{
	//{{AFX_DATA_INIT(CDeviceEdit)
	m_strDevName = _T("Test");
	m_nPort = 34567;
	m_strPsw = _T("");
	m_nChannel = 16;
	m_strUserName = _T("admin");
	m_strIP = _T("");
	m_DevSerialID = _T("");
	m_HostPort = 0;
	m_strHostIP = _T("");
	m_bIP = FALSE;
	m_bCloud = FALSE;
	//}}AFX_DATA_INIT
	
	m_nType = 0;	//"0" means to add device
	memset(&m_devInfo, 0, sizeof(m_devInfo));
	m_info = NULL;
}

CDeviceEdit::~CDeviceEdit()
{
	if (m_info)
	{
		delete m_info;
		m_info = NULL;
	}
}

void CDeviceEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceEdit)
	/*DDX_Control(pDX, IDC_LIST_DDNS_DEV, m_listDDNSDev);*/
	DDX_Control(pDX, IDC_LIST_Upgrade, m_listDevice);
	DDX_Control(pDX, IDC_EDIT_SVR, m_editIP);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_EditUserName);
	DDX_Control(pDX, IDC_EDIT_TOTALCHANNEL, m_EditTotal);
	DDX_Control(pDX, IDC_EDIT_PSW, m_EditPsw);
	DDX_Control(pDX, IDC_EDIT_PORT, m_EditPort);
	DDX_Control(pDX, IDC_EDIT_DEVICE_NAME, m_EditDevName);
	DDX_Text(pDX, IDC_EDIT_DEVICE_NAME, m_strDevName);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_PSW, m_strPsw);
	DDV_MaxChars(pDX, m_strPsw, 16);
	DDX_Text(pDX, IDC_EDIT_TOTALCHANNEL, m_nChannel);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_SVR, m_strIP);
	DDX_Text(pDX, IDC_EDIT_SERIALID, m_DevSerialID);
	DDX_Text(pDX, IDC_EDIT_HOSTPORT, m_HostPort);
	DDX_Text(pDX, IDC_EDIT_HOSTIP, m_strHostIP);
	DDX_Check(pDX, IDC_CHECK_IP, m_bIP);
	DDX_Check(pDX, IDC_CHECK_CLOUD, m_bCloud);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceEdit, CDialog)
//{{AFX_MSG_MAP(CDeviceEdit)
/*ON_BN_CLICKED(IDC_BTN_GETINFO, OnBtnGetinfo)*/
/*ON_NOTIFY(NM_CLICK, IDC_LIST_DDNS_DEV, OnClickListDdnsDev)*/
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_IP, OnCheckIp)
	//}}AFX_MSG_MAP
ON_BN_CLICKED(IDOK, CDeviceEdit::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, CDeviceEdit::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BTN_Search, CDeviceEdit::OnBnClickedBtnSearch)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BTN_Setting, CDeviceEdit::OnBnClickedBtnSetting)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_Upgrade, CDeviceEdit::OnNMDblclkListUpgrade)
ON_NOTIFY(NM_CLICK, IDC_LIST_Upgrade, CDeviceEdit::OnNMClickListUpgrade)
ON_WM_QUERYDRAGICON()
ON_WM_PAINT()
//ON_BN_CLICKED(IDC_BTN_GETINFO, &CDeviceEdit::OnBnClickedBtnGetinfo)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_Upgrade, &CDeviceEdit::OnLvnItemchangedListUpgrade)
ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_Upgrade, &CDeviceEdit::OnLvnColumnclickListUpgrade)
ON_BN_CLICKED(IDC_CHECK_CLOUD, &CDeviceEdit::OnBnClickedCheckCloud)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceEdit message handlers

BOOL CDeviceEdit::OnInitDialog() 
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
	
	m_PromptDlg.Create(IDD_DLG_PROMPT, this);
	
	//setting list item 
	m_listDevice.InsertColumn(0, _CS(_T("Upgrade.NO")), LVCFMT_LEFT, 50);
	m_listDevice.InsertColumn(1, _CS(_T("Upgrade.IPAdress")), LVCFMT_LEFT, 150);
	m_listDevice.InsertColumn(2, _CS(_T("Upgrade.Port")), LVCFMT_LEFT, 60);
	m_listDevice.InsertColumn(3, _CS(_T("Upgrade.SerailID")), LVCFMT_LEFT, 150);
	
	
	m_listDevice.SetExtendedStyle( m_listDevice.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	
	//DDNS EditCtrl
	m_strHostIP=_T("10.6.1.35");
	m_HostPort=15000;
	//m_DevSerialID=_T("0001020102af");
	m_DevSerialID=_T("1");
	UpdateData(FALSE);
	//DDNS List
	m_bIP=TRUE;
	GetDlgItem(IDC_EDIT_HOSTPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HOSTIP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SERIALID)->EnableWindow(FALSE);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDeviceEdit::OnOK() 
{
	UpdateData();
	BOOL bIPExists=FALSE;

	CClientDemoDlg *pParent = (CClientDemoDlg *)GetParent();
	
	if ( m_nPort <= 0 )
	{
		MessageBox(_CS("Error.PortInvalid"));
		return;
	}
	if ( m_strDevName == "" )
	{
		MessageBox(_CS("DeviceManage.NameInvalid"));
		return;
	}
	if ( m_strUserName == "" )
	{
		MessageBox(_CS("Error.UserNameInvalid"));
		return;
	}
	CMarkup xml;
	xml.Load(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml");
	while(xml.FindChildElem("ip"))
	{
		xml.IntoElem();
		CString IP="";
		xml.FindChildElem("ip2");
		IP=xml.GetChildData();
		xml.OutOfElem();
		if (IP==m_strIP)
		{
			MessageBox(_CS("Error.IPExits"));
			bIPExists=TRUE;
			OnBnClickedCancel();
			break;
		}
	}

	if(bIPExists==FALSE)
	{
		H264_DVR_DEVICEINFO OutDev;
		int nError = 0;
		//设置尝试连接设备次数和等待时间
		H264_DVR_SetConnectTime(3000, 1);//设置尝试连接1次，等待时间3s
		long lLogin = 0;
		if (m_bCloud)
		{
			lLogin = H264_DVR_Login_Cloud(m_strIP.GetBuffer(0), m_nPort, m_strUserName.GetBuffer(0), 
				m_strPsw.GetBuffer(0), &OutDev, &nError, NULL);
		}
		else
		{
			lLogin = H264_DVR_Login(m_strIP.GetBuffer(0), m_nPort, m_strUserName.GetBuffer(0), 
				m_strPsw.GetBuffer(0), &OutDev, &nError );
		}
		if ( lLogin <= 0 )
		{
			string strErr;
			switch (nError)
			{
			case H264_DVR_PASSWORD_NOT_VALID:
				strErr = _CS("Error.PwdErr");
				break;
			case H264_DVR_NOPOWER:
				strErr = _CS("Error.NoPower");
				break;
			case H264_DVR_LOGIN_USER_NOEXIST:
				strErr = _CS("Error.UserNotExisted");
				break;
			case H264_DVR_USER_LOCKED:
				strErr = _CS("Login.Locked");
				break;
			case H264_DVR_USER_IN_BLACKLIST:
				strErr = _CS("Error.InBlackList");
				break;
			case H264_DVR_USER_HAS_USED:
				strErr = _CS("Error.HasLogined");
				break;
			case H264_DVR_CONNECT_DEVICE_ERROR:
				strErr = _CS("Error.NotFound");
				break;
			case H264_DVR_CLOUD_LOGIN_ERR:
				strErr = _CS("Log.Error") + OutDev.sCloudErrCode;
				break;
			default:
				{
					char ch[10];
					itoa(nError, ch, 10);
					strErr = _CS("Log.Error") + ch;
				}
				break;
			}
			MessageBox(strErr.c_str());
			return;
		}
		m_devInfo.nTotalChannel = OutDev.byChanNum+OutDev.iDigChannel;
		m_devInfo.lLoginID = lLogin;
		m_devInfo.nPort = m_nPort;
		strcpy(m_devInfo.szDevName, m_strDevName);
		strcpy(m_devInfo.szUserName, m_strUserName);
		strcpy(m_devInfo.szPsw, m_strPsw);
		if(m_bCloud)
		{
			strcpy(m_devInfo.szIpaddress, "");
			strcpy(m_devInfo.szCloudID, m_strIP.GetBuffer(0));
		}
		else
		{
			strcpy(m_devInfo.szIpaddress, m_strIP.GetBuffer(0));
			strcpy(m_devInfo.szCloudID, OutDev.sSerialNumber);
		}

		m_devInfo.bCloud = 0;
		if(m_bCloud != 0)
		{
			m_devInfo.bCloud = 1;
		}
		

		pParent->m_ListDevInfo.push_back(&m_devInfo);

		WriteXML();
		H264_DVR_SetupAlarmChan(lLogin);
	}
	CDialog::OnOK();
}



void CDeviceEdit::OnPaint()
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
HCURSOR CDeviceEdit::OnQueryDragIcon()
{
	
	return (HCURSOR) m_hIcon;
}

void CDeviceEdit::OnBnClickedOk()
{
	OnOK();
}


void CDeviceEdit::OnBnClickedCancel()
{
	OnCancel();
}
BOOL CDeviceEdit::WriteXML()
{
	CMarkup xml;
	char szIpAddress[64] = {};
	if(m_devInfo.bCloud == 1)
	{
		strcpy(szIpAddress, m_devInfo.szCloudID);
	}
	else
	{
		strcpy(szIpAddress, m_devInfo.szIpaddress);
	}
	if(xml.Load(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml"))
	{
		xml.FindChildElem("ip");
		
		if (szIpAddress!=xml.GetChildData())
		{
			xml.AddChildElem( "ip",szIpAddress);
			xml.IntoElem();
			xml.AddChildElem("ip2",szIpAddress);
			xml.AddChildElem("DEVICENAME",m_devInfo.szDevName);
			xml.AddChildElem("nTotalChannel",m_devInfo.nTotalChannel);
			xml.AddChildElem("username",m_devInfo.szUserName);
			xml.AddChildElem("port",m_devInfo.nPort);
			xml.AddChildElem("pwd",m_devInfo.szPsw);
			xml.AddChildElem("byChanNum",m_devInfo.nTotalChannel);
			xml.AddChildElem("lID",m_devInfo.lID);

			xml.AddChildElem("bSerialID",m_devInfo.bSerialID);
			xml.AddChildElem("szSerIP",m_devInfo.szSerIP);
			xml.AddChildElem("nSerPort",m_devInfo.nSerPort);
			xml.AddChildElem("szSerialInfo",m_devInfo.szSerialInfo);
			xml.AddChildElem("bCloud", m_devInfo.bCloud);
			xml.AddChildElem("szCloudID", m_devInfo.szCloudID);
			
		}
		xml.Save(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml");
	}
	else
	{
		xml.AddElem( "INFO" );
		xml.AddChildElem( "ip",szIpAddress);
		xml.IntoElem();
		xml.AddChildElem("ip2",szIpAddress);
		xml.AddChildElem("DEVICENAME",m_devInfo.szDevName);
		xml.AddChildElem("nTotalChannel",m_devInfo.nTotalChannel);
		xml.AddChildElem("username",m_devInfo.szUserName);
		xml.AddChildElem("port",m_devInfo.nPort);
		xml.AddChildElem("pwd",m_devInfo.szPsw);
		xml.AddChildElem("byChanNum",m_devInfo.nTotalChannel);
		xml.AddChildElem("lID",m_devInfo.lID);

		xml.AddChildElem("bSerialID",m_devInfo.bSerialID);
		xml.AddChildElem("szSerIP",m_devInfo.szSerIP);
		xml.AddChildElem("nSerPort",m_devInfo.nSerPort);
		xml.AddChildElem("szSerialInfo",m_devInfo.szSerialInfo);
		xml.AddChildElem("bCloud", m_devInfo.bCloud);
		xml.AddChildElem("szCloudID", m_devInfo.szCloudID);
		xml.Save(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml");
	}
	return TRUE;
}


UINT CDeviceEdit::SearchDevice(LPVOID parm)
{
	CDeviceEdit *pThis = ( CDeviceEdit* ) parm;
	
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

void CDeviceEdit::OnBnClickedBtnSearch()
{
	m_nDevNum = 0;
	int count = m_listDevice.GetItemCount(); 
	for ( int i = 0; i < count; i ++ )
	{
		DEV_INFO *pData = (DEV_INFO *)m_listDevice.GetItemData(i);
		if ( (i == count - 1) && pData )
		{
			//delete pData;
			pData = NULL;
		}
	}
	
	m_nDevNum = 0; 

	
	m_listDevice.DeleteAllItems();
	
	//清除Map
	m_mapDev.clear();
	memset( &m_mapDev[0],0,sizeof(DEV_INFO));
	
	
	GetDlgItem(IDC_BTN_Search)->EnableWindow(FALSE);
	
	// 	BeginWaitCursor();
	// 	Sleep(5000);
	// 	EndWaitCursor();
	
	m_PromptDlg.ShowMsg(true);
	
	DWORD dwThreadID;
	m_hSearch = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)SearchDevice, this , 0, &dwThreadID );
	
	//HCURSOR hCursor;
	//hCursor = LoadCursorFromFile(GET_MODULE_FILE_INFO.strPath + _T("Wait.ani"));//  AfxGetApp()->LoadCursor(IDC_CURSOR1);
	//hCursor=LoadCursorFromFile(_T("res/Wait.ani"));
	//SetCursor(hCursor);
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	CWnd::SetCapture();
	
	//m_PromptDlg.ShowMsg(false);
	
	//	ReleaseCapture();
}
	
void CDeviceEdit::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent==1 )
	{
		KillTimer(1);
		
		m_PromptDlg.ShowMsg(true);
		
		//HCURSOR hCursor;
		//hCursor = LoadCursorFromFile(GET_MODULE_FILE_INFO.strPath+ _T("Wait.ani"));//  AfxGetApp()->LoadCursor(IDC_CURSOR1);
		//hCursor=LoadCursorFromFile(_T("res/Wait.ani"));
		//SetCursor(hCursor);
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		//CWnd::SetCapture();
	}
	else if ( nIDEvent == 2 )
	{
		GetDlgItem(IDC_BTN_Search)->EnableWindow(TRUE);
		KillTimer(2);
		
		m_PromptDlg.ShowMsg(false);
		
		int count = m_listDevice.GetItemCount();
		int i = 0; 
		//check
		for ( i = 0; i < count; i ++ )
		{
			CString strIp;
			CString strIp2;
			int nPort;
			int nPort2;
			strIp.Format(_T("%d.%d.%d.%d"), m_Device[i].HostIP.c[0], m_Device[i].HostIP.c[1],
				m_Device[i].HostIP.c[2], m_Device[i].HostIP.c[3]);
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
			
			//过滤已经存在的IP和mac相同的设备
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
						nFlag = 1; //存在相同的IP或者Mac
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
// 				CString strTemp(_T(""));
// 				strTemp.Format(_T("%d"), count + nIndex + 1);
// 				m_listDevice.InsertItem(count + nIndex, strTemp);
// 				unsigned char *str = NULL;
// 				str = m_Device[i].HostIP.c;
// 				//std::sort(m_Device[i].HostIP.c,m_nDevNum);
// 				strTemp.Format(_T("%d.%d.%d.%d"), m_Device[i].HostIP.c[0], m_Device[i].HostIP.c[1],m_Device[i].HostIP.c[2], m_Device[i].HostIP.c[3]);
// 				m_listDevice.SetItemText(count + nIndex, 1, strTemp);
// 				strTemp.Format(_T("%ld"), m_Device[i].TCPPort );
// 				m_listDevice.SetItemText(count + nIndex, 2, strTemp );
// 				
// 				pData->nListNum = count + nIndex;
// 				
// 				memcpy( &m_mapDev[ nIndex ], pData, sizeof(DEV_INFO));
// 				
// 				m_listDevice.SetItemData(count + nIndex,(long)pData );
// 				
// 				nIndex ++;
				delete pData;
				pData = NULL;
			}
		}
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
			delete[] m_info;
			m_info = NULL;
		}

		if(nIndex)
		{
			m_info = new DEV_INFO[nIndex];
			memset(m_info,0,sizeof(DEV_INFO));		
		}

		for(int i=0; i<nIndex; i++)
		{
			CString strTemp(_T(""));
			strTemp.Format(_T("%d"), i + 1);
			m_listDevice.InsertItem(i, strTemp);

			strTemp.Format(_T("%d.%d.%d.%d"), m_Device_Sort[i].HostIP.c[0], m_Device_Sort[i].HostIP.c[1],m_Device_Sort[i].HostIP.c[2], m_Device_Sort[i].HostIP.c[3]);
			//OutputDebugString(strTemp);
			m_listDevice.SetItemText(i, 1, strTemp);
			strTemp.Format(_T("%ld"), m_Device_Sort[i].TCPPort );
			m_listDevice.SetItemText(i, 2, strTemp );
			strTemp.Format(_T("%s"), m_Device_Sort[i].sSn);
			m_listDevice.SetItemText(i, 3, strTemp );

			m_info[i].nPort = m_Device_Sort[i].TCPPort;
			sprintf(m_info[i].szIpaddress, "%d.%d.%d.%d", m_Device_Sort[i].HostIP.c[0], m_Device_Sort[i].HostIP.c[1],
				m_Device_Sort[i].HostIP.c[2], m_Device_Sort[i].HostIP.c[3]);

			strcpy(m_info[i].szUserName,"admin");

			strcpy(m_info[i].szPsw,"");
			memcpy(&m_info[i].NetComm, &m_Device_Sort[i], sizeof(SDK_CONFIG_NET_COMMON_V2));
			
			m_listDevice.SetItemData(i,(long)&m_info[i] );
			
				
		}
		ReleaseCapture();  //释放鼠标
	}
	//close
	else if ( nIDEvent == 3 )
	{
		KillTimer( 3 );
		H264_DVR_CloseUpgradeHandle( m_lUpgradeHandle );
	}
	CDialog::OnTimer(nIDEvent);
}

bool GetMacByGetAdaptersInfo(char *pMacRet)
{
	bool ret = false;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if(pAdapterInfo == NULL)
	{
		return false;
	}

	if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL) 
		{
			return false;
		}
	}

	if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
	{
		for(PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			char acMAC[32] = {0};
			sprintf(acMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
				int (pAdapter->Address[0]),
				int (pAdapter->Address[1]),
				int (pAdapter->Address[2]),
				int (pAdapter->Address[3]),
				int (pAdapter->Address[4]),
				int (pAdapter->Address[5]));
			strcpy(pMacRet, acMAC);
			ret = true;
			break;
		}
	}

	free(pAdapterInfo);
	return ret;
}

void CDeviceEdit::OnBnClickedBtnSetting()
{
	int nSelect = m_listDevice.GetSelectionMark();
	if (nSelect != -1)
	{
		DEV_INFO *pData = (DEV_INFO *)m_listDevice.GetItemData(nSelect);
		if ( pData )
		{
			CIPEdit dlg;
			char localMac[64] = {0};
			bool bRet = GetMacByGetAdaptersInfo(localMac);

			char ip[16] = {0};
			char gateway[16] = {0};
			char submask[16] = {0};
			sprintf(ip, "%d.%d.%d.%d", pData->NetComm.HostIP.c[0], pData->NetComm.HostIP.c[1], pData->NetComm.HostIP.c[2], pData->NetComm.HostIP.c[3]);
			sprintf(gateway, "%d.%d.%d.%d", pData->NetComm.Gateway.c[0], pData->NetComm.Gateway.c[1], pData->NetComm.Gateway.c[2], pData->NetComm.Gateway.c[3]);
			sprintf(submask, "%d.%d.%d.%d", pData->NetComm.Submask.c[0], pData->NetComm.Submask.c[1], pData->NetComm.Submask.c[2], pData->NetComm.Submask.c[3]);

			dlg.m_NetComm.HostIP.l = inet_addr(ip);
			dlg.m_NetComm.Gateway.l = inet_addr(gateway);
			dlg.m_NetComm.Submask.l = inet_addr(submask);
			strcpy(dlg.m_NetComm.HostName, pData->NetComm.HostName);
			dlg.m_NetComm.HttpPort = pData->NetComm.HttpPort;
			strcpy(dlg.m_NetComm.LocalMac, localMac);
			dlg.m_NetComm.MaxBps = pData->NetComm.MaxBps;
			dlg.m_NetComm.MaxConn = pData->NetComm.MaxConn;
			dlg.m_NetComm.MonMode = 0;
			strcpy(dlg.m_NetComm.sMac, pData->NetComm.sMac);
			dlg.m_NetComm.SSLPort = pData->NetComm.SSLPort;
			dlg.m_NetComm.TCPPort = pData->NetComm.TCPPort;
			dlg.m_NetComm.TransferPlan = pData->NetComm.TransferPlan;
			dlg.m_NetComm.UDPPort = pData->NetComm.UDPPort;
			dlg.m_NetComm.bUseHSDownLoad = false;
			strcpy(dlg.m_NetComm.UserName, "admin");


			if ( dlg.DoModal() == IDOK )
			{
				//跨网段设置设备配置，目前只支持对网络配置进行设置。
				int nRet = H264_DVR_SetConfigOverNet(E_SDK_CONFIG_SYSNET, -1, (char *)&dlg.m_NetComm, sizeof(dlg.m_NetComm), 5000);
				if (nRet == H264_DVR_NOERROR || nRet == H264_DVR_SUCCESS)
				{
					MessageBox(_CS(_T("Error.SaveSuccess")), _CS(_T("Upgrade.Prompt")));
				}
				else if (nRet == H264_DVR_OPT_REBOOT)
				{
					MessageBox(_CS(_T("Upgrade.NeedReboot")), _CS(_T("Upgrade.Prompt")));
				}
				else
				{
					MessageBox(_CS(_T("Error.SetConfigFail")), _CS(_T("Upgrade.Prompt")));
				}
			}
		}
	}
}


// #define ulong unsigned long
// #define swap_long(x) (((((ulong)(x)) & 0xff000000) >> 24) | \
// 	((((ulong)(x)) & 0x00ff0000) >> 8) | \
// 	((((ulong)(x)) & 0x0000ff00) << 8) | \
// 	((((ulong)(x)) & 0x000000ff) << 24))
// 
// #include <IPHlpApi.h>
// #include <Iprtrmib.h>
void CDeviceEdit::DelArpEntry(DWORD dwAddr)
{
	// 	char tempChar;
	// 	DWORD dwListSize = 1;
	// 	DWORD dwRet;
	// 	in_addr inaddr;
	// 	const short nAdapterIndex = 0;
	// 
	// 	dwRet=GetIpNetTable((PMIB_IPNETTABLE)&tempChar,&dwListSize,TRUE);  // 关键函数
	// 	if (dwRet==ERROR_INSUFFICIENT_BUFFER)
	// 	{
	// 		PMIB_IPNETTABLE pIpNetTable = (PMIB_IPNETTABLE)new(char[dwListSize]);
	// 		dwRet =GetIpNetTable(pIpNetTable,&dwListSize,TRUE);
	// 		if (dwRet == ERROR_SUCCESS)
	// 		{
	// 			for (int i=0; i<(int)pIpNetTable->dwNumEntries; i++)
	// 			{
	// 				//if (dwAddr == swap_long(pIpNetTable->table[i].dwAddr))
	// 				unsigned char *pIP = (unsigned char*)&pIpNetTable->table[i].dwAddr; 
	// 				CString str1;
	// 				str1.Format(_T("%d.%d.%d.%d"), *pIP,*(pIP+1),*(pIP+2),*(pIP+3));
	// 
	// 				pIP =(unsigned char*)&dwAddr; 
	// 				CString str2;
	// 				str2.Format(_T("%d.%d.%d.%d"), *pIP,*(pIP+1),*(pIP+2),*(pIP+3));
	// 
	// 				if (dwAddr == pIpNetTable->table[i].dwAddr)
	// 				{
	// 					DeleteIpNetEntry(&pIpNetTable->table[i]);	
	// 					break;
	// 				}
	// 			}
	// 		}
	// 		delete [](char *)pIpNetTable;
	// 	}
}

//DBClick
DWORD sort_colum = 0; //记录点击的列
//int  method = 1; //记录比较的方法
bool metod = false;

static int _stdcall MyCompareProc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort) 
{
// 	int row1 = (int)lParam1;
// 	int row2 = (int)lParam2;
// 
// 	CListCtrl* lc = reinterpret_cast<CListCtrl*>(lParamSort);
// 
// 	CString lp1 = lc->GetItemText(row1,sort_colum);
// 	CString lp2 = lc->GetItemText(row2,sort_colum);
// 
// 	if (sort_colum < 2)
// 	{
// 		if (metod)
// 			return atoi(lp1) - atoi(lp2);
// 		else
// 			return atoi(lp1) - atoi(lp1);
// 	}
// 	else
// 	{
// 		if(metod)
// 			return lp1.CompareNoCase(lp2);
// 		else
// 			return lp2.CompareNoCase(lp1);
// 	}
// 	return 0;
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

void CDeviceEdit::OnNMDblclkListUpgrade(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

//Click
void CDeviceEdit::OnNMClickListUpgrade(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_listDevice.GetSelectionMark();
	if ( nIndex >= 0 )
	{	
		DEV_INFO *pData = (DEV_INFO *)m_listDevice.GetItemData( nIndex );
		if ( pData )
		{
			CDeviceEdit dlg;
			//dlg.m_bModify = true;
			memcpy( &dlg.m_devInfo, pData, sizeof(DEV_INFO) );
			m_nPort=pData->nPort;
			CString str_IP;
			if (m_bCloud)
			{
				str_IP.Format(_T("%s"), pData->NetComm.sSn); 
			}
			else
			{
				str_IP.Format(_T("%d.%d.%d.%d"),pData->NetComm.HostIP.c[0],pData->NetComm.HostIP.c[1],pData->NetComm.HostIP.c[2],pData->NetComm.HostIP.c[3]);
			}
			m_strDevName = str_IP;
			m_strIP=str_IP;
			m_strUserName=pData->szUserName;
			m_strPsw=pData->szPsw;
			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}


void CDeviceEdit::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CDeviceEdit::OnCheckIp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bIP)
	{
		m_bCloud = FALSE;
		GetDlgItem(IDC_EDIT_HOSTPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HOSTIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SERIALID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SVR)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}


void CDeviceEdit::OnLvnItemchangedListUpgrade(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDeviceEdit::OnLvnColumnclickListUpgrade(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//sort_colum = pNMLV->iSubItem; //点击的列
	//int cout = m_listDevice.GetItemCount();

	//for (int i = 0;i < cout;i++)
	//	m_listDevice.SetItemData(i,i);//每行的关键字，此处为序列号（点击的列号）

	//m_listDevice.SortItems(&MyCompareProc,(DWORD_PTR)&m_listDevice);
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

void CDeviceEdit::OnBnClickedCheckCloud()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_bCloud)
	{
		m_bIP=FALSE;
		GetDlgItem(IDC_EDIT_HOSTPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HOSTIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SERIALID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SVR)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}
