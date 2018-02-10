// ProcExplorer.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ProcExplorer.h"

#include "../include/Str.h"
#include "../include/Reg.h"
#include "../include/PE.h"
#include "../include/Alloc.h"
#include "../include/Lock.h"
#include "../include/Protection.h"
#include "../include/FileObj.h"
#include "../include/Res.h"
#include "../include/TreeList.h"
#include "../include/Graph.h"

#include "../include/Security.h"
#include "../include/Driver.h"
#include "../include/QQInject.h"

extern  __m128i *GetListCtrlColumnMagic();



extern void TestD();

extern BOOL InitServices();
extern void GetAllServicesName();

extern BOOL OpenServiceControlDlg(HWND, void*);
CMainWnd theMainWnd;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nShowCmd)
{
	HINSTANCE hInst = hInstance;
	LPTSTR lpCommandLine = lpCmdLine;
	InitServices();
	GetAllServicesName();

	OpenServiceControlDlg(NULL, &theMainWnd);
	////int code = 0xFFFFFF51;//-175 LVN_BEGINLABELEDITW
	////int code2 = 0xFFFFFDEE;//TTN_GETDISPINFOW
	////int code3 = 0xFFFFFD43;//UDN_DELTAPOS
	////
	////int code4 = NM_CLICK;//-2
	////int code5 = 0xFFFFFFEA;//-22 NM_THEMECHANGED

	////int code6 = 0xFFFFFF4D;// LVN_ODFINDITEMW

	////int code7 = 0xFFFFFF65; //LVN_KEYDOWN;

	////int code8 = 0xFFFFFF4F;//-177  LVN_GETDISPINFOW
	////int code9 = 0xFFFFFD43;//-701  TBN_BEGINDRAG  TBN_FIRST-TBN_LAST
	////int code10 = 0xFFFFFDEE;//-530 TTN_GETDISPINFOW  TTN_FIRST - TTN_LAST
	////int code11 = 0xFFFFFCC1;//-831 RBN_GETOBJECT RBN_FIRST-RBN_LAST

	//int code17 = 0xFFFFFDF7;// -521;TTN_FIRST
	//int code12 = 0xFFFFFCB9;//RBN_CHILDISZE;
	//int code15 = 0xFFFFF82F;

	//int code13 = 0xFFFFF82C; //
	//int code14 = 0xFFFFF82B;
#if DBGONLY
	//theProcExpDriver.Load();
	int nIDs[4] = { 0x16,0x15,4,3 };
	__m128i xxxxx;
	xxxxx.m128i_u32[0] = 0x16;
	xxxxx.m128i_u32[1] = 0x15;
	xxxxx.m128i_u32[2] = 0x4;
	xxxxx.m128i_u32[3] = 0x3;


	gProcessMap.insert(std::pair<DWORD,PVOID>(0,(void*)hInstance));//[0］ = (void*)hInstance;
	auto iter=gProcessMap.find(0);
	if (iter== gProcessMap.end())
	{
		HANDLE h = (HANDLE)iter->second;
		if (h != INVALID_HANDLE_VALUE)
		{
			DbgPrintMessage(TRACE_WARNING, "Handle: %x", (UINT)h);
		}
	}
#endif 

	////char szText[] = "I am a smallfool";
	////CString str;
	////////str.SetString((PXCHAR)szText, lstrlenA(szText));

	DEFINE_PROC_FROM_KERNEL(GetNativeSystemInfo)
	if (pfnGetNativeSystemInfo)
	{
		(*pfnGetNativeSystemInfo)(&gSystemInfo);
	}
	else
	{
		//该函数已经被抛弃，不建议再继续使用
		//
		DEFINE_PROC_FROM_KERNEL(GetSystemInfo)
		(*pfnGetSystemInfo)(&gSystemInfo);
	}

	/// IsWOW64Process ?
	////if (IsX64())
	////{
	////	_beginthread(DumpProcessThreadProc, 0, (void*)hInst);
	////	if (StartProcExp64(TRUE, nShowCmd))
	////	{
	////		return	0;
	////	}
	////	PEERRMSGBOX("Unable to extract 64 - bit image.Run Process Explorer from a writeable directory.", NULL);

	////	return	-1;
	////}

	///Win32系统继续进行
	DEFINE_PROC_FROM_KERNEL(GetVersion)

	gdwVersion = (*pfnGetVersion)();
	DWORD dwMajorVersion = (DWORD)(LOBYTE(LOWORD(gdwVersion)));
	DWORD dwMinorVersion = (DWORD)(HIBYTE(LOWORD(gdwVersion)));
	SetPriorityClass(__THISPROCESS__, HIGH_PRIORITY_CLASS);
	
	CQQInject::Instance().Start();

	theMainWnd.Create(hInstance, nShowCmd);

	return MessageLoop(hInstance);

}

////消息循环处理
BOOL MessageLoop(HINSTANCE hInstance)
{
	MSG msg;
	HACCEL hAccel;
	//加载消息加速键
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROCEXPLORER));
	if (!GetMessage(&msg, 0, 0, 0))
		return FALSE;
	HWND hWndThreadStack = NULL;
	//进入消息循环
	while (1)
	{
		if (ghWndThreadStack != NULL)
		{
			if (TranslateAccelerator(ghWndThreadStack, hAccel, &msg))
			{
				if (!GetMessage(&msg, 0, 0, 0))
					return	FALSE;
			}
			hWndThreadStack = ghWndThreadStack;
		}


		if (!ghDlgFindDialog || !IsDialogMessage(ghDlgFindDialog, &msg))
		{
			hWndThreadStack = ghWndThreadStack;

			if ((!hWndThreadStack || !IsDialogMessage(hWndThreadStack, &msg))
				&& !TranslateAccelerator(ghWndNewOwner, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (!GetMessage(&msg, 0, 0, 0))
			return	FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////






///处理TreeList消息
LRESULT CALLBACK TreeListWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CTreeList *pTreeListWnd =  GetTreeListCtrl( hWnd);

	if (pTreeListWnd)
		return pTreeListWnd->HanleMessage(hWnd, msg, wParam, lParam);
	else
		return  DefWindowProc(hWnd, msg, wParam, lParam);
	return 0;
}


// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    return (INT_PTR)FALSE;
}






///WM_COMMAND消息响应处理
LRESULT CALLBACK HandleCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return MSGHANDLER::HandleCommand(&gNormalWindowCmdHandler, hWnd, message, wParam, lParam);
}

///WM_PAINT消息处理响应
LRESULT CALLBACK HandlePaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; 
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);
	if (gbShowLowerpane)
		DrawEdge(hdc, &gRectRedrawMainWnd, BDR_RAISEDINNER | BDR_RAISEDOUTER, BF_MIDDLE | BF_LEFT|BF_BOTTOM);// 0x80A);
	EndPaint(hWnd, &ps);
	return	0;
}

///WM_SIZET消息处理响应5
LRESULT CALLBACK HandleSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rcStatusBar = { 0 };
	RECT rcRebar = { 0 };
	int nStatusBarHeight = 0;
	HWND hWndRebar = NULL;
	HWND hWndMain = hWnd;
	int nHeight = 0;
	SIZE size;
	size.cx = LOWORD(lParam);
	size.cy = HIWORD(lParam);

	if (wParam == SIZE_MINIMIZED)
	{
		if (gbHideWhenMinimized)
		{
			MainWnd_Hide(ghWndNewOwner);
			return 0;
		}
	}
	else
	{
		GetWindowRect(ghWndStatusBar, &rcStatusBar);
		nStatusBarHeight = rcStatusBar.bottom - rcStatusBar.top;
		hWndRebar = GetParent(ghWndGraphCPUUsage);
		GetWindowRect(hWndRebar, &rcRebar);
		GetClientRect(hWndMain, &gRectMainWnd);
		nHeight = size.cy - (rcRebar.bottom - rcRebar.top + gRectMainWnd.top);
		gRectMainWnd.top += rcRebar.bottom - rcRebar.top;
		gRectMainWnd.right = size.cx;
		gRectMainWnd.bottom = nHeight;
		InvalidateRect(hWndMain, &gRectRedrawMainWnd, FALSE);
		gRectRedrawMainWnd.right = size.cx;
		gRectRedrawMainWnd.top = ((gRectMainWnd.top + gRectMainWnd.bottom - nStatusBarHeight) * gdHeightScale);
		gRectRedrawMainWnd.bottom = gRectRedrawMainWnd.top + 7;
		MoveWindow(ghWndTreeListView, 
			0, gRectMainWnd.top, size.cx, gRectRedrawMainWnd.top - gRectMainWnd.top, 
			TRUE);
		MoveWindow(
			ghWndHandlesListCtrl,
			0,
			gRectRedrawMainWnd.bottom - 1,
			size.cx,
			gRectMainWnd.bottom - gRectRedrawMainWnd.bottom - nStatusBarHeight + gRectMainWnd.top + 1,
			TRUE);
		MoveWindow(
			ghWndDllsListCtrl,
			0,
			gRectRedrawMainWnd.bottom - 1,
			size.cx,
			gRectMainWnd.bottom - gRectRedrawMainWnd.bottom - nStatusBarHeight + gRectMainWnd.top + 1,
			TRUE);
		MoveWindow(ghWndStatusBar, 
			0, gRectMainWnd.top + gRectMainWnd.bottom - nStatusBarHeight, size.cx, nStatusBarHeight,
			TRUE);
		int bottom = gnToolbarHeight;
		int right = gRectMainWnd.right;
		hWndRebar = GetDlgItem(hWndMain, IDC_REBAR);
		MoveWindow(hWndRebar, 0, 0, right, bottom, TRUE);
		InvalidateRgn(ghWndStatusBar, 0, 0);
		MoveWindow(ghWndRebar, 0, 0, gRectMainWnd.right - gRectMainWnd.left, 0, TRUE);
	}
	return 0;
}

///WM_LBUTTONDOWN消息处理响应
LRESULT CALLBACK HandleLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	return DefWindowProc(hWnd, message, wParam, lParam);
}


TCHAR gszLocaleInfo[16] = { _TEXT(".") };

///WM_CREATE
LRESULT CALLBACK HandleCreate(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	INITCOMMONCONTROLSEX picce = { sizeof(picce) };
	WSADATA wsaData = { 0 };

	CoInitialize(NULL);
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	GetLocaleInfo(LOCALE_CUSTOM_UNSPECIFIED, LOCALE_SDECIMAL, gszLocaleInfo, sizeof(gszLocaleInfo));

	LoadHiddenProcsFromRegistry();
	LoadProcessCommentsFromReg();


	picce.dwICC =  ICC_COOL_CLASSES;
	InitCommonControlsEx(&picce);


	gpfnCreateRestrictedToken = (LPFN_CreateRestrictedToken)GetProcAddress(LoadLibraryW(L"Advapi32.dll"), "CreateRestrictedToken");
	gbWintrustInited = LoadWintrust();
	gbProcessDEPStatus = GetProcessDEPStatus();
	///记录下该窗口句柄，以便于将来使用
	ghWndNewOwner = hWnd;


	HMENU hMainMenu = GetMenu(hWnd);
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	ghCursorArrow = LoadCursor(NULL, IDC_ARROW);
	ghCursorFind = LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_FIND_WINDOW));

	EnableMenuItem(hMainMenu, IDM_VIEW_HIDE_PROCESSES, *gpHiddenProcs == NULL);

	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

	lf.lfUnderline = 1;

	ghDefaultGuiFontUnderline = CreateFontIndirect(&lf);
	gMSGCommdlg_FindReplace = RegisterWindowMessageW(L"commdlg_FindReplace");

	DeleteMenu(hMainMenu, IDM_VIEW_LOAD_COLUMN_SET, MF_BYCOMMAND);
	
	HMENU hViewSubMenu = GetSubMenu(hMainMenu, 2);	///&Views
	HMENU hColumnsetSubMeu = GetSubMenu(hViewSubMenu, gnMenuLastColumnsetPosLast);
	DeleteMenu(hColumnsetSubMeu, 0, MF_BYPOSITION);
	HMENU hSubMenu = GetSubMenu(hViewSubMenu, gnMenuLastColumnsetPosLast);
	DeleteMenu(hSubMenu, 0, MF_BYPOSITION);

	TCHAR szColumn[_MAX_PATH] = { 0 };
	for (DWORD i = 0; i < gdwNumColumnsets; i++)
	{
		if (i >= MAX_COLUMNSET)
		{
			_tcscpy_s(szColumn, _MAX_PATH, (TCHAR*)gstrMenuCulumnSet[i]);
		}
		else if (i == MAX_COLUMNSET - 1)
		{
			_sntprintf_s(szColumn, _MAX_PATH, _TEXT("%s\tCtrl+%i"), (TCHAR*)gstrMenuCulumnSet[i], 0);
		}
		else
		{
			_sntprintf_s(szColumn, _MAX_PATH, _TEXT("%s\tCtrl+%i"), (TCHAR*)gstrMenuCulumnSet[i], i+1);
		}
		AppendMenu(hColumnsetSubMeu, MF_BYCOMMAND, i + IDM_VIEW_COLUMN_SET_BASE, szColumn);
	}

	ghMenuPriority = LoadMenu(ghInstance, TEXT("IDM_POPUP_PRIORITY"));



	HMENU hFileSubMenu = GetSubMenu(hMainMenu, 0);
	if (gpfnCreateRestrictedToken != NULL)
	{
		InsertMenu(hFileSubMenu, 1, MF_BYPOSITION, IDM_OTHER_RUN_AS_LIMITED_USER, _TEXT("作为限制用户运行(&L)..."));
		gnFileMenuPosLast++;
	}
	InsertMenu(hFileSubMenu, 1, MF_BYPOSITION, IDM_OTHER_RUN_AS_ADMINISTATOR, _TEXT("作为管理员启动...(&A)"));
	++gnFileMenuPosLast;

	InsertMenu(
		hMainMenu,
		IDM_POPUP_PRIORITY_NORMAL,
		MF_BYCOMMAND,
		IDM_PROCESS_PRIORITY_ABOVE_NORMAL,
		_TEXT("高于普通(&A): 10"));
	InsertMenu(ghMenuPriority, 
		IDM_POPUP_PRIORITY_NORMAL, 
		MF_BYCOMMAND, 
		IDM_PROCESS_PRIORITY_ABOVE_NORMAL, 
		TEXT("高于普通(&A): 10"));
	InsertMenu(hMainMenu, 
		IDM_PROCESS_PRIORITY_IDLE,
		MF_BYCOMMAND, 
		IDM_PROCESS_PRIORITY_BELOW_NORMAL,
		_TEXT("低于普通(&B): 6"));

	//if (gdwOsType < 1)
	//{

	//}

	InsertMenu(
		ghMenuPriority,
		IDM_POPUP_PRIORITY_IDLE,
		MF_BYCOMMAND,
		IDM_PROCESS_PRIORITY_BELOW_NORMAL,
		_TEXT("低于普通(&B): 6"));

	/*if (gpfnChangeWindowMessageFilter)*/
	{
		ChangeWindowMessageFilter(PMT_MSG_7F2, 1);
	}

	InsertMenu(hFileSubMenu, 3, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
	++gnFileMenuPosLast;
	InsertMenu(hFileSubMenu, 4, MF_BYPOSITION, IDM_FILE_SHOW_DETAILS_FOR_ALL_PROCESSES,
		_TEXT("详细信息(&D)"));
	gnFileMenuPosLast++;

	UpdateSysIconInMenu(hMainMenu, IDM_FILE_SHOW_DETAILS_FOR_ALL_PROCESSES);
	
	HMENU hOpacityMenu = LoadMenu(ghInstance, _TEXT("IDM_POPUP_OPACITY"));
	InsertMenu(hMainMenu, IDM_VIEW_SHOW_LOWER_PANE, MF_POPUP, (UINT_PTR)hOpacityMenu, _TEXT("透明度(&O)"));
	InsertMenu(hMainMenu, IDM_VIEW_SHOW_LOWER_PANE, MF_SEPARATOR, (UINT_PTR)0, NULL);

	CheckMenuItem(hMainMenu, IDM_OPTIONS_VERIFY_IMAGE_SIGNATURES, gbVerifySignatures ? MF_CHECKED : MF_UNCHECKED);
	//CheckItemMenu(hMainMenu, IDM_)

	CheckMenuItem(hMainMenu, IDM_OPTIONS_ALLOW_ONLY_ONE_INSTANCE, gbAllOneInstance ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMainMenu, IDM_OPTIONS_HIDE_WHEN_MINIMIZED, gbHideWhenMinimized ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMainMenu, IDM_VIEW_SHOW_COLUMN_HEATMAPS, gbShowColumnHeatmaps ? MF_CHECKED : MF_UNCHECKED);

	OnAlwaysOnTop(ghWndNewOwner);
	
	//在视图菜单项增加System Information...项
	InsertMenu(hMainMenu, 
		IDM_VIEW_SHOW_PROCESS_TREE, 
		MF_BYCOMMAND, 
		IDM_VIEW_SYSTEM_INFORMATION, 
		_TEXT("系统信息(&I)...\tCtrl+I"));

	//
	InsertMenu(hMainMenu, 
		IDM_OPTIONS_HIDE_WHEN_MINIMIZED, 
		MF_BYCOMMAND, 
		IDM_OPTIONS_REPLACE_TASK_MANAGER, 
		_TEXT("取代任务管理器(&T)"));
	
	ULONG64 dbValue1 = 0;
	ULONG64 dwSegCommitLimit = 0L;
	if (Init3DStatistics(&dbValue1, &dwSegCommitLimit))
	{

	}

	//创建工具条
	ghWndToolbar = CMainWnd::CreateRebar(hWnd, IDC_REBAR);

	RECT rc = { 0 };
	GetWindowRect(ghWndToolbar, &rc);
	gnToolbarHeight = rc.bottom - rc.top + 2;

	//创建属性列表框控件
	ZeroMemory(&rc, sizeof(RECT));
	ghWndTreeListView = CreateTreeListView(hWnd, ghInstance, (HMENU)IDC_TREELIST, WS_CHILD | WS_BORDER | WS_TABSTOP | LVS_EDITLABELS, &rc);
	
	InitTreeList(ghWndTreeListView,ghDefaultGuiFont);
	
	
	CTreeList* pTreeList = (CTreeList*)GetTreeListCtrl(ghWndTreeListView);
	gShellIconMsg = RegisterWindowMessageW(L"TaskbarCreated");

	ghSystemInfoSheetThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	ghEventRefreshSearchResult = CreateEvent(NULL, FALSE, FALSE, NULL);
	ghRefreshEvent= CreateEvent(NULL, FALSE, FALSE, NULL);

	GetClientRect(hWnd, &gRectMainWnd);
	gRectRedrawMainWnd.left = 0;
	gRectMainWnd.top = gRectMainWnd.top - 1 + gnToolbarHeight;
	gRectMainWnd.bottom -= gRectMainWnd.top;
	gRectRedrawMainWnd.top = gRectMainWnd.top + ((gRectMainWnd.bottom - gRectMainWnd.top) * gdHeightScale);
	gRectRedrawMainWnd.bottom = gRectRedrawMainWnd.top + 7;
	gRectRedrawMainWnd.right = gRectMainWnd.right;

//	//pTreeList->DisplayCallback = ;
//	pTreeList->RestoreScrollbars(0);
/////	pTreeList->m_HandCursorCb = LoadHandCursor();
	pTreeList->SetSplitterCursor(LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_SPLITTER)));
//	
//	InitToolTips(pTreeList->m_hWndTooltip1);
//	InitToolTips(pTreeList->m_hWndTooltip2);
//	InitToolTips(pTreeList->m_hWndTooltip3);

	DWORD dwColumnCount = gdwProcessColumnCount;

	LV_COLUMN column;

	if (gdwIDMapForSelectColumn[0] != 3)
	{
		memmove(&gdwIDMapForSelectColumn[1], &gdwIDMapForSelectColumn[0], sizeof(DWORD)*gdwProcessColumnCount);
		gdwIDMapForSelectColumn[0] = 3;

		memmove(&gwProcessColumns[0], &gwProcessColumns[0], sizeof(WORD)*gdwProcessColumnCount);
		int width = gwProcessImageColumnWidth;
		if (!gwProcessImageColumnWidth)
			width = 200;
		gwProcessColumns[0] = width;
		dwColumnCount++;
		gdwProcessColumnCount++;
	}

	//if (!InitListCtrl(ghWndTreeListView, gdwIDMapForSelectColumn, gwProcessColumns,gdwProcessColumnCount))
	//	return	FALSE;

	return	TRUE;
}

void InitToolTips(HWND hWndTooltips)
{
	SendMessage(hWndTooltips, TTM_SETDELAYTIME, TTDT_INITIAL, 0);
	SendMessage(hWndTooltips, TTM_SETMAXTIPWIDTH, TTDT_AUTOMATIC, 4000);
	SendMessage(hWndTooltips, TTM_SETDELAYTIME, TTDT_AUTOPOP, 1000000);
}



///计算工具条的高度
DWORD GetToolbarButtonHeight()
{
	int w; // edi
	int h; // ax
	DWORD ButtonSize; // esi

	h = GetSystemMetrics(SM_CYSMICON);
	w = GetSystemMetrics(SM_CXSMICON);
	SendMessage(ghWndToolbar, TB_SETBITMAPSIZE, 0, MAKELPARAM(h, w));//  w | h);
	ButtonSize = SendMessage(ghWndToolbar, TB_GETBUTTONSIZE, 0, 0);
	SendMessage(ghWndToolbar, TB_SETBITMAPSIZE, 0, MAKELPARAM(15, 16));//0xF0010);
	SendMessage(ghWndToolbar, TB_SETBUTTONSIZE, 0, (LPARAM)ButtonSize);
	return ButtonSize;

}



///处理IDM_SHOW_PROPERTIES菜单命令
LRESULT CALLBACK OnShowProperties(HWND hWnd, UINT uMenuId, UINT bMenuMsg, LPARAM lParam)
{
	LVITEM item = { 0 };
	item.mask = LVIF_PARAM;
	HWND hListCtrl = NULL;
	switch (uMenuId)
	{
	case IDM_POPUP_DLL_PROPERTIES:
		hListCtrl = ghWndDllsListCtrl;
		break;
	case IDM_POPUP_HANDLE_PROPERTIES:
		hListCtrl = ghWndHandlesListCtrl;
		break;
	case IDM_PROCESS_PROPERTIES:
		hListCtrl = ghWndTreeListView;
		break;
	default:
		hListCtrl = GetFocus();
		break;
	}

	int nItemSelected = ListCtrl_GetSelectedItem(hListCtrl);
	if (nItemSelected == -1)
	{
		TCHAR *szErrorDesc = NULL;
		if (hListCtrl == ghWndTreeListView)
			szErrorDesc = _TEXT("No process is selected");
			return 0;
		if (hListCtrl == ghWndDllsListCtrl)
			szErrorDesc = _TEXT("No DLL is selected");
		if (hListCtrl == ghWndHandlesListCtrl)
			szErrorDesc = _TEXT("No object is selected");
		MessageBox(ghWndTreeListView, szErrorDesc, _TEXT("Process Explorer Error"), MB_ICONHAND);
		return 0L;
	}
	item.iItem = nItemSelected;
	if (!ListCtrl_GetItem(hListCtrl, nItemSelected, &item))
	{
		return	0L;
	}

	void* pInfo = (void*)item.lParam;

	if (hListCtrl == ghWndDllsListCtrl)
	{

	}
	if (hListCtrl == ghWndHandlesListCtrl)
	{

	}
	else
	{
		StartProcessPropertiesThread(pInfo);
	}
	return 0L;
}




LRESULT CALLBACK SummaryWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK CPUWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK MemoryWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK IOWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK GPUWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleLButtonUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleDrawItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleGetMinmaxInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

{
	MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
	pInfo->ptMinTrackSize.x = 200;
	pInfo->ptMinTrackSize.y = 200;
	return FALSE;
}

LRESULT CALLBACK HandleClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleMeasureItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleNotify(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	NMHDR* pNMhdr = (NMHDR*)lParam;
	switch (pNMhdr->code)
	{
	case LVN_BEGINLABELEDIT:
		return TRUE;
	case LVN_KEYDOWN:
		return CTreeList::OnKeydown((UINT)wParam, (NMLVKEYDOWN*)lParam);
	case LVN_COLUMNCLICK:
		return CTreeList::OnTreeListColumnClick(wParam, (NMLISTVIEW*)lParam);
	case LVN_DELETEITEM:
		return CTreeList::DeleteItem(wParam, (NMLISTVIEW*)lParam);
	case LVN_ITEMCHANGED:
		return CTreeList::OnItemChanged(wParam);
	case LVN_GETDISPINFO:
		return CTreeList::OnGetDispInfo(wParam, (NMLVDISPINFO*)lParam);

	case NM_THEMECHANGED:
		if (IDC_REBAR == wParam)
			GetToolbarButtonHeight();
		return	0;
	case NM_RCLICK:
		return CTreeList::OnRClick(wParam, (NMCLICK*)lParam);
	case NM_DBLCLK:
		return CTreeList::OnDblClick(wParam);
	case NM_CLICK:
	{
		POINT pt;
		GetCursorPos(&pt);
		return CTreeList::OnTreeListColumnClick(wParam, (NMCLICK*)lParam);
	}
	case TTN_GETDISPINFO:
		return CTreeList::GetTooltipGetDispInfo(wParam, (NMTTDISPINFO*)lParam);
	case LVN_ODFINDITEM:
		return CTreeList::OnFindItem(wParam);
	case RBN_GETOBJECT:
		return 0;
	case RBN_CHILDSIZE:
		InvalidateRect(pNMhdr->hwndFrom, NULL, FALSE);
		return 0;
	case PE_NM_7D1:
		return CMainWnd::OnNotify7D1();
	case PE_NM_7D2:
		return CMainWnd::OnNotify7D2();
	case PE_NM_7D4:
		return CMainWnd::OnNotify7D4(wParam, lParam);
	case PE_NM_7D5:
		return CMainWnd::OnNotify7D5(wParam, lParam);
	default:
		return 0;
	}
	return	0;
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleEndSession(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleSetFocus(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	gdwTickCountLast = GetTickCount();
	if (ghWndFocus)
	{
		SetFocus(ghWndFocus);
		return TRUE;
	}
	else if (hWnd == ghWndNewOwner)
	{
		SetFocus(ghWndTreeListView);
		return TRUE;
	}

		return FALSE;
}

LRESULT CALLBACK HandleKillFocus(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleSysCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleContextMenu(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT nId; 
	int item; 
	HWND hWndMsg; 
	POINT point; 
	RECT rcItem; 

	HWND hWndContext = (HWND)wParam;
	if (hWndContext == ghWndTreeListView)
	{
		nId = IDC_TREELIST;
	}
	else if (hWndContext == ghWndHandlesListCtrl)
	{
		nId = IDC_HANDLES_LISTCTRL;
	}
	else
	{
		if (hWndContext != ghWndDllsListCtrl)
			return DefWindowProc(hWnd, message, wParam, lParam);
		nId = IDC_DLLS_LISTCTRL;
	}
	hWndMsg = hWnd;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	if (lParam != -1)
		return 0;
	item = SendMessage(hWndContext, LVM_GETNEXTITEM, lParam, LVIS_SELECTED);
	if (item != -1)
	{
		rcItem.left = 0;
		SendMessageW(hWndContext, LVM_GETITEMRECT, item, (LPARAM)&rcItem);
		point.y = rcItem.bottom;
		point.x = rcItem.left + 10;
		ClientToScreen(hWndContext, &point);
	}

	NMCLICK clk;
	clk.hdr.hwndFrom = hWndMsg;
	clk.pt = point;
	return CTreeList::OnRClick(nId, &clk);
}
////typedef struct _WTS_SESSION_INFO {
////	DWORD                  SessionId;
////	LPTSTR                 pWinStationName;
////	WTS_CONNECTSTATE_CLASS State;
////} WTS_SESSION_INFO, *PWTS_SESSION_INFO;
LRESULT CALLBACK HandleInitMenuPopup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT nIndex = 0;
	UINT iUser = 0;
	int idx = 0;
	HMENU hMenuUsers = NULL;
	UINT nPos = 0;
	MENUINFO mi = { sizeof mi };
	DWORD pdwBytesReturned = 0;
	UINT uPosition = 0;
	LPTSTR szDomainName = NULL;
	LPTSTR szUserName = NULL;
	DWORD nCount = 0;
	PWTS_SESSION_INFO ppSessionInfo = NULL;
	TCHAR NewItem[_MAX_PATH] = { 0 };



	HMENU hMenu = (HMENU)wParam;
	if (hMenu != ghMenuUsers)
		return 0;
	nCount = GetMenuItemCount(ghMenuUsers);
	nIndex = nCount - 1;
	if (nCount)
	{
		do
		{
			DeleteMenu(ghMenuUsers, nIndex--, MF_BYPOSITION);
		} while (nIndex != -1);
	}
	if (WTSEnumerateSessions(NULL, 0, 1,(PWTS_SESSION_INFO *) &ppSessionInfo, &nCount))
	{
		iUser = 0;
		uPosition = 0;
		if (nCount)
		{
			idx = 0;
			do
			{
				if (WTSQuerySessionInformation(
					NULL,
					ppSessionInfo[idx].SessionId,
					WTSUserName,
					&szUserName,
					&pdwBytesReturned))
				{
					if (WTSQuerySessionInformation(
						NULL,
						ppSessionInfo[idx].SessionId,
						WTSDomainName,
						&szDomainName,
						&pdwBytesReturned))
					{
						if (*szUserName)
						{
							_stprintf_s(NewItem, _MAX_PATH, L"%d: %s\\%s", ppSessionInfo[idx].SessionId, szDomainName, szUserName);
							hMenuUsers = LoadMenu(ghInstance, L"IDM_POPUP_USERS");
							mi.cbSize = 28;
							mi.fMask = MIM_APPLYTOSUBMENUS | MIM_MENUDATA | MIM_STYLE;            // MIM_APPLYTOSUBMENUS|MIM_MENUDATA|MIM_STYLE
							mi.dwStyle = MNS_NOTIFYBYPOS;
							mi.dwMenuData = ppSessionInfo[idx].SessionId;
							SetMenuInfo(hMenuUsers, &mi);
							switch (ppSessionInfo[idx].State)
							{
							case WTSActive:
							case WTSShadow:
								break;
							case WTSConnected:
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_CONNECT, MF_GRAYED);
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_LOGOFF, MF_GRAYED);
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_SEND_MESSAGE, MF_GRAYED);
								break;
							case WTSDisconnected:
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_DISCONNECT, MF_GRAYED);
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_SEND_MESSAGE, MF_GRAYED);
								break;
							default:
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_CONNECT, MF_GRAYED);
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_SEND_MESSAGE, MF_GRAYED);
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_LOGOFF, MF_GRAYED);
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_DISCONNECT, MF_GRAYED);
								EnableMenuItem(hMenuUsers, IDM_POPUP_USERS_REMOTE_CONTROL, MF_GRAYED);
								break;
							}
							nPos = uPosition;
							InsertMenu(ghMenuUsers, uPosition, MF_BYPOSITION| MF_POPUP, (UINT_PTR)hMenuUsers, NewItem);
							uPosition = nPos + 1;
						}
						WTSFreeMemory(szDomainName);
					}
					WTSFreeMemory(szUserName);
				}
				++iUser;
				++idx;
			} while (iUser < nCount);
		}
		WTSFreeMemory(ppSessionInfo);
	}
	return 0;
}

extern PTREELISTITEMPARAM gpTreeListItemParamList;

HANDLE StartProcessPropSheet(PTREELISTITEMPARAM pTreeListItemParam)
{
	InterlockedIncrement(&pTreeListItemParam->RefCount);

	return (HANDLE)_beginthread(StartProcessPropertiesThread, 0, pTreeListItemParam);

}

LRESULT CALLBACK HandleCopyData(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCOPYDATASTRUCT pData = (PCOPYDATASTRUCT)lParam;
	HWND hWndFrom = (HWND)wParam;
	CAtlString str;
	str = (CHAR*)pData->lpData;
	TREELISTITEMPARAM *pTreeListItem = (TREELISTITEMPARAM*)gpTreeListItemParamList;
	if (!pTreeListItem)
		return 1;
	while (pTreeListItem)
	{
		if (_tcsicmp(pTreeListItem->szImageName, (LPCTSTR)str) == 0)
			break;
		pTreeListItem = pTreeListItem->Prev;
		if (!pTreeListItem)
			return 1;
	}
	StartProcessPropSheet(pTreeListItem);

	if (!IsWindowVisible(ghWndNewOwner))
	{
		WINDOWPLACEMENT wdpl = { sizeof(WINDOWPLACEMENT) };
		GetWindowPlacement(ghWndNewOwner, &wdpl);
		ShowWindow(ghWndNewOwner, 4 * (wdpl.showCmd == SW_SHOWMINIMIZED) + SW_SHOW);
		SetForegroundWindow(ghWndNewOwner);
		SetFocus(ghWndNewOwner);
		gbProcessExplorerShow = true;
		gbProcessExplorerHide = false;
	}

	return 2;
}

LRESULT CALLBACK HandleDeviceChange(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HandleMenuCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}


TCHAR* GetNumberTextFormat(LPTSTR lpszText, UINT cbSize)
{
	TCHAR szFormat[1024] = { 0 };
	GetNumberFormat(LANG_USER_DEFAULT, 0, lpszText,NULL, szFormat, 1024);
	_tcscpy_s((TCHAR*)lpszText, cbSize, szFormat);
	TCHAR* psz = _tcsstr(lpszText, _TEXT("."));
	if (psz)
		*psz = 0;
	return lpszText;
}

BOOL SetDlgItemULong64(HWND hDlg, UINT nIDD, ULONG64 nValue, ...)
{
	va_list args; 
	TCHAR szBuffer[50] = { 0 };
	va_start(args, nValue);
	_vstprintf_s(szBuffer, 50, _TEXT("%I64d"), args);
	va_end(args);
	GetNumberTextFormat(szBuffer, 50);
	return SetDlgItemText(hDlg, nIDD, szBuffer);
}

////响应IDS_PROCEXPLORERMENU_EXIT菜单项
void OnExit(HWND hWnd)
{
	::DestroyWindow(hWnd);
}

INT_PTR CALLBACK SysInfoPropWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		break;
	}
	return 0L;
}

////
static void StartSystemInforSheetThreadProc(void* p)
{
	HICON hIcon; // edi
	BOOL ret; // eax
	MSG msg; // [esp+8h] [ebp-20h]
	HACCEL hAccTable; // [esp+24h] [ebp-4h]

	ghSysinfoPropSheetDlg = CreateDialogParam(ghInstance, 
		MAKEINTRESOURCE(IDD_SYSTEMINFORMATIONSHEET),
		///L"SYSINFOPROPSHEET", 
		0, 
		SysInfoPropWndProc,
		NULL);
	hIcon = LoadIcon(ghInstance, MAKEINTRESOURCE(IDC_PROCEXPLORER));
	SendMessage(ghSysinfoPropSheetDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(ghSysinfoPropSheetDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SetEvent(ghSystemInfoSheetThreadEvent);
	hAccTable = LoadAccelerators(ghInstance, L"PROCPROPERTIES");
	if (!hAccTable)
	{
		for (ret = GetMessage(&msg, 0, 0, 0); ret; ret = GetMessage(&msg, 0, 0, 0))
		{
			if (!IsDialogMessage(ghSysinfoPropSheetDlg, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	// 进入消息循环
	else
	{
		for (ret = GetMessage(&msg, 0, 0, 0); ret; ret = GetMessage(&msg, 0, 0, 0))
		{
			if (!TranslateAccelerator(ghSysinfoPropSheetDlg, hAccTable, &msg)
				&& !IsDialogMessage(ghSysinfoPropSheetDlg, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}

LRESULT CALLBACK OnShowSystemInformation(HWND hWnd, UINT wParam1, UINT wParam2, LPARAM lParam)
{
	if (ghSysinfoPropSheetDlg != NULL)
	{
		ShowWindow(ghSysinfoPropSheetDlg, SW_RESTORE);
		SetFocus(ghSysinfoPropSheetDlg);
		SetForegroundWindow(ghSysinfoPropSheetDlg);
	}
	else
	{
		HCURSOR hOldCursor = SetCursor(::LoadCursor(NULL,IDC_WAIT));
		_beginthread(StartSystemInforSheetThreadProc, 0, NULL);
		WaitForSingleObject(ghSystemInfoSheetThreadEvent, INFINITE);
		SetCursor(hOldCursor);
	}
	return 0L;
}

LRESULT CALLBACK OnShowProcessTree(HWND hWnd, UINT wParam1, UINT wParam2, LPARAM lParam)
{
	return 0L;


}

LRESULT CALLBACK OnReplaceTaskManager(HWND hWnd, UINT wParam1, UINT wParam2, LPARAM lParam)
{
	return 0L;
}

BOOL MainWnd_Hide(HWND hWnd)
{
	BOOL result; // eax

	result = ShowWindow(hWnd, SW_HIDE);
	gbProcessExplorerHide = 1;
	return result;
}
