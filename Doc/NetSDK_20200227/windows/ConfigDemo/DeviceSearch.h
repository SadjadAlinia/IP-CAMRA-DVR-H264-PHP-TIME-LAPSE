#if _MSC_VER > 1000
#pragma once
#endif 
#include "ConfigDemo.h"
#define GET_MODULE_FILE_INFO    (GetModuleFileInformations())
// CDeviceSearch 对话框

class CDeviceSearch : public CDialog
{
	DECLARE_DYNAMIC(CDeviceSearch)

public:
	CDeviceSearch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceSearch();

// 对话框数据
	enum { IDD = IDD_CONFIG_DEVICE_SEARCH };
	CComboBox	m_typeCombo;
	CListCtrl	m_listDevice;
	CEdit	m_editIP;
	CEdit	m_EditUserName;
	CEdit	m_EditTotal;
	CEdit	m_EditPsw;
	CEdit	m_EditPort;
	CEdit	m_EditDevName;
	CString	m_strDevName;
	UINT	m_nPort;
	CString	m_strPsw;
	UINT	m_nChannel;
	CString	m_strUserName;
	CString	m_strIP;
	CString	m_DevSerialID;
	UINT	m_HostPort;
	CString	m_strHostIP;
	BOOL	m_bIP;
	BOOL	m_bSerialID;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void OnOK();	
	afx_msg void OnDestroy();
	afx_msg void OnCHECKSerialID();
	afx_msg void OnCheckIp();
	afx_msg void OnSelchangeComboLogintype();
	
	DECLARE_MESSAGE_MAP()
public:
	int m_nType;		//0:add device;  1:modify device
	DEV_INFO m_devInfo;
	DEV_INFO *m_info;
	//BOOL WriteXML();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	static UINT SearchDevice(LPVOID parm);
	afx_msg void OnBnClickedBtnSearch();
	SDK_CONFIG_NET_COMMON_V2 m_Device[1000];
	SDK_CONFIG_NET_COMMON_V2 m_Device_Sort[1000];
	int m_nDevNum;
	HANDLE m_hSearch;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	long m_lUpgradeHandle;
	void DelArpEntry(DWORD dwAddr);
	afx_msg void OnNMDblclkListUpgrade(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListUpgrade(NMHDR *pNMHDR, LRESULT *pResult);
	//CPromptDlg m_PromptDlg;
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
private:
	std::map<int , DEV_INFO> m_mapDev;
public:
	afx_msg void OnLvnColumnclickListUpgrade(NMHDR *pNMHDR, LRESULT *pResult);
};
