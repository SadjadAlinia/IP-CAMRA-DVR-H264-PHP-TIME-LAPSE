#pragma once

#include "StdAfx.h"
// CSinglePlayWindow �Ի���

class CSinglePlayWindow : public CDialog
{
	DECLARE_DYNAMIC(CSinglePlayWindow)

public:
	CSinglePlayWindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSinglePlayWindow();

// �Ի�������
	enum { IDD = IDD_PLAYWINDOW_SIGNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
public:
	void SetWndIndex(int index){m_index=index;}
	int m_index;
	COLORREF m_CurBackColor;
};
