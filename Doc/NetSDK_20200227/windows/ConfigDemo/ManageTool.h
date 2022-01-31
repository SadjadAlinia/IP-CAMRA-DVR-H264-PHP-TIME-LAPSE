#if _MSC_VER > 1000
#pragma once
#endif	//_MSC_VER > 1000

#include "Restart.h"
#include "AutoRboot.h"
#include "RestoreDefault.h"
#include "ImportAndExport.h"	//导入导出
// CManageTool 对话框

class CManageTool : public CDialog
{
	DECLARE_DYNAMIC(CManageTool)

public:
	CManageTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CManageTool();

// 对话框数据
	enum { IDD = IDD_DLG_TOOL_MANAGE };
	CButton m_AutoManagement;
	CButton m_RestoreDefault;
	CButton m_ImportAExport;
	CButton m_Restart;

	CAutoRboot m_AutoRoot;
	CRestoreDefault m_Default;
	CImportAndExport m_Import;
	CRestart m_Ret;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAutomanage();
	afx_msg void OnBnClickedBtnRd();
	afx_msg void OnBnClickedBtnIae();
	afx_msg void OnBnClickedBtnRestart();
	DECLARE_MESSAGE_MAP()
public:
	void Control();
};
