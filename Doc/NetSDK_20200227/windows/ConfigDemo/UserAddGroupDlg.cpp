// UserAddGroupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "UserAddGroupDlg.h"





// CUserAddGroupDlg �Ի���

IMPLEMENT_DYNAMIC(CUserAddGroupDlg, CDialog)

CUserAddGroupDlg::CUserAddGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserAddGroupDlg::IDD, pParent)
	, m_strGroup(_T(""))
	, m_strDescribe(_T(""))
	, m_strName(_T(""))
{
	m_ntype = 0;	//Ĭ����ӣ�1 ���� �޸�
}

CUserAddGroupDlg::~CUserAddGroupDlg()
{
}

void CUserAddGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_cmbGroup);
	DDX_CBString(pDX, IDC_COMBO_GROUP, m_strGroup);
	DDX_Control(pDX, IDC_EDIT_DESCRIBE, m_editDescribe);
	DDX_Text(pDX, IDC_EDIT_DESCRIBE, m_strDescribe);
	DDX_Control(pDX, IDC_EDIT_GROUPNAME, m_editName);
	DDX_Text(pDX, IDC_EDIT_GROUPNAME, m_strName);
	DDX_Control(pDX, IDC_LIST1, m_listAuthority);
}


BEGIN_MESSAGE_MAP(CUserAddGroupDlg, CDialog)
	ON_BN_CLICKED(IDOK, CUserAddGroupDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, CUserAddGroupDlg::OnCbnSelchangeComboGroup)
	ON_BN_CLICKED(IDC_BTN_DELETE, CUserAddGroupDlg::OnBnClickedBtnDelete)
END_MESSAGE_MAP()


// CUserAddGroupDlg ��Ϣ�������

bool CUserAddGroupDlg::FindGroupName(char *szUserName)
{
	for ( int i = 0 ; i < m_userinfo->groupNum; i ++ )
	{
		if ( !strcmp(szUserName, m_userinfo->groupList[i].name ) )
		{
			return true;
		}
	}
	
	return false;	
}
void CUserAddGroupDlg::OnBnClickedOk()
{
	UpdateData();
	
	//��֤����������Ƿ�Ϸ�
	m_strName.TrimLeft();
	m_strName.TrimRight();

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

	if ( 1 == m_ntype )
	{
		if ( m_strName != m_strGroup )
		{
			//��֤�û����Ƿ��Ѿ�����			
				if ( FindGroupName(m_strName.GetBuffer(0)) )
				{
					MessageBox(_CS("AccountMSG.AddUserExist"), _CS("OneClick.Prompt"));
					return;
				}
			
		}
	}


	if ( m_ntype )
	{
		CONF_MODIFYGROUP groupInfo;
		strcpy ( groupInfo.GroupName, m_strGroup.GetBuffer(0) );
		//��ȡѡ���Ȩ���б�
		int nIndex = 0;
		int nSize = m_listAuthority.GetItemCount();
		for ( int i = 0; i < nSize; i ++ )
		{
			if ( m_listAuthority.GetCheck(i) )
			{
				CString strName("");
				strName = (char *)m_listAuthority.GetItemData(i);
				strcpy ( groupInfo.Group.rights[nIndex ++], strName.GetBuffer(0) );
			}
		}
		groupInfo.Group.rigthNum = nIndex;

		if ( nIndex <= 0 )
		{
			MessageBox(_CS("AccountMSG.EmptyAuthority"), _CS("OneClick.Prompt"));
			return;
		}

		strcpy ( groupInfo.Group.memo, m_strDescribe.GetBuffer(0) );
		strcpy ( groupInfo.Group.name, m_strName.GetBuffer(0) );

	
		//�����û�	
		int bRet = false;
		bRet = ModifyGroupInfo(&groupInfo);
		if ( bRet <= 0 )
		{
			MessageBox(_CS("Error.SetConfigFail"), _CS("OneClick.Prompt"));
			return;
		}
		
	}
	else
	{
		USER_GROUP_INFO groupInfo;
		//��ȡѡ���Ȩ���б�
		int nIndex = 0;
		int nSize = m_listAuthority.GetItemCount();
		for ( int i = 0; i < nSize; i ++ )
		{
			if ( m_listAuthority.GetCheck(i) )
			{
				CString strName("");
				strName = (char *)m_listAuthority.GetItemData(i);
				strcpy ( groupInfo.rights[nIndex ++], strName.GetBuffer(0) );
			}
		}
		groupInfo.rigthNum = nIndex;
		if ( nIndex <= 0 )
		{
			MessageBox(_CS("AccountMSG.EmptyAuthority"), _CS("OneClick.Prompt"));
			return;
		}

		strcpy ( groupInfo.memo, m_strDescribe.GetBuffer(0) );
		strcpy ( groupInfo.name, m_strName.GetBuffer(0) );


		
			//�����û�	
			int bRet = false;
			bRet = AddGroupInfo(&groupInfo);
			if ( bRet <= 0 )
			{
				MessageBox(_CS("Error.SetConfigFail"), _CS("OneClick.Prompt"));
				return;
			}
		
	}

	MessageBox(_CS("Error.SaveSuccess"), _CS("OneClick.Prompt"));
	OnOK();
}
int CUserAddGroupDlg::AddGroupInfo(USER_GROUP_INFO *pGroupinfo)
{
	int bRet = false;
	

		bRet = H264_DVR_SetDevConfig(m_loginID,E_SDK_CONFIG_ADD_GROUP, 0, (char *)pGroupinfo, sizeof(USER_GROUP_INFO));
		
		if ( bRet > 0 )
		{
			memcpy( &m_userinfo->groupList[m_userinfo->groupNum], pGroupinfo, sizeof(USER_GROUP_INFO) );
			m_userinfo->groupNum ++;
		}

	
	return bRet;
}
int CUserAddGroupDlg::ModifyGroupInfo(CONF_MODIFYGROUP *pGroupinfo)
{
	int bRet = false;
	
	
		bRet = H264_DVR_SetDevConfig(m_loginID,E_SDK_CONFIG_MODIFY_GROUP, 0, (char *)pGroupinfo,  sizeof(CONF_MODIFYGROUP));
		if ( bRet > 0 )
		{
			//�ҵ�ԭ�ȵ��û�
			for ( int i = 0; i < m_userinfo->groupNum ; i ++ )
			{
				if ( !strcmp(pGroupinfo->GroupName, m_userinfo->groupList[i].name) )
				{
					strcpy(m_userinfo->groupList[i].name, pGroupinfo->Group.name);
					strcpy(m_userinfo->groupList[i].memo, pGroupinfo->Group.memo);
					
					//����Ȩ��
					m_userinfo->groupList[i].rigthNum = 0;
					for ( int k = 0 ; k < pGroupinfo->Group.rigthNum; k ++ )
					{
						strcpy(m_userinfo->groupList[i].rights[k], pGroupinfo->Group.rights[k]);
					}
					m_userinfo->groupList[i].rigthNum = pGroupinfo->Group.rigthNum;
					break;
				}
			}
		}

	
	return bRet;
}


bool CUserAddGroupDlg::CheckRight(const char *szright, USER_GROUP_INFO *pUser)
{
	for ( int i = 0; i < pUser->rigthNum; i ++ )
	{
		if ( !strcmp( szright, pUser->rights[i] ) )
		{
			return true;
		}
	}

	return false;
}

USER_GROUP_INFO* CUserAddGroupDlg::GetGroupInfo(char * strName)
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
void CUserAddGroupDlg::UpdateAuthority(CString strGroup)
{
	//��ȡ��ǰ����Ϣ
	
		USER_GROUP_INFO *pGroupinfo = GetGroupInfo(strGroup.GetBuffer(0));
		if ( pGroupinfo )
		{
			m_editDescribe.SetWindowText(pGroupinfo->memo);

			int nSize = m_listAuthority.GetItemCount();

			CString strAuth;
			for ( int i = 0; i < nSize; i ++ )
			{
				strAuth = (char *)m_listAuthority.GetItemData(i);
				if ( CheckRight(strAuth.GetBuffer(0), pGroupinfo ) )
				{
					m_listAuthority.SetCheck(i);
				}
				else 
				{
					m_listAuthority.SetCheck(i, FALSE);
				}
			}
		}
	
}

void CUserAddGroupDlg::OnCbnSelchangeComboGroup()
{
	UpdateData();

	m_editName.SetWindowText(m_strGroup);

	UpdateAuthority(m_strGroup);
}

BOOL CUserAddGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	_CWndCS(this);

	m_editDescribe.SetLimitText(31);
	m_editName.SetLimitText(8);
	
	m_listAuthority.SetExtendedStyle(m_listAuthority.GetExtendedStyle() | LVS_EX_CHECKBOXES );
	//��ȡ�ܵ�Ȩ���б�
	//��ȡ����Ϣ
	//��ȡ����Ϣ
	
		int nIndex = 0;
		char szIndex[5];

		USER_MANAGE_INFO *pUer = m_userinfo;

		int nSize = pUer->rigthNum;
		sprintf(szIndex, "%d", nSize );
		m_listAuthority.InsertColumn(0, szIndex, LVCFMT_LEFT, 50);
		m_listAuthority.InsertColumn(1, _CS("UserManager.Authority"), LVCFMT_LEFT, 200 );
		
		for ( int i = 0; i < pUer->rigthNum; i ++ )
		{
			CString strName("Authority.");

			strName += pUer->rightList[i].name;
			sprintf(szIndex, "%d", i + 1);
			m_listAuthority.InsertItem(i, szIndex);
			m_listAuthority.SetItemText(i, 1, _CS(strName) );

			m_listAuthority.SetItemData(i, (long)pUer->rightList[i].name );
			m_listAuthority.SetCheck(i);
		}
	

	if ( 0 == m_ntype )
	{
		//����豸ʱ��������ѡ��
		GetDlgItem(IDC_STATIC_CMB)->ShowWindow(SW_HIDE);
		m_cmbGroup.ShowWindow(SW_HIDE);

		CRect rect;
	}
	else
	{
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

		UpdateAuthority(m_strGroup);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


bool CUserAddGroupDlg::HasUser(CString strGroup)
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
int CUserAddGroupDlg::DeleteGroupInfo(USER_GROUP_INFO *pGroupinfo)
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
CString CUserAddGroupDlg::GetSdkErrorInfo(int nError)
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

void CUserAddGroupDlg::OnBnClickedBtnDelete()
{
	UpdateData();

	int nIndex = m_cmbGroup.GetCurSel();

	if ( nIndex == -1 )
	{
		MessageBox(_CS("User.SelectOneUser"), _CS("OneClick.Prompt"));
		return;
	}
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
			}	
		}


	OnOK();
}
