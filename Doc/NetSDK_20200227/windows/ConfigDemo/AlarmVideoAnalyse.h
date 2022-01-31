#pragma once
#include "VideoOSCRuleSetDlg.h"
#include "VideoAnalyseSetDlg.h"
//#include "Interface/DlgSetBase.h"
// CAlarmVideoAnalyse �Ի���
#define RECTNUM 8

typedef struct MyValue 
{
	CString m_strValue;
	
	MyValue(CString strValue)
	{
		m_strValue = strValue;
	}
	
	CString ToCString()
	{
		return m_strValue;
	}
	
	char *ToBuffer()
	{
		return m_strValue.GetBuffer(0);
	}
	
	long Tolong()
	{
		return ::atol(m_strValue);
	}
	
	int Toint()
	{
		return ::atoi(m_strValue);
	}
	
	double Tofloat()
	{
		return ::atof(m_strValue);
	}
	
	DWORD ToDWORD()
	{
		return (DWORD)::atol(m_strValue);
	}
	
	BOOL ToBOOL()
	{
		return (BOOL)::atoi(m_strValue);
	}
	
	bool Tobool()
	{
		return (bool)::atoi(m_strValue);
	}
	
}MyValue;

class CAlarmVideoAnalyse : public CDialog
{
	DECLARE_DYNAMIC(CAlarmVideoAnalyse)

public:
	CAlarmVideoAnalyse(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlarmVideoAnalyse();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAlarmCtrlPeriod();
	afx_msg void OnBnClickedBtnRuleSet();
	afx_msg void OnChangeIntelTime();
	afx_msg void OnChangeIgnoreTime();
	afx_msg void OnChangeRecIgTime();
	afx_msg void OnCbnSelchangeComboChannel();
	afx_msg void OnBnClickedBtnRefush();
	afx_msg void OnBnClickedOk();
	//afx_msg void OnBnClickedCopy();
	//afx_msg void OnBnClickedPlaster();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void ReInit();
	afx_msg void OnEnKillfocusEditIgnoreTime();
	afx_msg void OnEnKillfocusEditRecordIgTime();

	afx_msg void OnBnClickedCheck();
private:
	CButton *m_pBtnsRecord[NET_MAX_CHANNUM];	//¼��ѡ��ť
	CButton *m_pBtnsTour[NET_MAX_CHANNUM];		//
	CButton *m_pBtnsAlarmOut[NET_MAX_CHANNUM];
	CButton *m_pBtnsSnap[NET_MAX_CHANNUM];

	bool WinDataToStruct();
	bool StructToWinData();
	void Clear();

	bool WinDataToStruct(int nChannelNum);
	bool StructToWinData(int nChannelNum, bool bCopy= false);

	int m_nLastChannelNum;
	void GetPosition();
	void ReSet();
	void ShowModuleType(int nChannelNum);	//�㷨�������ͨ����ͬ��ʾ
	CRect m_oldrect[RECTNUM];
	CRect m_newrect[RECTNUM];
	HINSTANCE OldResourceHanlde;
public:
	SDK_ANALYSECONFIG_ALL m_analyse;	//��Ƶ����
	SDK_ANALYSECONFIG_ALL m_Oldanalyse;	
	bool m_bGet[NET_MAX_CHANNUM];			//��¼ÿ��ͨ���Ƿ��ȡ������

	SDK_ANALYSECONFIG m_coptData;	//���渴�Ƶ�����
	bool	m_bCopy;					//�Ƿ��Ѿ����˸��ư�ť�����û�У�ճ��ʱ�򲻴���  
	SDK_ANALYZEABILITY m_analyseAbility;		//���ܷ�������
	SDK_ANALYZEABILITY m_oldAnalyseAbility;	
	bool m_bGetAnalyseAbility;
	long m_LoginID;
	bool m_bAnalyse;
	CVideoOSCRuleSetDlg dlgVideoOscRuleSet;
	CVideoAnalyseSetDlg dlgVideoAnalyseSet;
// �Ի�������
	enum { IDD = IDD_DLG_ALARM_VIDEOANALYSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPtzlink();
public:
	BOOL InitWndShow();
public:
	CButton *GetButton(UINT nID)
	{
		return (CButton *)GetDlgItem(nID);
	}
	CStatic *GetStatic(UINT nID)
	{
		return (CStatic *)GetDlgItem(nID);
	}
	CEdit *GetEdit(UINT nID)
	{
		return (CEdit *)GetDlgItem(nID);
	}

	CComboBox *GetComboBox(UINT nID)
	{
		return (CComboBox *)GetDlgItem(nID);
	}

	MyValue GetEditValue(UINT nID)
	{
		CString strValue("");
		GetDlgItemText(nID, strValue);

		return MyValue(strValue);
	}

	void SetEditText(UINT nID, CString strText)
	{
		CWnd *pWnd = GetDlgItem(nID);
		if (pWnd->GetSafeHwnd())
		{
			pWnd->SetWindowText(strText);
		}
	}

	void SetEditText(UINT nID, double fText, CString strFormat = "")
	{
		CString strText;
		strText.Format("%" + strFormat + "f", fText);
		SetEditText(nID, strText);
	}

	void SetEditText(UINT nID, unsigned int nText, CString strFormat = "")
	{
		CString strText;
		strText.Format("%" + strFormat + "d", nText);
		SetEditText(nID, strText);
	}

	void SetEditText(UINT nID, int nText, CString strFormat = "")
	{
		CString strText;
		strText.Format("%" + strFormat + "d", nText);
		SetEditText(nID, strText);
	}

	int GetComboBoxIndex(UINT nID)
	{
		int nIndex = 0;
		CComboBox *pComboBox = (CComboBox *)GetDlgItem(nID);
		if (pComboBox)
		{
			nIndex = pComboBox->GetCurSel();
		}

		return nIndex;
	}

	DWORD GetComboBoxData(UINT nID, int nIndex = -1)
	{
		DWORD dwData = 0;
		CComboBox *pComboBox = (CComboBox *)GetDlgItem(nID);
		if (pComboBox)
		{
			if (-1 == nIndex)
			{
				nIndex = pComboBox->GetCurSel();
			}

			if (-1 != nIndex)
			{
				dwData = pComboBox->GetItemData(nIndex);
			}
		}

		return dwData;
	}

	BOOL SetComBoxCurSel(UINT nID, DWORD dwData)
	{
		CComboBox *pComboBox = GetComboBox(nID); 
		if (!pComboBox)
		{
			return FALSE;
		}

		int nCount = pComboBox->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			if (pComboBox->GetItemData(i) == dwData)
			{
				pComboBox->SetCurSel(i);
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL SetComBoxValue(UINT nID, int nValue, DWORD dwData = 0, CString strFormat = "")
	{
		CString strText;
		strText.Format("%%" + strFormat + "d", nValue);
		return SetComBoxValue(nID, strText, dwData);
	}

	BOOL SetComBoxValue(UINT nID, double fValue, DWORD dwData = 0, CString strFormat = "")
	{
		CString strText;
		strText.Format("%%" + strFormat + "d", fValue);
		return SetComBoxValue(nID, strText, dwData);
	}

	BOOL SetComBoxValue(UINT nID, CString strValue, DWORD dwData = 0)
	{
		CComboBox *pComboBox = GetComboBox(nID);
		if (!pComboBox)
		{
			return FALSE;
		}

		int nInsert = pComboBox->AddString(strValue);
		pComboBox->SetItemData(nInsert, dwData);

		return TRUE;
	}
};
