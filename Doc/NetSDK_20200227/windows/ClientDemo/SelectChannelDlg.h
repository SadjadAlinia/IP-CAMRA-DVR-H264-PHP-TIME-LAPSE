#pragma once

#include "resource.h"
// CSelectChannelDlg 对话框

#define MAX_CHANNEL_PER_LINE 8
#define IDC_BTN_CHANNEL WM_USER + 70

class CSelectChannelDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectChannelDlg)

public:
	CSelectChannelDlg(int nTotalChannel,bool bDownloadByTime, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectChannelDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SElECTCHANNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()
public:
	int m_nChannel;
	BOOL m_bDownloadByTime;
	unsigned __int64 m_SelectChannel;
	unsigned __int64 m_StreamType;
	afx_msg void OnBnClickedOk();
	int m_nChannelStatus[NET_MAX_CHANNUM];
	CButton* m_pBtnChannel[NET_MAX_CHANNUM];
	CButton* m_pBtnChannel2[NET_MAX_CHANNUM];
	bool m_bClik[NET_MAX_CHANNUM];
	void Clear();
	int m_nPlayChannel;
};
