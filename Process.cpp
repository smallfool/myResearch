#include "stdafx.h"
#include "..\include\Process.h"
#include "../include/Common.h"
#include "../include/System.h"
#include "../include/Driver.h"

LPFN_NtQueryInformationProcess gpfnNtQueryInformationProcess = (LPFN_NtQueryInformationProcess)GetProcAddress(GetModuleHandleW(L"NTDLL"), "NtQueryInformationProcess");
extern HMODULE ghNTDLLModule;
extern LPFN_NtQuerySystemInformation gpfnNtQuerySystemInformation;

CProcess::CProcess()
{
	//GetSystemProcessInfo(&gpSystemProcessInfo, &gdwSystemProcessInfoLength);
	//_SYSTEM_PROCESS_INFORMATION* pInfo = gpSystemProcessInfo;
	//DWORD dwIndex = 0;

	//while (TRUE)
	//{
	//	ATLTRACE(TEXT("Process: %02x, ClientId: 0x%08X, Name: \"%ls\"\r\n"), dwIndex, pInfo->UniqueProcessId, UnicodeString2String((UNICODE_STRING*)&pInfo->ImageName));
	//	dwIndex++;
	//	for (ULONG j = 0; j < pInfo->NumberOfThreads;j++)
	//	{
	//		ATLTRACE(TEXT("			Thead: %2x, ThreadId: 0x%08X, StartAddress: 0x%08X, ThreadState: 0x%08X, Win32StartAddress: 0x08X\r\n"), j, pInfo->Threads[j].ClientId, pInfo->Threads[j].StartAddress, pInfo->Threads[j].ThreadState, pInfo->Threads[j].Win32StartAddress);
	//	}
	//	if (!pInfo->NextEntryOffset)
	//		break;
	//	pInfo = (_SYSTEM_PROCESS_INFORMATION*)((PUCHAR)pInfo + pInfo->NextEntryOffset);
	//}
}


CProcess::~CProcess()
{
}

// 打开进程
BOOL CProcess::Open(DWORD dwPid, DWORD dwAccessMode)
{
	PROCESS_ALL_ACCESS;
	m_Handle= OpenProcess(dwAccessMode, FALSE, dwPid);
	if (!m_Handle)
		return FALSE;
	m_ProcessId = dwPid;
	return	TRUE;
}
//#pragma intrinsic(_mm_cvtss_si64x) 
//
//#pragma intrinsic (__shiftleft128, __shiftright128)  
//
//int main()
//{
//	unsigned __int64 i = 0x1I64;
//	unsigned __int64 j = 0x10I64;
//	unsigned __int64 ResultLowPart;
//	unsigned __int64 ResultHighPart;
//
//	ResultLowPart = i << 1;
//	ResultHighPart = __shiftleft128(i, j, 1);
//
//	// concatenate the low and high parts padded with 0's  
//	// to display correct hexadecimal 128 bit values  
//	printf_s("0x%02I64x%016I64x << 1 = 0x%02I64x%016I64x\n",
//		j, i, ResultHighPart, ResultLowPart);
//
//	ResultHighPart = j >> 1;
//	ResultLowPart = __shiftright128(i, j, 1);
//
//	printf_s("0x%02I64x%016I64x >> 1 = 0x%02I64x%016I64x\n",
//		j, i, ResultHighPart, ResultLowPart);
//}



BOOL CProcess::Query(DWORD dwInfoClass)
{

 


	//{
	//	__m128 a;
	//	__int64 b = 54;

	//	// _mm_load_ps requires an aligned buffer.  
	//	__declspec(align(16)) float af[4] =
	//	{ 101.25, 200.75, 300.5, 400.5 };

	//	// Load a with the floating point values.  
	//	// The values will be copied to the XMM registers.  
	//	a = _mm_load_ps(af);

	//	// Extract the first element of a and convert to an integer  
	//	b = _mm_cvtss_si64x(a);

	//	printf_s("%I64d\n", b);
	//}

	return FALSE;
}

BOOL CProcess::QueryVMCounters(_VM_COUNTERS* pData)
{
	ULONG ulRet;
	NTSTATUS s = (*gpfnNtQueryInformationProcess)(m_Handle, ProcessVmCounters,
		pData,
		sizeof(_VM_COUNTERS),
		&ulRet);
	return s == STATUS_SUCCESS;
}

BOOL CProcess::QueryIOCounters(_IO_COUNTERS* pData)
{
	ULONG ulRet;
	NTSTATUS s = (*gpfnNtQueryInformationProcess)(m_Handle, ProcessIoCounters,
		pData,
		sizeof(_IO_COUNTERS),
		&ulRet);
	return s == STATUS_SUCCESS;
}

BOOL CProcess::QueryHandleCount(DWORD64* pdwCount)
{
	ULONG ulRet;
	NTSTATUS s = (*gpfnNtQueryInformationProcess)(m_Handle, ProcessHandleCount,
		pdwCount,
		sizeof(DWORD64),
		&ulRet);
	return s == STATUS_SUCCESS;
}

BOOL CProcess::QueryBasicInfo(PROCESS_BASIC_INFORMATION* pPBI)
{
	ULONG ulRet;
	NTSTATUS s = (*gpfnNtQueryInformationProcess)(m_Handle, ProcessBasicInformation,
		pPBI,
		sizeof(PROCESS_BASIC_INFORMATION),
		&ulRet);
	return s == STATUS_SUCCESS;
}

BOOL CProcess::QueryProcessTimes(FILETIME* pTimes)
{
	ULONG ulRet;
	NTSTATUS s = (*gpfnNtQueryInformationProcess)(m_Handle, ProcessTimes,
		pTimes,
		sizeof(FILETIME)*4,
		&ulRet);
	return s == STATUS_SUCCESS;
}

bool gbHasBuiltinAndAnministrativePriviledge = true;


///检测能否创建自建管理员的权限
BOOL IsBuiltinAndAnministrative()
{
	PSID Sid = NULL;
	SID_IDENTIFIER_AUTHORITY IdentifierAuth = SECURITY_NT_AUTHORITY;
	IdentifierAuth.Value[4] = 0x500;
	HANDLE TokenHandle = NULL;
	DWORD dwRet = 0L;
	DWORD dwSize = 0;
	TOKEN_GROUPS *pTokenGroups = NULL;

	static bool bInited = false;
	if (!bInited)
	{
		OpenProcessToken(__THISPROCESS__, TOKEN_QUERY, &TokenHandle);
		// Create a SID for the BUILTIN\Administrators group
		AllocateAndInitializeSid(&IdentifierAuth, 2, SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0, &Sid);
		GetTokenInformation(TokenHandle, TokenGroups, NULL, 0, &dwSize);
		TOKEN_GROUPS* pTokenGroups = (TOKEN_GROUPS*)::malloc(dwSize);
		if (GetTokenInformation(TokenHandle, TokenGroups, (void*)pTokenGroups, dwSize, &dwRet))
		{
			gbHasBuiltinAndAnministrativePriviledge = false;
			SID_AND_ATTRIBUTES * pSidAndAttributes = (SID_AND_ATTRIBUTES*)&pTokenGroups->Groups[0];
			for (DWORD i = 0; i < pTokenGroups->GroupCount; i++)
			{
				if (pSidAndAttributes->Sid == Sid)
				{
					gbHasBuiltinAndAnministrativePriviledge = true;
					break;
				}
			}
		}
		free(pTokenGroups);
		FreeSid(Sid);
		CloseHandle(TokenHandle);
		bInited = true;
	}
	return gbHasBuiltinAndAnministrativePriviledge ? TRUE : FALSE;
}

TCHAR* gszWindowNameFound = NULL;
HWND ghWindowFound = NULL;
BOOL gbWindowFoundInited = FALSE;
DWORD gdwWindowNameFoundSize = 0L;


BOOL WINAPI EnumWindowsCallback(HWND hWnd, LPARAM dwPid)
{
	HWND hWndParent; // eax
	DWORD dwProcessId; // [esp+8h] [ebp-208h]
	TCHAR szText[256]; // [esp+Ch] [ebp-204h]

	if (GetParent(hWnd))
	{
		if (!GetParent(hWnd))
			return TRUE;
		hWndParent = GetParent(hWnd);
		if (IsWindowVisible(hWndParent))
			return TRUE;
	}
	if (!IsWindowVisible(hWnd))
		return TRUE;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwProcessId != (DWORD)dwPid)
		return TRUE;
	GetWindowText(hWnd, szText, 256);
	if (!*szText)
		return TRUE;
	if (gbWindowFoundInited)
	{
		*gszWindowNameFound = 0;
	}
	else
	{
		gbWindowFoundInited = 1;
		_tcsncpy_s(gszWindowNameFound, gdwWindowNameFoundSize, szText, 0xFFFFFFFF);
		ghWindowFound = hWnd;
	}
	return FALSE;
}
static CProcess theProcess;

_SYSTEM_PROCESS_INFORMATION* gpSystemProcessInfo = NULL;
DWORD gdwSystemProcessInfoLength = 0;

BOOL GetSystemProcessInfo(_SYSTEM_PROCESS_INFORMATION** ppv, DWORD* pdwSystemProcessInfoLength)
{
	FILETIME timeNow;
	TAlloc<CHeapAlloc> alloc;
	DWORD dwSize = 1000L;
	if (pdwSystemProcessInfoLength&&*pdwSystemProcessInfoLength)
		dwSize = *pdwSystemProcessInfoLength;
	if (!*ppv)
	{
		*ppv = (_SYSTEM_PROCESS_INFORMATION*)alloc.Allocate(dwSize);
	}
	GetSystemTimeAsFileTime(&timeNow);

	if (!gpfnNtQuerySystemInformation)
	{
		gpfnNtQuerySystemInformation = (LPFN_NtQuerySystemInformation)GetProcAddress(ghNTDLLModule, "NtQuerySystemInformation");
	}
	if ((*gpfnNtQuerySystemInformation)(SystemProcessInformation, *ppv, dwSize, pdwSystemProcessInfoLength))
	{
		do
		{
			alloc.Free(*ppv);
			dwSize += 1000;
			*ppv = (_SYSTEM_PROCESS_INFORMATION*)alloc.Allocate(dwSize);
		} while ((*gpfnNtQuerySystemInformation)(SystemProcessInformation, *ppv, dwSize, pdwSystemProcessInfoLength));
	}
	return	TRUE;
}