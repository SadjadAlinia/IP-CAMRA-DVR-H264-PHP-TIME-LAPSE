#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CAutoRboot �Ի���

class CAutoRboot : public CDialog
{
	DECLARE_DYNAMIC(CAutoRboot)

public:
	CAutoRboot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAutoRboot();

	CComboBox	m_cmdWeek;
	CComboBox	m_cmdDate;
	CComboBox	m_cmdClearFile;

	int nIndex;

	SDK_AutoMaintainConfig m_AutoConfig;

// �Ի�������
	enum { IDD = IDD_DIALOG_AUTOMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	DECLARE_MESSAGE_MAP()
public:
	void InitDlgAuto();
};
