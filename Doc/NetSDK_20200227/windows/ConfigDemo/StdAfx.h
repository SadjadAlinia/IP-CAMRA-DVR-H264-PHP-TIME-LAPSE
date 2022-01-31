// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1AB56EE8_B58E_4EDA_B4FE_C579222C9EE9__INCLUDED_)
#define AFX_STDAFX_H__1AB56EE8_B58E_4EDA_B4FE_C579222C9EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#pragma warning(disable: 4786)
#include "define.h"

#include "multilanguage.h"

#pragma warning(disable:  4083 4514 4786 4800 )

#ifdef x64
#define ONTIMER_UINT UINT_PTR
#include "..\\bin_x64\\NetSdk.h"
#pragma comment (lib,"..\\bin_x64\\NetSdk") 
#include "..\\bin_x64\\H264Play.h"
#pragma comment (lib,"..\\bin_x64\\H264Play") 

#else

#define ONTIMER_UINT UINT
#include "..\\bin\\NetSdk.h"
#pragma comment (lib,"..\\bin\\NetSdk") 
#include "..\\bin\\H264Play.h"
#pragma comment (lib,"..\\bin\\H264Play") 
#endif
//device information
typedef struct _DEV_INFO 
{
	int	nListNum;		   //position in the list

	long lLoginID;			//login handle
	long lID;				//device ID
	char szDevName[60];		//device name
	char szIpaddress[15];		//device IP
	char szUserName[15];		//user name
	char szPsw[15];			//pass word
	int nPort;				//port number
	int nTotalChannel;		//total channel

	SDK_CONFIG_NET_COMMON NetComm;                  // net config
	H264_DVR_DEVICEINFO  NetDeviceInfo;

	BOOL bSerialID;//be SerialNumber login
	char szSerIP[DDNS_MAX_DDNS_IPSIZE];//server ip
	int nSerPort;           //server port
	char szSerialInfo[DDNS_MAX_SERIALINFO_SIZE];  //SerialNumber
	int devctype;
}DEV_INFO;

typedef struct _DEV_ATTRIBUTE  
{
	H264_DVR_DEVICEINFO deviveInfo;
	SDK_CONFIG_NORMAL   NormalConfig;
}DEV_ATTRIBUTE;

typedef struct
{
	SDK_MOTIONCONFIG_ALL MotionCfgAll;
	SDK_BLINDDETECTCONFIG_ALL DectectCfgAll;
	SDK_VIDEOLOSSCONFIG_ALL VideoLossCfgAll;
	SDK_ALARM_INPUTCONFIG_ALL AlarmInCfgAll;
	SDK_AlarmOutConfigAll  AlarmOutCfgAll;
	SDK_NETALARMCONFIG_ALL AlarmRmCfgAll;
	

}ALARMCONFIGALL;

typedef struct  
{
	SDK_ChannelNameConfigAll ChanNameCfgAll;
	SDK_GUISetConfig GUISetCfg;
	SDK_VideoWidgetConfigAll   VideoWdtCfgAll;
    SDK_BlindDetectFunction BlindDetFunc[NET_MAX_CHANNUM];
	SDK_VGAResolutionAbility VGAResolutionAbility;
}OUTPUTCONFIGALL;

enum LanguageTypes
{
		ENGLISH,		//Ӣ��
		CHINESE_S,		//��������
		CHINESE_T,		//��������
		ITALIAN,		//�������
		SPANISH,		//��������
		JAPANESE,		//����
		RUSSIAN,		//����
		FRENCH,			//����
		GERMAN,			//����
		PORTUGAL,		//��������
		TURKEY,			//��������
		POLAND,			//������
		ROMANIAN,       //��������
		HUNGARIAN,      //��������
		FINNISH,		//������
		ESTONIAN,	    //��ɳ������
		KOREAN,		    //����
		FARSI,          //��˹�� 
		DANSK,			//������
		THAI,			//̩����
		GREEK,			//ϣ����
		VIETNAMESE,		//Խ����
		UKRAINIAN,      //�ڿ�����
		BRAZILIAN,      //������
		HEBREW,         //ϣ������
		INDONESIAN,     //ӡ����
		ARABIC,         //��������
		SWEDISH,        //�����
		CZECH,          //�ݿ���
		BULGARIAN,      //��������
		SLOVAKIA,		//˹�工����
		DUTCH,			//������
		LANG_NR
};

struct ConfigPair
{
	const char* name;
	int value;
};


static ConfigPair s_language[]= 
{
	{"English", ENGLISH},
	{"SimpChinese", CHINESE_S},
	{"TradChinese", CHINESE_T},
	{"Italian", ITALIAN},
	{"Spanish", SPANISH},
	{"Japanese", JAPANESE},
	{"Russian", RUSSIAN},
	{"French", FRENCH},
	{"German", GERMAN},
	{"Portugal",PORTUGAL},
	{"Turkey",TURKEY},
	{"Poland",POLAND},
	{"Romanian",ROMANIAN},
	{"Hungarian",HUNGARIAN},
	{"Finnish",FINNISH},
	{"Estonian",ESTONIAN},
	{"Korean",KOREAN},
	{"Farsi",FARSI},
	{"Dansk",DANSK},
	{"Thai", THAI},
	{"Greek", GREEK},
	{"Vietnamese", VIETNAMESE},
	{"Ukrainian", UKRAINIAN},
	{"Brazilian", BRAZILIAN},
	{"Hebrew", HEBREW},
	{"Indonesian", INDONESIAN},
	{"Arabic", ARABIC},
	{"Swedish", SWEDISH},
	{"Czech", CZECH},
	{"Bulgarian", BULGARIAN},
	{"Slovakia", SLOVAKIA},
	{"Dutch", DUTCH},
	{NULL,}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1AB56EE8_B58E_4EDA_B4FE_C579222C9EE9__INCLUDED_)
