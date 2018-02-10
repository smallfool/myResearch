#pragma once

typedef int (WINAPI* LPFN_WinMain)(HINSTANCE, HINSTANCE, LPTSTR lpszCmdLine, UINT nCmdShow);
typedef int (WINAPI* LPFN_DllMain)(HINSTANCE, DWORD, void*);

class CPE
{
public:
	CPE();
	~CPE();
	// 从指定的文件名称中加载
	BOOL LoadFromFile(LPCTSTR lpszFileName);
	BOOL LoadFromMemory(void* pBuffer);
private:
	void* m_pBuffer;
protected:
	void* m_pImageBase;
	IMAGE_NT_HEADERS* m_pNtHeaders;
	IMAGE_DOS_HEADER* m_pDosHeader;
	///函数入口
	FARPROC m_pfnOEP;
public:
	// 处理资源节和资源数据
	BOOL HandleResData(void* pResBaseInFile,void* pResBaseInMem);
};

//////////////////////////////////////////////////////////////////////////
//以下不用管了
#define RTL_DEBUG_QUERY_MODULES                             0x01
#define RTL_DEBUG_QUERY_BACKTRACES                          0x02
#define RTL_DEBUG_QUERY_HEAPS                               0x04
#define RTL_DEBUG_QUERY_HEAP_TAGS                           0x08
#define RTL_DEBUG_QUERY_HEAP_BLOCKS                         0x10
#define RTL_DEBUG_QUERY_LOCKS                               0x20


//typedef struct _RTL_PROCESS_MODULE_INFORMATION
//{
//	ULONG Reserved[2];
//	PVOID Base;
//	ULONG Size;
//	ULONG Flags;
//	USHORT Index;
//	USHORT Unknown;
//	USHORT LoadCount;
//	USHORT ModuleNameOffset;
//	CHAR ImageName[256];
//} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;
//
//typedef struct _RTL_PROCESS_MODULES
//{
//	ULONG ModuleCount;
//	RTL_PROCESS_MODULE_INFORMATION ModuleEntry[1];
//} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;

typedef struct _RTL_PROCESS_HEAP_INFORMATION
{
	PVOID Base;
	ULONG Flags;
	USHORT Granularity;
	USHORT Unknown;
	ULONG Allocated;
	ULONG Committed;
	ULONG TagCount;
	ULONG BlockCount;
	ULONG Reserved[7];
	PVOID Tags;
	PVOID Blocks;
} RTL_PROCESS_HEAP_INFORMATION, *PRTL_PROCESS_HEAP_INFORMATION;

typedef struct _RTL_PROCESS_HEAPS
{
	ULONG HeapCount;
	RTL_PROCESS_HEAP_INFORMATION HeapEntry[1];
} RTL_PROCESS_HEAPS, *PRTL_PROCESS_HEAPS;

typedef struct _RTL_PROCESS_LOCK_INFORMATION
{
	PVOID Address;
	USHORT Type;
	USHORT CreatorBackTraceIndex;
	ULONG OwnerThreadId;
	ULONG ActiveCount;
	ULONG ContentionCount;
	ULONG EntryCount;
	ULONG RecursionCount;
	ULONG NumberOfSharedWaiters;
	ULONG NumberOfExclusiveWaiters;
} RTL_PROCESS_LOCK_INFORMATION, *PRTL_PROCESS_LOCK_INFORMATION;

typedef struct _RTL_PROCESS_LOCKS
{
	ULONG LockCount;
	RTL_PROCESS_LOCK_INFORMATION LockEntry[1];
} RTL_PROCESS_LOCKS, *PRTL_PROCESS_LOCKS;

typedef struct _RTL_PROCESS_BACKTRACE_INFORMATION
{
	PVOID SymbolicBackTrace;
	ULONG TraceCount;
	USHORT Index;
	USHORT Depth;
	PVOID BackTrace[16];
} RTL_PROCESS_BACKTRACE_INFORMATION, *PRTL_PROCESS_BACKTRACE_INFORMATION;

typedef struct _RTL_PROCESS_BACKTRACES
{
	ULONG CommittedMemory;
	ULONG ReservedMemory;
	ULONG NumberOfBackTraceLookups;
	ULONG NumberOfBackTraces;
	RTL_PROCESS_BACKTRACE_INFORMATION BackTraces[1];
} RTL_PROCESS_BACKTRACES, *PRTL_PROCESS_BACKTRACES;

typedef struct _RTL_DEBUG_BUFFER
{
	HANDLE SectionHandle;
	PVOID SectionBase;
	PVOID RemoteSectionBase;
	ULONG SectionBaseDelta;
	HANDLE EventPairHandle;
	ULONG Unknown[2];
	HANDLE RemoteThreadHandle;
	ULONG InfoClassMask;
	ULONG SizeOfInfo;
	ULONG AllocatedSize;
	ULONG SectionSize;
	PRTL_PROCESS_MODULES ModuleInformation;
	PRTL_PROCESS_BACKTRACES BackTraceInformation;
	PRTL_PROCESS_HEAPS HeapInformation;
	PRTL_PROCESS_LOCKS LockInformation;
	PVOID Reserved[8];
} RTL_DEBUG_BUFFER, *PRTL_DEBUG_BUFFER;
