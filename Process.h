#pragma once

//// http://www.longene.org/techdoc/0734380001224576878.html

#include "ntdefs.h"

//#define ProcessIoCounters	2
//#define ProcessVmCounters	3
//#define ProcessTimes		4
//#define ProcessHandleCount	20
//#define ProcessDEPStatus	34

typedef enum _PROCESSINFOCLASS
{
	ProcessBasicInformation = 0, // 0, q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION  
	ProcessQuotaLimits, // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX  
	ProcessIoCounters, // q: IO_COUNTERS  
	ProcessVmCounters, // q: VM_COUNTERS, VM_COUNTERS_EX  
	ProcessTimes, // q: KERNEL_USER_TIMES  
	ProcessBasePriority, // s: KPRIORITY  
	ProcessRaisePriority, // s: ULONG  
	ProcessDebugPort, // q: HANDLE  
	ProcessExceptionPort, // s: HANDLE  
	ProcessAccessToken, // s: PROCESS_ACCESS_TOKEN  
	ProcessLdtInformation, // 10  
	ProcessLdtSize,
	ProcessDefaultHardErrorMode, // qs: ULONG  
	ProcessIoPortHandlers, // (kernel-mode only)  
	ProcessPooledUsageAndLimits, // q: POOLED_USAGE_AND_LIMITS  
	ProcessWorkingSetWatch, // q: PROCESS_WS_WATCH_INFORMATION[]; s: void  
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup, // s: BOOLEAN  
	ProcessPriorityClass, // qs: PROCESS_PRIORITY_CLASS  
	ProcessWx86Information,
	ProcessHandleCount, // 20, q: ULONG, PROCESS_HANDLE_INFORMATION  
	ProcessAffinityMask, // s: KAFFINITY  
	ProcessPriorityBoost, // qs: ULONG  
	ProcessDeviceMap, // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX  
	ProcessSessionInformation, // q: PROCESS_SESSION_INFORMATION  
	ProcessForegroundInformation, // s: PROCESS_FOREGROUND_BACKGROUND  
	ProcessWow64Information, // q: ULONG_PTR  
	ProcessImageFileName, // q: UNICODE_STRING  
	ProcessLUIDDeviceMapsEnabled, // q: ULONG  
	ProcessBreakOnTermination, // qs: ULONG  
	ProcessDebugObjectHandle, // 30, q: HANDLE 

	ProcessDebugFlags, // qs: ULONG  
	ProcessHandleTracing, // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables  
	ProcessIoPriority, // qs: ULONG  
	ProcessExecuteFlags, // qs: ULONG  
	ProcessResourceManagement,
	ProcessCookie, // q: ULONG  
	ProcessImageInformation, // q: SECTION_IMAGE_INFORMATION  
	ProcessCycleTime, // q: PROCESS_CYCLE_TIME_INFORMATION  
	ProcessPagePriority, // q: ULONG  
	ProcessInstrumentationCallback, // 40  

	ProcessThreadStackAllocation, // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX  
	ProcessWorkingSetWatchEx, // q: PROCESS_WS_WATCH_INFORMATION_EX[]  
	ProcessImageFileNameWin32, // q: UNICODE_STRING  
	ProcessImageFileMapping, // q: HANDLE (input)  
	ProcessAffinityUpdateMode, // qs: PROCESS_AFFINITY_UPDATE_MODE  
	ProcessMemoryAllocationMode, // qs: PROCESS_MEMORY_ALLOCATION_MODE  
	ProcessGroupInformation, // q: USHORT[]  
	ProcessTokenVirtualizationEnabled, // s: ULONG  
	ProcessConsoleHostProcess, // q: ULONG_PTR  
	ProcessWindowInformation, // 50, q: PROCESS_WINDOW_INFORMATION  
	ProcessHandleInformation, // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8  
	ProcessMitigationPolicy, // s: PROCESS_MITIGATION_POLICY_INFORMATION  
	ProcessDynamicFunctionTableInformation,
	ProcessHandleCheckingMode,
	ProcessKeepAliveCount, // q: PROCESS_KEEPALIVE_COUNT_INFORMATION  
	ProcessRevokeFileHandles, // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION  
	MaxProcessInfoClass
}PROCESSINFOCLASS;


typedef struct _VM_COUNTERS
{
	ULONG PeakVirtualSize;                  //虚拟存储峰值大小；
	ULONG VirtualSize;                      //虚拟存储大小；
	ULONG PageFaultCount;                   //页故障数目；
	ULONG PeakWorkingSetSize;               //工作集峰值大小；
	ULONG WorkingSetSize;                   //工作集大小；
	ULONG QuotaPeakPagedPoolUsage;          //分页池使用配额峰值；
	ULONG QuotaPagedPoolUsage;              //分页池使用配额；
	ULONG QuotaPeakNonPagedPoolUsage;       //非分页池使用配额峰值；
	ULONG QuotaNonPagedPoolUsage;           //非分页池使用配额；
	ULONG PagefileUsage;                    //页文件使用情况；
	ULONG PeakPagefileUsage;                //页文件使用峰值；
}VM_COUNTERS, *PVM_COUNTERS;


typedef struct _SYSTEM_THREADS
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG         WaitTime;
	PVOID         StartAddress;
	CLIENT_ID     ClientId;
	KPRIORITY     Priority;
	KPRIORITY     BasePriority;
	ULONG         ContextSwitchCount;
	THREAD_STATE  State;
	KWAIT_REASON  WaitReason;
}SYSTEM_THREADS, *PSYSTEM_THREADS;

////typedef struct _SYSTEM_PROCESSES
////{
////	ULONG          NextEntryDelta;
////	ULONG          ThreadCount;
////	ULONG          Reserved1[6];
////	LARGE_INTEGER  CreateTime;
////	LARGE_INTEGER  UserTime;
////	LARGE_INTEGER  KernelTime;
////	UNICODE_STRING ProcessName;
////	KPRIORITY      BasePriority;
////	ULONG          ProcessId;
////	ULONG          InheritedFromProcessId;
////	ULONG          HandleCount;
////	ULONG          Reserved2[2];
////	VM_COUNTERS    VmCounters;
////	IO_COUNTERS    IoCounters;
////	SYSTEM_THREADS Threads[1];
////}SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

////typedef PPEB(__stdcall *PFNPsGetProcessPeb)(PEPROCESS pEProcess);

typedef ULONG  PPS_POST_PROCESS_INIT_ROUTINE;

typedef struct _PEB_LDR_DATA {
	BYTE       Reserved1[8];
	PVOID      Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	BYTE           Reserved1[16];
	PVOID          Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
	BYTE                          Reserved1[2];
	BYTE                          BeingDebugged;
	BYTE                          Reserved2[1];
	PVOID                         Reserved3[2];
	PPEB_LDR_DATA                 Ldr;
	PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
	BYTE                          Reserved4[104];
	PVOID                         Reserved5[52];
	PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
	BYTE                          Reserved6[128];
	PVOID                         Reserved7[1];
	ULONG                         SessionId;
} PEB, *PPEB;

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	DWORD SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	DWORD Flags;
	WORD LoadCount;
	WORD TlsIndex;
	LIST_ENTRY HashLinks;
	PVOID SectionPointer;
	DWORD CheckSum;
	DWORD TimeDateStamp;
	PVOID LoadedImports;
	PVOID EntryPointActivationContext;
	PVOID PatchInformation;
}LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;


typedef struct _PROCESS_BASIC_INFORMATION 
{
	NTSTATUS ExitStatus;
	PPEB PebBaseAddress;
	DWORD AffinityMask;
	DWORD BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;



class CProcess
{
public:
	CProcess();
	~CProcess();
	BOOL Query(DWORD dwType);
	BOOL QueryVMCounters(_VM_COUNTERS* pData);
	BOOL QueryIOCounters(_IO_COUNTERS* pData);
	BOOL QueryHandleCount(DWORD64* pdwCount);
	BOOL QueryBasicInfo(PROCESS_BASIC_INFORMATION* pPBI);
	BOOL QueryProcessTimes(FILETIME* pTimes);
protected:
	HANDLE	m_Handle;
	DWORD	m_ProcessId;
public:
	// 打开进程
	BOOL Open(DWORD dwPid, DWORD dwAccessMode);
};

extern LPFN_NtQueryInformationProcess gpfnNtQueryInformationProcess;

extern bool gbHasBuiltinAndAnministrativePriviledge;
//检测当前是否为内建系统管理员权限
extern BOOL IsBuiltinAndAnministrative();

extern TCHAR *gszWindowNameFound;
extern DWORD gdwWindowNameFoundSize;
extern HWND ghWindowFound;
extern BOOL gbWindowFoundInited;

extern _SYSTEM_PROCESS_INFORMATION* gpSystemProcessInfo;
extern DWORD gdwSystemProcessInfoLength;
extern BOOL GetSystemProcessInfo(_SYSTEM_PROCESS_INFORMATION**, DWORD*);