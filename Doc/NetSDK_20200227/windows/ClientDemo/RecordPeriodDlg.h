#pragma once
#include "staticex.h"
#include "MyButton.h"
#include "TimeLineShowWnd.h"
#include "resource.h"
//#define ID_TIME_DRAW		(WM_USER + 1000)
#define ID_BTN_THUM			(WM_USER + 2000)
#define ID_CAPTION_STATIC	(WM_USER + 2001)
#define ID_STATIC_HOUR		(WM_USER + 3000)
#define ID_STATIC_MINUTE      (WM_USER + 3100)

#define DLG_WIDTH				708
#define STATIC_HEIGTH			12
#define STATIC_WIDTH			50

//���ݽṹ
struct DEC
{
	long pthis;
	long decHandle;
	long playHandle;
	bool bOver;
};
// CRecordPeriodDlg �Ի���

class CRecordPeriodDlg : public CDialog
{
//	DECLARE_DYNAMIC(CRecordPeriodDlg)

public:
	CRecordPeriodDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordPeriodDlg();
	void SetSliderPos(short nHour, short nMin, short nSec = 0) ;
	void UpdateTimeSect(const unsigned char cRecordBitMap[720]);
//SDK�������ص�
//�������ص�.�ڴ˺����еõ��豸����ط�ʱ��ѹ������
//...................................................
// �Ի�������

	//{{AFX_DATA(CRecordPeriodDlg)
	enum { IDD = IDD_DLG_RECORDPERIOD };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordPeriodDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordPeriodDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	afx_msg LRESULT OnSliderMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderLButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTimeLineLButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTimeLineDblClk(WPARAM wParam, LPARAM lParam);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



private:
	COLORREF m_recTime[24][60];	//¼��ʱ���
	CStatic m_StaticHour;
	CStatic m_StaticMin;
	CStatic m_StaticH[25];
	CStatic m_StaticM[13];
	int m_nTimeSect[24][60]; //��ʾһ��

	bool m_bShow;

	bool m_bDown;			//�������Ƿ񱻰���

	CRect m_rectHourSlider;	//1Сʱ������λ��
	CRect m_rectDaySlider;	//24Сʱ������λ��
	CSize m_sizeThumb;		//�������С

	//��ǰ������Сʱ�����ӡ����λ��
	int m_nCurHour;			
	int m_nCurMin;
	int m_nCurSec;

	CBrush m_brush;
	//¼��̶���
	CTimeLineShowWnd m_TimeHourDraw;
	CTimeLineShowWnd m_TimeDayDraw;	

	CMyButton m_DayThum;	//24Сʱ����
	CMyButton m_HourThum;	//1Сʱ����

	CStaticEx m_timeCaption;	//ʱ����ʾ��
	CFont m_font;
public:
	CBrush m_Brush;
	CRgn m_Rgn;
	CBitmap m_bmpBack_Up;  //ǰ��λͼ
	DWORD m_lLoginID;
	H264_DVR_TIME m_tmStart;
	H264_DVR_TIME m_tmEnd;
	int m_nRecType; //¼������
	int m_nChannelNO;
	map<DWORD,WORD> m_nPort;
	int m_nCount;

	DEC *m_dec[64];
#ifdef MULTISTREAM_RECORD	
	long m_pPlayDlg;
#endif

	short GetSelectTime(short * nStartHour, short * nStartMin, short * nEndHour, short * nEndMin) ;

		
};
