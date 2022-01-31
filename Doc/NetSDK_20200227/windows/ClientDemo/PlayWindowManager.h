#pragma once

#include "StdAfx.h"
#include "SinglePlayWindow.h"
// CPlayWindowManager �Ի���

class CPlayWindowManager : public CDialog
{
	DECLARE_DYNAMIC(CPlayWindowManager)

public:
	CPlayWindowManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayWindowManager();

// �Ի�������
	enum { IDD = IDD_PLAYWINDOW_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
private:
	bool UpDataWnd();//�Ӵ��ڵķָ����и���
	bool upDataBordColor();//���ݵ�ǰ�ӿ�Ĵ�С������ȫ���ӿ�ı߿���ɫ
	COLORREF m_BGColor;
	COLORREF m_subWndBGColor;
	COLORREF m_subWndBordColor;

	int m_bordWidth;//�Ӵ��ڱ߿��С	
	long m_CurrSubWndIndex;//��ǰ���ڵ�����
	long m_nWindowsCount;//��ǰ�ܵĴ�����

	int  m_splitMode;
public:
	bool SetWndSplitMode(int mode);                 //0: ������; 1: 1/2����; 2: 1/4����,   ����ӿ�ֻ�ڵ����ڵ�ʱ��������//Ϊ����ϻ��л�ģʽ
	bool SetShowWindowNum(int numb);                //������ʾ���ڵ�����	
	CSinglePlayWindow	   m_vdWnd[MAXWNDNUM];
};
