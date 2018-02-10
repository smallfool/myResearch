#pragma once

#include "Process.h"

#define NT_PROCESSTHREAD_INFO        0x05





//typedef struct _SYSTEM_BASIC_INFORMATION
//{
//	DWORD dwUnknown1; // 0
//	ULONG uKeMaximumIncrement; // x86: 0x0002625A or 0x00018730
//	ULONG uPageSize; // bytes
//	ULONG uMmNumberOfPhysicalPages;
//	ULONG uMmLowestPhysicalPage;
//	ULONG uMmHighestPhysicalPage;
//	ULONG uAllocationGranularity; // bytes
//	PVOID pLowestUserAddress;
//	PVOID pMmHighestUserAddress;
//	KAFFINITY uKeActiveProcessors;
//	BYTE bKeNumberProcessors;
//	BYTE bUnknown2;
//	WORD wUnknown3;
//} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

//typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
//{
//	LARGE_INTEGER IdleTime;
//	LARGE_INTEGER KernelTime;
//	LARGE_INTEGER UserTime;
//	LARGE_INTEGER DpcTime;
//	LARGE_INTEGER InterruptTime;
//	DWORD InterruptCount;
//	DWORD dwUnknown1;
//} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, *PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION

//typedef struct _SYSTEM_PERFORMANCE_INFORMATION
//{
//	LARGE_INTEGER liIdleTime;
//	LARGE_INTEGER IoReadTransferCount;
//	LARGE_INTEGER IoWriteTransferCount;
//	LARGE_INTEGER IoOtherTransferCount;
//	ULONG IoReadOperationCount;
//	ULONG IoWriteOperationCount;
//	ULONG IoOtherOperationCount;
//	ULONG AvailablePages;
//	ULONG CommittedPages;
//	ULONG CommitLimit;
//	ULONG PeakCommitment;
//	ULONG PageFaultCount;
//	ULONG CopyOnWriteCount;
//	ULONG TransitionCount;
//	ULONG CacheTransitionCount;
//	ULONG DemandZeroCount;
//	ULONG PageReadCount;
//	ULONG PageReadIoCount;
//	ULONG CacheReadCount;
//	ULONG CacheIoCount;
//	ULONG DirtyPagesWriteCount;
//	ULONG DirtyWriteIoCount;
//	ULONG MappedPagesWriteCount;
//	ULONG MappedWriteIoCount;
//	ULONG PagedPoolPages;
//	ULONG NonPagedPoolPages;
//	ULONG PagedPoolAllocs;
//	ULONG PagedPoolFrees;
//	ULONG NonPagedPoolAllocs;
//	ULONG NonPagedPoolFrees;
//	ULONG FreeSystemPtes;
//	ULONG ResidentSystemCodePage;
//	ULONG TotalSystemDriverPages;
//	ULONG TotalSystemCodePages;
//	ULONG NonPagedPoolLookasideHits;
//	ULONG PagedPoolLookasideHits;
//	ULONG Spare3Count;
//	ULONG ResidentSystemCachePage;
//	ULONG ResidentPagedPoolPage;
//	ULONG ResidentSystemDriverPage;
//	ULONG CcFastReadNoWait;
//	ULONG CcFastReadWait;
//	ULONG CcFastReadResourceMiss;
//	ULONG CcFastReadNotPossible;
//	ULONG CcFastMdlReadNoWait;
//	ULONG CcFastMdlReadWait;
//	ULONG CcFastMdlReadResourceMiss;
//	ULONG CcFastMdlReadNotPossible;
//	ULONG CcMapDataNoWait;
//	ULONG CcMapDataWait;
//	ULONG CcMapDataNoWaitMiss;
//	ULONG CcMapDataWaitMiss;
//	ULONG CcPinMappedDataCount;
//	ULONG CcPinReadNoWait;
//	ULONG CcPinReadWait;
//	ULONG CcPinReadNoWaitMiss;
//	ULONG CcPinReadWaitMiss;
//	ULONG CcCopyReadNoWait;
//	ULONG CcCopyReadWait;
//	ULONG CcCopyReadNoWaitMiss;
//	ULONG CcCopyReadWaitMiss;
//	ULONG CcMdlReadNoWait;
//	ULONG CcMdlReadWait;
//	ULONG CcMdlReadNoWaitMiss;
//	ULONG CcMdlReadWaitMiss;
//	ULONG CcReadAheadIos;
//	ULONG CcLazyWriteIos;
//	ULONG CcLazyWritePages;
//	ULONG CcDataFlushes;
//	ULONG CcDataPages;
//	ULONG ContextSwitches;
//	ULONG FirstLevelTbFills;
//	ULONG SecondLevelTbFills;
//	ULONG SystemCalls;
//}SYSTEM_PERFORMANCE_INFORMATION, *PSYSTEM_PERFORMANCE_INFORMATION, *LPSYSTEM_PERFORMANCE_INFORMATION;




#define SystemInfoclass108 108

class CSystem
{
public:
	CSystem();
	~CSystem();
	// 返回系统中所有的进程
	BOOL GetAllProcesses();
	///
	BOOL GetIoCounters();
};

typedef NTSTATUS(WINAPI* LPFN_NtQueryObject)(HANDLE   Handle, OBJECT_INFORMATION_CLASS ObjectInformationClass, PVOID ObjectInformation, ULONG  ObjectInformationLength, PULONG   ReturnLength);

extern void DbgPrintMessage(int, const char*, ...);
extern LPFN_NtQueryObject gpfnNtQueryObject;

extern LPFN_NtQuerySystemInformation gpfnNtQuerySystemInformation;
extern LPFN_NtOpenProcess gpfnNtOpenProcess;

///后台查询内核对象线程
class CObjectQueryThread
{
public:
	CObjectQueryThread();
	~CObjectQueryThread();

	BOOL Start();
	static UINT __stdcall BackQueryObjectThreadProc(void *pTheadParam);
public:

	HANDLE m_hBackQueryObjectThreadHandle ;
	HANDLE m_hBackQueryObjectThreadQueryOKEvent ;
	HANDLE m_hBackQueryObjectEvent ;
	HANDLE m_hObjectHandleToQuery ;
	OBJECT_NAME_INFORMATION* m_pObjectNameInfo ;
	NTSTATUS m_uQueryObjectResult;
};

extern CObjectQueryThread theQueryThread;
extern LPFN_NtOpenProcessTokenEx gpfnNtOpenProcessToken;
extern LPFN_NtQueryInformationToken gpfnNtQueryInformationToken;
extern LPFN_RtlLengthSid gpfnRtlLengthSid;
extern LPFN_RtlCopySid gpfnRtlCopySid;