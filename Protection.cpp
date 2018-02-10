#include "stdafx.h"
#include "..\include\Protection.h"


CProtection::CProtection()
{
	m_OldProtection = PAGE_READONLY;
}


CProtection::~CProtection()
{
}

BOOL CProtection::AddProtection(void* Address,DWORD Size,DWORD dwProtection)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	VirtualQuery(Address, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	m_OldProtection = mbi.Protect;
	DWORD dwRet = 0L;
	if (!VirtualProtect(Address, Size, m_OldProtection | dwProtection, &dwRet))
		return	FALSE;
	return	TRUE;
}
BOOL CProtection::RemoveProtection(DWORD dwProtection)
{
	return	TRUE;
}
