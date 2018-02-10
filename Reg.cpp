#include "stdafx.h"
#include "..\include\Reg.h"
#include "../ProcExplorer/ProcExplorer.h"
#include "../include/Alloc.h"

CReg::CReg()
{
	m_Key = NULL;
}


CReg::~CReg()
{
	this->Close();
}

int CReg::Open()
{
	if (RegOpenKeyEx(HKEY_CURRENT_USER, GetKeyName(), REG_NONE,KEY_READ, &m_Key))
		return	FALSE;
	return	TRUE;
}

int CReg::Open(HKEY hKey, LPCTSTR lpszKeyName,BOOL bReadOnly)
{
	//if(RegOpenKeyEx(hKey, lpszKeyName, bReadOnly?KEY_READ:KEY_EXECUTE,&m_Key))
	//	return	FALSE;
	return	TRUE;
}


int CReg::GetValue(void*& pBuffer, DWORD& dwLength, LPCTSTR lpszKeyName)
{
	TAlloc<CHeapAlloc> alloc;
	DWORD dwType = REG_NONE;
	DWORD cbData = 0L;
	if (pBuffer == NULL||dwLength==0)
	{
		if (RegQueryValueEx(m_Key, lpszKeyName, NULL, &dwType, NULL, &cbData))
		{
			return FALSE;
		}
		void* pBuf = alloc.Allocate(cbData);
		if (!RegQueryValueEx(m_Key, lpszKeyName, NULL, &dwType, (BYTE*)pBuf,&cbData))
		{
			pBuffer = pBuf;
			dwLength = cbData;
			return	TRUE;
		}
		alloc.Free(pBuf);
	}
	ATLASSERT(AtlIsValidAddress(pBuffer, dwLength));
	if (RegQueryValueEx(m_Key, lpszKeyName, NULL, &dwType, (BYTE*)pBuffer, &cbData))
	{
		if (__ERROR__ == ERROR_OUTOFMEMORY)
		{
			void* pBuf = alloc.Allocate(cbData);
			if (!RegQueryValueEx(m_Key, lpszKeyName, NULL, &dwType, (BYTE*)pBuf, &cbData))
			{
				alloc.Free(pBuffer);
				pBuffer = pBuf;
				dwLength = cbData;
				return	TRUE;
			}
			alloc.Free(pBuf);
		}
		return FALSE;
	}
	dwLength = cbData;

	return	TRUE;
}


ULONG CReg::GetValueLength(LPCTSTR lpszKeyName)
{
	DWORD dwType = REG_NONE;
	DWORD cbData = 0L;
	if (!RegQueryValueEx(m_Key, lpszKeyName, NULL, &dwType, NULL, &cbData))
	{
		return cbData;
	}
	return -1;
}


void CReg::Close()
{
	if (m_Key)
	{
		RegCloseKey(m_Key);
		m_Key = NULL;
	}
}

ATL::CString CReg::GetStrValue(LPCTSTR lpszValueName)
{
	ATL::CString str;
	TCHAR szBuf[1024] = { 0 };
	DWORD cbSize = 1024 * sizeof(TCHAR);
	if (!RegQueryValueEx(m_Key, lpszValueName, NULL, NULL, (BYTE*)szBuf, &cbSize))
	{
		str = szBuf;
		return	str;
	}
	if (__ERROR__ == ERROR_OUTOFMEMORY)
	{

	}
	return str;
}

CProcExpReg::CProcExpReg()
{
	m_KeyName = gszMainProgramRegKeyName;
}


// 从注册表中读取一组整形数据到内存中来
LONG CReg::ReadInts(LPCTSTR lpszKeyName, int nSize, LPBYTE lpData)
{
	CReg temp;
	if (temp.Open(m_Key, lpszKeyName, TRUE))
	{
		TCHAR szName[16] = { 0 };
		INT* pValue = (INT*)lpData;
		for (int i = 0; i < nSize;i++)
		{
			_stprintf_s(szName, 16, _TEXT("%d"), i++);
			if (!temp.ReadInt(szName,&pValue[i])) 
				return	FALSE;
		}
		return	TRUE;
	}
	return 0;
}


// 从注册表中读取一个整形数据
BOOL CReg::ReadInt(LPCTSTR lpszName, int* pnValue, int nDefValue)
{
	LONG dwSize = sizeof(INT);
	//BOOL ret = (0 == RegQueryValueEx(m_Key, lpszName, REG_NONE, KEY_READ, (LPBYTE)pnValue, &dwSize));
	//if (!ret)
	//{
	//	*pnValue = nDefValue;
	//}
	//return ret;
	return	FALSE;
}

// 从注册表中读取一个宽度整形数据
int CReg::ReadWidth(LPCTSTR lpszKeyName, int* pnWidth, int nDefValue)
{
	int n = 0;
	if (!ReadInt(lpszKeyName, &n, nDefValue))
	{
		*pnWidth = nDefValue;
	}
	*pnWidth = MulDiv(*pnWidth, gLogPixelsX, 96);
	return	*pnWidth;
}

//	从注册表中读一组整形宽度数据
int CReg::ReadWidths(LPCTSTR lpszKeyName, int nSize, int* pnWidths)
{
	CReg temp;
	if (temp.Open(m_Key, lpszKeyName, TRUE))
	{
		TCHAR szName[16] = { 0 };
		for (int i = 0; i < nSize;i++)
		{
			_stprintf_s(szName, 16, _TEXT("%d"), i++);
			if (!temp.ReadWidth(szName, &pnWidths[i])) 
				return	FALSE;
		}
		return	TRUE;
	}
	return	FALSE;
}
