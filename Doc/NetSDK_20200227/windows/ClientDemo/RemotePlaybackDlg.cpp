// RemotePlaybackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "RemotePlaybackDlg.h"
#include "SelectChannelDlg.h"
#include "ClientDemoDlg.h"
#include <vector>
#include <IO.H>
#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CRemotePlaybackDlg::s_sSlowType = 0;
int CRemotePlaybackDlg::s_sFastType = 0;

/////////////////////////////////////////////////////////////////////////////
// CRemotePlaybackDlg dialog
//SDK download position callback
void __stdcall CRemotePlaybackDlg::SDKDownLoadPosCallback ( long lPlayHandle, 
														   long lTotalSize, 
														   long lDownLoadSize, 
														   long dwUser)
{	
	CRemotePlaybackDlg * pDeviceChannel = (CRemotePlaybackDlg*)dwUser;
	
	if ( NULL== pDeviceChannel)
	{
		ASSERT(FALSE);
		
		return;
	}
	
	if ( 0xffffffff == lDownLoadSize )
	{

		//pDeviceChannel->Invalidate(TRUE);	
	//	pDeviceChannel->m_sliderbackplay.SetPos( 0 * 1000 );
	//	pDeviceChannel->KillTimer(2);
		// data callback finished
		//	pDeviceChannel->SetPlayPos(1);
	}
}

int __stdcall 
CRemotePlaybackDlg::NetDataCallBack( long lRealHandle, long dwDataType, unsigned char *pBuffer,long lbufsize,long dwUser)
{	
	BOOL bResult = TRUE;
	try
	{
		//CRemotePlaybackDlg *pDataChnl = (CRemotePlaybackDlg*)dwUser;
		DEC *dec = (DEC*)dwUser;
		if ( 0 == dwDataType )
		{
			if ( (dec->decHandle) >= 0 )
			{
				return H264_PLAY_InputData( dec->decHandle ,pBuffer, lbufsize );
			}	
		}
	}
	catch (...)
	{
		TRACE("\n the input data to decoder is abnormal\n");
	}
	
	// it must return TRUE if decode successfully,or the SDK will consider the decode is failed
	return bResult;
	
}

CFile g_file;
int __stdcall 
CRemotePlaybackDlg::NetDataCallBack_V2(long lRealHandle, const PACKET_INFO_EX *pFrame, unsigned int dwUser)
{	
	BOOL bResult = TRUE;
	try
	{
		//CRemotePlaybackDlg *pDataChnl = (CRemotePlaybackDlg*)dwUser;
		DEC* dec = (DEC*)dwUser;
		if ( dec->decHandle >= 0 )
		{
			bResult = H264_PLAY_InputData( dec->decHandle , (unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
			CString str("c:\\test.jpg");
			
					g_file.Write((unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
			if (bResult)
			{
#if 0
				TRACE("timestamp: %d, year: %d, nMonth: %d, nDay: %d, nHour: %d, nMinute: %d, nSecond: %d\n",
					pFrame->dwTimeStamp, pFrame->nYear, pFrame->nMonth,
					pFrame->nDay, pFrame->nHour, pFrame->nMinute, pFrame->nSecond);
				
				if (pFrame->nPacketType == AUDIO_PACKET)
				{
					fwrite(pFrame->pPacketBuffer, pFrame->dwPacketSize, 1, fp);
					fclose(fp);
				}
			}
			else
			{
				char filename[256];
				static int index = 0;
				static int iFrame = 0;
				
				FILE *fp = NULL;
				if (pFrame->nPacketType == VIDEO_I_FRAME)
				{
					sprintf(filename, "stream/stream_%02d.idx", index++);
					fp = fopen(filename, "ab+");
					if (fp)
					{
						fwrite(pFrame->pPacketBuffer, pFrame->dwPacketSize, 1, fp);
						fclose(fp);
					}
					char filename[256];
					static int index = 0;
					
					sprintf(filename, "stream/Audio%02d.idx", index++);
					fp = fopen(filename, "ab+");
					if (fp)
					{
						fwrite(pFrame->pPacketBuffer, pFrame->dwPacketSize, 1, fp);
						fclose(fp);
					}
				}
				else
				{
					char filename[256];
					static int index = 0;
					static int iFrame = 0;
					
					FILE *fp = NULL;
					if (pFrame->nPacketType == VIDEO_I_FRAME)
					{
						sprintf(filename, "stream/stream_%02d.idx", index++);
						fp = fopen(filename, "ab+");
						if (fp)
						{
							fwrite(pFrame->pPacketBuffer, pFrame->dwPacketSize, 1, fp);
							fclose(fp);
						}
						iFrame = 1;
					}
					
					if (iFrame == 1)
					{
						fp = fopen("stream/StreamTotal.h264", "ab+");
						if (fp)
						{
							fwrite(pFrame->pPacketBuffer, pFrame->dwPacketSize, 1, fp);
							fclose(fp);
						}
					}
				}
#endif
			}
			return bResult;
		}	
	}
	catch (...)
	{
		TRACE("\n the input data to decoder is abnormal\n");
	}
	
	// it must return TRUE if decode successfully,or the SDK will consider the decode is failed
	return bResult;
	
}

CRemotePlaybackDlg::CRemotePlaybackDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRemotePlaybackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemotePlaybackDlg)
	int i = 0;
	m_endData = 0;
	m_begData = 0;
	m_begTime = 0;
	m_endTime = 0;
	m_sliderPos=0;
	m_bDownloadByTime = FALSE;
	m_bDownloadByName = TRUE;
	//}}AFX_DATA_INIT
	
	m_lSaveHandle = -1;
	m_lFileHandle = -1;
	m_hFileThread = NULL;
	m_bPause = FALSE;
	m_nCurRecNum=0;	
	m_mapNum=0;

	m_strPath = "";
	m_strSaveName = "";
	m_TotalChannel = 0;
	for (i =0; i<64;i++)
	{
		m_nPlayHandle[i] = -1;
		m_dec[i] = new DEC;
		m_dec[i]->bOver = false;
	}
	for (i =0 ;i<64; i++)
	{
		m_nPlaybackDecHandle[i] = -1;
	}
	m_SelectChannel = 0;
	memset(&m_SearchByTimeInfo, 0, sizeof(SDK_SearchByTimeResult));
}


void CRemotePlaybackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemotePlaybackDlg)
	DDX_Control(pDX, IDC_SLIDER_BACKPLAY, m_sliderbackplay);
	DDX_Control(pDX, IDC_LIST1, m_fileList);
	DDX_Control(pDX, IDC_STATIC_VIDEO, m_videoWnd);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_cmbDevice);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_endData);
	DDX_DateTimeCtrl(pDX, IDC_DATE_BEGIN, m_begData);
	DDX_DateTimeCtrl(pDX, IDC_TIME_BEGIN, m_begTime);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_endTime);
	DDX_Check(pDX, IDC_CHECK_TIME, m_bDownloadByTime);
	DDX_Check(pDX, IDC_CHECK_NAME, m_bDownloadByName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemotePlaybackDlg, CDialog)
//{{AFX_MSG_MAP(CRemotePlaybackDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, OnSelchangeComboDevice)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_FAST, OnFast)
	ON_BN_CLICKED(IDC_SLOW, OnSlow)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_NAME, OnCheckDownLoadByName)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckDownLoadByTime)
	ON_BN_CLICKED(IDC_PAGE_UP, OnPageUp)
	ON_BN_CLICKED(IDC_PAGE_DOWN, OnPageDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_Audio, OnBUTTONAudio)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BACKPLAY, OnReleasedcaptureSliderBackplay)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, OnNMCustomdrawProgress1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BACKPLAY, OnNMCustomdrawSliderBackplay)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SELECTCHANNLE, OnBnClickedBtnSelectchannle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemotePlaybackDlg message handlers

BOOL CRemotePlaybackDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	_CWndCS(this);
	
	m_fileList.SetExtendedStyle( m_fileList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	GetDlgItem(IDC_PAGE_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAGE_DOWN)->EnableWindow(FALSE);
	
	int nIndex = m_cmbDevice.AddString(_CS("PlayBack.SelectDevice"));
	m_cmbDevice.SetItemData(nIndex, 1);
	int Index = 1;
	
	//get device info
	CClientDemoDlg *pMain = (CClientDemoDlg *)AfxGetApp()->m_pMainWnd;
	if ( pMain )
	{
		Devc_Map *pMap = pMain->GetDeviceMap();
		if ( pMap )
		{
			Devc_Map::iterator bIter;
			for ( bIter = pMap->begin(); bIter != pMap->end(); bIter ++ )
			{
				DEV_INFO *pDev = bIter->second;
				if ( pDev )
				{
					nIndex = m_cmbDevice.AddString(pDev->szDevName);
					m_cmbDevice.SetItemData(nIndex, (DWORD)pDev );
					if(pDev->lLoginID != 0)
					{
						Index = nIndex;
					}
				}
			}
		}
	}	
	if (m_cmbDevice.GetCount() > 1)
	{
		m_cmbDevice.SetCurSel(Index);
	}
	else
	{
		m_cmbDevice.SetCurSel(0);
	}
	OnSelchangeComboDevice();
	
	m_cmbType.AddString(_CS("PlayBack.ALL"));
	m_cmbType.AddString(_CS("PlayBack.AlarmRecordFile"));
	m_cmbType.AddString(_CS("PlayBack.DetectRecordfile"));
	m_cmbType.AddString(_CS("PlayBack.GeneralRecordFile"));
	m_cmbType.AddString(_CS("PlayBack.HandRecordFile"));

	m_cmbType.AddString(_CS("PlayBack.AllPic"));
	m_cmbType.AddString(_CS("PlayBack.AlarmRecordPic"));
	m_cmbType.AddString(_CS("PlayBack.DetectRecordPic"));
	m_cmbType.AddString(_CS("PlayBack.GeneralRecordPic"));
	m_cmbType.AddString(_CS("PlayBack.HandRecordPic"));

	m_cmbType.SetCurSel(0);
	
	CTime cur_time = CTime::GetCurrentTime();
	CTime time = CTime(cur_time.GetYear(),cur_time.GetMonth(), cur_time.GetDay(),
		cur_time.GetHour(),cur_time.GetMinute(),cur_time.GetSecond());
	
	m_begData = m_endData = m_endTime = time;
	
	CTime bTime = CTime( cur_time.GetYear(),cur_time.GetMonth(), cur_time.GetDay(), 0 , 0 , 0 );
	m_begTime = bTime;
	
	UpdateData( FALSE );
	
	m_fileList.InsertColumn(0, _CS("PlayBack.NO"),  LVCFMT_LEFT, 50,-1 );
	m_fileList.InsertColumn(1, _CS("PlayBack.File"), LVCFMT_LEFT,450,-1);
	
	m_progress.SetRange(0,100);
	
	
	m_nGetPage=0;
	m_nCurPage=-1;
	m_bOpenAudio = false;
	m_nTotalPage=-1;
	
	m_sliderbackplay.SetPos(0);	
	
	// EXCEPTION: OCX Property Pages should return FALSE
	GetDlgItem(IDC_SLIDER_BACKPLAY)->EnableWindow(FALSE);
	
	m_TimeReplayerView.Create(IDD_DLG_RECORDPERIOD, this);
	m_TimeReplayerView.ShowWindow(SW_SHOW);
	CRect rectTimeReplayer;
	CRect rectSlider;
	m_sliderbackplay.GetWindowRect(&rectSlider);

	ScreenToClient(&rectSlider);
	

	CRect rtWnd;
	GetClientRect(&rtWnd);

	rectTimeReplayer.left = rectSlider.left;
	rectTimeReplayer.top = rectSlider.bottom;
	rectTimeReplayer.right = rectSlider.right;
	rectTimeReplayer.bottom = rtWnd.bottom;
	
	m_TimeReplayerView.MoveWindow(&rectTimeReplayer);	

	m_playWindowManager = new CPlayWindowManager;
	m_playWindowManager->Create(CPlayWindowManager::IDD,this);
	CRect rt;
	m_videoWnd.GetClientRect(&rt);
	rt.left = rt.left + rt.right/2 + 200;
	rt.top = rt.top + rt.bottom/2 - 105;
	rt.right = rt.left + rt.right;
	rt.bottom = rt.top + rt.bottom;
	m_playWindowManager->MoveWindow(&rt);

	return TRUE;
}

void CRemotePlaybackDlg::OnSelchangeComboDevice() 
{
	int nSelectedIndex = m_cmbDevice.GetCurSel();
	if ( nSelectedIndex <= 0 )
	{
		return;
	}
	
	DEV_INFO *pDev = (DEV_INFO *)m_cmbDevice.GetItemData(nSelectedIndex);
	if ( pDev )
	{
		//��ȡ�豸��ͨ������pDev->nTotalChannel
		m_TotalChannel = pDev->nTotalChannel;
		m_lLoginID = pDev->lLoginID;
	}
}

void CRemotePlaybackDlg::OnSearch() 
{
	if (m_Bitmap.m_hObject)
	{
		DeleteObject(m_Bitmap.m_hObject);
	}
	if(m_SelectChannel < 0)
	{
		MessageBox(_CS("PlayBack.SelectChannel"), _CS("PlayBack.Prompt"));
		return;
	}
	m_nGetPage=0;
	m_nCurPage=-1;
	m_nTotalPage=-1;
	
	UpdateData();
	
	OnStop();  //��ѯ֮ǰ�ȹر����еĲ���

	int nSelectedIndex = m_cmbDevice.GetCurSel();
	if ( nSelectedIndex <= 0)
	{
		MessageBox(_CS("PlayBack.SelectDevice"));
		return;
	}
	
	ClearResult();
	
	ClearMapResult();
	
	unsigned __int64 nChannel = m_SelectChannel;	//��ѡ���ͨ�����и�ֵ
	int nFileType = m_cmbType.GetCurSel();		//file type

	if (m_cmbType.GetCurSel()>=5)
	{
		nFileType += 5;
	}
	
	H264_DVR_TIME StartTime;
	H264_DVR_TIME StopTime;
	
	COleDateTime beginTime(m_begData.GetYear(), m_begData.GetMonth(), m_begData.GetDay(),
		m_begTime.GetHour(), m_begTime.GetMinute(), m_begTime.GetSecond());
	COleDateTime endTime(m_endData.GetYear(), m_endData.GetMonth(), m_endData.GetDay(),
		m_endTime.GetHour(), m_endTime.GetMinute(), m_endTime.GetSecond());
	
	if(beginTime >= endTime)//ʱ�����
	{
		MessageBox(_CS("PlayBack.TimeERROR"));
		return ;
	} 
	
	StartTime.dwYear = (WORD)m_begData.GetYear();
	StartTime.dwMonth = (WORD)m_begData.GetMonth();
	StartTime.dwDay = (WORD)m_begData.GetDay();
	StartTime.dwHour = (char)m_begTime.GetHour();
	StartTime.dwMinute = (char)m_begTime.GetMinute();
	StartTime.dwSecond = (char)m_begTime.GetSecond();
	StopTime.dwYear = (WORD)m_endData.GetYear();
	StopTime.dwMonth = (WORD)m_endData.GetMonth();
	StopTime.dwDay = (WORD)m_endData.GetDay();
	StopTime.dwHour = (char)m_endTime.GetHour();
	StopTime.dwMinute = (char)m_endTime.GetMinute();
	StopTime.dwSecond = (char)m_endTime.GetSecond();
	
	DEV_INFO *pDevTemp=(DEV_INFO*)m_cmbDevice.GetItemData(nSelectedIndex);
	
	H264_DVR_FILE_DATA *szSend=new H264_DVR_FILE_DATA[100];
	
	
	if (pDevTemp)
	{
		m_lLoginID=pDevTemp->lLoginID;
		
		int nMaxLen = 100 * sizeof(H264_DVR_FILE_DATA);
		
		const int WAITTIME = 4000;   //waitting time
		int nNum = 0; //number of files return
		
		GetDlgItem(IDC_SEARCH)->EnableWindow(FALSE);
		H264_DVR_FINDINFO searchInfo;

		int nRet = 0;
		if ( m_bDownloadByTime )
		{
			SDK_SearchByTime searchInfo;
			searchInfo.iSync = FALSE;
			searchInfo.nFileType = nFileType;
			searchInfo.nHighChannel = nChannel >> 32;
			searchInfo.nLowChannel = nChannel & 0xFFFFFFFF;
			searchInfo.stBeginTime.day = StartTime.dwDay;
			searchInfo.stBeginTime.hour = StartTime.dwHour;
			searchInfo.stBeginTime.minute = StartTime.dwMinute;
			searchInfo.stBeginTime.month = StartTime.dwMonth;
			searchInfo.stBeginTime.second = StartTime.dwSecond;
			searchInfo.stBeginTime.year = StartTime.dwYear;
			searchInfo.stEndTime.day = StopTime.dwDay;
			searchInfo.stEndTime.hour = StopTime.dwHour;
			searchInfo.stEndTime.minute = StopTime.dwMinute;
			searchInfo.stEndTime.month = StopTime.dwMonth;
			searchInfo.stEndTime.second = StopTime.dwSecond;
			searchInfo.stEndTime.year = StopTime.dwYear;
			
			nRet = H264_DVR_FindFileByTime(m_lLoginID,&searchInfo, &m_SearchByTimeInfo);

 			if ( nRet > 0 )
 			{
 				m_TimeReplayerView.UpdateTimeSect(m_SearchByTimeInfo.ByTimeInfo[nChannel].cRecordBitMap);
 			}				
			nNum = m_SearchByTimeInfo.nInfoNum;
			int nWndNum = 4;
			if (nNum <= 1)
			{
				nWndNum = 1;
			}
			else if ( nNum <= 4 )
			{
				nWndNum = 4;
			}
			else if ( nNum <= 9 )
			{
				nWndNum = 9;
			}
			else if (nNum <= 16)
			{
				nWndNum = 16;
			}
			else if (nNum <= 25)
			{
				nWndNum = 25;
			}
			else if (nNum <= 35)
			{
				nWndNum = 36;
			}
			else
			{
				nWndNum = 64;
			}
			m_videoWnd.ShowWindow(SW_HIDE);
			m_playWindowManager->ShowWindow(SW_SHOW);
			m_playWindowManager->SetShowWindowNum( nWndNum);
		}
		else
		{
			unsigned char cRecordBitMap[720] = {0};
			m_TimeReplayerView.UpdateTimeSect(cRecordBitMap);
			searchInfo.nChannelN0 = nChannel;
			searchInfo.nFileType = nFileType;
			memcpy( &searchInfo.startTime, &StartTime, sizeof(searchInfo.startTime));
			memcpy( &searchInfo.endTime, &StopTime, sizeof(searchInfo.endTime));

			nRet = H264_DVR_FindFile(m_lLoginID,&searchInfo,szSend,100,&nNum);
			m_videoWnd.ShowWindow(SW_SHOW);
			m_playWindowManager->ShowWindow(SW_HIDE);
		}
		m_nCurRecNum=nNum;
		
		memcpy(&m_mapTemp[m_mapNum++],&searchInfo,sizeof(H264_DVR_FINDINFO));
		
		if ( nRet > 0 && nNum > 0)  //����û��¼��������
		{
			m_nGetPage ++;
			m_nCurPage = 1;
			
			m_pstrFileVector.reserve(m_nCurRecNum);
			
			for(int i=0;i<m_nCurRecNum;i++)
			{
				m_pstrFileVector.push_back(szSend+i);
			} 
			
			m_nSearchInfo.nChannelN0=nChannel;
			m_nSearchInfo.nFileType=nFileType;
			m_nSearchInfo.startTime.dwYear=szSend[m_nCurRecNum-1].stEndTime.year;
			m_nSearchInfo.startTime.dwMonth=szSend[m_nCurRecNum-1].stEndTime.month;
			m_nSearchInfo.startTime.dwDay=szSend[m_nCurRecNum-1].stEndTime.day;
			m_nSearchInfo.startTime.dwHour=szSend[m_nCurRecNum-1].stEndTime.hour;
			m_nSearchInfo.startTime.dwMinute=szSend[m_nCurRecNum-1].stEndTime.minute;
			m_nSearchInfo.startTime.dwSecond=szSend[m_nCurRecNum-1].stEndTime.second;
			memcpy(&m_nSearchInfo.endTime,&searchInfo.endTime,sizeof(m_nSearchInfo.endTime));
			
			if(nNum<PLAYBACK_MAX_FILE_NUM)
			{
				m_nTotalPage=m_nGetPage;
			}
            
			AddFileListInfo(m_nCurRecNum);//add list item
			SetPageBtnState();
		}else
		{
			if (szSend)
			{
				delete szSend;
				szSend = NULL;
			}
			char ch[100]={0};
// 			sprintf(ch,"%s,Error:%d\n",_CS("PlayBack.SearchFail"),
// 				H264_DVR_GetLastError());
			sprintf(ch, "%s,%s\n",_CS("PlayBack.SearchFail"),_CS("PlayBack.NoVideo"));

			MessageBox(ch);
		}
		GetDlgItem(IDC_SEARCH)->EnableWindow(TRUE);
	}
}

void CRemotePlaybackDlg::OnDownload()
{
	UpdateData();
	
	int nSelectedIndex = m_cmbDevice.GetCurSel();
	if ( nSelectedIndex <= 0 )
	{
		MessageBox(_CS("PlayBack.SelectDevice"));
		return;
	}
	
	if (m_bDownloadByTime)
	{
		OnDownloadByTime();
	}
	else
	{
		OnDownloadByName();
	}
}

void CRemotePlaybackDlg::OnDownloadByName(bool bDown) 
{
	UpdateData(TRUE);
	
	if ( m_lSaveHandle > 0 )
	{
		KillTimer(1);
		H264_DVR_StopGetFile(m_lSaveHandle);
		m_lSaveHandle = -1;
		m_progress.SetPos(0);
		
		GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_CS("PlayBack.Download"));
		
		return;
	}
	
	CString csFileName;
	
	int nSelectedIndex = m_fileList.GetSelectionMark();
	if ( nSelectedIndex < 0 )
	{
		MessageBox(_CS("PlayBack.SelectFile"));
		return;
	}
	
	H264_DVR_FILE_DATA *pstrFileInfo = (H264_DVR_FILE_DATA *)m_fileList.GetItemData(nSelectedIndex);
	if (  !pstrFileInfo )
	{
		return;
	}
	
	m_strSaveName = "";
	//m_strSaveName.Format("c:\\record\\record.h264");
	nSelectedIndex = m_cmbDevice.GetCurSel();
	if ( nSelectedIndex <= 0)
	{
		MessageBox(_CS("PlayBack.SelectDevice"));
		return;
	}
    DEV_INFO *pDevTemp=(DEV_INFO*)m_cmbDevice.GetItemData(nSelectedIndex);
	
	CString strInitDir;
	
	strInitDir = GET_MODULE_FILE_INFO.strPath;
	if (strInitDir.Right(1) == '\\') 
	{
		strInitDir += "Download";
	}
	else
	{
		strInitDir += "\\Download";
	}	
	if (-1 == _access(strInitDir, 0)) 
	{
		CreateDirectory(strInitDir, NULL);
	}
	
	CString strFileTpye("h264");
	CString strFileName("");
	if (strstr(pstrFileInfo->sFileName, ".h264"))
	{
		strFileTpye = _T("h264");
	}
	else
	{
		strFileTpye = _T("jpg");
	}
	strFileName.Format("%s_%02d_%04d%02d%02d_%02d%02d%02d.%s", 
		pDevTemp->szDevName, pstrFileInfo->ch + 1, 
		pstrFileInfo->stBeginTime.year, pstrFileInfo->stBeginTime.month,
		pstrFileInfo->stBeginTime.day, pstrFileInfo->stBeginTime.hour,
		pstrFileInfo->stBeginTime.minute, pstrFileInfo->stBeginTime.second, strFileTpye);
	
	if (!bDown)
	{
		CFileDialog dlg( FALSE , strFileTpye, strFileName , OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,
			"All File(*.h264;*.jpg)|*.*||" , this );
		dlg.m_ofn.lpstrInitialDir = strInitDir;		//Ĭ��·��
		if( dlg.DoModal() == IDOK )
		{
			m_strSaveName = dlg.GetPathName();
			pstrFileInfo->stBeginTime.second =33;
			pstrFileInfo->stEndTime.second = 59;
			//m_lSaveHandle = H264_DVR_GetFileByName(m_lLoginID, pstrFileInfo,m_strSaveName.GetBuffer(0),SDKDownLoadPosCallback,(DWORD)this);
			m_lSaveHandle = H264_DVR_GetFileByName(m_lLoginID, pstrFileInfo, m_strSaveName.GetBuffer(0));
			if(m_lSaveHandle >= 0)
			{
				m_progress.SetPos(0);
				GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_CS("PlayBack.Stop"));
				//update the progress control
				SetTimer(1,0,NULL);
			}
			else
			{
				CString sTemp;
				sTemp.Format("dowload fail ERROR = %d",H264_DVR_GetLastError());
				AfxMessageBox(sTemp);
				if (H264_DVR_GetLastError() < 0)
				{
					AfxMessageBox(_CS("PlayBack.DownloadErro"));
				}
			}
		}
	}else
	{
		m_strPath ="c:\\temp.jpg";
		m_lSaveHandle = H264_DVR_GetFileByName(m_lLoginID, pstrFileInfo,(char*)m_strPath);
		if(m_lSaveHandle >= 0)
		{
			m_progress.SetPos(0);
			GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_CS("PlayBack.Stop"));
			//update the progress control
			SetTimer(1,0,NULL);
		}
		else
		{
			CString sTemp;
			sTemp.Format("dowload fail ERROR = %d",H264_DVR_GetLastError());
			//AfxMessageBox(sTemp);
			if(H264_DVR_GetLastError < 0 )
			{
				AfxMessageBox(_CS("PlayBack.DownloadErro"));
			}
		}
	}
	

}

void CRemotePlaybackDlg::OnDownloadByTime() 
{
	UpdateData(TRUE);
	
	if ( m_lSaveHandle > 0 )
	{
		KillTimer(1);
		H264_DVR_StopGetFile(m_lSaveHandle);
		m_lSaveHandle = -1;
		m_progress.SetPos(0);
		
		GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_CS("PlayBack.Download"));
		return;
	}
	
	CString strSaveName("");
	//strSaveName.Format("c:\\record");
	strSaveName = BrowseForFolder(GetSafeHwnd()) ;
	
	if ( strSaveName == _T(""))  //δѡ��·��ʱ�˳�
	{
		return;
	}
	H264_DVR_FINDINFO info;
	
	memset(&info, 0, sizeof(info));
	info.nChannelN0 = m_SelectChannel - 1;	//channel No.��ʱ�����أ�ͨ����ֵ
	info.nFileType = m_cmbType.GetCurSel();		//file type
	if (m_cmbType.GetCurSel()>=5)
	{
		info.nFileType += 5;
	}
	info.startTime.dwYear = (WORD)m_begData.GetYear();
	info.startTime.dwMonth = (WORD)m_begData.GetMonth();
	info.startTime.dwDay = (WORD)m_begData.GetDay();
	info.startTime.dwHour = (char)m_begTime.GetHour();
	info.startTime.dwMinute = (char)m_begTime.GetMinute();
	info.startTime.dwSecond = (char)m_begTime.GetSecond();
	info.endTime.dwYear = (WORD)m_endData.GetYear();
	info.endTime.dwMonth = (WORD)m_endData.GetMonth();
	info.endTime.dwDay = (WORD)m_endData.GetDay();
	info.endTime.dwHour = (char)m_endTime.GetHour();
	info.endTime.dwMinute = (char)m_endTime.GetMinute();
	info.endTime.dwSecond = (char)m_endTime.GetSecond();
	int nRecordFileType = m_cmbType.GetCurSel();
	info.nFileType = (nRecordFileType <= SDK_RECORD_MANUAL) ?  nRecordFileType : (SDK_PIC_ALL + nRecordFileType - SDK_RECORD_MANUAL - 1);
	
	m_lSaveHandle = H264_DVR_GetFileByTime(m_lLoginID, &info, strSaveName.GetBuffer(0));
	if(m_lSaveHandle > 0)
	{
		m_progress.SetPos(0);
		
		GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_CS("PlayBack.Stop"));
		
		//update the progress control
		SetTimer(1,0,NULL);
	}
	else
	{
		CString sTemp;
		sTemp.Format("dowload fail ERROR = %d",H264_DVR_GetLastError());
		AfxMessageBox(sTemp);
		if (H264_DVR_GetLastError() < 0)
		{
			AfxMessageBox(_CS("PlayBack.DownloadError"));
		}
	}
}

void CRemotePlaybackDlg::OnPlay() 
{
	UpdateData();
	
	s_sSlowType = 0;
	s_sFastType = 0;
	GetDlgItem(IDC_SLOW)->SetWindowText(_CS("PlayBack.slow"));
	H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_SLOW, s_sSlowType);
	GetDlgItem(IDC_FAST)->SetWindowText(_CS("PlayBack.Fast"));
	H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_FAST, s_sFastType);

	int nSelectedIndex = m_cmbDevice.GetCurSel();
	if ( nSelectedIndex <= 0 )
	{
		MessageBox(_CS("PlayBack.SelectDevice"));
		return;
	}
	
	if (m_bDownloadByTime)
	{
		
		OnPlayByTime(NULL);
	}
	else
	{
		if (m_cmbType.GetCurSel() < 5)
		{
			OnPlayByName(NULL);
		}
		else
		{
			OnPlayPic();
		}	
	}
	GetDlgItem(IDC_SLIDER_BACKPLAY)->EnableWindow(TRUE);
}

bool static GetBitmapFromPicture(LPCTSTR strFilePath, CBitmap* lpBitmap)
{
	//open file
	HANDLE hFile  =  CreateFile(strFilePath, GENERIC_READ, 0, NULL,
													OPEN_EXISTING, 0 , NULL);
	_ASSERTE(INVALID_HANDLE_VALUE   !=   hFile);   
	
	//   get   file   size   
	DWORD   dwFileSize   =   GetFileSize(hFile,   NULL);   
	_ASSERTE(-1   !=   dwFileSize);   
	
	LPVOID   pvData   =   NULL;   
	//   alloc   memory   based   on   file   size   
	HGLOBAL   hGlobal   =   GlobalAlloc(GMEM_MOVEABLE,   dwFileSize);   
	_ASSERTE(NULL   !=   hGlobal);   
	
	pvData   =   GlobalLock(hGlobal);   
	_ASSERTE(NULL   !=   pvData);   
	
	DWORD   dwBytesRead   =   0;   
	//   read   file   and   store   in   global   memory   
	BOOL   bRead   =   ReadFile(hFile,   pvData,   dwFileSize,   &dwBytesRead,   NULL);   
	_ASSERTE(FALSE   !=   bRead);   
	GlobalUnlock(hGlobal);   
	CloseHandle(hFile);   
	
	LPSTREAM   pstm   =   NULL;   
	//   create   IStream*   from   global   memory   
	HRESULT   hr   =   CreateStreamOnHGlobal(hGlobal,   TRUE,   &pstm);   
	_ASSERTE(SUCCEEDED(hr)   &&   pstm);   
	if ( pstm == NULL )
		return FALSE;

	//   Create   IPicture   from   image   file   
	LPPICTURE   gpPicture;   
	
	hr   =   ::OleLoadPicture(pstm,   dwFileSize,   FALSE,   IID_IPicture,   (LPVOID   *)&gpPicture);   
	if ( gpPicture == NULL )
		return FALSE;

	_ASSERTE(SUCCEEDED(hr)   &&   gpPicture);   
	pstm->Release();   
	
	OLE_HANDLE   m_picHandle;   
	/*   
	long   hmWidth,   hmHeight;   
	gpPicture->get_Width(&hmWidth);   
	gpPicture->get_Height(&hmHeight);   
	int   nWidth =   MulDiv(hmWidth,   GetDeviceCaps(hdc,   LOGPIXELSX),   HIMETRIC_INCH);   
	int   nHeight =   MulDiv(hmHeight,   GetDeviceCaps(hdc,   LOGPIXELSY),   HIMETRIC_INCH);   
	*/   
	gpPicture->get_Handle(&m_picHandle); 
	if (lpBitmap->m_hObject)
	{
		lpBitmap->DeleteObject();  
	}
	
	 
	lpBitmap->Attach((HGDIOBJ)m_picHandle);    

	return TRUE;

}
void CRemotePlaybackDlg::OnPlayPic()
{
	if (m_Bitmap.m_hObject)
	{
		DeleteObject(m_Bitmap.m_hObject);
	}
	OnDownloadByName(true);
}

void  CRemotePlaybackDlg::OnPlayByTime(char* sTime, bool bAgain) 
{
	for (int i=0; i<m_SearchByTimeInfo.nInfoNum; ++i)
	{
		if(m_nPlayHandle[i] == -1)
		{
			H264_DVR_FINDINFO info;
			
			memset(&info, 0, sizeof(info));
			info.nChannelN0 = m_SearchByTimeInfo.ByTimeInfo[i].iChannel;	//channel No.��ʱ�䲥��
			info.nFileType = m_cmbType.GetCurSel();		//file type
			info.startTime.dwYear = (WORD)m_begData.GetYear();
			info.startTime.dwMonth = (WORD)m_begData.GetMonth();
			info.startTime.dwDay = (WORD)m_begData.GetDay();
			info.endTime.dwYear = (WORD)m_endData.GetYear();
			info.endTime.dwMonth = (WORD)m_endData.GetMonth();
			info.endTime.dwDay = (WORD)m_endData.GetDay();
			if (bAgain)//��һ�λط�
			{
				info.startTime.dwHour = (char)m_begTime.GetHour();
				info.startTime.dwMinute = (char)m_begTime.GetMinute();
				info.startTime.dwSecond = (char)m_begTime.GetSecond();

				
			}else
			{
				if (sTime)
				{
					SDK_SYSTEM_TIME* s1 = (SDK_SYSTEM_TIME*)sTime;

					info.startTime.dwHour = s1->hour;
					info.startTime.dwMinute = s1->minute;
					info.startTime.dwSecond = s1->second;
				}
			}
			info.endTime.dwHour = (char)m_endTime.GetHour();
			info.endTime.dwMinute = (char)m_endTime.GetMinute();
			info.endTime.dwSecond = (char)m_endTime.GetSecond();
			
			//open decode channel
			/*H264_PLAY_GetPort(&m_nPlaybackDecHandle[i]);
			H264_PLAY_SetDisplayRegion(m_nPlaybackDecHandle[i],0,NULL,NULL,FALSE);

			BYTE byFileHeadBuf;
			if (H264_PLAY_OpenStream(m_nPlaybackDecHandle[i], &byFileHeadBuf, 1, SOURCE_BUF_MIN*50))
			{	
				H264_PLAY_SetStreamOpenMode(m_nPlaybackDecHandle[i], STREAME_FILE);	
				H264_PLAY_Play(m_nPlaybackDecHandle[i], m_playWindowManager->m_vdWnd[i].m_hWnd);

			}	*/
			
	 				/*m_nPlayHandle = H264_DVR_PlayBackByTime_V2(m_lLoginID, info.nChannelN0, 
	 				 			&info.startTime, &info.endTime,
	 				 			NetDataCallBack, (DWORD)this, SDKDownLoadPosCallback, (DWORD)this);*/
			m_dec[i]->pthis=(long)this;
			m_dec[i]->decHandle = m_nPlaybackDecHandle[i];
			/*m_nPlayHandle[i] = H264_DVR_PlayBackByTimeEx(m_lLoginID, &info, 
				NetDataCallBack, (DWORD)m_dec[i], SDKDownLoadPosCallback, (DWORD)m_dec[i]);*/
			info.hWnd = (void*)(m_playWindowManager->m_vdWnd[i].m_hWnd);
			m_nPlayHandle[i] = H264_DVR_PlayBackByTime(m_lLoginID, &info, 
				NULL, NULL, (DWORD)m_dec[i]);

			if(m_nPlayHandle[i] <= 0 )
			{
				AfxMessageBox(_CS("PlayBack.Fail"));
				m_nPlayHandle[i] = -1;
				return;
			}
		}
		else
		{
			//if in pause, normal it
			if (m_bPause)
			{
				H264_DVR_PlayBackControl(m_nPlayHandle[i], SDK_PLAY_BACK_CONTINUE, 0 );
				H264_PLAY_Pause(m_nPlaybackDecHandle[i], !m_bPause);
				m_bPause = !m_bPause;
			}
			//set play speed to normal
			H264_DVR_PlayBackControl(m_nPlayHandle[i], SDK_PLAY_BACK_FAST, 0);
			//set decode speed to normal
			H264_PLAY_Play(m_nPlaybackDecHandle[i], m_playWindowManager->m_vdWnd[i].m_hWnd);
			s_sFastType = 0;
			s_sSlowType = 0;
		}
	}
}
void CRemotePlaybackDlg::OnPlayByName(char* sTime , bool bAgain) 
{
	if(m_nPlayHandle[0] == -1)
	{
		CString csFileName;
		
		int nSelectedIndex = m_fileList.GetSelectionMark();
		if ( nSelectedIndex < 0 )
		{
			MessageBox(_CS("PlayBack.SelectFile"));
			return;
		}
		
		H264_DVR_FILE_DATA *pstrFileInfo = (H264_DVR_FILE_DATA *)m_fileList.GetItemData(nSelectedIndex);
 		if (  !pstrFileInfo )
 		{
 			return;
 		}
		if (bAgain)//��һ�λط�
		{
			pstrFileInfo->stBeginTime.hour = (char)m_begTime.GetHour();
			pstrFileInfo->stBeginTime.minute = (char)m_begTime.GetMinute();
			pstrFileInfo->stBeginTime.second = (char)m_begTime.GetSecond();


		}else
		{
			if (sTime)
			{
				SDK_SYSTEM_TIME* s1 = (SDK_SYSTEM_TIME*)sTime;

				pstrFileInfo->stBeginTime.hour = s1->hour;
				pstrFileInfo->stBeginTime.minute = s1->minute;
				pstrFileInfo->stBeginTime.second = s1->second;
			}
		}
		int nitem=m_fileList.GetSelectionMark();
		pstrFileInfo->stEndTime.hour = m_pstrFileVector[nitem]->stEndTime.hour;//(char)m_endTime.GetHour();
		pstrFileInfo->stEndTime.minute = m_pstrFileVector[nitem]->stEndTime.minute;//(char)m_endTime.GetMinute();
		pstrFileInfo->stEndTime.second =m_pstrFileVector[nitem]->stEndTime.second; //(char)m_endTime.GetSecond();
		
		//open decode channel
		if ( m_nPlaybackDecHandle[0] == -1 )
		{
			//H264_PLAY_GetPort(&m_nPlaybackDecHandle[0]);
			//H264_PLAY_SetDisplayRegion(m_nPlaybackDecHandle[0],0,NULL,NULL,FALSE);
			//BYTE byFileHeadBuf;
			//if (H264_PLAY_OpenStream(m_nPlaybackDecHandle[0], &byFileHeadBuf, 1, SOURCE_BUF_MIN*50))
			//{	
			//	H264_PLAY_SetStreamOpenMode(m_nPlaybackDecHandle[0], STREAME_FILE);	
			//	H264_PLAY_Play(m_nPlaybackDecHandle[0], m_videoWnd.m_hWnd);
			//	
			//	//m_nPlaybackDecHandle[0] = 40;
			//}	
		}
		/*if ( m_nPlaybackDecHandle[0] < 0 )
		{
			return;
		}*/
		m_dec[0]->pthis = (long)this;
		m_dec[0]->decHandle = m_nPlaybackDecHandle[0];
		pstrFileInfo->hWnd = (void*)m_videoWnd.m_hWnd;
		m_nPlayHandle[0] = H264_DVR_PlayBackByName(m_lLoginID, pstrFileInfo, NULL/*SDKDownLoadPosCallback*/, NULL/*NetDataCallBack*/, (DWORD)m_dec[0] );
		//m_nPlayHandle[0] = H264_DVR_PlayBackByName_V2(m_lLoginID, pstrFileInfo, SDKDownLoadPosCallback, NetDataCallBack_V2, (DWORD)this );
		if(m_nPlayHandle[0] <= 0 )
		{
			AfxMessageBox(_CS("Playback fail"));
			return;
		}
		CString str("c:\\AudioTest.h264");
		g_file.Open(str, CFile::modeCreate | CFile::modeReadWrite);
	}
	else
	{
		//if in pause, normal it
		if (m_bPause)
		{
			H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_CONTINUE, 0 );
			H264_PLAY_Pause(m_nPlaybackDecHandle[0], !m_bPause);
			m_bPause = !m_bPause;
		}
		//set play speed to normal
		H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_FAST, 0);
		//set decode speed to normal
		H264_PLAY_Play(m_nPlaybackDecHandle[0], m_videoWnd.m_hWnd);
		s_sFastType = 0;
		s_sSlowType = 0;
	}
	SetTimer(2,1000,NULL);
}

void CRemotePlaybackDlg::OnStop() 
{
	KillTimer(2);
	for(int i=0; i<64; ++i)
	{
		if(m_nPlayHandle[i] >= 0)
		{
			H264_DVR_StopPlayBack(m_nPlayHandle[i]);
			m_nPlayHandle[i] = -1;
		}
		if ( m_nPlaybackDecHandle[i])
		{
			H264_PLAY_Stop(m_nPlaybackDecHandle[i]);
			H264_PLAY_CloseStream(m_nPlaybackDecHandle[i]);
			m_nPlaybackDecHandle[i] = -1;
		}
	}
	m_sliderbackplay.SetPos( 0 * 1000 );
 	m_bPause=FALSE;
	m_bOpenAudio=false;
	SetDlgItemText(IDC_BUTTON_Audio,_CS("PlayBack.OpenAudio"));
	Invalidate();

	GetDlgItem(IDC_SLIDER_BACKPLAY)->EnableWindow(FALSE);
	UpdateWindow();
}

void CRemotePlaybackDlg::OnFast() 
{
	s_sSlowType = 0;
	GetDlgItem(IDC_SLOW)->SetWindowText(_CS("PlayBack.slow"));
	H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_SLOW, s_sSlowType);
	if (++s_sFastType > 4)
	{
		s_sFastType = 1;
	}
	CString temp;
	temp.Format("%d",s_sFastType);
	GetDlgItem(IDC_FAST)->SetWindowText(_CS("PlayBack.Fast") + _CS(temp));
	H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_FAST, s_sFastType);
	H264_PLAY_Fast(m_nPlaybackDecHandle[0]);
}

void CRemotePlaybackDlg::OnSlow() 
{
	s_sFastType = 0;
	GetDlgItem(IDC_FAST)->SetWindowText(_CS("PlayBack.Fast"));
	H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_FAST, s_sFastType);
	if (++s_sSlowType > 4)
	{
		s_sSlowType = 1;
	}
	CString temp;
	temp.Format("%d",s_sSlowType);
	GetDlgItem(IDC_SLOW)->SetWindowText(_CS("PlayBack.Slow") + _CS(temp));
	H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_SLOW, s_sSlowType);
	H264_PLAY_Slow(m_nPlaybackDecHandle[0]);
}

void CRemotePlaybackDlg::OnPause() 
{
	if (!m_bPause)
		H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_PAUSE, 0 );
	else
		H264_DVR_PlayBackControl(m_nPlayHandle[0], SDK_PLAY_BACK_CONTINUE, 0 );
	H264_PLAY_Pause(m_nPlaybackDecHandle[0], !m_bPause);
	m_bPause = !m_bPause;
}

void CRemotePlaybackDlg::OnDestroy() 
{
	for (int i =0; i<64;i++)
	{
		delete m_dec[i];
		m_dec[i]=NULL;
		if (m_nPlayHandle[i])
		{
			H264_DVR_StopPlayBack(m_nPlayHandle[i]);
			m_nPlayHandle[i]=-1;
		}
	}
	//vector<H264_DVR_FILE_DATA*>::iterator p=m_pstrFileVector.begin();
// 	while( p!=m_pstrFileVector.end())
// 	{
// 		H264_DVR_FILE_DATA* pData = (*p);
// 		if(pData)
// 			delete pData;
// 		pData = NULL;
// 		p++;		
// 	}
	vector<H264_DVR_FILE_DATA*>::iterator p=m_pstrFileVector.begin();
	if ( p!=m_pstrFileVector.end())
	{
		H264_DVR_FILE_DATA* pData = (*p);
		delete pData;
		pData = NULL;
	}

	ClearResult();
	ClearMapResult();
	KillTimer(1);
	OnStop();
	if (m_playWindowManager)
	{
		delete m_playWindowManager;
		m_playWindowManager=NULL;
	}
	g_file.Close();

	CDialog::OnDestroy();
}

void CRemotePlaybackDlg::ClearResult()
{
	m_fileList.DeleteAllItems();
	
	int nTotal = m_TotalChannel;	//��ȡͨ������
	
	for ( int i = 0; i < nTotal; i ++ )
	{
		/*DEV_INFO *pInfo = (DEV_INFO *)m_cmbChannel.GetItemData(i);
		if ( pInfo )
		{
			delete pInfo;
			pInfo = NULL;
		}*/
	}
	m_pstrFileVector.clear();
}


void CRemotePlaybackDlg::OnTimer(ONTIMER_UINT nIDEvent) 
{	
	if(nIDEvent==2)
	{	
		if(m_nPlayHandle[0] == -1)
			exit;
		float pos = H264_DVR_GetPlayPos(m_nPlayHandle[0]);
		SetSliderPos(pos);
		if(pos == 1)
			KillTimer(2);
		DWORD playtime=H264_PLAY_GetPlayedTime(40);
		
		if ( m_preTime > 0 )
		{
			DWORD dwTime = GetTickCount();
			if ( (dwTime - m_preTime) >= 2000 )
			{
				m_sliderbackplay.SetPos( playtime );
				
				m_preTime = 0;
			}
		}
		else
		{
			m_sliderbackplay.SetPos( playtime);
		}
		CDialog::OnTimer(nIDEvent);
	}
	if( m_lSaveHandle > 0 )
	{
		int nPos = H264_DVR_GetDownloadPos(m_lSaveHandle);
		if(nPos < 0)		//fail
		{
			H264_DVR_StopGetFile(m_lSaveHandle);
			m_progress.SetPos(0);
			m_lSaveHandle = -1;
			GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_CS("PlayBack.Download"));
			AfxMessageBox("get download process fail");
		}
		else if(nPos == 100)		//download end
		{
			H264_DVR_StopGetFile(m_lSaveHandle);
			m_lSaveHandle = -1;
			KillTimer(1);
			m_progress.SetPos(0);
			GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_CS("PlayBack.Download"));
			if (!m_bDown)
			{
					AfxMessageBox(_CS("PlayBack.DownloadFinish"));
			}else
			{
					GetBitmapFromPicture(m_strPath, &m_Bitmap);
					RedrawWindow();
			}

			//exit;
		}
		else if(nPos > 100)		//abnormal
		{
			H264_DVR_StopGetFile(m_lSaveHandle);
			m_lSaveHandle = -1;
			m_progress.SetPos(0);
			GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_CS("PlayBack.Download"));
			AfxMessageBox(_CS("PlayBack.DownloadError"));
		}
		else
		{
			m_progress.SetPos(nPos);
		}
	}
	//KillTimer(1);
	//UpdateProgressCtrl(m_dwDownSize,m_dwTotalSize);
	CDialog::OnTimer(nIDEvent);
}

void CRemotePlaybackDlg::OnCheckDownLoadByName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_bDownloadByName)
	{
		m_bDownloadByTime = FALSE;
	}
	UpdateData(FALSE);
}

void CRemotePlaybackDlg::OnCheckDownLoadByTime() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_bDownloadByTime)
	{
		m_bDownloadByName = FALSE;
	}
	UpdateData(FALSE);
}

void CRemotePlaybackDlg::OnPageUp() 
{
	
	// TODO: Add your control notification handler code here	
	if ( m_nGetPage <= 1 || m_nCurPage <= 1)
	{
		return;
	}
	
	m_nCurPage --;
	m_nGetPage --;
	
	ClearResult();
	
	m_mapNum=m_mapNum-1;
	
	if(GetNextPageInfo(&m_mapTemp[--m_mapNum]))
	{
		H264_DVR_FILE_DATA *szSend = new H264_DVR_FILE_DATA[100];
		
		int nNum=0;
		
		int nRet=H264_DVR_FindFile(m_lLoginID,&m_mapTemp[m_mapNum],szSend,100,&nNum);
		
		m_nCurRecNum=nNum;
		
		if(nRet > 0 && nNum > 0) //����û��¼������
		{
			m_pstrFileVector.reserve(m_nCurRecNum);
			
			for ( int i = 0; i < m_nCurRecNum; i ++)
			{
				m_pstrFileVector.push_back(szSend+i);
			}
			m_nSearchInfo.nChannelN0=m_nSearchInfo.nChannelN0;
			m_nSearchInfo.nFileType=m_nSearchInfo.nFileType;
			m_nSearchInfo.startTime.dwYear=szSend[m_nCurRecNum-1].stEndTime.year;
			m_nSearchInfo.startTime.dwMonth=szSend[m_nCurRecNum-1].stEndTime.month;
			m_nSearchInfo.startTime.dwDay=szSend[m_nCurRecNum-1].stEndTime.day;
			m_nSearchInfo.startTime.dwHour=szSend[m_nCurRecNum-1].stEndTime.hour;
			m_nSearchInfo.startTime.dwMinute=szSend[m_nCurRecNum-1].stEndTime.minute;
			m_nSearchInfo.startTime.dwSecond=szSend[m_nCurRecNum-1].stEndTime.second;
			memcpy(&m_nSearchInfo.endTime,&m_mapTemp[m_mapNum].endTime,sizeof(m_nSearchInfo.endTime));
			
			m_mapNum++;
		}else
		{
			if (szSend)
			{
				delete szSend;
				szSend = NULL;
			}
		}
		AddFileListInfo(m_nCurRecNum);
		SetPageBtnState();
	}
}

void CRemotePlaybackDlg::OnPageDown() 
{
	
	// TODO: Add your control notification handler code here
	if ( m_nGetPage <= 0 )
	{
		return;
	}
	
	if((m_nTotalPage!=-1)&&(m_nCurPage == m_nTotalPage))
	{
		SetPageBtnState();
		return;
	}
	
	ClearResult();
	
	if ( GetNextPageInfo(&m_nSearchInfo)&&(m_nCurPage == m_nGetPage))
	{
		H264_DVR_FILE_DATA *szSend = new H264_DVR_FILE_DATA[100];
		
		int nNum=0;
		
		int nRet=H264_DVR_FindFile(m_lLoginID,&m_nSearchInfo,szSend,100,&nNum);
		
		m_nCurRecNum=nNum;
		
		if (nRet> 0 && nNum > 0)  //����û��¼������
		{
			m_nGetPage ++;
			
			memcpy(&m_mapTemp[m_mapNum++],&m_nSearchInfo,sizeof(H264_DVR_FINDINFO));
			
			m_pstrFileVector.reserve(m_nCurRecNum);
			
			for ( int i = 0; i < m_nCurRecNum; i ++ )
			{
				m_pstrFileVector.push_back(szSend+i);
			}
			
			m_nSearchInfo.nChannelN0=m_nSearchInfo.nChannelN0;
			m_nSearchInfo.nFileType=m_nSearchInfo.nFileType;
			m_nSearchInfo.startTime.dwYear=szSend[m_nCurRecNum-1].stEndTime.year;
			m_nSearchInfo.startTime.dwMonth=szSend[m_nCurRecNum-1].stEndTime.month;
			m_nSearchInfo.startTime.dwDay=szSend[m_nCurRecNum-1].stEndTime.day;
			m_nSearchInfo.startTime.dwHour=szSend[m_nCurRecNum-1].stEndTime.hour;
			m_nSearchInfo.startTime.dwMinute=szSend[m_nCurRecNum-1].stEndTime.minute;
			m_nSearchInfo.startTime.dwSecond=szSend[m_nCurRecNum-1].stEndTime.second;
			memcpy(&m_nSearchInfo.endTime,&m_nSearchInfo.endTime,sizeof(m_nSearchInfo.endTime));
			
			if ( m_nCurRecNum <PLAYBACK_MAX_FILE_NUM)
			{
				m_nTotalPage = m_nGetPage;
			}
			
			m_nCurPage ++;
			
			AddFileListInfo(m_nCurRecNum);
			
			SetPageBtnState();
		}
		else
		{
			if (szSend)
			{
				delete szSend;
				szSend = NULL;
			}
		}
	}
}
void CRemotePlaybackDlg::AddFileListInfo(int iNum)
{
	if ( m_bDownloadByTime )
	{
		iNum = 1;
		m_nCurPage=1;
	    m_nTotalPage=1;
	}
	
	for ( int i = 0; i < iNum ; i++)
	{
		CString tempstring;
		tempstring.Format("%d", i + 1);
		m_fileList.InsertItem(i,tempstring);
		
		CString strBtme,strEtime;
		if ( m_bDownloadByTime )
		{
			strBtme.Format("%04d-%02d-%02d-%02d:%02d:%02d",m_begData.GetYear(), 
				m_begData.GetMonth(),
				m_begData.GetDay(),
				m_begTime.GetHour(),
				m_begTime.GetMinute(),
				m_begTime.GetSecond());
			
			strEtime.Format("%04d-%02d-%02d-%02d:%02d:%02d",m_endData.GetYear(), 
				m_endData.GetMonth(),
				m_endData.GetDay(),
				m_endTime.GetHour(),
				m_endTime.GetMinute(),
				m_endTime.GetSecond());
			
			tempstring.Format("%s-%s", strBtme, strEtime);
		}else
		{
			
			strBtme.Format("%04d-%02d-%02d-%02d:%02d:%02d",m_pstrFileVector[i]->stBeginTime.year, 
				m_pstrFileVector[i]->stBeginTime.month,
				m_pstrFileVector[i]->stBeginTime.day,
				m_pstrFileVector[i]->stBeginTime.hour,
				m_pstrFileVector[i]->stBeginTime.minute,
				m_pstrFileVector[i]->stBeginTime.second);
			
			strEtime.Format("%04d-%02d-%02d-%02d:%02d:%02d",
				m_pstrFileVector[i]->stEndTime.year, 
				m_pstrFileVector[i]->stEndTime.month,
				m_pstrFileVector[i]->stEndTime.day,
				m_pstrFileVector[i]->stEndTime.hour,
				m_pstrFileVector[i]->stEndTime.minute,
				m_pstrFileVector[i]->stEndTime.second);
			
			tempstring.Format("%s-%s(%dKB)", strBtme, strEtime, m_pstrFileVector[i]->size);
		}
		
		m_fileList.SetItemText(i, 1, tempstring);
		
		DWORD addr=(DWORD)(m_pstrFileVector[i]);
		
		m_fileList.SetItemData(i,addr);
	}
}

void CRemotePlaybackDlg::SetPageBtnState()
{
	if ( m_nTotalPage == -1 || m_nGetPage < m_nTotalPage)
	{
		GetDlgItem(IDC_PAGE_DOWN)->EnableWindow(TRUE);
	}
	else
	{
		if ( m_nCurPage < m_nGetPage )
		{
			GetDlgItem(IDC_PAGE_DOWN)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_PAGE_DOWN)->EnableWindow(FALSE);
		}
	}
	
	if ( m_nCurPage > 1 )
	{
		GetDlgItem(IDC_PAGE_UP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_PAGE_UP)->EnableWindow(FALSE);
	}
}


int CRemotePlaybackDlg::GetNextPageInfo(H264_DVR_FINDINFO* searchInfo)
{
	H264_DVR_FILE_DATA *szSend = new H264_DVR_FILE_DATA[100];
	int nNum=0;
	
	if (!searchInfo)
	{
		return 0;
	}
	
	int ret = H264_DVR_FindFile(m_lLoginID,searchInfo,szSend,100,&nNum);
	
	delete []szSend;
	
	return ( ret > 0 );
}

void CRemotePlaybackDlg::ClearMapResult()
{
	m_mapTemp.clear();
	m_mapNum=0;
}
void CRemotePlaybackDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	SetDlgItemText(IDC_BUTTON_Audio,_CS("PlayBack.OpenAudio"));
	m_bOpenAudio=false;
	*pResult = 0;
	for(int i=0;i< 64;i++)
	{
		if (m_nPlaybackDecHandle[i] >= 0)
		{
			H264_PLAY_CloseStream(m_nPlaybackDecHandle[i]);
			H264_PLAY_Stop(m_nPlaybackDecHandle[i]);
			H264_PLAY_ResetSourceBuffer(m_nPlaybackDecHandle[i]);
			H264_PLAY_FreePort(m_nPlaybackDecHandle[i]);
			m_nPlaybackDecHandle[i] = -1;
		}
		if(m_nPlayHandle[i] > 0)
		{
			H264_DVR_StopPlayBack(m_nPlayHandle[i]);
			m_nPlayHandle[i] = -1;
		}
		/*if (!m_bDownloadByName)
		{
			m_playWindowManager->m_vdWnd[i].RedrawWindow();
		}*/
	}
	Invalidate();
	if (m_cmbType.GetCurSel() >= 5)
	{
		m_bDown = true;
	}else
	{
		m_bDown = false;
	}
	OnPlay();
	//�õ�¼����ʱ�䣬��λ/s
	int nitem=m_fileList.GetSelectionMark();//
	//��ֹ�û����������Ӷ����³������
	if(nitem < 0)
	{
		return ;
	}
	int byear=m_pstrFileVector[nitem]->stBeginTime.year;
	int bmonth=m_pstrFileVector[nitem]->stBeginTime.month;
	int bday=m_pstrFileVector[nitem]->stBeginTime.day;
	int bhour=m_pstrFileVector[nitem]->stBeginTime.hour;
	int bminute=m_pstrFileVector[nitem]->stBeginTime.minute;
	int bsecond=m_pstrFileVector[nitem]->stBeginTime.second;

	int nyear=m_pstrFileVector[nitem]->stEndTime.year;
	int nmonth=m_pstrFileVector[nitem]->stEndTime.month;
	int nday=m_pstrFileVector[nitem]->stEndTime.day;
	int nhour=m_pstrFileVector[nitem]->stEndTime.hour;
	int nminute=m_pstrFileVector[nitem]->stEndTime.minute;
	int nsecond=m_pstrFileVector[nitem]->stEndTime.second;
	
	totaltime=(nyear-byear)*365*24*3600+(nmonth-bmonth)*30*24*3600+(nday-bday)*24*3600
              +(nhour-bhour)*3600+(nminute-bminute)*60+(nsecond-bsecond);	       
	m_sliderbackplay.SetRange(0,totaltime);
	unsigned char cRecordBitMap[720] = {0};
	if (m_bDownloadByName)
	{
		int ibyte = 0;
		if (nhour == 0 & nminute == 0)
		{
			ibyte = 0;
		}
		else	
			ibyte = (bhour * 60 + bminute) / 2 ;
		for (int i =0;i < totaltime/120;++i )
		{
			cRecordBitMap[ibyte] = 85;
			++ibyte;			   
		}

		m_TimeReplayerView.UpdateTimeSect(cRecordBitMap);
	}

// 	if (m_bDownloadByTime)
// 	{
// 		int ibyte = 0;
// 		if (nhour == 0 & nminute == 0)
// 		{
// 			ibyte = 0;
// 		}
// 		else	
// 			ibyte = (bhour * 60 + bminute) / 2 ;
// 		for (int i =0;i < totaltime/120;++i )
// 		{
// 			cRecordBitMap[ibyte] = 85;
// 			++ibyte;			   
// 		}
// 	}
	
}

CString CRemotePlaybackDlg::BrowseForFolder(HWND hWnd)
{
	TCHAR szTitle[] = _T("Select a folder");
	TCHAR szDisplayName[MAX_PATH] = _T("");
	TCHAR szPath[MAX_PATH] = _T("");
	BROWSEINFO bi;
	
	bi.hwndOwner= hWnd;
	bi.pidlRoot= NULL;
	bi.lpszTitle= szTitle;
	bi.pszDisplayName = szDisplayName;
	bi.ulFlags= BIF_RETURNONLYFSDIRS;
	bi.lpfn= NULL;
	bi.lParam= 0;
	
	LPITEMIDLIST pItemIDList = SHBrowseForFolder( &bi );
	if( pItemIDList )
	{
		SHGetPathFromIDList(pItemIDList,szPath);
		
		IMalloc *pMalloc;
		if( SHGetMalloc( &pMalloc ) != NOERROR )
		{
			//TRACE( "Failed to get pointer to shells task allocator" ) ;
			return szPath;
		}
		pMalloc->Free( pItemIDList );
		if( pMalloc )
			pMalloc->Release();
	}
	return szPath;
}

void CRemotePlaybackDlg::OnBUTTONAudio() 
{
	// TODO: Add your control notification handler code here
	//H264_PLAY_PlaySoundShare(40);
	if(m_nPlayHandle)
	{
		if (!m_bOpenAudio)
		{
			if(	H264_PLAY_PlaySoundShare(40))
			{

				SetDlgItemText(IDC_BUTTON_Audio,_CS("PlayBack.CloseAudio"));
				m_bOpenAudio=true;
			}
		}
		else
		{
			if(	H264_PLAY_StopSoundShare(40))
			{
				SetDlgItemText(IDC_BUTTON_Audio,_CS("PlayBack.OpenAudio"));
				m_bOpenAudio=false;
			}
		}
	}
}

void CRemotePlaybackDlg::OnReleasedcaptureSliderBackplay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_sliderPos=m_sliderbackplay.GetPos();
	
	m_preTime=GetTickCount();
	float a=(float)(((float)m_sliderPos)/((float)totaltime));
	H264_DVR_SetPlayPos( m_nPlayHandle[0] ,a);
	//H264_PLAY_SetPlayPos(40, a);
	*pResult = 0;
}

void CRemotePlaybackDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CRemotePlaybackDlg::OnNMCustomdrawSliderBackplay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	m_sliderPos = m_sliderbackplay.GetPos();
// 	m_preTime = GetTickCount();
// 	H264_DVR_SetPlayPos(m_nPlayHandle,(float)m_sliderPos / 1000.0);
	*pResult = 0;
}

void CRemotePlaybackDlg::SetSliderPos(float nplaypos)
{
	if(m_preTime > 0)
	{
		DWORD dwTime = GetTickCount();
		if((dwTime - m_preTime) > 2000)
		{
			m_sliderbackplay.SetPos(nplaypos * 1000);
			m_preTime = 0;
		}
		else
		{
			m_sliderbackplay.SetPos(nplaypos * 1000);
		}
	}
}
void CRemotePlaybackDlg::OnCbnSelchangeComboType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CRemotePlaybackDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	if (m_Bitmap.m_hObject)
	{
		CRect rt;
		m_videoWnd.GetWindowRect(&rt);
		ScreenToClient(&rt);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);//�������ڴ��б����������DC
		CBitmap * oldBitmap = memDC.SelectObject(&m_Bitmap);//ѡ�����memDC
		BITMAP bp;
		GetObject(m_Bitmap.m_hObject, sizeof(bp), &bp);
		dc.StretchBlt(rt.left, rt.top + 10, rt.Width()-10, rt.Height() - 10,&memDC,
			0, 0, bp.bmWidth, bp.bmHeight, SRCCOPY);
		memDC.SelectObject(oldBitmap);
		DeleteDC(memDC);
	}
}
//ѡ��ͨ����ѯ¼���ļ�
void CRemotePlaybackDlg::OnBnClickedBtnSelectchannle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSelectChannelDlg SelectChannelDlg(m_TotalChannel,m_bDownloadByTime);
	if(IDOK == SelectChannelDlg.DoModal())
	{
		m_SelectChannel = SelectChannelDlg.m_SelectChannel;
		
	}
	if (!m_bDownloadByTime)
	{
		m_SelectChannel = 0;
	}
}
