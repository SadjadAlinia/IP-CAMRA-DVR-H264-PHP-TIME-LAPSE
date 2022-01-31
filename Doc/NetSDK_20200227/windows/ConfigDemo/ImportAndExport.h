#if _MSC_VER > 1000
#pragma once
#endif	//_MSC_VER > 1000

// CImportAndExport 对话框

class CImportAndExport : public CDialog
{
	DECLARE_DYNAMIC(CImportAndExport)

public:
	CImportAndExport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportAndExport();

	CButton m_Import;
	CButton m_Export;
	CString m_ImportFile;
// 对话框数据
	enum { IDD = IDD_DIALOG_EPTIMT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedEptimt();

	DECLARE_MESSAGE_MAP()
public:
	CString BrowseForFolder(HWND hWnd);
	bool FileRead(CString strFileName, char *pBuf, int& iLen);
	bool FileWrite(CString strFileName, char *pBuf, int iLen);
};
