#pragma once

#include "StdAfx.h"
// CSinglePlayWindow 对话框

class CSinglePlayWindow : public CDialog
{
	DECLARE_DYNAMIC(CSinglePlayWindow)

public:
	CSinglePlayWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSinglePlayWindow();

// 对话框数据
	enum { IDD = IDD_PLAYWINDOW_SIGNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
public:
	void SetWndIndex(int index){m_index=index;}
	int m_index;
	COLORREF m_CurBackColor;
};
