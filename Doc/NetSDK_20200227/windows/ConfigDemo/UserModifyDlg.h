#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "Resource.h"

// CUserModifyDlg �Ի���

class CUserModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserModifyDlg)

public:
	CUserModifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserModifyDlg();

// �Ի�������
	enum { IDD = IDD_DLG_USER_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	USER_INFO *m_pUserinfo;		//��ǰ�û���Ϣ

	HINSTANCE OldResourceHanlde;
	bool m_bresverved;			//�Ƿ����˻�
	bool CheckRight(const char *szright, USER_INFO *pUser);
	void UpdateUserInfo(CString strUser);
	
public:
	CButton m_checkReuseable;
public:
	CComboBox m_cmbGroup;
public:
	CString m_strGroup;
public:
	CComboBox m_cmbUserName;
public:
	CString m_strUserName;
public:
	CEdit m_editDescribe;
public:
	CString m_strDescribe;
public:
	CEdit m_editName;
public:
	CString m_strName;
public:
	CListCtrl m_listAuthrioty;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeComboUser();
public:
	afx_msg void OnCbnSelchangeComboGroup();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedBtnDelete();
	USER_MANAGE_INFO *m_userinfo;		//�û���Ϣ
	USER_GROUP_INFO* GetGroupInfo(char * strName);
	bool FindUserName(char *szUserName);
	USER_INFO* GetUserInfo(char * strName);
	long m_loginID;
	int ModifyuserInfo(CONF_MODIFYUSER *pUserinfo);
	int DeleteuserInfo(USER_INFO *pUserinfo);
};
