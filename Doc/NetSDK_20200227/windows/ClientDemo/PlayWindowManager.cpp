// PlayWindowManager.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "PlayWindowManager.h"


// CPlayWindowManager 对话框

IMPLEMENT_DYNAMIC(CPlayWindowManager, CDialog)

CPlayWindowManager::CPlayWindowManager(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayWindowManager::IDD, pParent)
{
	m_CurrSubWndIndex=0;
	m_nWindowsCount=0;

	//默认颜色搭配
	m_BGColor=RGB(0,0,0);
	m_subWndBGColor=RGB(192,192,192);
	m_subWndBordColor=2686790; //RGB(28,255,46)
	
	for(int i=0;i<MAXWNDNUM;i++)
	{
		m_vdWnd[i].m_CurBackColor=m_subWndBGColor;
	}

	m_bordWidth=1;
	m_splitMode=0;
}

CPlayWindowManager::~CPlayWindowManager()
{
}

void CPlayWindowManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayWindowManager, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CPlayWindowManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	for ( int i = 0; i < MAXWNDNUM; i ++ )
	{
		m_vdWnd[i].Create(CSinglePlayWindow::IDD, this);
		m_vdWnd[i].SetWndIndex(i);		
	}

	return TRUE; 
}

void CPlayWindowManager::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect Rec;		
	GetClientRect( &Rec );	
	if(m_BGColor)
	{

		dc.FillSolidRect( &Rec , m_BGColor);
	}
	else
	{
		dc.FillSolidRect( &Rec , RGB(0,0,0));		
	}

	upDataBordColor();
	UpDataWnd();
}

void CPlayWindowManager::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CDialog::OnShowWindow(bShow, nStatus);
}
bool CPlayWindowManager::SetWndSplitMode(int mode)
{
	if(m_nWindowsCount>1)//只支持一个画面的情况
	{
		return false;
	}

	CRect rt;
	GetClientRect(&rt);
	int nSingleWidth=rt.Width();
	int nSingleHeight=rt.Height();
	switch(mode)
	{
	case 0: //1/1
		{

		}
		break;
	case 1: //1/2
		{
			rt.left += (nSingleWidth - 707 * nSingleWidth / 1000) / 2;
			rt.right = rt.left +  707 * nSingleWidth / 1000;
			rt.top += (nSingleHeight - 707 * nSingleHeight / 1000) / 2;
			rt.bottom = rt.top + 707 * nSingleHeight / 1000;
		}
		break;
	case 2: //1/4
		{
			rt.left += nSingleWidth / 4;
			rt.right = rt.left + nSingleWidth / 2;
			rt.top += nSingleHeight / 4;
			rt.bottom = rt.top + nSingleHeight / 2;
		}
		break;
	default:
		{

		}
		break;
	}

	m_vdWnd[0].MoveWindow(&rt);
	m_vdWnd[0].ShowWindow(SW_SHOW);
	m_splitMode=mode;
	return true;
}
bool CPlayWindowManager::UpDataWnd()
{
	if(m_nWindowsCount>MAXWNDNUM || m_nWindowsCount<=0)
	{
		return false;
	}

	int k=0;
	switch(m_nWindowsCount)
	{
	case  1:
		{	
			k=1;			
			break;
		}
	case 4:
		{	
			k=2;

			break;
		}
	case 9:
		{
			k=3;
			break;
		}
	case 16:
		{	
			k=4;

			break;
		}
	case 25:
		{
			k=5;

			break;
		}
	case 36:
		{
			k=6;		
			break;
		}
	case 64:
		{
			k=8;
			break;
		}
	default: return false;
	}

	CRect Rec;		
	GetClientRect( &Rec );
	float x=0;
	float y=0;
	float w=0;
	float h=0;	
	m_bordWidth=2;

	w=((float)(Rec.Width()-(k+1)*m_bordWidth))/k;
	h=((float)(Rec.Height()-(k+1)*m_bordWidth))/k;	

	if(m_splitMode>0)
	{
		SetWndSplitMode(m_splitMode);
	}
	else
	{				
		for(int i=0;i<m_nWindowsCount;i++)
		{	
			x=(i%k)*(w+m_bordWidth)+m_bordWidth;
			y=(i/k)*(h+m_bordWidth)+m_bordWidth;		
			m_vdWnd[i].MoveWindow(x,y,w,h);
			m_vdWnd[i].ShowWindow(SW_SHOW);
			if (i==0)
			{
				m_vdWnd[m_CurrSubWndIndex].RedrawWindow();
			}
		}		
	}

	return true;	
}

bool CPlayWindowManager::upDataBordColor()
{	
	if(m_nWindowsCount<=0 ||m_nWindowsCount>MAXWNDNUM)
	{
		return false;
	}

	if(m_splitMode>0)//画中画的分隔，在单个窗口上，不用话边框
	{
		return true;
	}

	for(int i=0;i<m_nWindowsCount;i++)
	{

		CRect rt;
		CDC *pDC = NULL;	
		pDC=GetDC();
		m_vdWnd[i].GetWindowRect(&rt);
		ScreenToClient(&rt);		
		rt.InflateRect(1,1);

		if (!pDC) 
		{
			return 0 ;
		}	

		if(i==m_CurrSubWndIndex)
		{		
			//只有一个窗口不画边框颜色
			if (1!=m_nWindowsCount)
			{
				pDC->Draw3dRect(&rt, m_subWndBordColor, m_subWndBordColor); 
			}
		}
		else
		{			
			pDC->Draw3dRect(&rt, m_BGColor, m_BGColor); 		

		}

		ReleaseDC(pDC);
	}

	return TRUE;

}
bool CPlayWindowManager::SetShowWindowNum( int numb )
{
	if(numb>MAXWNDNUM || numb<=0)
	{
		return false;
	}

	int i=0;
	for(;i<MAXWNDNUM;i++)//先清除
	{			
		m_vdWnd[i].ShowWindow(SW_HIDE);
	}

	//先清理之前的边框
	CRect rt;
	CDC *pDC = NULL;	
	pDC=GetDC();
	m_vdWnd[m_CurrSubWndIndex].GetWindowRect(&rt);
	ScreenToClient(&rt);		
	rt.InflateRect(1,1);
	pDC->Draw3dRect(&rt, m_BGColor, m_BGColor); 

	m_nWindowsCount=numb;
	m_CurrSubWndIndex=0;//每次切换，活动窗口都倒第一窗口
	UpDataWnd();
	upDataBordColor();

	return true;
}


// CPlayWindowManager 消息处理程序
