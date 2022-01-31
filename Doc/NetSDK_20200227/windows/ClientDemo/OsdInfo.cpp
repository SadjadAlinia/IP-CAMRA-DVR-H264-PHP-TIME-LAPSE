// OsdInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OsdInfo.h"
#include "ClientDemoDlg.h"


// COsdInfo �Ի���

IMPLEMENT_DYNAMIC(COsdInfo, CDialog)

COsdInfo::COsdInfo(CWnd* pParent /*=NULL*/)
	: CDialog(COsdInfo::IDD, pParent)
{
	m_pBuf=NULL;
	m_nWidth=0;
	m_nHeight=0;
	m_nChannelTotal=NET_MAX_CHANNUM;

	for (int i = 0; i < NET_MAX_CHANNUM; ++i)
	{
		m_edit[i] = NULL;
		m_static[i] = NULL;
	}
}

COsdInfo::~COsdInfo()
{
}

void COsdInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OSD, m_OsdInfo);
}


BEGIN_MESSAGE_MAP(COsdInfo, CDialog)
	ON_BN_CLICKED(IDOK, &COsdInfo::OnBnClickedOk)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COsdInfo ��Ϣ�������

void COsdInfo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveChannel();
	GetData();
	CClientDemoDlg*MainDlg = ((CClientDemoDlg*)AfxGetMainWnd());
	MainDlg->m_pBuf = m_pBuf;
	MainDlg->m_nWidth = m_nWidth;
	MainDlg->m_nHeigth = m_nHeight;
	m_OsdName.strEnc = SDK_wordEnc_UTF8;
	((CClientDemoDlg*)AfxGetMainWnd())->setOsdInfo(&m_OsdName);
	OnOK();
}

BOOL COsdInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	_CWndCS(this);
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void COsdInfo::CleanAll()
{

	int i = 0;
	for ( i = 0; i < m_nChannelTotal; ++i )
	{
		if ( m_edit[i] && m_static[i])
		{
			delete m_edit[i];
			delete m_static[i];
			m_edit[i] = NULL;
			m_static[i] = NULL;
		}
	}
}

void COsdInfo::InitDlgInfo(SDK_OSDInfoConfigAll *pChannelName,int nbyChanNum )
{

	m_nChannelTotal = nbyChanNum;
	if ( pChannelName != NULL)
	{
		memcpy( &m_OsdName ,pChannelName ,sizeof(m_OsdName));
	}else
	{
		memset( &m_OsdName ,0 ,sizeof(m_OsdName));
	}
	BOOL value=InitWindow();
	if (!value)
	{
		MessageBox("Init Wnd File!");
	}
}
BOOL COsdInfo::InitWindow()
{
	CRect rcStatic;
	CRect rcEdit;
	CString Num;
#define MAX_CHANNEL_PER_ROW 2
	int nSect = m_nChannelTotal / MAX_CHANNEL_PER_ROW;
	int nOffset = m_nChannelTotal % MAX_CHANNEL_PER_ROW;
	GetDlgItem(IDC_STATIC_OSD)->GetWindowRect(rcStatic);
	ScreenToClient(rcStatic);
	GetDlgItem(IDC_STATIC_OSD)->MoveWindow(&rcStatic);

	GetDlgItem(IDC_EDIT_OSD)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	GetDlgItem(IDC_EDIT_OSD)->MoveWindow(&rcEdit);

	GetDlgItem(IDC_STATIC_OSD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_OSD)->ShowWindow(SW_HIDE);
	rcStatic.left += 10;
	rcStatic.right += 10;
	rcStatic.bottom += 5;
	rcEdit.left += 10;
	rcEdit.right += 50;
	for (int i = 0; i < nSect; i++)
	{
		CRect orgrcStatic = rcStatic;
		CRect orgrcEdit = rcEdit;
		//	char *Num;
		for (int j = 0; j < MAX_CHANNEL_PER_ROW; j++)
		{

			Num.Format("%s%d",_CS("ConfigManage.CAM"),1+j+i*MAX_CHANNEL_PER_ROW);
			if (m_edit[j+i*MAX_CHANNEL_PER_ROW]==NULL&&m_static[j+i*MAX_CHANNEL_PER_ROW] == NULL)
			{

				m_edit[j+i*MAX_CHANNEL_PER_ROW] = new CEdit();
				m_static[j+i*MAX_CHANNEL_PER_ROW] = new CStatic();
				m_edit[j+i*MAX_CHANNEL_PER_ROW]->Create(SS_BLACKFRAME|WS_BORDER|WS_CHILD|WS_VISIBLE | ES_AUTOVSCROLL,orgrcEdit,this,IDC_EDIT_CNS+j+i*MAX_CHANNEL_PER_ROW);
				m_static[j+i*MAX_CHANNEL_PER_ROW]->Create(Num,WS_CHILD|WS_VISIBLE,orgrcStatic,this,IDC_STATIC_CNS+j+i*MAX_CHANNEL_PER_ROW);
				orgrcEdit.left += 245;
				orgrcEdit.right += 245;
				orgrcStatic.left += 240;
				orgrcStatic.right += 240;
				m_edit[j+i*MAX_CHANNEL_PER_ROW]->LimitText(53);
			}
		}
		if (i < nSect)
		{
			rcStatic.top += 30;
			rcStatic.bottom += 30;
			rcEdit.top += 30;
			rcEdit.bottom += 30;
		}
	}
	if (nOffset>0)
	{
		CRect orgrcStatic = rcStatic;
		CRect orgrcEdit = rcEdit;

		for (int i = 0; i < nOffset; i++)
		{
			Num.Format("%s%d",_CS("ConfigManage.CAM"),1+i+nSect*MAX_CHANNEL_PER_ROW);
			if (m_edit[i+nSect*MAX_CHANNEL_PER_ROW] == NULL&&m_static[i+nSect*MAX_CHANNEL_PER_ROW] == NULL)
			{

				m_edit[i+nSect*MAX_CHANNEL_PER_ROW] = new CEdit();
				m_static[i+nSect*MAX_CHANNEL_PER_ROW] = new CStatic();
				m_edit[i+nSect*MAX_CHANNEL_PER_ROW]->Create(SS_BLACKFRAME|WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL,orgrcEdit,this,IDC_EDIT_CNS+i+nSect*MAX_CHANNEL_PER_ROW);
				m_static[i+nSect*MAX_CHANNEL_PER_ROW]->Create(Num,WS_CHILD|WS_VISIBLE,orgrcStatic,this,IDC_STATIC_CNS+i+nSect*MAX_CHANNEL_PER_ROW);

				orgrcEdit.left += 245;
				orgrcEdit.right += 245;
				orgrcStatic.left += 240;
				orgrcStatic.right += 240;
				m_edit[i+nSect*MAX_CHANNEL_PER_ROW]->LimitText(53);
			}
		}
	}
	return TRUE;
}

void COsdInfo::SaveChannel()
{
	CString ChannelName;
	sdkRect Osd22;
	Osd22.top = (int)((double)(((double)40/(double)450)*8192));
	Osd22.left = (int)((double)(((double)20/(double)385)*8192));
	SDK_OSDInfoConfig Osd21;
	memcpy(&Osd21.OSDInfoWidget.rcRelativePos,&Osd22,sizeof(Osd22));
	Osd21.OSDInfoWidget.rgbaBackground = 0xf00000ff;		/// ������ɫ��16���Ʊ�ʾΪ0xAABBGGRR��A(Alpha)��ʾ͸���ȡ�
	Osd21.OSDInfoWidget.rgbaFrontground = 0x40000000;			/// ǰ����ɫ��16���Ʊ�ʾΪ0xAABBGGRR��A(Alpha)��ʾ͸����,0��ʾȫ͸����
	Osd21.OSDInfoWidget.bEncodeBlend = true;
	Osd21.OSDInfoWidget.bPreviewBlend = 0;
	for(int i = 0;i < NET_MAX_LINE_PER_OSD_AREA;i++)
		for (int j = 0;j <NET_CHANNEL_NAME_MAX_LEN;j++)
		{
			Osd21.ppInfo[i][j] = 0;
		}
	for (int i = 0; i < m_nChannelTotal; i++)
	{
		if (m_edit[i] != NULL)
		{
			m_edit[i]->GetWindowText(ChannelName);
			memcpy(&m_OsdName.vOSDInfoAll[i],&Osd21,sizeof(Osd21));
			strcpy( m_OsdName.vOSDInfoAll[i].ppInfo[0], ChannelName.GetBuffer(0) );
		}
	}
}

//��������ת��Ϊ�̶���������
FIXED FixedFromDouble(double d)
{
	long l;
	l = (long) (d * 65536L);
	return *(FIXED *)&l;
}
void SetMat(LPMAT2 lpMat)
{
	lpMat->eM11 = FixedFromDouble(2);
	lpMat->eM12 = FixedFromDouble(0);
	lpMat->eM21 = FixedFromDouble(0);
	lpMat->eM22 = FixedFromDouble(2);
}

//��ȡһ���ַ��ĵ�����Ϣ
int COsdInfo::GetBuffer(int ch, int nFont)
{
	freeData();

	//�������塣
	CFont   hFont; 
	hFont.CreateFont(nFont,0,0,0, 
		FW_NORMAL,0, 
		0,0,DEFAULT_CHARSET, 
		OUT_TT_PRECIS,CLIP_TT_ALWAYS,   
		PROOF_QUALITY   ,DEFAULT_PITCH,   
		"���� "); 

	//�������嵽��ǰ�豸��
	CDC*pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject( &hFont);

	//��������ͼ�α任����         
	MAT2 mat2;
	SetMat(&mat2);

	GLYPHMETRICS gm;

	//��ȡ����ַ�ͼ����Ҫ���ֽڵĴ�С��
	DWORD dwNeedSize = pDC->GetGlyphOutline(ch,GGO_BITMAP,&gm,0,NULL,&mat2);
	if (dwNeedSize > 0 && dwNeedSize < 0xFFFF)
	{
		if( ch == '\t' ) 
			return 4 * gm.gmCellIncX; 

		m_pBuf = (byte*)(malloc(dwNeedSize));
		if ( m_pBuf)
		{
			//��ȡ�ַ�ͼ�ε����ݵ���������
			pDC->GetGlyphOutline(ch,GGO_BITMAP,&gm,dwNeedSize,m_pBuf,&mat2);
			m_nWidth = gm.gmBlackBoxX;
			m_nHeight = gm.gmBlackBoxY;
		}
	}
	else if( dwNeedSize == 0xFFFFFFFF || dwNeedSize == 0 || ch < 0x21 ) 
	{ 
		//Ҫ���ɵ������ǿո��tabֻ�践�ؿ�� 
		if( ch == ' ') 
			return gm.gmCellIncX; 
		else   if( ch == '\t' ) 
			return 4 * gm.gmCellIncX; 
		else 
		{
			UINT nSubstituteChar = ' '; 
			pDC-> GetGlyphOutline( nSubstituteChar, GGO_NATIVE, &gm, 
				0, NULL, &mat2 ); 
			if( ch < 256 ) //���� 
				return gm.gmCellIncX; 
			else //���Ŀ������ո� 
				return 2*gm.gmCellIncX; 
		} 
	} 
	else
	{
		dwNeedSize = 0;
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	return dwNeedSize;
}
#include <winsock.h>	//htons��Ҫ
void COsdInfo::GetData()//��ȡ������Ϣ
{
	{
		if (m_nChannelTotal == 1)
		{
			//��ʱ��¼��Ϣ���߶����24
			char g_tmp[24][256] = {0};
			int g_len = 0;		//���������ȵ��ֽ���
			int g_Height = 24;	//����߶ȣ���ʱ�̶�

			int nFont = 12;
			int nSize = strlen(m_OsdName.vOSDInfoAll[0].ppInfo[0]);

			for ( int i = 0; i < nSize; )
			{
				int nRet = 0;
				if ( m_OsdName.vOSDInfoAll[0].ppInfo[0][i] >= 0 )
				{
					while(1)
					{
						nRet = GetBuffer(m_OsdName.vOSDInfoAll[0].ppInfo[0][i], nFont);
						if ( nRet && m_nHeight > g_Height )
						{
							nFont --;
						}
						else
							break;
					}
				}
				else
				{
					short *pbuf = (short *)(m_OsdName.vOSDInfoAll[0].ppInfo[0] + i);
					nRet = GetBuffer(htons(pbuf[0]), nFont);
					while(1)
					{
						nRet = GetBuffer(htons(pbuf[0]), nFont);
						if ( nRet && m_nHeight > g_Height )
						{
							nFont --;
						}
						else
							break;
					}

					i ++;
				}
				i ++ ;

				if ( nRet > 0 )
				{
					if ( m_pBuf == NULL )
					{
						//˵���������ַ���
						int widthByte = ((nRet +7) >> 3);	//��ȴ�����ֽ�������8Ϊ��λ
						g_len += widthByte;
					}
					else
					{
						//������õ�����߶ȴ���24��������Χ����С����
						int nPos = (g_Height-m_nHeight)/2;	//�߶�ƫ������ʹ���������
						int widthTrue = nRet/m_nHeight;		//ʵ��buf��ռ�õĿ���ֽ�
						int widthByte = ((m_nWidth +7) >> 3);	//���ֵ���ռ�õĿ�ȣ���8Ϊ��λ

						for ( int iTmp = 0; iTmp < m_nHeight; iTmp ++ )
						{
							memcpy( g_tmp[iTmp + nPos] + g_len, m_pBuf + iTmp*widthTrue, widthByte);
						}
						g_len += widthByte;
					}
				}
			}

			//ȷ����8�ı�����һ���ֽ�8λ
			int nLen = ((g_len +3) >> 2) << 2; 

			freeData();
			m_pBuf = new byte[g_Height*nLen];

			for ( int iTmp = 0; iTmp < g_Height; iTmp ++ )
			{
				memcpy(m_pBuf + iTmp*nLen, g_tmp[iTmp], nLen);
			}
			m_nWidth = nLen*8;
			m_nHeight = g_Height;

		}
	}
}

void COsdInfo::freeData()
{
	if ( m_pBuf )
	{
		free( m_pBuf );
		m_pBuf = NULL;
	}
}

void COsdInfo::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	CleanAll();
}
