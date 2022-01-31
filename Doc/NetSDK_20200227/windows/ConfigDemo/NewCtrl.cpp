// NewCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "ConfigModule.h"
#include "NewCtrl.h"


// CNewCtrl

IMPLEMENT_DYNAMIC(CNewCtrl, CWnd)

CNewCtrl::CNewCtrl()
{
	m_nFlags = MODIFY_SIZE;
	m_rtMove.SetRectEmpty();
}

CNewCtrl::~CNewCtrl()
{
}


BEGIN_MESSAGE_MAP(CNewCtrl, CWnd)
	ON_WM_SETCURSOR()//��������������ĳ���������ƶ����������û�б�����ʱ���ͷ���Ϣ��ĳ������
	ON_WM_NCHITTEST()//�ƶ���꣬��ס���ͷ����ʱ����  
	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CNewCtrl ��Ϣ�������



BOOL CNewCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nHitTest)
	{
	case HTTOPLEFT:
	case HTBOTTOMRIGHT:
		{
			SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		}
		break;
	case HTTOPRIGHT:
	case HTBOTTOMLEFT:
		{
			SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		}
		break;
	case HTLEFT:
	case HTRIGHT:
		{
			SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		break;
	case HTTOP:
	case HTBOTTOM:
		{
			SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		}
		break;
	default:
		{
			SetCursor(::LoadCursor(NULL, IDC_ARROW));
		}
		break;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CNewCtrl::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_nFlags == NOMODIFY_SIZE)
	{
		
	}else
	{
		if (IsZoomed())
		{
			return 0;
		}
		
		CRect rect;
		GetClientRect(&rect);
		
		CPoint pt = point;
		ScreenToClient(&pt);
		
		static int nFrame=4; // ���ڱ߿�Ŀ��
		
		rect.DeflateRect(nFrame,nFrame);
		
		if (!rect.PtInRect(pt))
		{
			if (pt.x<=nFrame && pt.y>=rect.bottom-nFrame)
			{
				return HTBOTTOMLEFT;
			}
			else if (pt.x<=nFrame && pt.y<=nFrame)
			{
				return HTTOPLEFT;
			}
			else if (pt.x>=rect.right-nFrame && pt.y<=nFrame)
			{
				return HTTOPRIGHT;
			}
			else if (pt.x>=rect.right-nFrame && pt.y>=rect.bottom-nFrame)
			{
				return HTBOTTOMRIGHT;
			}
			else if (pt.x<=nFrame)
			{
				return HTLEFT;
			}
			else if (pt.y<=nFrame)
			{
				
				return HTTOP;
			}
			else if (pt.y>=rect.bottom-nFrame)
			{
				return HTBOTTOM;
			}
			else if (pt.x>=rect.right-nFrame)
			{
				return HTRIGHT;
			}
		}
		else
		{
			return HTCLIENT;
		}
	}
	
	return CWnd::OnNcHitTest(point);
}

void CNewCtrl::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nHitTest == HTTOP)
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTBOTTOM)
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTLEFT)
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTRIGHT)
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTTOPLEFT)
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTTOPRIGHT)
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTBOTTOMLEFT)
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTBOTTOMRIGHT)
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));
	else if (nHitTest == HTSYSMENU)
		SendMessage(WM_SYSCOMMAND, SC_MOUSEMENU,MAKELPARAM(point.x, point.y));

	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CNewCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	//// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CWnd::OnMouseMove(nFlags, point);
}

void CNewCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_OldPoint.x = point.x;
	m_OldPoint.y = point.y;

	SetTimer(1, 40,NULL);
	CWnd::OnLButtonDown(nFlags, point);
}

void CNewCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	CWnd::OnLButtonUp(nFlags, point);
}

void CNewCtrl::OnTimer(ONTIMER_UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(1 == nIDEvent)
	{
		//�ؼ�����
		CRect rect;
		GetWindowRect(rect);
		GetParent()->ScreenToClient(&rect);

		//��ȡ��ǰ����
		CPoint curPos;
		GetCursorPos(&curPos);
		GetParent()->ScreenToClient(&curPos);

		//�ϴ�����
		CPoint lastPos = m_OldPoint;
		ClientToScreen(&lastPos);
		GetParent()->ScreenToClient(&lastPos);
	

		CRect parentRect;
		GetParent()->GetClientRect(parentRect);
	
		int nX = rect.left + (curPos.x - lastPos.x);
		int nY = rect.top + (curPos.y - lastPos.y);
	
		if ( m_rtMove.IsRectEmpty())
		{
			if (nX < 0)
			{
				nX = 0;
			}
			if (nY <0)
			{
				nY =0;
			}
			if (nX + rect.Width() > parentRect.Width())
			{
				nX = parentRect.Width() - rect.Width();
			}
			if (nY +rect.Height() > parentRect.Height())
			{
				nY = parentRect.Height() - rect.Height();
			}
		}
		else
		{
			if (nX < m_rtMove.left)
			{
				nX = m_rtMove.left;
			}
			if (nY < m_rtMove.top)
			{
				nY = m_rtMove.top;
			}
			if (nX + rect.Width() > m_rtMove.right)
			{
				nX = m_rtMove.right - rect.Width();
			}
			if (nY + rect.Height() > m_rtMove.bottom)
			{
				nY = m_rtMove.bottom - rect.Height();
			}
		}
		MoveWindow(nX, nY, rect.Width(), rect.Height());
	}
	Invalidate(TRUE);

	CWnd::OnTimer(nIDEvent);
}
void CNewCtrl::SetMoveRect(CRect rtMove)
{
	m_rtMove = rtMove;
}