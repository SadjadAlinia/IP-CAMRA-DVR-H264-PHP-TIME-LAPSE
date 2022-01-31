// ImportAndExport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "ImportAndExport.h"
#include "ConfigDemoDlg.h"

#define MAX_BUF_LEN (64 * 1024)
// CImportAndExport �Ի���

IMPLEMENT_DYNAMIC(CImportAndExport, CDialog)

CImportAndExport::CImportAndExport(CWnd* pParent /*=NULL*/)
	: CDialog(CImportAndExport::IDD, pParent)
{

}

CImportAndExport::~CImportAndExport()
{
}
BOOL CImportAndExport::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_BTN_IMPORT)->SetWindowText(_CS("Import.ImportConfig"));
	GetDlgItem(IDC_BTN_EXPORT)->SetWindowText(_CS("Import.ExportConfig"));
	GetDlgItem(IDOK_EPTIMT)->SetWindowText(_CS("OneClick.OK"));

	return TRUE;
}
void CImportAndExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_IMPORT, m_Import);
	DDX_Control(pDX, IDC_BTN_EXPORT, m_Export);
}


BEGIN_MESSAGE_MAP(CImportAndExport, CDialog)
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_EXPORT, OnBnClickedBtnExport)
	ON_BN_CLICKED(IDOK_EPTIMT, OnBnClickedEptimt)
END_MESSAGE_MAP()


// CImportAndExport ��Ϣ�������
//���õ���
void CImportAndExport::OnBnClickedBtnImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	long lLoginID = ((CClientDemo5Dlg*)AfxGetMainWnd())->m_LoginID;
	
	if(!lLoginID)
	{
		MessageBox(_CS("Upgrade.PleaseConnectDevice"));
		return ;
	}
	//ѡ�����豸��.cfg�ļ�
	CString str("");
	char szFilter[] = _T("cfg(*.cfg)|*.*||");	

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,szFilter, this);
	if ( dlg.DoModal() == IDOK)
	{
		m_ImportFile = dlg.GetPathName();
	}
	else
	{
		return;
	}
	//��ȡ�豸�ļ����ݵ�SDK_CONFIG_TYPE�ṹ����
	DWORD iLen = 0;
	int nLen = MAX_BUF_LEN;
	char *pBuf = new char[nLen];
	if (NULL == pBuf)
	{
		return;
	}
	memset(pBuf, 0, nLen);
	FileRead(m_ImportFile, pBuf, nLen);
	//�����豸��Ϣ�ɹ�
	int nRet = H264_DVR_SetDevConfig(lLoginID, E_SDK_CONFIG_IMPORT, -1, pBuf, nLen, 5000);
	if ( nRet > 0 )
	{
		MessageBox(_CS("Error.ImportSuccess"));  
	}
	else
	{
		if ( nRet != H264_DVR_OPT_REBOOT )
		{
			switch(nRet)
			{
			case H264_DVR_NOPOWER:
				{
					CString strMsg;
					strMsg.Format("%s", _CS("Error.NoPower") );
					MessageBox(strMsg, _CS("OneClick.Prompt"));
				}
				break;
			default:
				MessageBox(_CS("AccountMSG.ImportFail"), _CS("OneClick.Prompt"));
				break;
			}
		}
		else
		{
			H264_DVR_ControlDVR(lLoginID, 0, 10000);//����
			MessageBox(_CS("AccountMSG.ImportOK"), _CS("OneClick.Prompt"));
		}
	}
	delete [] pBuf;
}
//���õ���
void CImportAndExport::OnBnClickedBtnExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	long lLoginID = ((CClientDemo5Dlg*)AfxGetMainWnd())->m_LoginID;
	if(!lLoginID)
	{
		MessageBox(_CS("Upgrade.PleaseConnectDevice"));
		return ;
	}
	//ѡ���ļ����е��ļ�
	CString strPath = _T("");
	strPath = BrowseForFolder(GetSafeHwnd());
	if ( strPath == _T("") )
	{
		return;
	}
	//�½��ļ�����
	CTime tm;
	CString str;
	tm=CTime::GetCurrentTime();
	str = "\\cj" + tm.Format("%Y%m%d%H%M%S") + ".cfg";
	strPath += str;
	/*strPath += "\\chenjian.cfg";*/
	//��ȡ�豸��ϢH264_DVR_SetDevConfig����
	DWORD iLen = 0;
	int nLen = MAX_BUF_LEN;
	char* pBuf = new char[MAX_BUF_LEN];
	if(NULL == pBuf)
	{
		return;
	}
	memset(pBuf, 0, nLen);
	int nRet = H264_DVR_GetDevConfig(lLoginID, E_SDK_CONFIG_EXPORT, -1, pBuf, nLen, &iLen, 3000);
	if(nRet > 0)
	{
		//���豸��Ϣд�뵽.cfg�ļ���
		if(!FileWrite(strPath, pBuf, iLen))
		{
			MessageBox(_CS("Error.FileFail"));
		}
		//�����ɹ�
		MessageBox(_CS("Error.ExportSuccess"));
	}
	else
	{
		//����ʧ��
		MessageBox(_CS("Error.ExportFail"));
	}
	delete [] pBuf;
}

void CImportAndExport::OnBnClickedEptimt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
CString CImportAndExport::BrowseForFolder(HWND hWnd)
{
	TCHAR szTitle[] = _T("Select a folder");
	TCHAR szDisplayName[MAX_PATH] = _T("");
	TCHAR szPath[MAX_PATH] = _T("");
	BROWSEINFO bi;

	bi.hwndOwner= hWnd;
	bi.pidlRoot= NULL;
	bi.lpszTitle= szTitle;
	bi.lpszTitle = _T("");
	bi.pszDisplayName = szDisplayName;
	bi.ulFlags= BIF_RETURNONLYFSDIRS;
	bi.lpfn= NULL;
	bi.lParam= 0;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder( &bi );
	if( pItemIDList )
	{
		SHGetPathFromIDList(pItemIDList,szPath);

		IMalloc *pMalloc;
		if( SHGetMalloc( &pMalloc ) != NOERROR )
		{
			//TRACE( "Failed to get pointer to shells task allocator" ) ;
			return szPath;
		}
		pMalloc->Free( pItemIDList );
		if( pMalloc )
			pMalloc->Release();
	}
	return szPath;
}
//��ȡ�ļ���Ϣ
bool CImportAndExport::FileRead(CString strFileName, char *pBuf, int& iLen)
{
	CFile file;	
	if(!file.Open(strFileName, CFile::modeRead))
	{
		return false;
	}
	iLen = file.Read(pBuf, iLen);
	file.Close();
	return true;
}
//д���ļ�
bool CImportAndExport::FileWrite(CString strFileName, char *pBuf, int iLen)
{
	CFile file;
	if(!file.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		return false;
	}
	file.Write(pBuf, iLen);
	file.Close();
	return true;
}