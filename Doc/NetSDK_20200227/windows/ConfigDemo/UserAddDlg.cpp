// UserAddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserAddDlg.h"


// CUserAddDlg �Ի���



IMPLEMENT_DYNAMIC(CUserAddDlg, CDialog)

CUserAddDlg::CUserAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserAddDlg::IDD, pParent)
	, m_strConfirm(_T(""))
	, m_strDescribe(_T(""))
	, m_strPsw(_T(""))
	, m_strName(_T(""))
	, m_strGroupName(_T(""))
{
   	
}

CUserAddDlg::~CUserAddDlg()
{
}

void CUserAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_REUSEABLE, m_checkReuseable);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_cmbGroup);
	DDX_Control(pDX, IDC_EDIT_CONFIRM, m_editConfirm);
	DDX_Text(pDX, IDC_EDIT_CONFIRM, m_strConfirm);
	DDX_Control(pDX, IDC_EDIT_DESCRIBE, m_editDescribe);
	DDX_Text(pDX, IDC_EDIT_DESCRIBE, m_strDescribe);
	DDX_Control(pDX, IDC_EDIT_PSW, m_editPsw);
	DDX_Text(pDX, IDC_EDIT_PSW, m_strPsw);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editName);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strName);
	DDX_Control(pDX, IDC_LIST1, m_listAuthority);
	DDX_CBString(pDX, IDC_COMBO_GROUP, m_strGroupName);
}


BEGIN_MESSAGE_MAP(CUserAddDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, CUserAddDlg::OnCbnSelchangeComboGroup)
	ON_BN_CLICKED(IDOK, CUserAddDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserAddDlg ��Ϣ�������
BOOL CUserAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_CWndCS(this);
	
	//��ʾ�ַ�������ʵ�������ں��豸�Ľ��������ַ�����ʽ������Ҳû̫���Ҫ����
	m_editName.SetLimitText(8);
	m_editPsw.SetLimitText(6);
	m_editConfirm.SetLimitText(6);
	m_editDescribe.SetLimitText(31);
	m_listAuthority.SetExtendedStyle(m_listAuthority.GetExtendedStyle() | LVS_EX_CHECKBOXES );
	m_listAuthority.InsertColumn(0, _CS("UserManager.No"), LVCFMT_LEFT, 50);
	m_listAuthority.InsertColumn(1, _CS("UserManager.Authority"), LVCFMT_LEFT, 200);
	
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
	m_cmbGroup.GetWindowText(m_strGroupName);
	UpdateAuthorityList(m_strGroupName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

USER_GROUP_INFO* CUserAddDlg::GetGroupInfo(char * strName)
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
void CUserAddDlg::UpdateAuthorityList(CString strGroup)
{
	
		char szIndex[5];

		int nIndex = 0;
		USER_GROUP_INFO *pGroup = GetGroupInfo(strGroup.GetBuffer(0));
		if ( pGroup )
		{
			for ( int i = 0; i < pGroup->rigthNum; i ++ )
			{
				CString strRigths("Authority.");

				strRigths += pGroup->rights[i];
				sprintf(szIndex, "%d", i +  1);
				m_listAuthority.InsertItem(i,szIndex );
				m_listAuthority.SetItemText(i, 1, _CS(strRigths) );
				m_listAuthority.SetItemData(i, long(pGroup->rights[i]) );
				m_listAuthority.SetCheck(i, true);
			}

// 			sprintf(szIndex, "%d", pGroup->rigthNum );
// 			LVCOLUMN count;
// 			count.cx = 50;
// 			count.pszText = szIndex;
// 			m_listAuthority.SetColumn(0, &count);
		}
				
}

void CUserAddDlg::OnCbnSelchangeComboGroup()
{
	m_listAuthority.DeleteAllItems();

	UpdateData();
	CString strGroup;
	m_cmbGroup.GetWindowText(strGroup);

	UpdateAuthorityList(strGroup);
}

bool CUserAddDlg::FindUserName(char *szUserName)
{
	for ( int i = 0 ; i < m_userinfo->userNum; i ++ )
	{
		if ( !strcmp(szUserName, m_userinfo->userList[i].name ) )
		{
			return true;
		}
	}
	
	return false;	
}
void CUserAddDlg::OnBnClickedOk()
{
	UpdateData();

	m_strName.TrimLeft();
	m_strName.TrimRight();
	m_strGroupName.TrimLeft();
	m_strGroupName.TrimRight();
	
	//��֤����������Ƿ�Ϸ�
	if ( "" == m_strName )
	{
		MessageBox(_CS("AccountMSG.EmptyName"), _CS("OneClick.Prompt"));
		return;
	}
	if(m_strName[0]=='&')
	{
		
		MessageBox(_CS("AccountMSG.ErrorName"), _CS("OneClick.Prompt"));
		
		return;
	}
	if(m_strName.Find('.')!=-1||m_strName.Find('[')!=-1||m_strName.Find(']')!=-1 || m_strName.Find('%') != -1)
	{
		
		MessageBox(_CS("AccountMSG.ErrorName"), _CS("OneClick.Prompt"));
		return;
	}

	//��֤�����Ƿ�ƥ��
	if ( m_strPsw != m_strConfirm )
	{
		MessageBox(_CS("Error.PwdErr"), _CS("OneClick.Prompt"));
		return;
	}
	
	//��֤�û����Ƿ��Ѿ�����

	if ( FindUserName(m_strName.GetBuffer(0)) )
	{
		MessageBox(_CS("AccountMSG.AddUserExist"), _CS("OneClick.Prompt"));
		return;
	}

	USER_INFO userInfo;

	//��ȡѡ���Ȩ���б�
	int nIndex = 0;
	int nSize = m_listAuthority.GetItemCount();
	for ( int i = 0; i < nSize; i ++ )
	{
		if ( m_listAuthority.GetCheck(i) )
		{
			CString strName = (char *)m_listAuthority.GetItemData(i);
			strcpy( userInfo.rights[nIndex ++], strName.GetBuffer(0) );
		}
	}
	userInfo.rigthNum = nIndex;
	if ( nIndex <= 0 )
	{
		MessageBox(_CS("AccountMSG.EmptyAuthority"), _CS("OneClick.Prompt"));

		return;
	}

	userInfo.reserved = false;
	userInfo.shareable = m_checkReuseable.GetCheck();
	strcpy ( userInfo.Groupname, m_strGroupName.GetBuffer(0) );
	strcpy ( userInfo.memo, m_strDescribe.GetBuffer(0) );
	strcpy ( userInfo.name, m_strName.GetBuffer(0) );
	strcpy ( userInfo.passWord, m_strPsw.GetBuffer(0) );
	
		//�����û�	
		int bRet = SaveuserInfo(&userInfo);
		if ( bRet <= 0 )
		{
			CString strMsg(_CS("User.AddUserFailed"));
			CString strError = GetSdkErrorInfo( bRet );
			strMsg += strError;
			MessageBox(strMsg, _CS("OneClick.Prompt"));

			return;
		}
	

	MessageBox(_CS("AccountMSG.AddUserSuccess"), _CS("OneClick.Prompt"));
	OnOK();
}
//���ݴ���������ɾ���Ĵ�����Ϣ
CString CUserAddDlg::GetSdkErrorInfo(int nError)
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
int CUserAddDlg::SaveuserInfo(USER_INFO *pUserinfo)
{
	int bRet = 0;
		

		bRet = H264_DVR_SetDevConfig(m_loginID,E_SDK_CONFIG_ADD_USER, 0, (char *)pUserinfo, sizeof(USER_INFO));

	if ( bRet > 0 && m_userinfo)
	{
		//���¸����ڴ�����
		memcpy( &m_userinfo->userList[m_userinfo->userNum], pUserinfo, sizeof(USER_INFO) );
		m_userinfo->userNum ++;		
	}
	return bRet;
}
