// SelectChannelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "SelectChannelDlg.h"


// CSelectChannelDlg 对话框

IMPLEMENT_DYNAMIC(CSelectChannelDlg, CDialog)

CSelectChannelDlg::CSelectChannelDlg(int nTotalChannel,bool bDownloadByTime, CWnd* pParent /*=NULL*/)
	: CDialog(CSelectChannelDlg::IDD, pParent)
{
	m_nChannel = nTotalChannel;
	m_bDownloadByTime = bDownloadByTime;
	m_SelectChannel = 0;
	m_StreamType = 0;
	for ( int i = 0; i < NET_MAX_CHANNUM; i++ )
	{
		m_pBtnChannel[i] = NULL;
		m_pBtnChannel2[i] = NULL;
		m_bClik[i]=true;
	}
}

CSelectChannelDlg::~CSelectChannelDlg()
{
	Clear();
}

void CSelectChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectChannelDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectChannelDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CSelectChannelDlg 消息处理程序

void CSelectChannelDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int i= 0;
	if (m_bDownloadByTime)//按时间
	{
		for ( i = 0;i < m_nChannel ;i ++)
		{
			if ( m_pBtnChannel[i]&&m_pBtnChannel[i]->GetCheck()
				|| m_nChannel == 1)
			{
				m_SelectChannel  |= ((unsigned __int64)1 << i);
				m_StreamType &= ~((unsigned __int64)1 << i);

			}else if(m_pBtnChannel2[i]&&m_pBtnChannel2[i]->GetCheck())
			{
				m_SelectChannel  |= ((unsigned __int64)1 << i);
				m_StreamType |= ((unsigned __int64)1 << i);
			}
		}
		if (0 == m_SelectChannel && m_nChannel > 1 )
		{
			MessageBox(_CS("AlarmManage.SelectVideoChn"), _CS("PlayBack.Prompt")) ;
			return;
		}
	}
	else				  //按文件
	{
		for ( i = 0;i < m_nChannel ;i ++)
		{
			if (  m_pBtnChannel[i]&&m_pBtnChannel[i]->GetCheck())
			{
				m_SelectChannel = i;
				m_StreamType = 0;
				break;
			}else if (m_pBtnChannel2[i]&&m_pBtnChannel2[i]->GetCheck())
			{
				m_SelectChannel = i;
				m_StreamType = 1;
				break;
			}
		}
		if (i == m_nChannel && m_nChannel > 1)
		{
			MessageBox(_CS("AlarmManage.SelectVideoChn"), _CS("PlayBack.Prompt")) ;
			return;
		}
	}
	OnOK();
}

BOOL CSelectChannelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	_CWndCS(this);

	return TRUE;
}

void CSelectChannelDlg::OnShowWindow( BOOL bShow, UINT nStatus )
{
	Clear();
	if(bShow)
	{
		CString strBtnTitle;
		int ChannelNum = m_nChannel;
		ZeroMemory(m_pBtnChannel, sizeof(m_pBtnChannel) );
		ZeroMemory(m_pBtnChannel2, sizeof(m_pBtnChannel2) );
		int i = 0, j = 0;
		CRect rcChannel ,orgrcChannel;
		GetDlgItem(IDC_STATIC_CHANNEL)->GetWindowRect(orgrcChannel);
		ScreenToClient(orgrcChannel);
		orgrcChannel.left = orgrcChannel.right + 4;
		orgrcChannel.right = orgrcChannel.left + 20;
		int nSect = ChannelNum / MAX_CHANNEL_PER_LINE;
		int nOffset = ChannelNum % MAX_CHANNEL_PER_LINE;	
		if ( ChannelNum >0)
		{
			for (j = 0; j < nSect ;j++)
			{
				rcChannel = orgrcChannel;
				for ( i = 0; i < MAX_CHANNEL_PER_LINE; ++i )
				{
					strBtnTitle.Format("%d", i + 1 + j*MAX_CHANNEL_PER_LINE);

					m_pBtnChannel[i + j*MAX_CHANNEL_PER_LINE] = new CButton();
					m_pBtnChannel[i + j*MAX_CHANNEL_PER_LINE]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE, rcChannel, this, IDC_BTN_CHANNEL + i + j*MAX_CHANNEL_PER_LINE);	
					m_pBtnChannel2[i + j*MAX_CHANNEL_PER_LINE] = new CButton();
					m_pBtnChannel2[i + j*MAX_CHANNEL_PER_LINE]->Create(NULL, BS_AUTOCHECKBOX|WS_CHILD, rcChannel, this, 100+IDC_BTN_CHANNEL + i + j*MAX_CHANNEL_PER_LINE);
					rcChannel.left  += 20;
					rcChannel.right += 20;
				}
				if ( j < nSect - 1)//最后一行就不增加
				{
					orgrcChannel.top += 20;
					orgrcChannel.bottom += 20;
				}
			}
			if ( nOffset)
			{
				if ( nSect )//存在多行
				{
					orgrcChannel.top += 20;
					orgrcChannel.bottom += 20;
				}
				//orgrcChannel.left = orgrcChannel.right + 24;
				//orgrcChannel.right = orgrcChannel.left + 20;
				for ( i = 0; i < nOffset; ++i )
				{
					strBtnTitle.Format("%d", i + 1 + nSect * MAX_CHANNEL_PER_LINE);

					m_pBtnChannel[i + nSect * MAX_CHANNEL_PER_LINE] = new CButton();
					m_pBtnChannel[i + nSect * MAX_CHANNEL_PER_LINE]->Create(strBtnTitle, BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|BS_PUSHLIKE, orgrcChannel, this, IDC_BTN_CHANNEL + i + nSect * MAX_CHANNEL_PER_LINE);	
					m_pBtnChannel2[i + j*MAX_CHANNEL_PER_LINE] = new CButton();
					m_pBtnChannel2[i + j*MAX_CHANNEL_PER_LINE]->Create(NULL, BS_AUTOCHECKBOX|WS_CHILD, orgrcChannel, this, 100+IDC_BTN_CHANNEL + i + j*MAX_CHANNEL_PER_LINE);			
					orgrcChannel.left  += 20;
					orgrcChannel.right += 20;
				}
			}
		}
	}
}

void CSelectChannelDlg::Clear()
{
	int nChannelNum = NET_MAX_CHANNUM;
	int i = 0;

	for (i = 0; i<nChannelNum; ++i)
	{
		if (m_pBtnChannel[i])
		{
			delete m_pBtnChannel[i];
			m_pBtnChannel[i] = NULL;
		}
		if (m_pBtnChannel2[i])
		{
			delete m_pBtnChannel2[i];
			m_pBtnChannel2[i] = NULL;
		}
	}
	m_nPlayChannel = 0;
}
