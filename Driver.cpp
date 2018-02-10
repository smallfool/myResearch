#include "stdafx.h"
#include "..\include\Driver.h"
#include "../include/Common.h"
#include "../include/Alloc.h"
#include "../ProcExplorer/Resource.h"
#include "../include/System.h"

HMODULE ghNTDLLModule = GetModuleHandleW(L"NTDLL.DLL");
LPFN_RtlNtStatusToDosError gpfnRtlNtStatusToDosError = (LPFN_RtlNtStatusToDosError)GetProcAddress(ghNTDLLModule, "RtlNtStatusToDosError");
LPFN_RtlInitUnicodeString  gpfnRtlInitUnicodeString = (LPFN_RtlInitUnicodeString)GetProcAddress(ghNTDLLModule, "RtlInitUnicodeString");
LPFN_NtLoadDriver gpfnNtLoadDriver = (LPFN_NtLoadDriver) GetProcAddress(ghNTDLLModule, "NtLoadDriver");

extern CHAR* GetErrMsg(DWORD dwErr);


BOOL TmAdjustPrivilege(LPCTSTR lpName)
{
	BOOL bRet; // esi
	LUID Luid; // [esp+4h] [ebp-20h]
	HANDLE TokenHandle; // [esp+Ch] [ebp-18h]
	TOKEN_PRIVILEGES NewState; // [esp+10h] [ebp-14h]

	//OBJ_CASE_INSENSITIVE|OBJ_KERNEL_HANDLE
	NTSTATUS status = (*gpfnNtOpenProcessToken)(__THISPROCESS__, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, &TokenHandle);

	if (status == STATUS_SUCCESS)
	{
		if (!LookupPrivilegeValue(NULL, lpName, &Luid))
		{
			DbgPrintMessage(TRACE_ERROR, "LookupPrivilegeValue(\"%ls\") for error: %x", lpName, __ERROR__);
			return FALSE;
		}
	}
	NewState.Privileges[0].Luid = Luid;
	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	bRet = AdjustTokenPrivileges(TokenHandle, 0, &NewState, sizeof(TOKEN_PRIVILEGES), 0, 0);
	if (!bRet)
	{
		if (GetLastError())
			bRet = FALSE;
	}
	CloseHandle(TokenHandle);
	return bRet;
}

CDriver theProcExpDriver;
CDriver::CDriver()
{
	this->m_Driver = NULL;
}



CDriver::~CDriver()
{
	this->Close();
}

BOOL CDriver::Close()
{
	if (m_Driver != INVALID_HANDLE_VALUE)
	{
		return CloseHandle(m_Driver);
	}
	return	TRUE;
}

static TCHAR* SSZDRIVERNAMEFOR64X = _TEXT("PROCEXP152.SYS");
static TCHAR* SSZDRIVERNAMEFOR64XKEY = _TEXT("PROCEXP152");
static TCHAR* SSZDRIVERNAMEFORX86 = _TEXT("PROCEXP150");

BOOL CDriver::Open(LPCTSTR lpszName, HANDLE* phHandles)
{
	HANDLE Handle; // eax
	TCHAR szFileName[_MAX_PATH]; // [esp+8h] [ebp-20Ch]

	_stprintf_s(szFileName, _MAX_PATH, _TEXT("\\\\.\\%s"), lpszName);
	Handle = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	*phHandles = Handle;
	if (Handle == INVALID_HANDLE_VALUE)
	{
		_stprintf_s(szFileName, _MAX_PATH, _TEXT("\\\\.\\Global\\%s"), lpszName);
		*phHandles = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	}
	return phHandles[1]!= NULL;
}

BOOL CDriver::Open(LPCTSTR lpszName, LPCTSTR lpszPathName, HANDLE* phHandles)
{
	BOOL result; // eax
	HANDLE DriverHandle; // eax
	UNICODE_STRING strDriverName;
	const TCHAR* pszPathName;
	NTSTATUS status;
	HKEY hKey; 
	int dwData; 
	TCHAR szDriverName[_MAX_PATH]; 
	TCHAR szName[_MAX_PATH]; 
	TCHAR szFileName[_MAX_PATH]; 
	TCHAR szKeyName[_MAX_PATH];
	TCHAR szSubKeyName[_MAX_PATH];

	pszPathName = lpszPathName;
	TmAdjustPrivilege(_TEXT("SeImpersonatePrivilege"));
	TmAdjustPrivilege(_TEXT("SeSecurityPrivilege"));
	TmAdjustPrivilege(_TEXT("SeDebugPrivilege"));
	TmAdjustPrivilege(_TEXT("SeLoadDriverPrivilege"));

	_stprintf_s(szSubKeyName,_MAX_PATH, _TEXT("System\\CurrentControlSet\\Services\\%s"), lpszName);
	if (RegCreateKey(HKEY_LOCAL_MACHINE, szSubKeyName, &hKey))
	{
		DbgPrintMessage(TRACE_ERROR, "RegCreateKey(HKLM,\"%ls\") failed for error: %x(\"%s\")", szSubKeyName, __ERROR__, GetErrMsg(__ERROR__));
		return FALSE;
	}
	dwData = 1;
	RegSetValueEx(hKey, _TEXT("Type"), 0, REG_DWORD, (BYTE*)&dwData, sizeof(DWORD));
	dwData = 1;
	RegSetValueEx(hKey, _TEXT("ErrorControl"), 0, REG_DWORD, (BYTE*)&dwData, sizeof(DWORD));
	dwData = 3;
	RegSetValueEx(hKey, _TEXT("Start"), 0, REG_DWORD, (BYTE*)&dwData, sizeof(DWORD));
	_stprintf_s(szName,_MAX_PATH, _TEXT("\\??\\%s"), pszPathName);
	RegSetValueEx(hKey, _TEXT("ImagePath"), 0, REG_SZ, (BYTE*)szName, sizeof(TCHAR) * _tcslen(szName));
	RegCloseKey(hKey);

	_stprintf_s(szDriverName, _MAX_PATH, _TEXT("\\Registry\\Machine\\System\\CurrentControlSet\\Services\\%s"), lpszName);

	(*gpfnRtlInitUnicodeString)(&strDriverName, szDriverName);
	status = (*gpfnNtLoadDriver)(&strDriverName);

	_stprintf_s(szKeyName,_MAX_PATH, _TEXT("%s\\Enum"), szSubKeyName);
	RegDeleteKey(HKEY_LOCAL_MACHINE, szKeyName);
	_stprintf_s(szKeyName, _MAX_PATH, _TEXT("%s\\Security"), szSubKeyName);
	RegDeleteKey(HKEY_LOCAL_MACHINE, szKeyName);
	RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKeyName);

	if (status &&status != STATUS_IMAGE_ALREADY_LOADED)
	{
		SetLastError((*gpfnRtlNtStatusToDosError)(status));
		return FALSE;
	}
	else
	{
		_stprintf_s(szFileName,_MAX_PATH, L"\\\\.\\%s", lpszName);
		DriverHandle = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		*phHandles = DriverHandle;
		if (DriverHandle == INVALID_HANDLE_VALUE)
		{
			_stprintf_s(szFileName, _MAX_PATH, L"\\\\.\\Global\\%s", lpszName);
			*phHandles = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		}
		result = phHandles[1] != NULL;
	}
	return result;
}

BOOL CDriver::LoadDriverFromRes(UINT nID, LPCTSTR lpszFileName)
{
	HRSRC hResc; // eax
	HGLOBAL hResGlobal; // esi
	DWORD dwSize; // edi
	void *pBuffer; // esi
	FILE *pFile; // [esp+4h] [ebp-4h]

	TmAdjustPrivilege(_TEXT("SeImpersonatePrivilege"));
	TmAdjustPrivilege(_TEXT("SeSecurityPrivilege"));
	TmAdjustPrivilege(_TEXT("SeDebugPrivilege"));
	TmAdjustPrivilege(_TEXT("SeLoadDriverPrivilege"));
	hResc = FindResource(NULL, MAKEINTRESOURCE(nID), _TEXT("BIN"));
	if (!hResc)
		return FALSE;
	hResGlobal = LoadResource(NULL, hResc);
	dwSize = SizeofResource(NULL, hResc);
	pBuffer = LockResource(hResGlobal);
	if (_tfopen_s(&pFile, lpszFileName, _TEXT("wb")))
	{
		DbgPrintMessage(TRACE_ERROR, "OpenFile(\"%ls\") failed for error: %x.", lpszFileName, __ERROR__);
		return FALSE;
	}

	fwrite(pBuffer, 1, dwSize, pFile);
	fclose(pFile);
	return TRUE;
}


BOOL CDriver::Load()
{
	TCHAR szFileName[_MAX_PATH]; // [esp+0h] [ebp-414h]
	TCHAR szPathName[_MAX_PATH]; // [esp+208h] [ebp-20Ch]

	if (!this->Open(SSZDRIVERNAMEFOR64X, &m_Driver))
	{
		GetSystemDirectory(szFileName, _MAX_PATH);
		_stprintf_s(szPathName, _MAX_PATH, _TEXT("%s\\Drivers\\%s"), szFileName, SSZDRIVERNAMEFOR64X);
		if (!LoadDriverFromRes(IDR_BIN_DRIVER, szPathName))
		{
			_stprintf_s(szFileName, _MAX_PATH, _TEXT("%%TEMP%%\\%s"), SSZDRIVERNAMEFOR64X);
			ExpandEnvironmentStrings(szFileName, szPathName, _MAX_PATH);
			if (!LoadDriverFromRes(IDR_BIN_DRIVER, szPathName))
			{
				GetCurrentDirectory(_MAX_PATH, szFileName);
				_stprintf_s(szPathName, _MAX_PATH, _TEXT("%s\\%s"), szFileName, SSZDRIVERNAMEFOR64X);
				if (!LoadDriverFromRes(IDR_BIN_DRIVER, szPathName))
					return FALSE;
			}
		}
		if (!Open(SSZDRIVERNAMEFOR64XKEY, szPathName, &m_Driver))
		{
			DeleteFile(szPathName);
			return FALSE;
		}
		DeleteFile(szPathName);
	}
	return TRUE;
}

BOOL CDriver::IsIllegalVersion(DWORD dwVersion)
{
	if (m_Driver != INVALID_HANDLE_VALUE)
	{
		TAlloc<CVirtualAlloc> Alloc;
		void* OutBuffer = Alloc.Allocate(sizeof(DWORD));

		DWORD dwRet;
		if (!DeviceIoControl(m_Driver, CMD_VerifyVersion, &dwVersion, sizeof(DWORD), OutBuffer, sizeof(DWORD), &dwRet, NULL))
		{
			Alloc.Free(OutBuffer);
			return	FALSE;
		}
		DWORD dwVer = *(DWORD*)OutBuffer;
		Alloc.Free(OutBuffer);
		return	dwVer== dwVersion;
	}
	return	FALSE;
}


HANDLE CDriver::OpenProcess(DWORD dwDesiredAccess, DWORD dwProcessId)
{
	HANDLE Handle = NULL;
	DWORD dwLength = 0L;
	HANDLE ProcessHandle = ::OpenProcess(dwDesiredAccess, FALSE, dwProcessId);
	if (!ProcessHandle)
	{
		if (__ERROR__ == ERROR_ACCESS_DENIED)
		{
			DeviceIoControl(this->m_Driver, CMD_OpenThread, &dwProcessId, sizeof(DWORD), &ProcessHandle, sizeof(HANDLE), &dwLength, NULL);
		}
	}
	return	ProcessHandle;
}

///通过注册表检测
BOOL IsServer(LPCTSTR lpszName)
{
	BOOL ret = FALSE; 
	TCHAR *pBuffer; 
	TCHAR *pBuf;	
	DWORD dwType = REG_NONE;
	DWORD cbData = 0L; 
	HKEY hKey= NULL;

	// 本机值为 Terminal Server
	if (!RegOpenKey(HKEY_LOCAL_MACHINE, _TEXT("System\\CurrentControlSet\\Control\\ProductOptions"), &hKey)
		&& !RegQueryValueEx(hKey, _TEXT("ProductSuite"), NULL, &dwType, NULL, &cbData))
	{
		if (cbData)
		{
			pBuffer = (TCHAR*)LocalAlloc(64, cbData);
			if (!pBuffer)
			{
				if (hKey)
					RegCloseKey(hKey);
				return FALSE;
			}
			if (!RegQueryValueEx(hKey, _TEXT("ProductSuite"), NULL, &dwType, (BYTE*)pBuffer, &cbData)
				&& dwType == REG_MULTI_SZ)
			{
				pBuf = pBuffer;
				if (*pBuf)
				{
					while (_tcscmp(pBuf, lpszName))
					{
						pBuf += _tcslen(pBuf) + 1;
						if (!*pBuf)
							goto __exit;
					}
					ret = TRUE;
				}
			}
		__exit:
			LocalFree(pBuffer);
		}
	}
	if (hKey)
		RegCloseKey(hKey);
	return ret;

}


BOOL IsWinServer()
{
	OSVERSIONINFOEX osvi; 

	typedef DWORD (WINAPI* LPFN_GetVersion)();
	LPFN_GetVersion pfnGetVersion = (LPFN_GetVersion)GetProcAddress(__KERNEL__, "GetVersion");

	DWORD dwVersion		 = (*pfnGetVersion)();
	WORD dwOsVer		 = LOWORD(dwVersion);
	WORD dwBuild		 = HIWORD(dwVersion);

	DWORD dwMajorVersion = (DWORD)(LOBYTE(dwOsVer));
	DWORD dwMinorVersion = (DWORD)(HIBYTE(dwOsVer));


	if ((dwVersion & 0x80000000) != 0)
		return FALSE;

	if (dwVersion <= 4)	//WIN32
		return IsServer(_TEXT("Terminal Server"));

	//Windows 2000
	if (dwVersion >= 5 && dwVersion > 0x893)
		return TRUE;

	LPFN_VerSetConditionMask pfnVerSetConditionMask = (LPFN_VerSetConditionMask)GetProcAddress(__NTDLL__, "VerSetConditionMask");
	if (!pfnVerSetConditionMask)
		return FALSE;
	// VER_SUITENAME
	//wSuiteMask
	ULONGLONG res = (*pfnVerSetConditionMask)(((ULONGLONG)0), VER_SUITENAME, VER_AND);

	LPFN_VerifyVersionInfoA pfnVerifyVersionInfoA =(LPFN_VerifyVersionInfoA)GetProcAddress(__KERNEL__, "VerifyVersionInfoA");
	if (!pfnVerifyVersionInfoA)
		return FALSE;
	memset(&osvi, 0, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.wProductType = VER_SERVICEPACKMINOR;// 0x10;                     // VER_SUITENAME
	return (*pfnVerifyVersionInfoA)(&osvi, VER_SUITENAME, 0);     // VER_PRODUCT_TYPE
}



