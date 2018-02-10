#include "stdafx.h"
#include "..\include\Security.h"


CSecurity::CSecurity()
{
}


CSecurity::~CSecurity()
{
}

extern PSIDDATA gSIDDATA;
extern TCHAR gpszTargetName[];
////
bool QuerySID(PSID Sid, 
	LPTSTR Name, 
	DWORD cchName, 
	LPTSTR ReferencedDomainName, 
	DWORD cchReferencedDomainName)
{
	SIDDATA *pSID; 
	SIDDATA *Buffer; 
	DWORD Length; 
	SID_NAME_USE peUse;

	if (gSIDDATA)
	{
		//扫描现有队列是否有合适的
		pSID = gSIDDATA;
		while (!EqualSid(pSID->m_SID, Sid))
		{
			pSID = pSID->m_Next;
			if (!pSID)
				break;
		}
		if (pSID)
		{
			_tcscpy_s(Name, cchName, pSID->m_Name);
			_tcscpy_s(ReferencedDomainName, cchReferencedDomainName, pSID->m_DomainName);
			return	true;
		}
	}
	
	if (LookupAccountSid(gpszTargetName, Sid, Name, &cchName, ReferencedDomainName, &cchReferencedDomainName, &peUse))
	{
		//创建一个新SID账号信息
		Buffer = (SIDDATA*)malloc(sizeof(SIDDATA));
		Length = GetLengthSid(Sid);
		Buffer->m_SID = malloc(Length);
		CopySid(Length, Buffer->m_SID, Sid);
		_tcscpy_s(Buffer->m_Name, _MAX_PATH, Name);
		_tcscpy_s(Buffer->m_DomainName, _MAX_PATH, ReferencedDomainName);
		///插入到队列末尾
		Buffer->m_Next = gSIDDATA;
		gSIDDATA = Buffer;
		return	true;
	}
	return	false;
}

BOOL FormatSID(PSID pSid, TCHAR *szText, DWORD *pdwLength)
{
	BOOL result; // eax
	PSID_IDENTIFIER_AUTHORITY pIdentity; // edi
	DWORD dwLength; // ecx
	int nRet; // eax
	int res; // eax
	DWORD dwIndex; // edi
	int nOffset; // esi
	PDWORD pdwAuth; // eax
	DWORD dwCount; // [esp+4h] [ebp-4h]

	result = IsValidSid(pSid);
	if (result)
	{
		pIdentity = GetSidIdentifierAuthority(pSid);
		dwCount = *GetSidSubAuthorityCount(pSid);
		dwLength = 12 * dwCount + 28;
		if (*pdwLength >= dwLength)
		{
			nRet = _stprintf_s(szText, *pdwLength, _TEXT("S-%lu-"), 1);
			if (pIdentity->Value[0] || pIdentity->Value[1])
				res = _stprintf_s(
					&szText[nRet],
					*pdwLength - nRet,
					_TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
					pIdentity->Value[0],
					pIdentity->Value[1],
					pIdentity->Value[2],
					pIdentity->Value[3],
					pIdentity->Value[4],
					pIdentity->Value[5]);
			else
				res = _stprintf_s(
					&szText[nRet],
					*pdwLength - nRet,
					_TEXT("%lu"),
					pIdentity->Value[5]
					+ ((pIdentity->Value[4] + ((pIdentity->Value[3] + (pIdentity->Value[2] << 8)) << 8)) << 8));
			dwIndex = 0;
			nOffset = res + nRet;
			if (dwCount)
			{
				do
				{
					pdwAuth = GetSidSubAuthority(pSid, dwIndex++);
					nOffset += _stprintf_s(&szText[nOffset], *pdwLength - nOffset, _TEXT("-%lu"), *pdwAuth);
				} while (dwIndex < dwCount);
			}
			result = TRUE;
		}
		else
		{
			*pdwLength = dwLength;
			SetLastError(ERROR_INSUFFICIENT_BUFFER);
			result = FALSE;
		}
	}
	return result;
}