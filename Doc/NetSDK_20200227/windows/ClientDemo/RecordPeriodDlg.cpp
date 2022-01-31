// RecordPeriodDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RecordPeriodDlg.h"
#include "RemotePlaybackDlg.h"

#define RGB_REC_NORMAL	RGB(0,255,0)
#define RGB_REC_DETECT  RGB(100,100,100)
#define RGB_REC_ALARM  RGB(255,0,0)
#define RGB_REC_HAND   RGB(0,0,255)

#define ID_TIME_DRAW		(WM_USER + 1100)
#define ID_TIME_HOUR		(WM_USER + 1132)

const unsigned int COLOR[]={0, RGB_REC_NORMAL, RGB_REC_ALARM, RGB_REC_DETECT, 0, RGB_REC_HAND};
// CRecordPeriodDlg �Ի���

//IMPLEMENT_DYNAMIC(CRecordPeriodDlg, CDialog)

CRecordPeriodDlg::CRecordPeriodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordPeriodDlg::IDD, pParent)
{
	m_nCurHour = 0 ;
	m_nCurMin = 0;
	m_nCurSec = 0;
	m_bDown = false;
	m_bShow = false;
	memset( &m_recTime, 0, sizeof(m_recTime) );
}

CRecordPeriodDlg::~CRecordPeriodDlg()
{
}

void CRecordPeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRecordPeriodDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_SLIDER_MOUSEMOVE, OnSliderMouseMove)
	ON_MESSAGE(WM_SLIDER_LBUTDOWN, OnSliderLButtonDown)
	ON_MESSAGE(WM_SLIDER_LBUTUP, OnSliderLButtonUp)
	ON_MESSAGE(WM_LBUTUP, OnTimeLineLButtonUp)
	ON_MESSAGE(WM_LDBUTUP, OnTimeLineDblClk)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CRecordPeriodDlg ��Ϣ�������

BOOL CRecordPeriodDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	_CWndCS(this);
	//CRect WndRect;
	//GetClientRect( &WndRect );
	//��ȡ�������ͻ������λ����Ϣ
	//m_rectHourSlider.SetRect(17,30, WndRect.Width() - 31, 38 );
	//m_rectDaySlider.SetRect( 17, 80, WndRect.Width() - 19, 92);
	//m_sizeThumb.cx = 9;
	//m_sizeThumb.cy = 8;
	CString strText;
	CRect drawRect;
	drawRect.left = 0;
	drawRect.right = 0;
	drawRect.top = 0;
	drawRect.bottom = 0;
	strText = _CS(_T("Playback.Minutes"));
	m_StaticMin.Create(strText, WS_VISIBLE, drawRect,this, ID_TIME_HOUR + 1 );
	strText = _CS(_T("Playback.Hours"));
	m_StaticHour.Create(strText, WS_VISIBLE, drawRect,this, ID_TIME_HOUR );
	
	CRect RectStatic;
	RectStatic.left = 0;
	RectStatic.right = 0;
	RectStatic.top = 0;
	RectStatic.bottom = 0;
	for (int i = 0; i <= 24; i ++)
	{
		CString strHour;
		strHour.Format(_T("%d"), i);
		m_StaticH[i].Create(strHour, WS_VISIBLE, RectStatic,this, ID_STATIC_HOUR + i );
	}
	for ( int j = 0; j <= 12; j ++)
	{
		CString strMin;
		strMin.Format(_T("%d"), j * 5 );
		m_StaticM[j].Create(strMin, WS_VISIBLE, RectStatic,this, ID_STATIC_MINUTE + j );
	}
	
	m_TimeHourDraw.Create(NULL, NULL, WS_VISIBLE | WS_BORDER , m_rectHourSlider, this, ID_TIME_DRAW);
	//	m_TimeHourDraw.InitColourNum(60*60);
	//�Է��Ӽ��㣬��ʱ�����㵽��
	m_TimeHourDraw.InitColourNum(60);
	
	m_TimeDayDraw.Create(NULL, NULL, WS_VISIBLE | WS_BORDER , m_rectDaySlider, this, ID_TIME_DRAW + 1);
	//m_TimeDayDraw.InitColourNum(24 * 60 * 60);
	m_TimeDayDraw.InitColourNum(24 * 60);
	
	
	//24Сʱ�Ļ�����
	//CRect rectThum(m_rectDaySlider.left - m_sizeThumb.cx / 2, m_rectDaySlider.bottom, 
	//	m_rectDaySlider.left + m_sizeThumb.cx / 2, m_rectDaySlider.bottom + m_sizeThumb.cy);
	CRect rectThum;
	m_DayThum.Create( _T(""), WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, rectThum, this, ID_BTN_THUM );
	//m_DayThum.SetLabelPosition( m_rectDaySlider.bottom, m_rectDaySlider.left, m_rectDaySlider.right - m_rectDaySlider.left );
	
	//1Сʱ�Ļ�����
	//CRect rectThum2(m_rectHourSlider.left - m_sizeThumb.cx / 2, m_rectHourSlider.bottom, 
	//	m_rectHourSlider.left + m_sizeThumb.cx / 2, m_rectHourSlider.bottom + m_sizeThumb.cy);
	CRect rectThum2;
	m_HourThum.Create( _T(""), WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, rectThum2, this, ID_BTN_THUM + 1 );
	//m_HourThum.SetLabelPosition( m_rectHourSlider.bottom, m_rectHourSlider.left, m_rectHourSlider.right - m_rectHourSlider.left );
	
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	CFont *pFont = GetFont();
	pFont->GetLogFont(&lf);
	lf.lfHeight = 12;	
	m_font.CreateFontIndirect(&lf);
	///CRect staticRect(rectThum2.right + 5, rectThum2.top + 2, rectThum2.right + 50, rectThum2.top + 14);
	CRect staticRect;
	m_timeCaption.Create(_T(""), WS_CHILD|WS_VISIBLE, staticRect, this, ID_CAPTION_STATIC);
	m_timeCaption.SetFont(&m_font);
	
	m_timeCaption.SetTextColor(RGB(255,255,255));
	m_timeCaption.SetBackFlush(true);
	m_timeCaption.SetBackColor(RGB(34,34,34));
	m_timeCaption.SetWindowText(_T("00:00:00"));  //��Сʱ�ͷ�����ʾ����ʱ����ʾ��
	
	m_bShow = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CRecordPeriodDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CRecordPeriodDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
}

//�������ƶ��¼�
LRESULT CRecordPeriodDlg::OnSliderMouseMove(WPARAM wParam, LPARAM lParam)
{
	CRect wndRect;
	GetClientRect(&wndRect);
	//24Сʱ������
	if ( wParam == ID_BTN_THUM )
	{	
		//�ƶ�1Сʱ������		
		int nUnit = m_rectDaySlider.right - m_rectDaySlider.left;

		int nTime = ((24 * 3600) * (int)lParam) / nUnit;

		int nSecond = 	nTime % 3600;

		int nMin = nSecond / 60;
		int nSec = nSecond % 60;

		if ( nMin == 60 )
		{
			nMin = 59;
			nSec = 59;
		}
		m_nCurMin = nMin;
		m_nCurSec = nSec;

		int nHourUnit = m_rectHourSlider.right - m_rectHourSlider.left;
		int nHourPos = (nSecond * nHourUnit) / (3600);

		CRect rectHourThum(m_rectHourSlider.left + nHourPos - m_sizeThumb.cx / 2, 
			m_rectHourSlider.bottom, 
			m_rectHourSlider.left + nHourPos + m_sizeThumb.cx / 2, 
			m_rectHourSlider.bottom + m_sizeThumb.cy);
		m_HourThum.MoveWindow(rectHourThum);
		m_HourThum.Invalidate();

		int nHour = nTime / 3600;
		//���ڻ�������ƶ���1Сʱ��¼��ʱ����ʾ����Ҫ�仯����ͬ��Сʱ�в�ͬ��¼��ʱ���
		if ( m_nCurHour != nHour && nHour != 24 )
		{
			m_TimeHourDraw.SetPeriod( (COLORREF *)m_recTime[nHour] );
			m_TimeHourDraw.Draw();

			m_nCurHour = nHour;	
			m_HourThum.Invalidate();
		}

		CString strCaption;
		strCaption.Format(_T("%02d:%02d:%02d"), m_nCurHour, nMin, nSec);
		//�����ʾ��λ�ó����Ի���Χ
		if ( rectHourThum.right + 5 + STATIC_WIDTH > wndRect.Width())
		{
			CRect staticRect(rectHourThum.left - 55, rectHourThum.top + 2, rectHourThum.left - 5, rectHourThum.top + 14);
			m_timeCaption.MoveWindow(staticRect);
		}
		else
		{
			CRect staticRect(rectHourThum.right + 5, rectHourThum.top + 2, rectHourThum.right + 55, rectHourThum.top + 14);
			m_timeCaption.MoveWindow(staticRect);
		}
		m_timeCaption.SetWindowText(strCaption);
	}
	//1Сʱ�Ļ�����
	else if ( wParam == ID_BTN_THUM + 1 )
	{
		//�ƶ�24Сʱ������
		int nHourUnit =  m_rectHourSlider.right - m_rectHourSlider.left;
		int nTime = (3600 * (int)lParam) / nHourUnit;

		DWORD lSecond = m_nCurHour * 3600 + nTime;

		//�����ж��ٸ����ص㣬�������Ͷ��ٵȷ�
		int nUnit = m_rectDaySlider.right - m_rectDaySlider.left;

		int nPos = (lSecond * nUnit) / (24 * 3600);

		//�õ�������λ��
		CRect rectThum(m_rectDaySlider.left + nPos - m_sizeThumb.cx / 2, 
			m_rectDaySlider.bottom, 
			m_rectDaySlider.left + nPos + m_sizeThumb.cx / 2,
			m_rectDaySlider.bottom + m_sizeThumb.cy);
		m_DayThum.MoveWindow(rectThum);
		m_DayThum.Invalidate();


		CRect rectHourThum;
		m_HourThum.GetWindowRect(&rectHourThum);
		ScreenToClient(rectHourThum);
		int nMin = nTime / 60;
		int nSec = nTime % 60;
		if ( nMin == 60 )
		{
			nMin = 59;
			nSec = 59;
		}

		m_nCurMin = nMin;
		m_nCurSec = nSec;

		CString strCaption;
		strCaption.Format(_T("%02d:%02d:%02d"), m_nCurHour, nMin, nSec);
		//�����ʾ��λ�ó����Ի���Χ
		if ( rectHourThum.right + 5 + STATIC_WIDTH > wndRect.Width() )
		{
			CRect staticRect(rectHourThum.left - 55, rectHourThum.top + 2, rectHourThum.left - 5, rectHourThum.top + 14);
			m_timeCaption.MoveWindow(staticRect);
		}
		else
		{
			CRect staticRect(rectHourThum.right + 5, rectHourThum.top + 2, rectHourThum.right + 55, rectHourThum.top + 14);
			m_timeCaption.MoveWindow(staticRect);
		}
		m_timeCaption.SetWindowText(strCaption);

	}
	//��ʼʱ����
	else if ( wParam == ID_BTN_THUM + 2 )
	{

	}
	//����ʱ����
	else if ( wParam == ID_BTN_THUM + 3 )
	{

	}

	m_bShow = true;
	return 0;
}

/************************************************************************/
/*      
���ò��Ž���
SetSliderPos��char *pTime��
������pTime ʱ��ṹ��
������ʱ��Ϊ׼�����뵱ǰ���ŵ�ʱ�䣬�ڲ�����pos                                                                
*/
/************************************************************************/
void CRecordPeriodDlg::SetSliderPos(short nHour, short nMin, short nSec) 
{
	if ( m_bDown )
	{
		return;
	}
	DWORD lSecond = nHour * 3600 + nMin * 60 + nSec;

	//�Ƿ�Ƿ�ֵ
	if ( lSecond > 24 * 3600 )
	{
		return;
	}

	//�����ж��ٸ����ص㣬�������Ͷ��ٵȷ�
	int nUnit = m_rectDaySlider.right - m_rectDaySlider.left;

	int nPos = (lSecond * nUnit) / (24 * 3600);

	//�õ�������λ��
	CRect rectThum(m_rectDaySlider.left + nPos - m_sizeThumb.cx / 2, 
		m_rectDaySlider.bottom, 
		m_rectDaySlider.left + nPos + m_sizeThumb.cx / 2, 
		m_rectDaySlider.bottom + m_sizeThumb.cy);

	m_DayThum.MoveWindow(rectThum);

	//���ڻ�������ƶ���1Сʱ��¼��ʱ����ʾ����Ҫ�仯����ͬ��Сʱ�в�ͬ��¼��ʱ���
	if ( m_nCurHour != nHour )
	{
		m_TimeHourDraw.SetPeriod( (COLORREF *)m_recTime[nHour] );
		m_TimeHourDraw.Draw();

		m_HourThum.Invalidate();
		m_nCurHour = nHour ;
	}

	//���洦��1Сʱ�Ļ�����λ��
	int nHourUnit = m_rectHourSlider.right - m_rectHourSlider.left;

	DWORD lHourSecond = nMin * 60 + nSec;
	int nHourPos = (lHourSecond * nHourUnit) / (3600);

	CRect rectHourThum(m_rectHourSlider.left + nHourPos - m_sizeThumb.cx / 2, 
		m_rectHourSlider.bottom, 
		m_rectHourSlider.left + nHourPos + m_sizeThumb.cx / 2, 
		m_rectHourSlider.bottom + m_sizeThumb.cy);
	m_HourThum.MoveWindow(rectHourThum);


	CString strCaption;
	strCaption.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);

	CRect WndRect;
	GetClientRect(&WndRect);
	//�����ʾ��λ�ó����Ի���Χ
	if ( rectHourThum.right + 5 + STATIC_WIDTH > WndRect.Width() )
	{
		CRect staticRect(rectHourThum.left - 55, rectHourThum.top + 2, rectHourThum.left -5, rectHourThum.top + 14);
		m_timeCaption.MoveWindow(staticRect);
	}
	else
	{
		CRect staticRect(rectHourThum.right + 5, rectHourThum.top + 2, rectHourThum.right + 55, rectHourThum.top + 14);
		m_timeCaption.MoveWindow(staticRect);
	}
	m_timeCaption.SetWindowText(strCaption);
}

short CRecordPeriodDlg::GetSelectTime(short * nStartHour, short * nStartMin, short * nEndHour, short * nEndMin) 
{
	*nStartHour = m_nCurHour;
	*nStartMin = m_nCurMin;

	//���ݽ���ʱ����λ�ã��������ʱ��	
	*nEndHour = 23;
	*nEndMin = 59;

	return 1;
}

LRESULT CRecordPeriodDlg::OnTimeLineLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if (((CRemotePlaybackDlg*)GetParent())->m_bDownloadByTime)
	{
		((CRemotePlaybackDlg*)GetParent())->OnStop();
	}
	if (((CRemotePlaybackDlg*)GetParent())->m_bDownloadByName)
	{
		((CRemotePlaybackDlg*)GetParent())->OnStop();
	}
	int nHour = 0;
	int nMin  = 0;
	int nSec  = 0;
	//Сʱʱ����
	if ( wParam == ID_TIME_DRAW )
	{
		nHour = m_rectHourSlider.right - m_rectHourSlider.left;
		int nTime = (3600 * (int)lParam) / nHour;
		//�����Сʱ���֣���
		nMin = nTime / 60;
		nSec = nTime % 60; 
		nHour = m_nCurHour;
	//	if ( g_pispView->IsSyncMode() )
		{
		//	for ( int i = 0; i < MAXWNDTOTAL; i ++)
			{
			//	g_pispView->SeekPlay(i, m_nCurHour, nMin, nSec);
			}
		}
	//	else
		{
			//g_pispView->SeekPlay(-1, m_nCurHour, nMin, nSec);
		}

		//��Ҫ��Ϊ���ƶ�������
		SetSliderPos( m_nCurHour, nMin, nSec);

		m_nCurMin = nMin;
	}
	//24Сʱʱ����
	else if ( wParam == ID_TIME_DRAW + 1 )
	{
		nHour = m_rectDaySlider.right - m_rectDaySlider.left;

		//�õ���ǰλ�õ�ʱ��
		int nTime = ((24 * 3600) * (int)lParam) / nHour;
		//�����Сʱ���֣���
		nHour = nTime / 3600;
		nMin = ( nTime % 3600) / 60;
		nSec = (nTime % 3600) % 60; 

	//	if ( g_pispView->IsSyncMode() )
		{
		//	for ( int i = 0; i < MAXWNDTOTAL; i ++)
			{
			//	g_pispView->SeekPlay(i, nHour, nMin, nSec);
			}
		}
	//	else
		{
		//	g_pispView->SeekPlay(-1, nHour, nMin, nSec);
		}

		//g_pispView->SeekPlay(-1, nHour, nMin, nSec);

		//��Ҫ��Ϊ���ƶ�������
		SetSliderPos( nHour, nMin, nSec);

		m_nCurHour = nHour;
		m_nCurMin = nMin;
		m_nCurSec = nSec;
	}
	if (((CRemotePlaybackDlg*)GetParent())->m_bDownloadByTime)
	{
		SDK_SYSTEM_TIME sTime;
		sTime.hour = nHour;
		sTime.minute = nMin;
		sTime.second = nSec;
		((CRemotePlaybackDlg*)GetParent())->OnPlayByTime((char*)&sTime,false);
	}
	if (((CRemotePlaybackDlg*)GetParent())->m_bDownloadByName)
	{
		SDK_SYSTEM_TIME sTime;
		sTime.hour = nHour;
		sTime.minute = nMin;
		sTime.second = nSec;
		((CRemotePlaybackDlg*)GetParent())->OnPlayByName((char*)&sTime,false);
	}
	return 0;
}

LRESULT CRecordPeriodDlg::OnSliderLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_bDown = true;	

	return 0;
}

LRESULT CRecordPeriodDlg::OnSliderLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if (((CRemotePlaybackDlg*)GetParent())->m_bDownloadByTime)
	{
		((CRemotePlaybackDlg*)GetParent())->OnStop();
	}
	if (((CRemotePlaybackDlg*)GetParent())->m_bDownloadByName)
	{
		((CRemotePlaybackDlg*)GetParent())->OnStop();
	}
	int nHour = 0;
	int nMin = 0;
	int nSec = 0;
	//24Сʱ������
	if ( wParam == ID_BTN_THUM )
	{
		int nUnit = m_rectDaySlider.right - m_rectDaySlider.left;

		//�õ���ǰλ�õ�ʱ��
		int nTime = ((24 * 3600) * (int)lParam) / nUnit;
		//�����Сʱ���֣���
		nHour = nTime / 3600;
		nMin = ( nTime % 3600) / 60;
		nSec = (nTime % 3600) % 60; 

		//if ( g_pispView->IsSyncMode() )
		{
		//	for ( int i = 0; i < MAXWNDTOTAL; i ++)
			{
			//	g_pispView->SeekPlay(i, nHour, nMin,nSec);
			}
		}
		//else
		{
		//	g_pispView->SeekPlay(-1, nHour, nMin,nSec);
		}
		
	}
	//1Сʱ�Ļ�����
	else if ( wParam == ID_BTN_THUM + 1 )
	{
		int nHourUnit = m_rectHourSlider.right - m_rectHourSlider.left;
		int nTime = (3600 * (int)lParam) / nHourUnit;
		//�����Сʱ���֣���
		nMin = nTime / 60;
		nSec = nTime % 60; 
		nHour = m_nCurHour;
	//	if ( g_pispView->IsSyncMode() )
		{
			//for ( int i = 0; i < MAXWNDTOTAL; i ++)
			{
			//	g_pispView->SeekPlay(i, m_nCurHour, nMin, nSec);
			}
		}
	//	else
		{
			//g_pispView->SeekPlay(-1, m_nCurHour, nMin, nSec);
		}
	}
	if (((CRemotePlaybackDlg*)GetParent())->m_bDownloadByTime)
	{
		SDK_SYSTEM_TIME sTime;
		sTime.hour = nHour;
		sTime.minute = nMin;
		sTime.second = nSec;
		((CRemotePlaybackDlg*)GetParent())->OnPlayByTime((char*)&sTime,false);
	}
	if (((CRemotePlaybackDlg*)GetParent())->m_bDownloadByName)
	{
		SDK_SYSTEM_TIME sTime;
		sTime.hour = nHour;
		sTime.minute = nMin;
		sTime.second = nSec;
		((CRemotePlaybackDlg*)GetParent())->OnPlayByName((char*)&sTime,false);
	}
	m_bDown = false;

	return 0;
}

LRESULT CRecordPeriodDlg::OnTimeLineDblClk(WPARAM wParam, LPARAM lParam)
{

	m_bDown = true;
	SetTimer(1, 1000, NULL);

	m_bDown = false;
	return 0;
}

void CRecordPeriodDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����Сʱ����Χ
	if ( point.x > m_rectHourSlider.left && point.x < m_rectHourSlider.right
		&& point.y >= m_rectHourSlider.top - 5 && point.y <= m_rectHourSlider.bottom + 5 )
	{
		OnTimeLineLButtonUp(ID_TIME_DRAW, point.x - m_rectHourSlider.left);	
	}
	else if ( point.x > m_rectDaySlider.left && point.x < m_rectDaySlider.right
		&& point.y >= m_rectDaySlider.top - 5 && point.y <= m_rectDaySlider.bottom + 5 )
	{
		OnTimeLineLButtonUp(ID_TIME_DRAW + 1, point.x - m_rectDaySlider.left);	
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CRecordPeriodDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bDown = true;
	//����Сʱ����Χ
	if ( point.x > m_rectHourSlider.left && point.x < m_rectHourSlider.right
		&& point.y >= m_rectHourSlider.top - 5 && point.y <= m_rectHourSlider.bottom + 5 )
	{
		SetTimer(1, 1000, NULL);
	}
	else if ( point.x > m_rectDaySlider.left && point.x < m_rectDaySlider.right
		&& point.y >= m_rectDaySlider.top - 5 && point.y <= m_rectDaySlider.bottom + 5 )
	{
		SetTimer(1, 1000, NULL);
	}
	m_bDown = false;
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CRecordPeriodDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nHour = 0;
	int nMin = 0;
	int nSec = 0;

	//CTime tim;

	time_t timestamp = H264_PLAY_GetCurTimeStamp(40);

	tm *newtime = localtime( &timestamp ); 

	if (newtime)
	{
		SetSliderPos( newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
	}
	CDialog::OnTimer(nIDEvent);
}

//����¼����Ϣ
void CRecordPeriodDlg::UpdateTimeSect(const unsigned char cRecordBitMap[720])
{
	//����¼����Ϣ
	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			int nIndex = (i * 60 + j) / 2;
			if ((i * 60 + j) % 2)
			{
				m_nTimeSect[i][j] = (cRecordBitMap[nIndex] >> 4) & 0xF;
			}
			else
			{
				m_nTimeSect[i][j] = cRecordBitMap[nIndex] & 0xF;
			}
			//����¼�����͵�m_recTime
			switch ( m_nTimeSect[i][j] )
			{
			case 0:
				m_recTime[i][j] = 0;
				break;
			case 1:
				m_recTime[i][j] = RGB_REC_NORMAL;  //��ͨ¼��
				break;
			case 2:
				m_recTime[i][j] = RGB_REC_ALARM;   //����¼��
				break;
			case 3:
				m_recTime[i][j] = RGB_REC_DETECT;   //���¼��
				break;
			case 4:
				m_recTime[i][j] = RGB_REC_HAND;   //�ֶ�¼��
				break;
			case 5:
				m_recTime[i][j] = RGB_REC_HAND;   //�ֶ�¼��
				break;
			default:
				m_recTime[i][j] = 0;   
				break;
			}
		}
	}

	//���ƿ̶�
	m_TimeDayDraw.SetPeriod( (COLORREF* )m_recTime );
	m_TimeDayDraw.Draw();

	m_TimeHourDraw.SetPeriod( m_recTime[m_nCurHour] );
	m_TimeHourDraw.Draw();

	SetTimer(1, 1000, NULL);
}
void CRecordPeriodDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if ( !m_bShow )
	{
		return;
	}

	CRect WndRect;
	GetClientRect(&WndRect);
	CRect StaticHour,staticMin;
	staticMin.SetRect( 17,0, 60, 20 );
	StaticHour.SetRect( 17, 50, 60, 70);
	m_StaticHour.MoveWindow(StaticHour);
	m_StaticMin.MoveWindow(staticMin);

	//��ȡ�������ͻ������λ����Ϣ
	m_rectHourSlider.SetRect( 17,20, WndRect.Width() - 31, 28 );
	m_rectDaySlider.SetRect( 17, 70, WndRect.Width() - 19, 82);
	m_TimeHourDraw.MoveWindow(m_rectHourSlider);
	m_TimeDayDraw.MoveWindow(m_rectDaySlider);
	m_sizeThumb.cx = 9;
	m_sizeThumb.cy = 8;
	CRect rectThum, rectThum2;
	int npos,nHourPos;
	int nHourUnit = m_rectHourSlider.right - m_rectHourSlider.left;
	nHourPos = m_nCurMin * nHourUnit / 60;
	int nDayUnit = m_rectDaySlider.right - m_rectDaySlider.left;
	npos = ( m_nCurHour * 60 + m_nCurMin ) * nDayUnit / ( 24 * 60 );
	rectThum.SetRect(m_rectDaySlider.left + npos - m_sizeThumb.cx / 2, m_rectDaySlider.bottom, 
	m_rectDaySlider.left + npos + m_sizeThumb.cx / 2, m_rectDaySlider.bottom + m_sizeThumb.cy);
	m_DayThum.MoveWindow(rectThum);
	m_DayThum.SetLabelPosition( m_rectDaySlider.bottom, m_rectDaySlider.left, m_rectDaySlider.right - m_rectDaySlider.left );
	rectThum2.SetRect(m_rectHourSlider.left + nHourPos - m_sizeThumb.cx / 2, m_rectHourSlider.bottom,
	m_rectHourSlider.left + nHourPos + m_sizeThumb.cx / 2, m_rectHourSlider.bottom + m_sizeThumb.cy);
	m_HourThum.MoveWindow(rectThum2);
	m_HourThum.SetLabelPosition( m_rectHourSlider.bottom, m_rectHourSlider.left, m_rectHourSlider.right - m_rectHourSlider.left );
	CRect staticRect;
	staticRect.SetRect(rectThum2.right + 5, rectThum2.top + 2, rectThum2.right + 55, rectThum2.top + 14);
	m_timeCaption.MoveWindow(staticRect);

	int VWidth = (float)nDayUnit / 24.0;
	//��עСʱ
	for ( int i = 0 ; i <= 24; i ++)
	{
		CRect SRect;
		SRect.SetRect(i * VWidth + 17, 83, ( i + 1 ) * VWidth + 17, 103);
		m_StaticH[i].MoveWindow(SRect);
	}
	VWidth = (float)nHourUnit / 12;
	//��ע����
	for ( int j = 0 ; j<= 12 ; j ++)
	{
		CRect SRect;
		SRect.SetRect(j * VWidth + 17, 29, ( j + 1 ) * VWidth + 30, 49);
		m_StaticM[j].MoveWindow(SRect);
	}

	//m_HourThum.CreateRgnFromMask(RGB(255, 0, 255), m_Rgn);
	//if (m_Rgn.m_hObject != NULL)
	//{
	//	SetWindowRgn(m_Rgn, TRUE);
	//}

	//Բ��
	CRgn rgn;
	rgn.CreateRoundRectRgn(WndRect.left, WndRect.top, WndRect.right, WndRect.bottom,  25, 25);
	SetWindowRgn(rgn, true);
	rgn.DeleteObject();
}

HBRUSH CRecordPeriodDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	int nId = pWnd->GetDlgCtrlID();

	if ( nId == ID_TIME_HOUR || nId == ID_TIME_HOUR + 1
		|| (nId >= ID_STATIC_HOUR && nId <= ID_STATIC_HOUR + 24)
		|| (nId >= ID_STATIC_MINUTE && nId <= ID_STATIC_MINUTE + 12))
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_Brush;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

int CRecordPeriodDlg::OnCreate(LPCREATESTRUCT lpCreateStruct )
{
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return CDialog::OnCreate(lpCreateStruct);
}
