#include "stdafx.h"
#include "../include/Common.h"
#include "..\include\System.h"
#include "../include/Driver.h"
#include "../include/Process.h"
#include "../include/Alloc.h"

//#include <ntifs.h>
extern HMODULE ghNTDLLModule;
extern LPFN_NtQueryInformationProcess gpfnNtQueryInformationProcess;

LPFN_NtQuerySystemInformation gpfnNtQuerySystemInformation = (LPFN_NtQuerySystemInformation)GetProcAddress(ghNTDLLModule, "NtQuerySystemInformation");
LPFN_NtOpenProcess gpfnNtOpenProcess = (LPFN_NtOpenProcess)GetProcAddress(ghNTDLLModule, "NtOpenProcess");
LPFN_NtQueryObject gpfnNtQueryObject = (LPFN_NtQueryObject)GetProcAddress(ghNTDLLModule, "NtQueryObject");
LPFN_OpenProcessToken gpfnOpenProcessToken = (LPFN_OpenProcessToken)GetProcAddress(LoadLibraryW(L"ADVAPI32.DLL"), "OpenProcessToken");
LPFN_NtOpenProcessTokenEx gpfnNtOpenProcessToken=(LPFN_NtOpenProcessTokenEx)GetProcAddress(ghNTDLLModule, "NtOpenProcessTokenEx");
LPFN_NtQueryInformationToken gpfnNtQueryInformationToken = (LPFN_NtQueryInformationToken)GetProcAddress(ghNTDLLModule, "NtQueryInformationToken");
LPFN_RtlLengthSid gpfnRtlLengthSid = (LPFN_RtlLengthSid)GetProcAddress(ghNTDLLModule, "RtlLengthSid");
LPFN_RtlCopySid gpfnRtlCopySid = (LPFN_RtlCopySid)GetProcAddress(ghNTDLLModule, "RtlCopySid");


CSystem::CSystem()
{
}


CSystem::~CSystem()
{
}

BOOL CSystem::GetIoCounters()
{
	void* pBuffer = NULL;
	DWORD dwSize = 0L;
	SYSTEM_PERFORMANCE_INFORMATION* pPerfInfo = NULL;
	ULONG ulSizeReturned = 0L;
	NTSTATUS s = (*gpfnNtQuerySystemInformation)((SYSTEM_INFORMATION_CLASS)SystemPerformanceInformation,
		pBuffer,
		dwSize,
		&ulSizeReturned);
	if (s == STATUS_SUCCESS)
	{
	}

	return	FALSE;
}



// 返回系统中所有的进程
BOOL CSystem::GetAllProcesses()
{
	void* pBuffer = NULL;
	DWORD dwSize = 0L;
	ULONG ulSizeReturned = 0L;
	NTSTATUS s = (*gpfnNtQuerySystemInformation)((SYSTEM_INFORMATION_CLASS)NT_PROCESSTHREAD_INFO,
		pBuffer,
		dwSize,
		&ulSizeReturned);
	if (s == STATUS_INFO_LENGTH_MISMATCH)
	{
		pBuffer = ::HeapAlloc(::GetProcessHeap(), 0, ulSizeReturned);
		dwSize = ulSizeReturned;
		s= (*gpfnNtQuerySystemInformation)((SYSTEM_INFORMATION_CLASS)NT_PROCESSTHREAD_INFO,
			pBuffer,
			dwSize,
			&ulSizeReturned);
	}

	if (s == STATUS_SUCCESS)
	{
		
#ifdef USE_SYSTEM_PROCESSES
		_SYSTEM_PROCESSES* pProcesses = (_SYSTEM_PROCESSES*)pBuffer;
		do
		{

			pProcesses = (_SYSTEM_PROCESSES*)((PUCHAR)pProcesses + pProcesses->NextEntryDelta);
		} while (pProcesses->NextEntryDelta);
#endif 
		return	TRUE;
	}
	return FALSE;
}



////从Winsta.dll中逆向而来
NTSTATUS __stdcall GetSidFromProcessId(HANDLE TokenHandle, ULONG64 timeCreate, PSID DestinationSid, ULONG64 *ulLength)
{
	NTSTATUS ret; // esi
	PSID *pSID; // ebx
	__int64 ProcessInformation; // [esp+Ch] [ebp-48h]
	_OBJECT_ATTRIBUTES ObjectAttributes; // [esp+2Ch] [ebp-28h]
	_CLIENT_ID ClientId; // [esp+44h] [ebp-10h]
	ULONG ReturnLength; // [esp+4Ch] [ebp-8h]
	HANDLE ProcessHandle; // [esp+50h] [ebp-4h]

	ClientId.UniqueProcess = TokenHandle;
	ProcessHandle = 0;
	ObjectAttributes.Length = 24;
	ObjectAttributes.RootDirectory = 0;
	ObjectAttributes.Attributes = 0;
	ObjectAttributes.ObjectName = 0;
	ObjectAttributes.SecurityDescriptor = 0;
	ObjectAttributes.SecurityQualityOfService = 0;
	ClientId.UniqueThread = 0;

	ret = (*gpfnNtOpenProcess)(&ProcessHandle, PROCESS_QUERY_LIMITED_INFORMATION, &ObjectAttributes, &ClientId);
	if (ret >= 0)
	{
		ret = (*gpfnNtQueryInformationProcess)(ProcessHandle, ProcessTimes, &ProcessInformation, sizeof(ULONG64), 0);
		if (ret < 0)
		{
			DbgPrintMessage(TRACE_ERROR, "NtQueryInformationProcess failed: 0x%x", ret);
			CloseHandle(ProcessHandle);
			return ret;
		}
		if (ProcessInformation != timeCreate)
		{
			CloseHandle(ProcessHandle);
			DbgPrintMessage(TRACE_ERROR, "GetSidFromProcessId: mimatching create time");
			return STATUS_INVALID_HANDLE;
		}
		ret = (*gpfnNtOpenProcessToken)(ProcessHandle, TOKEN_QUERY, OBJ_KERNEL_HANDLE, &TokenHandle);
		if (ret < 0)
		{
			CloseHandle(ProcessHandle);
			DbgPrintMessage(TRACE_ERROR, "NtOpenProcessToken: failed for: %x",(*gpfnRtlNtStatusToDosError)(ret));
			return ret;
		}
		ret = (*gpfnNtQueryInformationToken)(TokenHandle, TokenUser, 0, 0, &ReturnLength);
		if (!ReturnLength)
		{
			CloseHandle(ProcessHandle);
			CloseHandle(TokenHandle);
			DbgPrintMessage(TRACE_ERROR, "NtQueryInformationToken failed: 0x%x", (*gpfnRtlNtStatusToDosError)(ret));
			return ret;
		}
		pSID = (PSID *)operator new(ReturnLength);
		if (!pSID)
		{
			CloseHandle(ProcessHandle);
			CloseHandle(TokenHandle);
			return	STATUS_NO_MEMORY;
		}
		ret = (*gpfnNtQueryInformationToken)(TokenHandle, TokenUser, pSID, ReturnLength, &ReturnLength);
		CloseHandle(TokenHandle);
		if (ret >= 0)
		{
			ret = (*gpfnRtlLengthSid)(*pSID);
			if (ret > *ulLength || !DestinationSid)
			{
				ret = STATUS_BUFFER_TOO_SMALL;
			}
			else
			{
				ret = (*gpfnRtlCopySid)(ret, DestinationSid, *pSID);
			}
		}
		operator delete(pSID);
		CloseHandle(ProcessHandle);
	}
	return ret;
}

extern SYSTEM_HANDLE_INFORMATION* gbSystemHandleInformation;
extern SYSTEM_HANDLE_INFORMATION* gbSystemHandleInformation2;
extern SYSTEM_HANDLE_INFORMATION*  gbSystemHandleInformation2Copy;
extern DWORD gdwSystemHandleInfoLength2;
extern DWORD gdwProcessIdOfSystemHandleInformation;
extern DWORD g_dwProcessId;
extern bool gbServiceNotImplemented;

void * GetSystemHandlesInfo(ULONG ReturnedLength)
{
	NTSTATUS result; // eax
	DWORD dwType; // eax


	if (ReturnedLength)
	{
		if (gbSystemHandleInformation)
			free(gbSystemHandleInformation);
		gbSystemHandleInformation = (SYSTEM_HANDLE_INFORMATION*)malloc(gdwSystemHandleInfoLength2);
		dwType = SystemHandleInformation;
		if (gbServiceNotImplemented)
			dwType = SystemExtendedHandleInformation;
		result = (*gpfnNtQuerySystemInformation)((SYSTEM_INFORMATION_CLASS)dwType, gbSystemHandleInformation, gdwSystemHandleInfoLength2, &ReturnedLength);
		if (result)
		{
			while (result != STATUS_ACCESS_DENIED)
			{
				free(gbSystemHandleInformation);
				gdwSystemHandleInfoLength2 += gdwSystemHandleInfoLength2;
				gbSystemHandleInformation = (SYSTEM_HANDLE_INFORMATION*)malloc(gdwSystemHandleInfoLength2);
				memset(gbSystemHandleInformation, 0, gdwSystemHandleInfoLength2);
				dwType = SystemHandleInformation;
				if (gbServiceNotImplemented)
					dwType = SystemExtendedHandleInformation;
				result = (*gpfnNtQuerySystemInformation)((SYSTEM_INFORMATION_CLASS)dwType, gbSystemHandleInformation, gdwSystemHandleInfoLength2, &ReturnedLength);
				if (result == STATUS_SUCCESS)
					return gbSystemHandleInformation;
			}
			memset(gbSystemHandleInformation, 0, gdwSystemHandleInfoLength2);
		}
	}
	else
	{
		/// 第一次来取数据，所以要自动调整内存大小
		if (gbSystemHandleInformation2Copy)
			free(gbSystemHandleInformation2Copy);

		gbSystemHandleInformation2Copy = 0;
		if (gdwProcessIdOfSystemHandleInformation == g_dwProcessId)
		{
			gbSystemHandleInformation2Copy = gbSystemHandleInformation2;
		}
		else
		{
			if (gbSystemHandleInformation2)
			{
				free(gbSystemHandleInformation2);
			}
			gdwProcessIdOfSystemHandleInformation = g_dwProcessId;
		}
		gbSystemHandleInformation2 = (SYSTEM_HANDLE_INFORMATION*)malloc(gdwSystemHandleInfoLength2);
		dwType = SystemHandleInformation;
		if (gbServiceNotImplemented)
			dwType = SystemExtendedHandleInformation;
		result = (*gpfnNtQuerySystemInformation)((SYSTEM_INFORMATION_CLASS)dwType, gbSystemHandleInformation2, gdwSystemHandleInfoLength2, &ReturnedLength);
		if (result)
		{
			//SYSTEM_HANDLE_INFORMATION_EX *p;
			while (result != STATUS_ACCESS_DENIED)
			{
				if (result == STATUS_INSUFFICIENT_RESOURCES)
				{
					//MessageBoxW(0, L"Insufficient system resources to get handle information.", L"Process Explorer", 0x10u);
					//v9 = gdwSystemHandleInfoLength2;
					//memset(gbSystemHandleInformation2, 0, gdwSystemHandleInfoLength2);
					//return memset(gbSystemHandleInformation2, 0, v9);
					return	NULL;
				}
				free(gbSystemHandleInformation2);
				//UNICODE
				gdwSystemHandleInfoLength2 += gdwSystemHandleInfoLength2;
				gbSystemHandleInformation2 = (SYSTEM_HANDLE_INFORMATION*)malloc(gdwSystemHandleInfoLength2);
				dwType = SystemHandleInformation;
				if (gbServiceNotImplemented)
					dwType = SystemExtendedHandleInformation;
				result = (*gpfnNtQuerySystemInformation)((SYSTEM_INFORMATION_CLASS)dwType, gbSystemHandleInformation2, gdwSystemHandleInfoLength2, &ReturnedLength);
				if (result == STATUS_SUCCESS)
					return gbSystemHandleInformation2;
			}
			memset(gbSystemHandleInformation2, 0, gdwSystemHandleInfoLength2);
		}
	}
	return NULL;
}



////复制指定进程的进程句柄
HANDLE  DuplicateProcessHandle(DWORD dwProcessId, HANDLE hSourceHandle, DWORD dwDesiredAccess)
{
	DWORD dwPID; // edi
	HANDLE ProcessHandle; // esi

	int InBuffer[4]; // [esp+8h] [ebp-20h]
	DWORD BytesReturned; // [esp+1Ch] [ebp-Ch]
	DWORD InBuffer1; // [esp+20h] [ebp-8h]
	HANDLE OutBuffer; // [esp+24h] [ebp-4h]

	dwPID = dwProcessId;
	OutBuffer = NULL;
	InBuffer1 = dwProcessId;

	ProcessHandle = OpenProcess(PROCESS_DUP_HANDLE, FALSE, dwProcessId);
	dwProcessId = (DWORD)ProcessHandle;
	if (ProcessHandle)
		goto __SYSKERNELPROC;

	// 使用自定义驱动加载进程
	if (GetLastError() == ERROR_ACCESS_DENIED)
		DeviceIoControl(theProcExpDriver.GetHandle(), 0x8335003C, &InBuffer1, 4, &dwProcessId, 4, &BytesReturned, 0);

	ProcessHandle = (HANDLE)dwProcessId;
	if (dwProcessId)
	{
		// 系统核心进程
__SYSKERNELPROC:
		if (dwPID <= 8)
		{
			InBuffer[0] = dwPID;
			InBuffer[3] = (int)hSourceHandle;
			if (theProcExpDriver.IsValid()
				|| !DeviceIoControl(theProcExpDriver.GetHandle(), 0x83350014, InBuffer, 16, &OutBuffer, 4, &BytesReturned, 0))
			{
				DuplicateHandle(ProcessHandle, hSourceHandle, GetCurrentProcess(), &OutBuffer, dwDesiredAccess, 0, 0);
			}
		}
		else
		{
			DuplicateHandle(ProcessHandle, hSourceHandle, GetCurrentProcess(), &OutBuffer, dwDesiredAccess, 0, 0);
		}
		CloseHandle(ProcessHandle);
	}
	return OutBuffer;
}

CObjectQueryThread theQueryThread;
CObjectQueryThread::CObjectQueryThread()
{
	m_hBackQueryObjectThreadHandle = NULL;
	m_hBackQueryObjectThreadQueryOKEvent = NULL;
	m_hBackQueryObjectEvent = NULL;
	m_hObjectHandleToQuery = NULL;
	m_pObjectNameInfo = NULL;
	m_uQueryObjectResult=STATUS_SUCCESS;
}


CObjectQueryThread::~CObjectQueryThread()
{
}


BOOL CObjectQueryThread::Start()
{
	m_pObjectNameInfo = (OBJECT_NAME_INFORMATION*)::malloc(1024);
	m_pObjectNameInfo->Name.Length = 1024 - sizeof(ULONG)- sizeof(ULONG);
	UINT ThreadId;
	if (!m_hBackQueryObjectThreadHandle)
		m_hBackQueryObjectThreadHandle =(HANDLE) _beginthreadex(0, 0, CObjectQueryThread::BackQueryObjectThreadProc, 0, 0, &ThreadId);
	if (!m_hBackQueryObjectEvent)
	{
		m_hBackQueryObjectEvent = CreateEvent(NULL, 0, 0, NULL);
		m_hBackQueryObjectThreadQueryOKEvent = CreateEvent(0, 0, 0, NULL);
	}
	SetEvent(m_hBackQueryObjectEvent);
	return	TRUE;
}


///后台检索内核对象线程
UINT __stdcall CObjectQueryThread::BackQueryObjectThreadProc(void *pTheadParam)
{
	ULONG nLength; // [esp+4h] [ebp-4h]

	// 等待前台触发这个查询事件
	while (!WaitForSingleObject(theQueryThread.m_hBackQueryObjectEvent, INFINITE))
	{
		nLength = theQueryThread.m_pObjectNameInfo->Name.Length + 8;
		theQueryThread.m_uQueryObjectResult = (*gpfnNtQueryObject)(theQueryThread.m_hObjectHandleToQuery, ObjectNameInformation, theQueryThread.m_pObjectNameInfo, nLength, &nLength);
		// 重置该事件
		SetEvent(theQueryThread.m_hBackQueryObjectThreadQueryOKEvent);
	}
	return 0;
}