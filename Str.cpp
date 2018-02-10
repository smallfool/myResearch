#include "stdafx.h"
#include "..\include\Str.h"
#include "../ProcExplorer/ProcExplorer.h"


CRegStr::CRegStr()
{
}


CRegStr::~CRegStr()
{
}
int CRegStr::Open()
{
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		this->m_strName,
		NULL,
		KEY_READ,
		&this->m_Key))

		return FALSE;
	return TRUE;
}


extern int gLogPixelsX;


int CRegStr::ReadWidth(LPCTSTR lpszName, WORD *pData, int nDefValue)
{
	int nValue = 0;
	DWORD cbData = 4;
	long ret = RegQueryValueEx(this->m_Key, lpszName, 0, REG_NONE, (BYTE*)&pData, &cbData);
	if (ret)
		nValue = nDefValue;
	else
		nValue = *pData;
	int result = MulDiv(nValue, gLogPixelsX, 96);
	*pData = LOWORD(result);
	return result;
}


ATL::CString UnicodeString2String(const UNICODE_STRING* UnicodeString)
{
	ATL::CString str;
	if (!UnicodeString || !UnicodeString->Length)
	{
		return TEXT("NONE");
	}
	TCHAR* pBuffer = str.GetBuffer(UnicodeString->Length+1);

#ifdef _UNICODE /*|| defined(UNICODE)*/
	CopyMemory(pBuffer, UnicodeString->Buffer, UnicodeString->Length+ UnicodeString->Length);
	pBuffer[UnicodeString->Length] = 0;
#else
//	WideStringToAscii()
#endif 

	str.ReleaseBuffer(-1);
	return	str;
}


