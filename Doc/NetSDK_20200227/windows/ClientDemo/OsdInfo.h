#pragma once
#include "resource.h"

#define IDC_STATIC_CNS WM_USER+4000
#define IDC_EDIT_CNS WM_USER+4600
// COsdInfo �Ի���

class COsdInfo : public CDialog
{
	DECLARE_DYNAMIC(COsdInfo)

public:
	COsdInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COsdInfo();
	byte *m_pBuf;		//��¼������Ϣ
	int m_nWidth;		//����Ŀ�Ⱥ͸߶�
	int m_nHeight;
	SDK_OSDInfoConfigAll m_OsdName;
	int m_nChannelTotal;
	CEdit *m_edit[NET_MAX_CHANNUM];
	CStatic *m_static[NET_MAX_CHANNUM];
	BOOL InitWindow();
// �Ի�������
	enum { IDD = IDD_OSDINFO };
	CEdit m_OsdInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
