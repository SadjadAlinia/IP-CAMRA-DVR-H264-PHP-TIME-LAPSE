#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CRestart �Ի���

class CRestart : public CDialog
{
	DECLARE_DYNAMIC(CRestart)

public:
	CRestart(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRestart();

// �Ի�������
	enum { IDD = IDD_DIALOG_RESTART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRes();
	afx_msg void OnBnClickedRestart();
	
	DECLARE_MESSAGE_MAP()
};
