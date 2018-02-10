#pragma once

#include "resource.h"
#include "../include/Common.h"

#include "..\include\MainWnd.h"


#define MAX_LOADSTRING 256
#define MAX_COLUMNSET 10


typedef BOOLEAN(WINAPI *LPFN_SetSuspendState)(BOOLEAN bHibernate, BOOLEAN bForce, BOOLEAN bWakeupEventsDisabled);
typedef BOOL(WINAPI* LPFN_LockWorkStation)();
typedef BOOLEAN(WINAPI *LPFN_IsPwrHibernateAllowed)();
typedef BOOLEAN(WINAPI *LPFN_IsPwrSuspendAllowed)();
typedef int(WINAPI *LPFN_SetLayeredWindowAttributes)(DWORD, DWORD, DWORD, DWORD);
typedef int(WINAPI *LPFN_SHAutoComplete)(DWORD, DWORD);

typedef CRYPT_PROVIDER_SGNR *  (WINAPI* LPFN_WinVerifyTrust)(CRYPT_PROVIDER_DATA *pProvData, DWORD idxSigner, BOOL fCounterSigner, DWORD idxCounterSigner);
typedef CRYPT_PROVIDER_DATA *  (WINAPI* LPFN_WTHelperGetProvSignerFromChain)(HANDLE hStateData);
typedef BOOL(WINAPI* LPFN_WTHelperProvDataFromStateData)(HCATADMIN hCatAdmin, DWORD dwFlags);
typedef BOOL(WINAPI* LPFN_CryptCATAdminReleaseContext)(HCATADMIN hCatAdmin, HCATINFO hCatInfo, DWORD dwFlags);
typedef BOOL(WINAPI* LPFN_CryptCATAdminReleaseCatalogContext)(HCATINFO hCatInfo, CATALOG_INFO *psCatInfo, DWORD dwFlags);
typedef BOOL(WINAPI* LPFN_CryptCATCatalogInfoFromContext)(HCATINFO hCatInfo, CATALOG_INFO *psCatInfo, DWORD dwFlags);
typedef HCATINFO(WINAPI* LPFN_CryptCATAdminEnumCatalogFromHash)(HCATADMIN hCatAdmin, BYTE *pbHash, DWORD cbHash, DWORD dwFlags, HCATINFO *phPrevCatInfo);
typedef BOOL(WINAPI* LPFN_CryptCATAdminCalcHashFromFileHandle)(HANDLE hFile, DWORD *pcbHash, BYTE *pbHash, DWORD dwFlags);
typedef BOOL(WINAPI* LPFN_CryptCATAdminAcquireContext)(HCATADMIN *phCatAdmin, const GUID *pgSubsystem, DWORD dwFlags);
typedef DWORD(WINAPI* LPFN_CertNameToStrW)(DWORD dwCertEncodingType, PCERT_NAME_BLOB pName, DWORD dwStrType, LPWSTR psz, DWORD csz);
typedef HCATINFO(WINAPI* LPFN_CryptCATAdminAddCatalog)(HCATADMIN hCatAdmin, PWSTR pwszCatalogFile, PWSTR pwszSelectBaseName, DWORD dwFlags);
typedef BOOL(WINAPI* LPFN_CryptCATAdminRemoveCatalog)(HCATADMIN hCatAdmin, LPCWSTR pwszCatalogFile, DWORD dwFlags);
typedef BOOL(WINAPI* LPFN_IsCatalogFile)(HANDLE hFile, WCHAR *pwszFileName);


extern int PE_GetDeviceCaps(int idx);
#define GETDEVICECAPS_VAR(idx) PE_GetDeviceCaps(idx) 

extern int gLogPixelsX;
extern int gLogPixelsY;


static DWORD gdwNumberOfProcessColumn[12] =
{
	0x28041F,
	0x500424,
	0x500427,
	0x280004,
	0x960026,
	0x8C0409,
};

static DWORD gdwNumberOfHandleColumnMap[2] =
{
	0x640015, 0x1C20016,
};

static DWORD gdwNumberODllColumnMap[4] =
{
	0x6E001A, 0x0B4002A, 0x8C0409, 0x12C0457,
};
#define DEFINE_CALLBACK(Name) LRESULT CALLBACK Handle##Name(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern MSGHANDLER gNormalWindowMsgHandler;
extern MSGHANDLER gNormalWindowCmdHandler;

typedef std::map<DWORD, void*> CProcessMap;
extern CProcessMap gProcessMap;

extern HINSTANCE ghInstance;

extern SYSTEM_INFO gSystemInfo;
extern DWORD gdwVersion;

extern TCHAR gszMainProgramTitle[];
extern TCHAR gszMainProgramRegKeyName[];
extern TCHAR gszMainProgramInnerRegKeyName[];

extern TCHAR PROCESSEXPLORER_CLASSNAME[] ;
extern TCHAR PROCESSEXPLORER_WINDOWNAME[];

extern UINT gShellIconMsg ;

extern UINT gMSGCommdlg_FindReplace;
extern HWND ghWndNewOwner;
//带属性的ListCtrl
extern HWND ghWndTreeListView;
//TreeList窗口的上一次纵向顶部位置
extern DWORD gdwTopLastTreeList;
///显示句柄的列表窗口
extern HWND ghWndHandlesListCtrl;
///显示DLL的列表窗口
extern HWND ghWndDllsListCtrl;
extern HWND ghWndStatusBar;
extern HWND ghWndToolbar;
extern HWND ghSysinfoPropSheetDlg;
extern HWND ghWndCpuGraphClassGraph1;
extern HWND ghWndCpuGraphClassGraph2;
extern HWND ghWndCpuGraphClassGraph3;
extern HWND ghWndCpuGraphClassGraph4;
extern HWND ghWndCpuGraphClassGraph5;
extern HWND ghWndCpuGraphClassGraph6;
extern HWND ghWndCpuGraphClassGraph7;
extern HANDLE ghEventRefresh;

extern HWND ghWndRebar;
extern DWORD gdwStatusBarCount;

extern HWND ghDlgFindDialog;
extern HWND ghWndThreadStack;
extern HWND ghWndListCtrlInFindDialog;

extern TCHAR** gpHiddenProcs;

extern int gnMenuLastColumnsetPosLast;

extern int gnFileMenuPosLast;

extern TCHAR gpszTargetName[];

extern ATOM gAtomTreeListProperty;

extern HMODULE ghuxthemeModule;

extern HFONT ghDefaultGuiFontUnderline;
extern bool gbHide1;
extern bool gbHide2;

///IsThemeActive;
//CloseThemeData;
//OpenThemeData;
//EnableThemeDialogTexture;
extern LPFN_DrawThemeBackground gpfnDrawThemeBackground;
extern LPFN_IsThemeActive gpfnIsThemeActive;
extern LPFN_CloseThemeData gpfnCloseThemeData;
extern LPFN_OpenThemeData gpfnOpenThemeData;
extern LPFN_EnableThemeDialogTexture gpfnEnableThemeDialogTexture;


extern BOOL IsX64();
extern int LoadDbgHelp(LPCTSTR lpszModuleName);
extern int LoadDriverFromRes(LPCTSTR lpszResId, LPCTSTR lpszFileName);
extern int StartProcExp64(BOOL fWaiting, int nShowCmd);
extern int DumpProcessToFile(HANDLE hFile, HANDLE hProcess, BOOL bDumpFullMemory);
static void DumpProcessThreadProc(void* p);
extern BOOL CreateMainWindow(HINSTANCE hInstance, int nShowCmd);
extern void InitDrawEngine();
extern int HandleShellIconMsg();

extern int OnShowProcessProperties(UINT uMenuId);

extern LONG LoadHiddenProcsFromRegistry();
extern DWORD GetToolbarButtonHeight();
extern BOOL IsBuiltinAndAnministrative();


extern void StartProcessPropertiesThread(void* pInfo);



inline LPARAM TabCtrl_GetItemParam(HWND hTabCtrl, int nTab)
{
	TCITEM item = { 0 };
	item.mask = TCIF_PARAM;
	if (!SendMessage(hTabCtrl, TCM_GETITEM, (WPARAM)nTab, (LPARAM)&item))
		return -1;
	return	item.lParam;
}


extern BOOL MessageLoop(HINSTANCE hInstance);

extern SYSINFOPAGEFUNCITEM gSystemInfoPageItem[];
extern LRESULT CALLBACK SummaryWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK CPUWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK MemoryWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK IOWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK GPUWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK PEMainWndProc(HWND, UINT, WPARAM, LPARAM);
extern LRESULT CALLBACK GraphWndProc(HWND, UINT, WPARAM, LPARAM);

extern LRESULT CALLBACK SysInfoPropertiesDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK OnShowProperties(HWND, UINT, UINT, LPARAM);
extern LRESULT CALLBACK OnShowSystemInformation(HWND hWnd, UINT wParam1, UINT wParam2, LPARAM lParam);
extern LRESULT CALLBACK OnShowProcessTree(HWND hWnd, UINT wParam1, UINT wParam2, LPARAM lParam);
extern LRESULT CALLBACK OnReplaceTaskManager(HWND hWnd, UINT wParam1, UINT wParam2, LPARAM lParam);
extern LRESULT CALLBACK OnFindHandleOrDll(HWND hWnd, UINT wParam1, UINT wParam2, LPARAM lParam);

extern LRESULT CALLBACK HandleSize(HWND, UINT, WPARAM, LPARAM);
extern bool ListCtrlDrawItem(LPDRAWITEMSTRUCT lpdis);
extern BOOL ListCtrlMeasureItem(HWND hWnd, UINT msg, WPARAM wParam, LPMEASUREITEMSTRUCT lpmis);

extern TCHAR* GetNumberTextFormat(LPTSTR lpszText, UINT cbSize);
extern BOOL SetDlgItemULong64(HWND hDlg, UINT nIDD, ULONG64 nValue, ...);
extern BOOL MainWnd_Hide(HWND hWnd);

extern HCURSOR ghCursorArrow;
extern HCURSOR ghCursorFind;
extern HMENU  ghMenuPriority;
extern DWORD gdwAdapterRuntingTime;

extern int gnToolbarHeight;
extern HCURSOR ghCursorArrow0;
extern HCURSOR ghCursorArrow1;
extern DWORD gdwTickLastUpdateGraph;
extern RECT gRectMainWnd;
extern RECT gRectRedrawMainWnd;

extern HWND ghSysinfoPropSheetDlg;
extern HANDLE ghSystemInfoSheetThreadEvent;
//

extern double gdHeightScale;
extern double gdSavedDivider;


extern DWORD gdwSelectedItemInFindDialog;
extern DWORD gdwRefreshRateOfSearchCopy;

extern PSIDDATA gSIDDATA;
extern LPFN_CreateRestrictedToken  gpfnCreateRestrictedToken;

extern LPFN_WinVerifyTrust  gpfnWinVerifyTrust;
extern LPFN_WTHelperGetProvSignerFromChain  gpfnWTHelperGetProvSignerFromChain;
extern LPFN_WTHelperProvDataFromStateData  gpfnWTHelperProvDataFromStateData;
extern LPFN_CryptCATAdminReleaseContext  gpfnCryptCATAdminReleaseContext;
extern LPFN_CryptCATAdminReleaseCatalogContext  gpfnCryptCATAdminReleaseCatalogContext;
extern LPFN_CryptCATCatalogInfoFromContext  gpfnCryptCATCatalogInfoFromContext;
extern LPFN_CryptCATAdminEnumCatalogFromHash  gpfnCryptCATAdminEnumCatalogFromHash;
extern LPFN_CryptCATAdminCalcHashFromFileHandle  gpfnCryptCATAdminCalcHashFromFileHandle;
extern LPFN_CryptCATAdminAcquireContext  gpfnCryptCATAdminAcquireContext;
extern LPFN_CertNameToStrW  gpfnCertNameToStrW;
extern LPFN_CryptCATAdminAddCatalog  gpfnCryptCATAdminAddCatalog;
extern LPFN_CryptCATAdminRemoveCatalog  gpfnCryptCATAdminRemoveCatalog;
extern LPFN_IsCatalogFile  gpfnIsCatalogFile;

extern LONG RetrieveProcessComments();
extern void InitToolTips(HWND hWndTooltips);
extern bool LoadWintrust();
extern const TCHAR *CPtoLocaleName(int uACP);
extern bool GetProcessDEPStatus();
extern CTreeList* GetTreeListCtrl(HWND hWnd);
extern BOOL InitTreeList(HWND hTreeList, HFONT hFont);
extern HWND CreateTreeListView(HWND hWndParent, HINSTANCE hInst, HMENU hMenu, DWORD dwStyle, LPRECT rc);
extern BOOL ListCtrlNotifyInFindDlg(NMLISTVIEW* pNMListView);
extern bool gbFindHandlesOrDlls;
extern HWND ghDlgFindDialog;
extern int gnItemSortedInFindDialog;
extern bool gbFINDDLGLISTAscending;
extern HICON ghSheildIcon;


extern WORD		gwProcessImageColumnWidth;
extern WORD		gwProcessColumns[96];
extern WORD		HandleColumns[32];
extern WORD		DllColumns[32];
extern WNDPROC	gLastPEListCtrlWnd;
extern WINDOWPLACEMENT gWindowPlacement[6];
extern UINT		gShellIconMsg;
extern UINT		gMSGCommdlg_FindReplace;
extern TCHAR**	gpHiddenProcs;
extern TCHAR*	gszCommandLine;
extern TCHAR	gszDependsFileName[_MAX_PATH];
extern TCHAR	gszColumnsetName[_MAX_PATH];
extern TCHAR	gstrDefaultDebugger[_MAX_PATH];
extern TCHAR	gszSymbolPath[_MAX_PATH + 1];
extern TCHAR	gstrMenuCulumnSet[10][762];
extern TCHAR	gstrDbgHelpPath[_MAX_PATH];
extern SYSTEM_INFO gSystemInfo;;
extern RECT		gRectRedrawMainWnd;
extern RECT		gRectMainWnd;
extern LOGFONT	gUnicodeFont;
extern int		gnToolbarHeight;
extern HWND ghWndTreeListView;
extern HWND ghWndToolbar;
extern HWND ghWndThreadStack;
extern HWND ghWndStatusBar;
extern HWND ghWndPointLast;
extern HWND ghWndNewOwner;
extern HWND ghWndNetworkGraphCtrl;
extern HWND ghWndListCtrlInFindDialg;
extern HWND ghWndIOGraphCtrl;
extern HWND ghWndHandlesListCtrl;
extern HWND ghWndGraphPhysicalMemory;
extern HWND ghWndGraphMemory;
extern HWND ghWndGraphCPUUsage;
extern HWND ghWndGPUGraphCtrl;
extern HWND ghWndFocus;
extern HWND ghWndDllsListCtrl;
extern HWND ghWndDiskGraphCtrl;
extern HWND ghSysinfoPropSheetDlg;
extern HWND ghDlgFindDialog;
extern HMENU ghMenuUsers;
extern HMENU ghMenuPriority;
extern HMENU ghMenuHandlesMenu;
extern HMENU ghMenuDllsMenu;
extern HFONT ghDefaultUnicodeFont;
extern HFONT ghDefaultGuiFont;
extern HFONT ghDefaultFontUnderline;
extern HCURSOR ghCursorFind;
extern HCURSOR ghCursorArrow1;
extern HCURSOR ghArrowCursor;
extern HCURSOR gArrowCursor;
extern HANDLE ghSystemInfoSheetThreadEvent;
extern HANDLE ghRefreshEvent;
extern HANDLE ghEventRefreshSearchResult;
extern HANDLE ghEvent6;
extern HANDLE ghEvent5;
extern HANDLE ghEvent3;
extern HANDLE ghEvent2;
extern HANDLE ghBackupUpdateThreadHandle;
extern DWORD ProcessSortColumn;;
extern DWORD gdwTopLastTreeList;
extern DWORD gdwTickCountLast;
extern DWORD gdwStatusBarColumns;;
extern DWORD gdwRefreshRateOfSearch;
extern DWORD gdwProcessColumnCount;
extern DWORD gdwIDMapForSelectColumn[96];
extern DWORD gdwHighlightDuration;
extern DWORD gdwHandleColumnCount;;
extern DWORD gdwAdapterRuntingTime;
extern DWORD   HandleSortColumn;
extern DWORD   gpDllColumnMap[32];
extern DWORD   gdwSortableColumnVector[];
extern DWORD   *gGraphReBarCtrlIds;
extern DWORD   *gdwToolbarBands;
extern DWORD   gdwOpacity;
extern DWORD   gdwNumColumnsets;
extern DWORD   gdwHandleColumnMap[32];
extern DWORD   gdwGpuNodeUsageMask;
extern DWORD   gdwDllColumnCount;
extern DWORD   gdwDefaultSysInfoPage;
extern DWORD   gdwDefaultProcPropPage;
extern DWORD   gdwDefaultDllPropPage;
extern DWORD   gColorSuspend;
extern DWORD   gColorProtected;
extern DWORD   gColorNewProc;
extern DWORD   gColorNet;
extern DWORD   gColorJobs_0;
extern DWORD   gColorDelProc;
extern DWORD   DllSortColumn;
extern DWORD   ColorServices;
extern DWORD   ColorRelocatedDlls;
extern DWORD   ColorPacked;
extern DWORD   ColorOwn;
extern DWORD   ColorImmersive;
extern DWORD   ColorGraphBk;
extern double gdSavedDivider;
extern double gdHeightScale;
extern COLORREF gColors[16];
extern COLORREF gColorGraphBackground2;;
extern COLORREF gColorGraphBackground;;
extern CLock gStringListLock;;
extern CLock gLockFroSysInfoProperties;;
extern CLock gLock3;;
extern CLock gLock2;;
extern bool gbWintrustInited;
extern bool gbShowProcessTree;
extern bool gbProcessSortDirection;;
extern bool gbProcessExplorerShow;
extern bool gbProcessExplorerHide;
extern bool gbProcessDEPStatus;
extern bool gbLaunchDepends;
extern bool   ShowUnnamedHandles;
extern bool   ShowDllView;
extern bool   HandleSortDirection;
extern bool   gbVirusTotalSubmitUnknown;
extern bool   gbVirusTotalCheck;
extern bool   gbVerifySignatures;
extern bool   gbUseGoogle;
extern bool   gbTrayCPUHistory;
extern bool   gbSymbolWarningShown;
extern bool   gbShowPhysTray;
extern bool   gbShowNewProcesses;
extern bool   gbShowNetTray;
extern bool   gbShowLowerpane;
extern bool   gbShowIoTray;
extern bool   gbShowGpuTray;
extern bool   gbShowDiskTray;
extern bool   gbShowCpuFractions;
extern bool   gbShowCommitTray;
extern bool   gbShowColumnHeatmaps;
extern bool   gbShowAllUsers;
extern bool   gbShowAllGpus;
extern bool   gbShowAllCpus;
extern bool   gbHighlightSuspend;
extern bool   gbHighlightServices;
extern bool   gbHighlightRelocatedDlls;
extern bool   gbHighlightProtected;
extern bool   gbHighlightPacked;
extern bool   gbHighlightOwnProcesses;
extern bool   gbHighlightNewProc;
extern bool   gbHighlightNetProcess;
extern bool   gbHighlightJobs;
extern bool   gbHighlightImmersive;
extern bool   gbHighlightDelProc;
extern bool   gbHideWhenMinimized;
extern bool   gbFormatIoBytes;
extern bool   gbETWstandardUserWarning;
extern bool   gbConfirmKill;
extern bool   gbAlwaysOntop;
extern bool   gbAllOneInstance;
extern bool   DllSortDirection;
extern bool   byte_4F96D6;

extern HWND		ghSysinfoPropsheetDlg;
extern LONG		gnLock;
extern DWORD	gdwOsType;

extern BOOL LoadProcessCommentsFromReg();
extern void*  GetSystemHandlesInfo(int ReturnedLength);
extern int InitMMXBuffer(__m128i* pFixedBuffer, int nCount, const __m128i* Value);

extern BOOL SearchDlls(HWND hWndListCtrl, DWORD dwProcessId, BOOL *bCancelFindInFindDlg);
extern BOOL InsertRowToDllList(HWND hWnd, HANDLESANDDLLSLISTUSERPARAM* pUserParam);
extern BOOL UpdateSysIconInMenu(HMENU hMenu, UINT nId);
extern void OnAlwaysOnTop(HWND hWnd);
extern BOOL Init3DStatistics(ULONG64* a, ULONG64* b);