#if _MSC_VER > 1000
#pragma once
#endif	//_MSC_VER > 1000


// CUpgradeDemoDlg �Ի���

extern long loginID;

class CUpgradeDemoDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpgradeDemoDlg)

public:
	CUpgradeDemoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUpgradeDemoDlg();

	void CleanAll();
	CString GetUpgradeErrorMsg(int nError);

// �Ի�������
	enum { IDD = IDD_DLG_UpGrade };
	CProgressCtrl m_progress1;
	CButton m_Upgrade;
	CButton m_Browse;
	CString m_UpgradeFile;
	CString m_strProcess;
	CString m_strUpgradeFile;

	SDK_SystemFunction m_Upgradefunc;	//������������
	SDK_CloudUpgradeList m_UpgradeList;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	afx_msg void OnDropFile(HDROP hDropInfo);
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnUpgrade();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnSearch();

	static void  __stdcall cbUpgrade( long lLoginID, long lUpgradechannel,
		int nTotalSize, int nSendSize, long dwUser
		);
	DECLARE_MESSAGE_MAP()
public:
	long m_lUpgradeHandle;
};