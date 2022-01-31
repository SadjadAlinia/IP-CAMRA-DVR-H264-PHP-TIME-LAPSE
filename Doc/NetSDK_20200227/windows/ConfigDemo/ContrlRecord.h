#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CContrlRecord 对话框

class CContrlRecord : public CDialog
{
	DECLARE_DYNAMIC(CContrlRecord)

public:
	CContrlRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CContrlRecord();

	void CleanAll();
	void RadioAll(int nRet);
	void SetAll(BOOL nRet);
	void SetChannel1(BOOL nRet);
	void SetChannel2(BOOL nRet);
	void SetChannel3(BOOL nRet);
	void SetChannel4(BOOL nRet);
	void SetChannel5(BOOL nRet);
	BOOL GetChannel1();
	BOOL GetChannel2();
	BOOL GetChannel3();
	BOOL GetChannel4();
	BOOL GetChannel5();

// 对话框数据
	enum { IDD = IDD_CONFIG_CONTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnconctrlok();
	afx_msg void OnBnClickedBtnconctrlcnacel();
	afx_msg void OnBnClickedAll1();
	afx_msg void OnBnClickedAll2();
	afx_msg void OnBnClickedAll3();
	afx_msg void OnBnClickedAll4();
	afx_msg void OnBnClickedAll5();
	
	DECLARE_MESSAGE_MAP()
public:
	int m_All1;
	int m_Channel1;
	int m_Channel2;
	int m_Channel3;
	int m_Channel4;
	int m_Channel5;
	int m_Channel6;
	int m_Channel7;
	int m_Channel8;
	int m_Channel9;
	int m_Channel10;
	int m_Channel11;
	int m_Channel12;
	int m_Channel13;
	int m_Channel14;
	int m_Channel15;
	int m_Channel16;
};
