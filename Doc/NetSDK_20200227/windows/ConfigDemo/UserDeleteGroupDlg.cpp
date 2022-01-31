// UserDeleteGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigDemo.h"
#include "UserDeleteGroupDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserDeleteGroupDlg dialog


CUserDeleteGroupDlg::CUserDeleteGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserDeleteGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserDeleteGroupDlg)
	m_strGroup = _T("");
	m_strDescribe = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT

	OldResourceHanlde= AfxGetResourceHandle();//������ǰ����Դ����ģ��   
	     	
}


void CUserDeleteGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserDeleteGroupDlg)
	DDX_Control(pDX, IDC_EDIT_GROUPNAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_DESCRIBE, m_editDescribe);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_cmbGroup);
	DDX_CBString(pDX, IDC_COMBO_GROUP, m_strGroup);
	DDX_Text(pDX, IDC_EDIT_DESCRIBE, m_strDescribe);
	DDX_Text(pDX, IDC_EDIT_GROUPNAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserDeleteGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CUserDeleteGroupDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelchangeComboGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserDeleteGroupDlg message handlers

void CUserDeleteGroupDlg::OnSelchangeComboGroup() 
{
	UpdateData();
	
	m_editName.SetWindowText(m_strGroup);


	//��ȡ��ǰ����Ϣ

		USER_GROUP_INFO *pGroupinfo = GetGroupInfo(m_strGroup.GetBuffer(0));
		if ( pGroupinfo )
		{
			m_editDescribe.SetWindowText(pGroupinfo->memo);
		}
	

}

void CUserDeleteGroupDlg::OnOK() 
{
	UpdateData();
	
	int nIndex = m_cmbGroup.GetCurSel();
	
	//��ȡ�û���Ϣ

		if ( HasUser(m_strGroup) )
		{
			MessageBox(_CS("AccountMSG.DeleteInuseGroup"), _CS("OneClick.Prompt"));
			return;
		}
		
		USER_GROUP_INFO *pGroupInfo = GetGroupInfo(m_strGroup.GetBuffer(0));
		if ( pGroupInfo )
		{
			if ( IDYES == MessageBox(_CS("AccountMSG.SureToDelete"), _CS("OneClick.Prompt"), MB_YESNO | MB_ICONQUESTION ) )
			{
				//ɾ���û�	
				int bRet = DeleteGroupInfo( pGroupInfo );
				if ( bRet <= 0 )
				{
					CString strMsg(_CS("AccountMSG.Failed"));
					CString strError = GetSdkErrorInfo( bRet );
					strMsg += strError;
					MessageBox(strMsg, _CS("OneClick.Prompt"));

					return;
				}
				
				m_cmbGroup.DeleteString(nIndex);
				MessageBox(_CS("AccountMSG.DeleteGroupSuccess"), _CS("OneClick.Prompt"));


				m_cmbGroup.SetCurSel(0);

				OnSelchangeComboGroup();
			}	
		}
	
	
	//CDialog::OnOK();
}

BOOL CUserDeleteGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_CWndCS(this);
	
	//��ȡ����Ϣ

		USER_MANAGE_INFO *pUser = m_userinfo;
		for ( int i = 0; i < pUser->groupNum; i ++ )
		{
			if ( strcmp(pUser->groupList[i].name, "") != 0 )
			{
				m_cmbGroup.AddString(pUser->groupList[i].name);
			}		
		}

	m_cmbGroup.SetCurSel(0);
	
	m_cmbGroup.GetWindowText(m_strGroup);
	m_editName.SetWindowText(m_strGroup);

	//��ȡ��ǰ����Ϣ
	
	USER_GROUP_INFO *pGroupinfo = GetGroupInfo(m_strGroup.GetBuffer(0));
	if ( pGroupinfo )
	{
		m_editDescribe.SetWindowText(pGroupinfo->memo);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
USER_GROUP_INFO* CUserDeleteGroupDlg::GetGroupInfo(char * strName)
{
	for ( int i = 0 ; i < m_userinfo->groupNum; i ++ )
	{
		if ( !strcmp(strName , m_userinfo->groupList[i].name) )
		{
			return &m_userinfo->groupList[i];
		}
	}
	
	return NULL;
}
bool CUserDeleteGroupDlg::HasUser(CString strGroup)
{
	for ( int i = 0 ; i < m_userinfo->userNum; i ++ )
	{
		if ( !strcmp(m_userinfo->userList[i].Groupname, strGroup.GetBuffer(0)) )
		{
			return true;
		}
	}
	
	return false;
}
int CUserDeleteGroupDlg::DeleteGroupInfo(USER_GROUP_INFO *pGroupinfo)
{
	int bRet = false;
	
	
		bRet = H264_DVR_SetDevConfig(m_loginID,E_SDK_COFIG_DELETE_GROUP, 0, (char *)pGroupinfo,  sizeof(USER_GROUP_INFO));
		
		if ( bRet > 0 )
		{
			//�ҵ�ԭ�ȵ��û�
			for ( int i = 0 ; i < m_userinfo->groupNum; i ++ )
			{
				if ( !strcmp(pGroupinfo->name, m_userinfo->groupList[i].name) )
				{
					//��������û���Ϣȫ��ǰ��
					memcpy( &m_userinfo->groupList[i], &m_userinfo->groupList[i + 1], sizeof(USER_GROUP_INFO) * (m_userinfo->groupNum - i - 1) );
					m_userinfo->groupNum --;
					break;
				}
			}
		}
	
	
	return bRet;
}
//���ݴ���������ɾ���Ĵ�����Ϣ
CString CUserDeleteGroupDlg::GetSdkErrorInfo(int nError)
{
	CString strMsg(_CS("Error.UnkownErr"));
	switch(nError)
	{
	case H264_DVR_SDK_NOTVALID:			//�Ƿ�����
		strMsg = _CS("Error.ReqErr");
		break;
	case H264_DVR_NO_INIT:				//SDKδ����ʼ��
		strMsg = _CS("Error.NoInit");
		break;
	case H264_DVR_ILLEGAL_PARAM:			//�û��������Ϸ�
		strMsg = _CS("Error.ParamInvalid");
	    break;
	case H264_DVR_INVALID_HANDLE:		//�����Ч
		strMsg = _CS("Error.HandleInvalid");
	    break;
	case H264_DVR_SDK_UNINIT_ERROR:		//SDK�������
		strMsg = _CS("Error.ClearSDKError");
		break;
	case H264_DVR_SDK_TIMEOUT:			//�ȴ���ʱ
		strMsg = _CS("Error.Timeout");
		break;
	case H264_DVR_SDK_MEMORY_ERROR:		//�ڴ���󣬴����ڴ�ʧ��
		strMsg = _CS("Error.MallocErr");
	    break;
	case H264_DVR_SDK_NET_ERROR:			//�������
		strMsg = _CS("Error.NetErr");
	    break;
	case H264_DVR_DEV_VER_NOMATCH:		//�յ����ݲ���ȷ�����ܰ汾��ƥ��
		strMsg = _CS("Error.VerUnmatch");
	    break;
	case H264_DVR_SDK_NOTSUPPORT:		//�汾��֧��
		strMsg = _CS("Error.Unsupport");
	    break;
	case H264_DVR_OPEN_CHANNEL_ERROR:	//��ͨ��ʧ��
		strMsg = _CS("Error.OpenChnErr");
	    break;
	case H264_DVR_CLOSE_CHANNEL_ERROR:	//�ر�ͨ��ʧ��
		strMsg = _CS("Error.CloseChnErr");
	    break;
	case H264_DVR_SUB_CONNECT_ERROR:		//����ý��������ʧ��
		strMsg = _CS("Error.SubConErr");
	    break;
	case H264_DVR_SUB_CONNECT_SEND_ERROR://ý��������ͨѶʧ��
		strMsg = _CS("Error.SubConSendErr");
	    break;
	case H264_DVR_NOPOWER:				//��Ȩ��
		strMsg = _CS("Error.NoPower");
	    break;
	case H264_DVR_PASSWORD_NOT_VALID:	// �˺����벻��
		strMsg = _CS("Error.PwdErr");
	    break;
	case H264_DVR_LOGIN_USER_NOEXIST:	//�û�������
		strMsg = _CS("Error.UserNotExisted");
	    break;
	case H264_DVR_USER_LOCKED:			// ���û�������
		strMsg = _CS("Error.UserLock");
	    break;
	case H264_DVR_USER_IN_BLACKLIST:		// ���û����������(�ں�������)
		strMsg = _CS("Error.InBlackList");
	    break;
	case H264_DVR_USER_HAS_USED:			// ���û��ѵ�½
		strMsg = _CS("Error.HasLogined");
	    break;
	case H264_DVR_USER_NOT_LOGIN	:		// ���û�û�е�½
		strMsg = _CS("Error.NoLogin");
	    break;
	case H264_DVR_CONNECT_DEVICE_ERROR:	//�����豸������
		strMsg = _CS("Error.NotFound");
	    break;
	case H264_DVR_ACCOUNT_INPUT_NOT_VALID:	//�û��������벻�Ϸ�
		strMsg = _CS("Error.InputError");
	    break;
	case H264_DVR_ACCOUNT_OVERLAP:			//�����ظ�
		strMsg = _CS("AccountMSG.AccountUsed");
	    break;
	case H264_DVR_ACCOUNT_OBJECT_NONE:		//�����ڶ���, ���ڲ�ѯʱ
		strMsg = _CS("AccountMSG.EmptyName");
	    break;
	case H264_DVR_ACCOUNT_OBJECT_NOT_VALID:	//�����ڶ���
		strMsg = _CS("Error.NotFound");
	    break;
	case H264_DVR_ACCOUNT_OBJECT_IN_USE:		//��������ʹ��
		strMsg = _CS("Error.InUsing");
	    break;
	case H264_DVR_ACCOUNT_SUBSET_OVERLAP:	//�Ӽ�����Χ (�����Ȩ�޳���Ȩ�ޱ��û�Ȩ�޳������Ȩ�޷�Χ�ȵ�)
		strMsg = _CS("AccountMSG.TooLargeAuthority");
	    break;
	case H264_DVR_ACCOUNT_PWD_NOT_VALID:		//���벻��ȷ
		strMsg = _CS("Error.PwdErr");
	    break;
	case H264_DVR_ACCOUNT_PWD_NOT_MATCH:		//���벻ƥ��
		strMsg = _CS("AccountMSG.PasswordMismatch");
	    break;
	case H264_DVR_ACCOUNT_RESERVED:			//�����ʺ�
		strMsg = _CS("AccountMSG.TryModifyResvUser");
	    break;		
	case H264_DVR_OPT_FILE_ERROR:			// д�ļ�����
		strMsg = _CS("Error.SetConfigFail");
	    break;	
	case H264_DVR_OPT_CAPS_ERROR:			// �������Բ�֧��
		strMsg = _CS("Error.ConfigCharsetUnsupport");
	    break;
	case H264_DVR_OPT_VALIDATE_ERROR:		// ����У��ʧ��
		strMsg = _CS("Error.ConfigVerifyFail");
	    break;
	case H264_DVR_OPT_CONFIG_NOT_EXIST:		// ����������õ����ò�����
		strMsg = _CS("Error.ConfigNotFound");
	    break;	
	case H264_DVR_CTRL_PAUSE_ERROR:			//��ͣʧ��
		strMsg = _CS("Error.PauseFail");
	    break;
	default:
		{
			//���û�У��򷵻��豸�˴�����
			CString strError;
			strError.Format(_T("(%d)"),  nError * ( -1 ) ); //����ԭ���ķ���ֵ���෴ֵȡԭ���ķ���ֵ
			strMsg += strError;
		}
	    break;
	}

	return strMsg;
}
