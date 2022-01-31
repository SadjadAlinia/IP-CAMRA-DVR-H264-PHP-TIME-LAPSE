#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CRestoreDefault �Ի���

class CRestoreDefault : public CDialog
{
	DECLARE_DYNAMIC(CRestoreDefault)

public:
	CRestoreDefault(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRestoreDefault();

	BOOL n_update;	//״̬
	SDK_SetDefaultConfigTypes m_DefaultConfig;

	CButton m_ctrlCheckAll;
	CButton m_ctrlCheckGeneral;
	CButton m_ctrlCheckEncode;
	CButton m_ctrlCheckRecord;
	CButton m_ctrlCheckNet;
	CButton m_ctrlCheckComm;
	CButton m_ctrlCheckAlarm;
	CButton m_ctrlCheckService;
	CButton m_ctrlCheckUser;
	CButton m_ctrlCheckPreview;
	CButton m_ctrlCheckWebCamera;

// �Ի�������
	enum { IDD = IDD_DIALOG_RD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedSure();
	afx_msg void OnBnClickedCannel();
	afx_msg void OnBnClickedChkAll();
	afx_msg void OnBnClickedChkGeneral();
	afx_msg void OnBnClickedChkEncode();
	afx_msg void OnBnClickedChkRecord();
	afx_msg void OnBnClickedChkNetService();
	afx_msg void OnBnClickedChkNet();
	afx_msg void OnBnClickedChkAlarm();
	afx_msg void OnBnClickedChkComm();
	afx_msg void OnBnClickedChkUser();
	afx_msg void OnBnClickedChkPreview();
	afx_msg void OnBnClickedChkWebcamera();

	DECLARE_MESSAGE_MAP()
public:
	void InitDlgDefault();
};
