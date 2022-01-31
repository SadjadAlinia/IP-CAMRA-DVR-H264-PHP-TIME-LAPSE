
#pragma once
#include "Resource.h"
#include "afxcmn.h"

class CSysSettingUserManager : public CDialog
{
	DECLARE_DYNAMIC(CSysSettingUserManager)

public:
	CSysSettingUserManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSysSettingUserManager();

// �Ի�������
	enum { IDD = IDD_DLG_USER_MANAGER };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddUser();
	afx_msg void OnBnClickedMdUser();
	afx_msg void OnBnClickedAddGroup();
	afx_msg void OnBnClickedMdGroup();
	afx_msg void OnBnClickedMdPsw();
	afx_msg void OnBnClickedDeleteUser();
	afx_msg void OnBnClickedDeleteGroup();

private:	
	USER_MANAGE_INFO m_userinfo;		//�û���Ϣ
	bool m_bGetSuccess;					//��¼�Ƿ�ɹ���ȡ�˵�ǰ���ã�Ҳ��ʾ�ǲ���Ҫ���»�ȡ����
private:
	void ClearData();
	bool ShowUserInfo();
public:
	//���û�ȡ״̬���ǲ���Ҫ���»�ȡ
	void SetGetStatus( bool bGet )
	{
		m_bGetSuccess = bGet;
	}
public:
	void ClearAll();
	void InitDlgInfo();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CListCtrl m_listUser;

	virtual bool WinDataToStruct();
	virtual bool StructToWinData();
	afx_msg void OnBnClickedBtnRefush();
	afx_msg void OnNMRclickListUser(NMHDR *pNMHDR, LRESULT *pResult);
	long m_LoginID;
	int DeleteuserInfo(USER_INFO *pUserinfo);
	USER_INFO*	GetUserInfo(char * strName);
};
