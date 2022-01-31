#pragma once

#include "Resource.h"

// CSysInfoVersion �Ի���
//ϵͳ��Ϣ���汾��Ϣ
class CSysInfoVersion : public CDialog
{
	DECLARE_DYNAMIC(CSysInfoVersion)

public:
	CSysInfoVersion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSysInfoVersion();

// �Ի�������
	enum { IDD = IDD_DLG_SYSINFO_VERSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void InitDlgInfo(H264_DVR_DEVICEINFO *pSysinfo,long loginID);
	HINSTANCE OldResourceHanlde;
	H264_DVR_DEVICEINFO m_sysInfo;
public:
	int m_nAlarmInput;
	int m_nAlarmOut;
	CString m_strIssuancedate;
	int m_nTotalChannel;
	int m_nExtChannel;
	CString m_strVersion;
	CString m_strProduct;
	CString m_strCompany;
	CString m_strTel;
	
	CString m_strStatu;
	CString m_strInfo;
	CString m_strSerialID;	
};
