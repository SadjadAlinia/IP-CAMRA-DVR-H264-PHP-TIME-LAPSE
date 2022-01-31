#if _MSC_VER > 1000
#pragma once
#endif	//_MSC_VER > 1000

// CImportAndExport �Ի���

class CImportAndExport : public CDialog
{
	DECLARE_DYNAMIC(CImportAndExport)

public:
	CImportAndExport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImportAndExport();

	CButton m_Import;
	CButton m_Export;
	CString m_ImportFile;
// �Ի�������
	enum { IDD = IDD_DIALOG_EPTIMT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
