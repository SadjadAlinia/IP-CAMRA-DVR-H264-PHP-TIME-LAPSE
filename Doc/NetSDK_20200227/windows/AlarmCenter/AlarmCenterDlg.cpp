// AlarmCenterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlarmCenter.h"
#include "AlarmCenterDlg.h"
//#include "DumpFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmCenterDlg dialog

CAlarmCenterDlg::CAlarmCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarmCenterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarmCenterDlg)
	m_nPort = 15002;
	m_bStart = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlarmCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmCenterDlg)
	DDX_Control(pDX, IDC_LIST2, m_LogList);
	DDX_Text(pDX, IDC_EDIT1, m_nPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAlarmCenterDlg, CDialog)
	//{{AFX_MSG_MAP(CAlarmCenterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmCenterDlg message handlers
bool __stdcall cbAlarmInfo(long lLoginID, char *pBuf, unsigned long dwBufLen, long dwUser, int nType)
{
	if (nType == ALARM_TYPE)
	{
		if (dwBufLen == sizeof(SDK_NetAlarmCenterMsg))
		{
			CAlarmCenterDlg *pThis = (CAlarmCenterDlg *)dwUser;
			SDK_NetAlarmCenterMsg *pMsg = (SDK_NetAlarmCenterMsg *)pBuf;
			pThis->InsertData(pMsg);
			return true;
		}
	}
	return false;
}

BOOL CAlarmCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	//DeclareDumpFile();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	_CWndCS(this);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	int i = 0;
	m_LogList.InsertColumn(i++,_CS(_T("InfoLog.No")), LVCFMT_LEFT , 40);
	m_LogList.InsertColumn(i++,_CS(_T("Log.Type")), LVCFMT_LEFT , 76);
	m_LogList.InsertColumn(i++,_CS(_T("Log.Date")), LVCFMT_LEFT , 130);
	m_LogList.InsertColumn(i++,_CS(_T("Log.IP")), LVCFMT_LEFT , 100);
	m_LogList.InsertColumn(i++,_CS(_T("Log.SerialID")), LVCFMT_LEFT , 100);
	m_LogList.InsertColumn(i++, _CS(_T("Log.Event")), LVCFMT_LEFT , 80);
	m_LogList.InsertColumn(i++,_CS(_T("Log.nChannel")), LVCFMT_LEFT , 80);
	m_LogList.InsertColumn(i++,_CS(_T("Log.Status")), LVCFMT_LEFT , 80);
	m_LogList.InsertColumn(i++,_CS(_T("Log.Describe")), LVCFMT_LEFT , 270);
	//m_LogList.InsertColumn(i++,_T("Log.Type"), LVCFMT_LEFT , 76);
	//m_LogList.InsertColumn(i++,_T("Log.Date"), LVCFMT_LEFT , 130);
	//m_LogList.InsertColumn(i++,_T("Log.Device"), LVCFMT_LEFT , 100);
	//m_LogList.InsertColumn(i++,_T("Log.User"), LVCFMT_LEFT , 140);
	//m_LogList.InsertColumn(i++,_T("Log.Describe"), LVCFMT_LEFT , 248);	
	DWORD dwStyle = m_LogList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	m_LogList.SetExtendedStyle(dwStyle);
	
	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAlarmCenterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAlarmCenterDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAlarmCenterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAlarmCenterDlg::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	if (false == H264_DVR_StopAlarmCenterListen())
	{
		MessageBox(_CS("Log.StopAlarmCenterFail!"));
		return;
	}
	m_bStart = FALSE;
	GetDlgItem(IDC_BTN_START)->EnableWindow(true);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(false);
}

void CAlarmCenterDlg::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (false == H264_DVR_StartAlarmCenterListen(m_nPort,cbAlarmInfo, (unsigned long)this))
	{
		_CWndCS(this);
		MessageBox(_CS("Log.StartAlarmCenterFail!"));
		return;
	}
	m_bStart = TRUE;
	GetDlgItem(IDC_BTN_START)->EnableWindow(false);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(true);
}

void CAlarmCenterDlg::InsertData(SDK_NetAlarmCenterMsg *pMsg)
{
	if (pMsg)
	{
		int nID = m_LogList.GetItemCount();
		CString strID;
		strID.Format("%d", nID + 1);
		m_LogList.InsertItem(nID, strID);
		m_LogList.SetItemText(nID, 1, pMsg->nType == 0 ? _CS("Alarm") : _CS("Log"));

		CString strTime;
		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", pMsg->Time.year,
			pMsg->Time.month, pMsg->Time.day, pMsg->Time.hour,
			pMsg->Time.minute, pMsg->Time.second);
		m_LogList.SetItemText(nID, 2, strTime);

		CString strIP;
		strIP.Format("%d.%d.%d.%d", pMsg->HostIP.c[0], pMsg->HostIP.c[1], 
			pMsg->HostIP.c[2], pMsg->HostIP.c[3]);
		m_LogList.SetItemText(nID, 3, strIP);
		m_LogList.SetItemText(nID, 4, pMsg->sSerialID);
		char type[100] = "Log.";
		strncat(type,pMsg->sEvent,strlen(pMsg->sEvent));
		m_LogList.SetItemText(nID, 5, _CS(type));
// 		CString strChannel;
// 		strChannel.Format("Channel%d",pMsg->nChannel+1);
		char strChannel[50] = {0};
		sprintf(strChannel,"%s:%d",_CS("Log.Channel"),pMsg->nChannel + 1);
		m_LogList.SetItemText(nID, 6, strChannel);
		if (0 == pMsg->nStatus )
		{
			m_LogList.SetItemText(nID, 7, _CS("Log.Start"));
		}
		else
		{
			m_LogList.SetItemText(nID, 7, _CS("Log.End"));
		}
		
		m_LogList.SetItemText(nID, 8, _CS(pMsg->sDescrip));
	}
}

void CAlarmCenterDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_bStart)
	{
		H264_DVR_StopAlarmCenterListen();
	}
}

void CAlarmCenterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType,cx,cy);
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		Resize();
	}
}

void CAlarmCenterDlg::Resize()
{
	float fsp[2];  
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;      
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	Newp.x=recta.right-recta.left;  
	Newp.y=recta.bottom-recta.top;  
	fsp[0]=(float)Newp.x/old.x;  
	fsp[1]=(float)Newp.y/old.y;  
	CRect Rect;  
	int woc;  
	CPoint OldTLPoint,TLPoint; //���Ͻ�  
	CPoint OldBRPoint,BRPoint; //���½�  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�    
	while(hwndChild)      
	{      
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID  
		GetDlgItem(woc)->GetWindowRect(Rect);    
		ScreenToClient(Rect);    
		OldTLPoint = Rect.TopLeft();    
		TLPoint.x = long(OldTLPoint.x*fsp[0]);    
		TLPoint.y = long(OldTLPoint.y*fsp[1]);    
		OldBRPoint = Rect.BottomRight();    
		BRPoint.x = long(OldBRPoint.x *fsp[0]);    
		BRPoint.y = long(OldBRPoint.y *fsp[1]);    
		Rect.SetRect(TLPoint,BRPoint);    
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
	}  
	old=Newp;  
}