////////////////////////////////////////////////////////////////////////////////////
// MultiLanguage.h
//*******************************    ����˵��    *********************************//
// �����Խ��滷���ĳ�ʼ���������£�
// ������������ĳ�ʼ�� ���� ���淽������ĳ�ʼ�� ���� ѡ��ǰ�Ľ��淽��

#ifndef _MULTILANGUAGE_H
#define _MULTILANGUAGE_H

#include <afxwin.h>         // MFC core and standard components
#include <map>

// Ϊ����ʹ�ö��������µĺ꣺
//...............................................................

// ���������ļ�ȫ��.
#define _LANGUAGE_INIT(x)			CMultiLanguage::SetIniFileName(x)
// �����ַ�����CString.
#define _CS(x)						CMultiLanguage::ConvertString(x)              // x Ϊ�ַ�����CString����  
// ����Ի�������ַ�.
#define _CWndCS(x)					CMultiLanguage::SetWndStaticText(x)  //���öԻ����еľ�̬�ı�(Ӣ��-->��ǰ����) // x Ϊ CWnd*ָ��

//...............................................................


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

template<class T>
struct strless{
	inline bool operator()(const T& x, const T& y) const
	{
		return (strcmp(x ,y) < 0);
	}
};

typedef std::map<const char *, const char *, strless<const char *> > MAPSTRING;

class CMultiLanguage
{
public:
	CMultiLanguage();
	virtual ~CMultiLanguage();
	static CString ConvertString(CString strText);
	static void SetWndStaticText(CWnd * pWnd);
	static void SetIniFileName(LPCTSTR szLanguageIniFileName );
	static CString GetCurrenLanguage();
protected:
	static CString m_strInilangFilePath;
};



#endif //_MULTILANGUAGE_H