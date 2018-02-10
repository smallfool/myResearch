#pragma once

#define FDM_ENDSORT		0x464
#define FDM_STARTSORT   0x465


enum ProcessMessageType
{
	PE_NM_7D0 = 0x7D0,
	PE_NM_7D1 ,
	PE_NM_7D2 ,
	PE_NM_7D3,
	PE_NM_7D4 ,
	PE_NM_7D5 ,

	PMT_MSG_7E8 = 0x7E8,//WPARAM uID = ListCtrlId,LPARAM bShowTree
	PMT_MSG_7E9,
	PMT_MSG_7EA,
	PMT_MSG_7EB,
	PMT_MSG_7EC,
	PMT_MSG_7ED,
	PMT_MSG_7EF,
	PMT_MSG_7F0,
	PMT_MSG_7F1,
	PMT_MSG_7F2,
	PMT_MSG_7F3,
	PMT_MSG_7F4,
	PMT_MSG_7F5,
	PMT_MSG_7F6,
	PMT_MSG_7F7,
	PMT_MSG_7F8,
	PMT_MSG_7F9,

	TREELIST_MSG_2000 = 0x2000,
	TREELIST_MSG_2001 = 0x2001,
	TREELIST_MSG_2002 = 0x2002,
	TREELIST_MSG_2003 = 0x2003,
	TREELIST_MSG_2004 = 0x2004,
	TREELIST_MSG_2005 = 0x2005,
	TREELIST_MSG_2006 = 0x2006,

};

enum WindowType
{
	WINDOW_NONE,
	WINDOW_NORMAL,
	WINDOW_FRAME,
	WINDOW_DIALOG,
	WINDOW_MDICHILD,
};


enum WindowPlacementType
{
	Windowplacement,
	FindWindowplacement,
	SysinfoWindowplacement,
	PropWindowplacement,
	DllPropWindowplacement,
	StackWindowPlacement,
};

//输出风格
enum TRACEFLAGS
{
	TRACE_MSG = 1,
	TRACE_ALV = 2,
	TRACE_WARNING = 4,
	TRACE_ERROR = 8,
	TRACE_OUT = 0x10,
	TRACE_IN = 0x20,
	TRACE_DUMP = 0x40,
	TRACE_FATAL = 0x80,

	///	这似乎是WINSTA.DLL内部所使用
	TRACE_TSAPI_OUT = 0x100,
	TRACE_TSAPI_IN = 0x200,
	TRACE_TSRPC_OUT = 0x400,
	TRACE_TSRPC_IN = 0x800,
	TRACE_BVT = 0x1000,
	TRACE_PERF = 0x2000,
};

struct MapInfo
{
	DWORD dwProcessId;
	int  bDumpFullMemory;
	DWORD dwError;
	TCHAR szFileName[54];
};

struct MSGFUNC
{
	UINT	dwMsgId;
	WNDPROC pfnCallback;
};


struct MSGHANDLER
{
	DWORD Count;
	MSGFUNC* pFuncs;
	DWORD   Type;//WinowType

	static LRESULT HandleMessage(MSGHANDLER* pMsgHandler, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT HandleCommand(MSGHANDLER* pMsgHandler, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

//	变量初始化
typedef struct GlobalVariable
{
	TCHAR* Name;	//名称
	DWORD  Type;	//註冊表类型，覺得具体读写模式
	DWORD  Size;	//对应变量的结构大小
	PVOID  VA;		//全局变量的地址
	double DefaultValue;	//单位或系数。
}GLOBALVARIABLE, *PGLOBALVARIABLE, *LPGLOBALVARIABLE;


///自定义的SID数据
typedef struct _SIDDATA
{
	PSID		m_SID;
	TCHAR		m_Name[_MAX_PATH];
	TCHAR		m_DomainName[_MAX_PATH];
	_SIDDATA*	m_Next;
}SIDDATA,*PSIDDATA,*LPSIDDATA;

typedef struct _SysInfoPageFuncItem
{
	TCHAR*		TabName;
	WNDPROC		Callback;
	////TCHAR* TemplateName;
	UINT		nIDTemplate;
}SYSINFOPAGEFUNCITEM, *PSYSINFOPAGEFUNCITEM, *LPSYSINFOPAGEFUNCITEM;

struct COLUMNWIDTH
{
	short width1;
	short width2;
};

#define COUNTALIGN(nCount) (nCount - (nCount & 7))

#define SetWindowFont(hWnd,hFont)  SendMessage(hWnd,WM_SETFONT,(WPARAM)hFont,(LPARAM)0);


//从ProcExp中得到的访问权限
//目前网络上看不到具体的解释
#define PROCESS_TRACE_MODE_EVENT_RECORD		0x10000000

#define STATUS_SUCCESS						((NTSTATUS)0x00000000L) 
#define STATUS_INFO_LENGTH_MISMATCH			((NTSTATUS)0xC0000004L)
#define STATUS_IMAGE_ALREADY_LOADED			((NTSTATUS)0xC000010EL)
#define STATUS_BUFFER_TOO_SMALL				((NTSTATUS)0x00000023L)


#define __PID__			::GetCurrentProcessId()
#define __TID__			::GetCurrentThreadId()
#define __THISPROCESS__ ::GetCurrentProcess()
#define __THISTHREAD__	::GetCurrentThread()
#define __THISMODULE__	::GetModuleHandleW(NULL)
#define __KERNEL__		::GetModuleHandleW(L"Kernel32.dll")
#define __NTDLL__		::GetModuleHandleW(L"NTDLL.DLL")

#define __ERROR__		::GetLastError()

///具体的
typedef struct _PE_SYSTEM_BASIC_INFORMATION
{
	ULONG Reserved;
	ULONG TimerResolution;
	ULONG PageSize;
	ULONG NumberOfPhysicalPages;
	ULONG LowestPhysicalPageNumber;
	ULONG HighestPhysicalPageNumber;
	ULONG AllocationGranularity;
	ULONG MinimumUserModeAddress;
	ULONG MaximumUserModeAddress;
	ULONG ActiveProcessorsAffinityMask;
	BYTE NumberOfProcessors;
	BYTE Undefined[3];
}PE_SYSTEM_BASIC_INFORMATION, *PPE_SYSTEM_BASIC_INFORMATION, *LPPE_SYSTEM_BASIC_INFORMATION;

typedef enum _OBJECT_INFORMATION_CLASS {
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,//https://www.pediy.com/kssd/pediy12/143990.html   http://www.cnblogs.com/zhangdongsheng/archive/2013/06/06/3122752.html
	ObjectAllInformation,
	ObjectDataInformation
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_NAME_INFORMATION {
	UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;


typedef struct _OBJECT_BASIC_INFORMATION {
	ULONG Attributes;
	ACCESS_MASK DesiredAccess;
	ULONG HandleCount;
	ULONG ReferenceCount;
	ULONG PagedPoolUsage;
	ULONG NonPagedPoolUsage;
	ULONG Reserved[3];
	ULONG NameInformationLength;
	ULONG TypeInformationLength;
	ULONG SecurityDescriptorLength;
	LARGE_INTEGER CreationTime;
} OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;


typedef enum _SYSTEM_HANDLE_TYPE
{
	OB_TYPE_UNKNOWN = 0,   //0
	OB_TYPE_TYPE,    // 1,fixed
	OB_TYPE_DIRECTORY,   // 2,fixed
	OB_TYPE_SYMBOLIC_LINK, // 3,fixed
	OB_TYPE_TOKEN,    // 4,fixed
	OB_TYPE_PROCESS,   // 5,fixed
	OB_TYPE_THREAD,    // 6,fixed
	OB_TYPE_JOB,    // 7,fixed
	OB_TYPE_DEBUG_OBJECT, // 8,fixed
	OB_TYPE_EVENT,    // 9,fixed
	OB_TYPE_EVENT_PAIR,   //10,fixed
	OB_TYPE_MUTANT,    //11,fixed
	OB_TYPE_CALLBACK,   //12,fixed
	OB_TYPE_SEMAPHORE,   //13,fixed
	OB_TYPE_TIMER,    //14,fixed
	OB_TYPE_PROFILE,   //15,fixed
	OB_TYPE_KEYED_EVENT, //16,fixed
	OB_TYPE_WINDOWS_STATION,//17,fixed
	OB_TYPE_DESKTOP,   //18,fixed
	OB_TYPE_SECTION,   //19,fixed
	OB_TYPE_KEY,    //20,fixed
	OB_TYPE_PORT,    //21,fixed 
	OB_TYPE_WAITABLE_PORT, //22,fixed
	OB_TYPE_ADAPTER,   //23,fixed
	OB_TYPE_CONTROLLER,   //24,fixed
	OB_TYPE_DEVICE,    //25,fixed
	OB_TYPE_DRIVER,    //26,fixed
	OB_TYPE_IOCOMPLETION, //27,fixed
	OB_TYPE_FILE,    //28,fixed
	OB_TYPE_WMIGUID    //29,fixed
}SYSTEM_HANDLE_TYPE;

typedef void (WINAPI* LPFN_DbgTraceFunction)(UINT, LPCSTR lpszText);

///定义通用的函数指针声明
#define TYPEDEF_FUNC(rettype,FuncName,Param) typedef rettype (WINAPI* LPFN_##FuncName)Param;
///定义无参数的函数指针声明
#define TYPEDEF_FUNC_NOPARAM(rettype,FuncName) typedef rettype (WINAPI* LPFN_##FuncName)();
///定义无返回值的函数指针声明
#define TYPEDEF_FUNC_NORETURN(FuncName,Param) typedef void (WINAPI* LPFN_##FuncName)Param;

TYPEDEF_FUNC_NORETURN(GetNativeSystemInfo, (LPSYSTEM_INFO pSystemInfo))
TYPEDEF_FUNC_NORETURN(GetSystemInfo, (LPSYSTEM_INFO pSystemInfo))
TYPEDEF_FUNC_NOPARAM(DWORD, GetVersion)
TYPEDEF_FUNC(NTSTATUS, NtOpenThread, (PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId))
TYPEDEF_FUNC(NTSTATUS, NtQueryInformationProcess, (HANDLE ProcessHandle, ULONG SystemInfoClass, PVOID Buffer, ULONG BufferLength, ULONG* ReturnedLength))
TYPEDEF_FUNC(NTSTATUS, NtQuerySystemInformation, (_In_ SYSTEM_INFORMATION_CLASS SystemInformationClass, _Inout_ PVOID SystemInformation, _In_ ULONG SystemInformationLength, _Out_opt_ PULONG ReturnLength))
TYPEDEF_FUNC(NTSTATUS, NtOpenProcess, (PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttribtes, PCLIENT_ID ClientId))

TYPEDEF_FUNC(bool, WinStationGetProcessSid, (void *PublicBinding, HANDLE TokenHandle, LARGE_INTEGER a3, PSID DestionationSid, UINT *cbSizeReturned))



//typedef void (WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO pSystemInfo);
//typedef void (WINAPI *LPFN_GetSystemInfo)(LPSYSTEM_INFO pSystemInfo);
//typedef DWORD(WINAPI* LPFN_GetVersion)();
///typedef NTSTATUS(NTAPI* LPFN_NTOPENTHREAD)(PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId);

TYPEDEF_FUNC(BOOL, CreateRestrictedToken, (HANDLE ExistingTokenHandle, DWORD Flags, DWORD DisableSidCount, PSID_AND_ATTRIBUTES SidsToDisable, DWORD DeletePrivilegeCount, PLUID_AND_ATTRIBUTES PrivilegesToDelete, DWORD RestrictedSidCount, PSID_AND_ATTRIBUTES SidsToRestrict, PHANDLE NewTokenHandle))


///TYPEDEF_FUNC(HANDLE, OpenProcess, (DWORD ProcessAcessMode, BOOL bFlags, DWORD dwProcessId))

///定义通用的函数指针定义宏
#define DEFINE_PROC(ModuleName, FuncName)	LPFN_##FuncName pfn##FuncName = (LPFN_##FuncName)::GetProcAddress(LoadLibraryW(L#ModuleName), #FuncName);
///为从Kernel32.dll中函数指针定义宏
#define DEFINE_PROC_FROM_KERNEL(FuncName)	DEFINE_PROC(KERNEL32.DLL, FuncName)
///为从Ntdll.dll中函数指针定义宏
#define DEFINE_PROC_FROM_NTDLL(FuncName)	DEFINE_PROC(NTDLL.DLL,FuncName)
///为从DbgHelp.dll中函数指针定义宏
#define DEFINE_PROC_FROM_DBGHELP(FuncName)	DEFINE_PROC(DBGHELP.DLL,FuncName)
///为从Shell32.dll中函数指针定义宏
#define DEFINE_PROC_FROM_SHELL(FuncName)	DEFINE_PROC(SHELL32.DLL,FuncName)

#define ERRMSGBOX(Description,Caption, hWnd) MessageBox(hWnd,TEXT(#Description),Caption,MB_ICONERROR)
#define PEERRMSGBOX(Desc,hWnd) MessageBox(hWnd,TEXT(#Desc), gszMainProgramTitle,MB_ICONERROR)


typedef ULONGLONG (WINAPI *LPFN_VerSetConditionMask)(ULONGLONG dwlConditionMask, DWORD dwTypeBitMask,BYTE dwConditionMask);
typedef BOOL (WINAPI* LPFN_VerifyVersionInfoA)(LPOSVERSIONINFOEX lpVersionInfo, DWORD dwTypeMask, DWORDLONG dwlConditionMask);

typedef BOOL (WINAPI *LPFN_OpenProcessToken)(HANDLE  ProcessHandle, DWORD   DesiredAccess, PHANDLE TokenHandle);
typedef NTSTATUS(WINAPI* LPFN_NtQueryObject)(HANDLE   Handle, OBJECT_INFORMATION_CLASS ObjectInformationClass, PVOID ObjectInformation, ULONG  ObjectInformationLength, PULONG   ReturnLength);
typedef BOOL(WINAPI* LPFN_IsThemeActive)(void);
typedef HRESULT(WINAPI* LPFN_CloseThemeData)(HTHEME hTheme);
typedef HTHEME(WINAPI* LPFN_OpenThemeData)(HWND hWnd, LPCTSTR lpszName);
typedef HRESULT(WINAPI *LPFN_EnableThemeDialogTexture)(
	_In_ HWND  hwnd,
	_In_ DWORD dwFlags
	);
typedef HRESULT(WINAPI* LPFN_DrawThemeBackground)(
	_In_       HTHEME hTheme,
	_In_       HDC    hdc,
	_In_       int    iPartId,
	_In_       int    iStateId,
	_In_ const RECT   *pRect,
	_In_ const RECT   *pClipRect
	);

typedef NTSTATUS	(WINAPI* LPFN_NtLoadDriver)(PUNICODE_STRING DriverServiceName);
typedef void		(WINAPI* LPFN_RtlInitUnicodeString)(PUNICODE_STRING, LPCTSTR);
typedef DWORD		(WINAPI* LPFN_RtlNtStatusToDosError)(NTSTATUS status);

TYPEDEF_FUNC(NTSTATUS,NtOpenProcessTokenEx,(HANDLE ProcessHandle,ACCESS_MASK DesiredAccess,ULONG HandleAttributes,PHANDLE TokenHandle))

#define OBJ_INHERIT                         0x00000002L
#define OBJ_PERMANENT                       0x00000010L
#define OBJ_EXCLUSIVE                       0x00000020L
#define OBJ_CASE_INSENSITIVE                0x00000040L
#define OBJ_OPENIF                          0x00000080L
#define OBJ_OPENLINK                        0x00000100L
#define OBJ_KERNEL_HANDLE                   0x00000200L
#define OBJ_FORCE_ACCESS_CHECK              0x00000400L
#define OBJ_IGNORE_IMPERSONATED_DEVICEMAP   0x00000800L
#define OBJ_DONT_REPARSE                    0x00001000L
#define OBJ_VALID_ATTRIBUTES                0x00001FF2L

TYPEDEF_FUNC(NTSTATUS,NtQueryInformationToken,(HANDLE TokenHandle,TOKEN_INFORMATION_CLASS TokenInformationClass,PVOID TokenInformation,ULONG TokenInformationLength,PULONG ReturnLength))
TYPEDEF_FUNC(BOOL, RtlLengthSid,(PSID pSID))
TYPEDEF_FUNC(NTSTATUS, RtlCopySid,(ULONG DestinationSidLength,PSID  DestinationSid,PSID  SourceSid))


#define DlgSetFont(hDlg,hFont)			SendMessage(hDlg,WM_SETFONT,0,(LPARAM)hFont)
#define DlgSetSmallIcon(hDlg, hIcon)	SendMessage(hDlg,WM_SETICON,0,(LPARAM)hIcon)
#define DlgSetBigIcon(hDlg,hIcon)		SendMessage(hDlg,WM_SETICON,1,(LPARAM)hIcon)

#define GetStyle(hWnd)				(DWORD)GetWindowLong(hWnd,GWL_STYLE)
#define AddStyle(hWnd, dwStyle)			\
	{DWORD dwStyleOld = GetWindowLong(hWnd,GWL_STYLE);	\
	dwStyleOld |= dwStyle;				\
	SetWindowLong(hWnd,GWL_STYLE,(LONG)dwStyleOld);}
#define RemoveStyle(hWnd, dwStyle)			\
	{DWORD dwStyleOld = GetWindowLong(hWnd,GWL_STYLE);	\
	dwStyleOld &= ~dwStyle;				\
	SetWindowLong(hWnd,GWL_STYLE,(LONG)dwStyleOld);}

#define GetExStyle(hWnd)			(DWORD)GetWindowLong(hWnd,GWL_EXSTYLE)
#define AddExStyle(hWnd, dwExStyle)			\
	{DWORD dwExStyleOld = GetWindowLong(hWnd,GWL_EXSTYLE);	\
	dwExStyleOld |= dwExStyle;				\
	SetWindowLong(hWnd,GWL_EXSTYLE,(LONG)dwExStyleOld);}
#define RemoveExStyle(hWnd, dwExStyle)			\
	{DWORD dwExStyleOld =  GetWindowLong(hWnd,GWL_EXSTYLE);	\
	dwExStyleOld &= ~dwExStyle;				\
	SetWindowLong(hWnd,GWL_EXSTYLE,(LONG)dwExStyleOld);}

#define GetUserData(hWnd)			(PVOID)GetWindowLong(hWnd,GWL_USERDATA)
#define SetUserData(hWnd,pData)		SetWindowLong(hWnd,GWL_USERDATA,(LONG)pData)
#define LV_GetTooltips(hWnd)		(HWND)SendMessage(hWnd,LVM_GETTOOLTIPS,0,0);
#define HookWnd(hWnd,NewProc)		(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(LONG)NewProc)

///用在Handles表、DLLs表中LVITEM.param参数的自定义结构
////0xB8
struct HANDLESANDDLLSLISTUSERPARAM
{
	TCHAR*	ImgName;
	TCHAR	szText[64];
	DWORD	ProcessId;

	TCHAR*	szFileName;
};