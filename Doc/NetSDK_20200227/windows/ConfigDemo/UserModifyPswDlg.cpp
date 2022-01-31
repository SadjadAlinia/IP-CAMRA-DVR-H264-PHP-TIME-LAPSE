// UserModifyPswDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserModifyPswDlg.h"





// CUserModifyPswDlg �Ի���

IMPLEMENT_DYNAMIC(CUserModifyPswDlg, CDialog)

CUserModifyPswDlg::CUserModifyPswDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserModifyPswDlg::IDD, pParent)
	, m_strUser(_T(""))
	, m_strConfirm(_T(""))
	, m_strNewpsw(_T(""))
	, m_strOldpsw(_T(""))
{
   	
}

CUserModifyPswDlg::~CUserModifyPswDlg()
{
}

void CUserModifyPswDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USER, m_cmbUser);
	DDX_CBString(pDX, IDC_COMBO_USER, m_strUser);
	DDX_Control(pDX, IDC_EDIT_CONFIRM, m_editCofirm);
	DDX_Text(pDX, IDC_EDIT_CONFIRM, m_strConfirm);
	DDX_Control(pDX, IDC_EDIT_NEWPSW, m_editNewpsw);
	DDX_Text(pDX, IDC_EDIT_NEWPSW, m_strNewpsw);
	DDX_Control(pDX, IDC_EDIT_OLDPSW, m_editOldpsw);
	DDX_Text(pDX, IDC_EDIT_OLDPSW, m_strOldpsw);
}


BEGIN_MESSAGE_MAP(CUserModifyPswDlg, CDialog)
	ON_BN_CLICKED(IDOK, CUserModifyPswDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserModifyPswDlg ��Ϣ�������

BOOL CUserModifyPswDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	_CWndCS(this);

	m_editCofirm.SetLimitText(6);
	m_editNewpsw.SetLimitText(6);
	m_editOldpsw.SetLimitText(6);

	//��ȡ�û���Ϣ

		USER_MANAGE_INFO *pUser = m_userinfo;
		for ( int i = 0; i < pUser->userNum; i ++ )
		{
			if ( strcmp(pUser->userList[i].name, "") != 0)
			{
				m_cmbUser.AddString( pUser->userList[i].name );
			}
		}

	m_cmbUser.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUserModifyPswDlg::OnBnClickedOk()
{
	UpdateData();

	//��ȡ��ǰ�û���Ϣ

		USER_INFO*pUserinfo = GetUserInfo(m_strUser.GetBuffer(0));
		if ( pUserinfo )
		{
			char szMD5[100] = {0};
			H264_DVR_EncryptPassword((char *)szMD5, m_strOldpsw.GetBuffer(0));
		//	MD5Encrypt( szMD5, (unsigned char*)m_strOldpsw.GetBuffer(0));
			//��֤����
			if ( strcmp((char *)szMD5, pUserinfo->passWord) )
			{
				MessageBox(_CS("Error.PwdErr"), _CS("OneClick.Prompt"));
				return;
			}

			if ( m_strNewpsw != m_strConfirm )
			{
				MessageBox(_CS("Error.PasswordMismatch"), _CS("OneClick.Prompt"));
				return;
			}
		}

		char szMD5[100] = {0};
		H264_DVR_EncryptPassword((char *)szMD5, m_strNewpsw.GetBuffer(0));
	//	MD5Encrypt( szMD5, (unsigned char*)m_strNewpsw.GetBuffer(0));

		_CONF_MODIFY_PSW psw;
		strcpy ( psw.sUserName, m_strUser.GetBuffer(0) );
		strcpy ( psw.Password, pUserinfo->passWord );
		strcpy ( psw.NewPassword, (char *)szMD5 );

	
			//�����û�	
			int bRet = Modifypsw(&psw);
			if ( bRet <= 0 )
			{
				CString strMsg(_CS("Error.SetConfigFail"));
				CString strError = GetSdkErrorInfo( bRet );
				strMsg += strError;
				MessageBox(strMsg, _CS("OneClick.Prompt"));
				return;
			}
			else
			{
				MessageBox(_CS("Error.SaveSuccess"), _CS("OneClick.Prompt"));
			}
		


	OnOK();
}
USER_INFO* CUserModifyPswDlg::GetUserInfo(char * strName)
{
	for ( int i = 0; i < m_userinfo->userNum; i ++ )
	{
		if ( !strcmp(strName, m_userinfo->userList[i].name) )
		{
			return &m_userinfo->userList[i];
		}
	}
	
	return NULL;
}
//���ݴ���������ɾ���Ĵ�����Ϣ
CString CUserModifyPswDlg::GetSdkErrorInfo(int nError)
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
	case H264_DVR_USER_HAS_USED:			// ���û��Ե�½
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
int CUserModifyPswDlg::Modifypsw(_CONF_MODIFY_PSW *pPsw)
{
	int bRet = false;
	

		bRet = H264_DVR_SetDevConfig(m_loginID,E_SDK_CONFIG_MODIFY_PSW, 0, (char *)pPsw, sizeof(_CONF_MODIFY_PSW));
		if ( bRet > 0 )
		{
			//�ҵ�ԭ�ȵ��û�
			for ( int i = 0; i < m_userinfo->userNum; i ++ )
			{
				if ( !strcmp(pPsw->sUserName, m_userinfo->userList[i].name) )
				{
					strcpy(m_userinfo->userList[i].passWord, pPsw->NewPassword);
					break;
				}
			}
		}

	
	return bRet;
}