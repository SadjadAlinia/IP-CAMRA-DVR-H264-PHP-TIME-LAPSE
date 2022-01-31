#pragma once

#include "StdAfx.h"
#include "SinglePlayWindow.h"
// CPlayWindowManager 对话框

class CPlayWindowManager : public CDialog
{
	DECLARE_DYNAMIC(CPlayWindowManager)

public:
	CPlayWindowManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayWindowManager();

// 对话框数据
	enum { IDD = IDD_PLAYWINDOW_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
private:
	bool UpDataWnd();//子窗口的分隔进行更新
	bool upDataBordColor();//根据当前子框的大小，调整全体子框的边框颜色
	COLORREF m_BGColor;
	COLORREF m_subWndBGColor;
	COLORREF m_subWndBordColor;

	int m_bordWidth;//子窗口边框大小	
	long m_CurrSubWndIndex;//当前窗口的引用
	long m_nWindowsCount;//当前总的窗口数

	int  m_splitMode;
public:
	bool SetWndSplitMode(int mode);                 //0: 单画面; 1: 1/2画面; 2: 1/4画面,   这个接口只在单窗口的时候有意义//为了配合画中画模式
	bool SetShowWindowNum(int numb);                //设置显示窗口的数量	
	CSinglePlayWindow	   m_vdWnd[MAXWNDNUM];
};
