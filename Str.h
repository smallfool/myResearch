#pragma once

#include "ntdefs.h"

class CRegStr
{
public:
	CRegStr();
	~CRegStr();

	int Open();
	int ReadWidth(LPCTSTR lpszName, WORD *pData, int nDefValue);
public:
	BSTR	m_strName;
	HKEY	m_Key;
	DWORD	m_Length;
};

extern ATL::CString UnicodeString2String(const UNICODE_STRING* UnicodeString);


//class cHeap :public ATL::CStringT<WCHAR>
//{};

//class cHeap :public ATL::CRegKey
//{};
//class MyClass:public ATL::CAtlBaseModule
//{
//public:
//	MyClass();
//
//private:
//
//};

