#pragma once
#include "resource.h"

#define IDC_STATIC_CNS WM_USER+4000
#define IDC_EDIT_CNS WM_USER+4600
// COsdInfo 对话框

class COsdInfo : public CDialog
{
	DECLARE_DYNAMIC(COsdInfo)

public:
	COsdInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COsdInfo();
	byte *m_pBuf;		//记录点阵信息
	int m_nWidth;		//点阵的宽度和高度
	int m_nHeight;
	SDK_OSDInfoConfigAll m_OsdName;
	int m_nChannelTotal;
	CEdit *m_edit[NET_MAX_CHANNUM];
	CStatic *m_static[NET_MAX_CHANNUM];
	BOOL InitWindow();
// 对话框数据
	enum { IDD = IDD_OSDINFO };
	CEdit m_OsdInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	void InitDlgInfo(SDK_OSDInfoConfigAll *pChannelName,int nbyChanNum );
private:
	void freeData();
	int GetBuffer(int ch, int nFont);
	void GetData();
	void SaveChannel();
	void CleanAll();	
};
