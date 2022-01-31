#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CRestart 对话框

class CRestart : public CDialog
{
	DECLARE_DYNAMIC(CRestart)

public:
	CRestart(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRestart();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESTART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRes();
	afx_msg void OnBnClickedRestart();
	
	DECLARE_MESSAGE_MAP()
};
