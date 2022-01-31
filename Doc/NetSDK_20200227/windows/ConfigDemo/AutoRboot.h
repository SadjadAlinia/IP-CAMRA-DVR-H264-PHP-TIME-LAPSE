#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CAutoRboot 对话框

class CAutoRboot : public CDialog
{
	DECLARE_DYNAMIC(CAutoRboot)

public:
	CAutoRboot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoRboot();

	CComboBox	m_cmdWeek;
	CComboBox	m_cmdDate;
	CComboBox	m_cmdClearFile;

	int nIndex;

	SDK_AutoMaintainConfig m_AutoConfig;

// 对话框数据
	enum { IDD = IDD_DIALOG_AUTOMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	DECLARE_MESSAGE_MAP()
public:
	void InitDlgAuto();
};
