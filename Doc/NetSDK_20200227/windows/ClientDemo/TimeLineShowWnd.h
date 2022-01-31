// RecLineShowWnd.h: interface for the CRecLineShowWnd class.
// Create by Zhengdh  20070801 
// ���ڻ�¼��洢��� ����MS��������
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECLINESHOWWND_H__82887AFD_2546_4CEB_BB18_9125840D025E__INCLUDED_)
#define AFX_RECLINESHOWWND_H__82887AFD_2546_4CEB_BB18_9125840D025E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
typedef struct _REC_COLOR
{
	COLORREF clColor[100];

}REC_COLOR , *pREC_COLOR;
*/

#define WM_LBUTUP		(WM_USER + 1000)
#define WM_LDBUTUP		(WM_USER + 1001)


class CTimeLineShowWnd : public CWnd  
{
public:
	CTimeLineShowWnd();
	virtual ~CTimeLineShowWnd();

	//�Ƿ���ʾ�̶�
	void SetShowScale(BOOL bShow);

	//�����м�����
	void SetMiddleText(char * pchText , COLORREF clText = RGB(0,0,0));

	//��ʼ��ɫ��ҪInsertDrawSpace������
	void Draw();

	void SetColour(long lColour)
	{
		m_colour = lColour;		
	}
	void SetPeriod(COLORREF *pPeriod);

	void InitColourNum(int num);
protected:

	//{{AFX_MSG(CSubWindow)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF *m_pclRecColor;		//��¼��ɫ�ڴ��

	CString  m_MidText;				//��¼�м���ʾ����
	COLORREF m_MidTextColor;		//�м�������ʾ��ɫ 

	BOOL     m_bShowScale;//�Ƿ���ʵ�̶�

	long m_lColourNum;	//�ڴ����Ŀ

	COLORREF m_penColour;	// �ָ�����ɫ
	COLORREF m_colour;
	COLORREF m_unColour;

	DWORD m_dwTickcount;	//��¼����¼�ʱ�䣬���ֵ�����˫��
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#endif // !defined(AFX_RECLINESHOWWND_H__82887AFD_2546_4CEB_BB18_9125840D025E__INCLUDED_)
