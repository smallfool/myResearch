#include "stdafx.h"
#include "../include/NtDefs.h"
#include "../ProcExplorer/ProcExplorer.h"
#include "../include/Resize.h"

static LRESULT CALLBACK DefaultHandle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}


DEFINE_CALLBACK(Command)
DEFINE_CALLBACK(Paint)
DEFINE_CALLBACK(Size)
DEFINE_CALLBACK(LButtonDown)
DEFINE_CALLBACK(LButtonUp)
DEFINE_CALLBACK(MouseMove)
DEFINE_CALLBACK(DrawItem)
DEFINE_CALLBACK(GetMinmaxInfo)

DEFINE_CALLBACK(Create)
DEFINE_CALLBACK(Close)
DEFINE_CALLBACK(Destroy)
DEFINE_CALLBACK(MeasureItem)
DEFINE_CALLBACK(Notify)
DEFINE_CALLBACK(EndSession)
DEFINE_CALLBACK(SetFocus)
DEFINE_CALLBACK(KillFocus)
DEFINE_CALLBACK(SysCommand)
DEFINE_CALLBACK(ContextMenu)
DEFINE_CALLBACK(InitMenuPopup)
DEFINE_CALLBACK(CopyData)
DEFINE_CALLBACK(DeviceChange)
DEFINE_CALLBACK(MenuCommand)


void OnExit(HWND hWnd);

static MSGFUNC WindowMsgFunc[] =
{
	{ WM_COMMAND,HandleCommand },
	{ WM_PAINT,	 HandlePaint },
	{ WM_SIZE,   HandleSize },
	{ WM_LBUTTONDOWN,HandleLButtonDown },
	{ WM_CREATE,HandleCreate },

	{ WM_CLOSE,HandleClose },
	{ WM_DESTROY,HandleDestroy },
	{ WM_SETFOCUS,HandleSetFocus },
	{ WM_KILLFOCUS,HandleKillFocus },
	{ WM_LBUTTONUP,HandleLButtonUp },

	{ WM_MOUSEMOVE, HandleMouseMove },
	{ WM_DRAWITEM,HandleDrawItem },
	{ WM_MEASUREITEM,HandleMeasureItem },
	{ WM_NOTIFY,HandleNotify },
	{ WM_GETMINMAXINFO,HandleGetMinmaxInfo },

	{ WM_ENDSESSION,HandleEndSession },
	{ WM_DEVICECHANGE,HandleDeviceChange },
	{ WM_SYSCOMMAND,HandleSysCommand },
	{ WM_CONTEXTMENU,HandleContextMenu },
	{ WM_INITMENUPOPUP, HandleInitMenuPopup },

	{ WM_MENUCOMMAND,HandleMenuCommand },
	{ WM_COPYDATA,HandleCopyData },

};

static MSGFUNC CommandMsgFunc[] =
{
	{ 0,(WNDPROC)DefaultHandle },
	{ IDS_PROCEXPLORERMENU_EXIT,(WNDPROC)OnExit },
	{ IDM_VIEW_SHOW_PROCESS_TREE, (WNDPROC)OnShowProcessTree },
	{ IDM_VIEW_SYSTEM_INFORMATION,(WNDPROC)OnShowSystemInformation },
	{ IDM_OPTIONS_REPLACE_TASK_MANAGER ,(WNDPROC)OnReplaceTaskManager },

	{ IDM_FIND_HANDLE_OR_DLL,(WNDPROC)OnFindHandleOrDll },



};


MSGHANDLER gNormalWindowMsgHandler = { sizeof(WindowMsgFunc) / sizeof(*WindowMsgFunc), WindowMsgFunc, WINDOW_NORMAL };
MSGHANDLER gNormalWindowCmdHandler = { sizeof(CommandMsgFunc) / sizeof(*CommandMsgFunc), CommandMsgFunc,WINDOW_NORMAL };



bool gbFindHandlesOrDlls = false;
HICON ghIconMainOfFindDialg = NULL;
COLUMNWIDTH gdwFindDlgListColumnWidths = { 0 };
COLUMNWIDTH gdwFindDlgListColumDefnWidths = { 0 };
DWORD gdwSelectedItemInFindDialog = -1;
DWORD gdwRefreshRateOfSearchCopy = -1;

int gnItemSortedInFindDialog = -1;
TCHAR gszTextToFind[256] = { 0 };
TCHAR gszTextToFindCopy[256] = { 0 };
int gcxFindDlg = -1;
int gcyFindDlg = -1;
RECT gRectFindDialog = { 0 };
bool gbCancelFindInFindDlg = false;
HANDLE ghSearchThreadHandle = NULL;
int gnItemSortedInFindDlg = -1;
bool gbFINDDLGLISTAscending = true;


extern HWND ghWndListCtrlInFindDialog;
extern HWND ghDlgFindDialog;
extern HWND ghWndNewOwner;
extern HINSTANCE ghInstance;
extern WNDPROC OldListCtrlWndProc;
extern HFONT ghDefaultFont;
extern DWORD gdwRefreshRateOfSearch;

extern WNDPROC OldListCtrlWndProc;
extern void InitToolTips(HWND hWnd);
extern int __stdcall InitListCtrl(HWND hWnd, int *uIDs, short *nWidths, LONG dwCount);
extern LRESULT CALLBACK HookListCtrlWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam);
extern void InitSortFlagsImageList(HWND hWndList);
extern bool  InitTreeListSortableHeader(HWND hWnd, int item1, int item2, bool Ascending);
enum EFindDlgColumnIndex
{
	FDCL_Process,
	FDCL_PID,
	FDCL_Type,
	FDCL_Name,
	FDCL_Others,
};

int CALLBACK SortInFindDlgCallback(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int col = (int)lParamSort;
	int res = 0;
	switch (col)
	{
	case FDCL_Process:
	case FDCL_PID:
	case FDCL_Type:
	case FDCL_Name:
	default:
		return 0;
	}

	if (gbFINDDLGLISTAscending)
		res = -res;
	return res;
}


unsigned __stdcall SearchThreadProc(void* p)
{
	bool* bCancelFindInFindDlg = (bool*)p;
	BOOL b = *bCancelFindInFindDlg ? TRUE : FALSE;
	SearchDlls(ghWndListCtrlInFindDialog, 0, &b);


	PostMessage(GetParent(ghWndListCtrlInFindDialog), FDM_ENDSORT, 0, 0);
	return 0;
}




static INT_PTR CALLBACK Se(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	static bool sbInited = false;
	if (!sbInited)
	{
		sbInited = true;
		gdwFindDlgListColumnWidths.width1 = MulDiv(75, gLogPixelsX, 96);
		gdwFindDlgListColumnWidths.width2 = MulDiv(-1, gLogPixelsX, 96);
		gdwFindDlgListColumDefnWidths.width1 = MulDiv(50, gLogPixelsX, 96);
		gdwFindDlgListColumDefnWidths.width2 = MulDiv(150, gLogPixelsX, 96);
	}
	switch (msg)
	{
	case PMT_MSG_7F1:
	{
		ghDlgFindDialog = NULL;
		ghWndListCtrlInFindDialog = NULL;
		GetWindowRect(hWnd, &gRectFindDialog);
		gcxFindDlg = gRectFindDialog.right - gRectFindDialog.left;
		gcyFindDlg = gRectFindDialog.bottom - gRectFindDialog.top;
	}
		return 1;
	case WM_CLOSE:
		SendMessage(hWnd, WM_COMMAND, IDCANCEL, 0);
		return 1;
	case WM_SHOWWINDOW:
	{
		if (wParam == SW_PARENTCLOSING)
		{
			SetDlgItemText(hWnd, IDC_FINDDIALOG_VALUE_TO_FIND, gpszTargetName);
			InitTreeList(ghWndListCtrlInFindDialog, ghDefaultFont);
			SendMessage(ghWndListCtrlInFindDialog, LVM_DELETEALLITEMS, 0, 0);
			SetDlgItemText(hWnd, IDC_FINDDIALOG_TEXT_TO_FIND, _TEXT("Handle or DLL substring:"));
			gdwRefreshRateOfSearchCopy = gdwRefreshRateOfSearch;
			gdwRefreshRateOfSearch = 0;
			SendMessage(ghWndStatusBar, SB_SETTEXT, gdwStatusBarCount - 1, (LPARAM)(_TEXT("Paused")));
			return 1;
		}
		gdwRefreshRateOfSearch = gdwRefreshRateOfSearchCopy;
		SetEvent(ghEventRefreshSearchResult);
		if (!gdwRefreshRateOfSearchCopy)
			return 1;
		SendMessage(ghWndStatusBar, SB_SETTEXTW, gdwStatusBarCount - 1, (LPARAM)gpszTargetName);
		return 1;
	}
	break;
	case WM_DRAWITEM:
		ListCtrlDrawItem((LPDRAWITEMSTRUCT)lParam);
		return 1;
	case WM_MEASUREITEM:
		return ListCtrlMeasureItem(hWnd,msg,wParam, (LPMEASUREITEMSTRUCT)lParam);
		break;
	case WM_NOTIFY:
	{
		return ListCtrlNotifyInFindDlg((NMLISTVIEW*)lParam);
		return	0;
	}
	break;
	case WM_COMMAND:
	{
		HWND hCancel= GetDlgItem(hWnd, IDC_FINDDIALOG_BUTTON_CANCEL);
		if (wParam == IDC_FINDDIALOG_BUTTON_CANCEL)
		{
			gbCancelFindInFindDlg = true;
			return 0;
		}
		if (wParam == IDCANCEL)
		{
			if (IsWindowEnabled(hCancel))
			{
				SendMessageW(hWnd, WM_COMMAND, IDC_FINDDIALOG_BUTTON_CANCEL, 0);
				return 1;
			}
		}
		else if (wParam != IDOK)
		{
			if (wParam == IDC_FINDDIALOG_VALUE_TO_FIND)
			{
				gdwSelectedItemInFindDialog = -1;
				GetDlgItemTextW(hWnd, IDC_FINDDIALOG_VALUE_TO_FIND, gszTextToFind, 256);
				if (wcslen(gszTextToFind))
				{
					InitTreeListSortableHeader(
						ghWndListCtrlInFindDialog,
						gnItemSortedInFindDlg,
						gnItemSortedInFindDlg,
						gbFINDDLGLISTAscending);
					EnableWindow(GetDlgItem(hWnd, IDC_FINDDIALOG_TEXT_TO_FIND), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_FINDDIALOG_VALUE_TO_FIND), FALSE);
					EnableWindow(hCancel, TRUE);
					SetFocus(hCancel);
					if (ghSearchThreadHandle)
					{
						WaitForSingleObject(ghSearchThreadHandle, INFINITE);
						CloseHandle(ghSearchThreadHandle);
					}
					gbCancelFindInFindDlg = false;
					SendMessage(ghWndListCtrlInFindDialog, LVM_DELETEALLITEMS, 0, 0);
					SetDlgItemText(hWnd, IDC_FINDDIALOG_STATUS, _TEXT("Searching..."));
					ghSearchThreadHandle = (HANDLE)_beginthreadex(0, 0, SearchThreadProc, &gbCancelFindInFindDlg, 0, 0);
				}
			}
			return 0;
		}
		ShowWindow(hWnd, SW_HIDE);
		gbCancelFindInFindDlg = true;
		SendMessage(ghWndListCtrlInFindDialog, LVM_DELETEALLITEMS, 0, 0);
		int iItem = 0;
		short* pnWidthds = (short*)&gdwFindDlgListColumnWidths;
		do
		{
			pnWidthds[ iItem++] = MulDiv(SendMessage(ghWndListCtrlInFindDialog, LVM_GETCOLUMNWIDTH, iItem, 0), 96, gLogPixelsX);
		} while (iItem < 4);
		_tcscpy_s(gszTextToFindCopy, 256, gszTextToFind);
		return 1;
	}
	break;
	case FDM_ENDSORT:
	{
		CloseHandle(ghSearchThreadHandle);
		ghSearchThreadHandle = NULL;
		EnableWindow(GetDlgItem(hWnd, IDC_FINDDIALOG_TEXT_TO_FIND), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_FINDDIALOG_VALUE_TO_FIND), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_FINDDIALOG_BUTTON_CANCEL), FALSE);
		SendMessage(hWnd, FDM_STARTSORT, 0, 0);
		SetFocus(GetDlgItem(hWnd, IDC_FINDDIALOG_VALUE_TO_FIND));
	}
		break;
	case FDM_STARTSORT:
	{
		SendMessage(ghWndListCtrlInFindDialog, LVM_SORTITEMS, gnItemSortedInFindDlg, (LPARAM)SortInFindDlgCallback);
		int count = SendMessage(ghWndListCtrlInFindDialog, LVM_GETITEMCOUNT, 0, 0);
		TCHAR szText[256];
		_stprintf_s(szText, 256, _TEXT("%d matching items."), count);
		SetDlgItemText(hWnd, IDC_FINDDIALOG_STATUS, szText);
	}
	break;
	case WM_INITDIALOG:
	{
		LVCOLUMN;TRANSPARENT;

		CResizer* pResizer = new CResizer(hWnd);
		ghIconMainOfFindDialg = LoadIcon(ghInstance, MAKEINTRESOURCE(IDR_PROCESSEXPLORER));
		DlgSetSmallIcon(hWnd, ghIconMainOfFindDialg);
		HWND hWndList = GetDlgItem(hWnd, IDC_FINDDIALOG_LISTCTRL);
		DWORD dwStyle = GetStyle(hWndList);
		ghWndListCtrlInFindDialog = hWndList;
		SetUserData(hWndList, 3);

		RemoveStyle(hWndList, LVS_REPORT | LVS_SMALLICON);
		AddStyle(hWndList, LVS_REPORT);
		
		AddStyle(hWndList, LVS_SHAREIMAGELISTS);
		DWORD dwExStyle = LVS_EX_SINGLEROW | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT;
		AddExStyle(hWndList, dwExStyle);
		HWND hWndTooltips = LV_GetTooltips(hWndList);
		InitToolTips(hWndTooltips);
		OldListCtrlWndProc = HookWnd(hWndList, HookListCtrlWndProc);
		DlgSetFont(hWndList, ghDefaultFont);

		InitSortFlagsImageList(hWndList);
		if (gdwFindDlgListColumnWidths.width2 == -1)
			gdwFindDlgListColumnWidths.width2 = MulDiv(40, gLogPixelsX, 96);

		COLUMNWIDTH width[2] = { gdwFindDlgListColumnWidths , gdwFindDlgListColumnWidths };
		int nIDs[4] = { 0x16,0x15,4,3 };
		__m128i xxxxx;
		xxxxx.m128i_u32[0] = 0x16;
		xxxxx.m128i_u32[1] = 0x15;
		xxxxx.m128i_u32[2] = 0x4;
		xxxxx.m128i_u32[3] = 0x3;
		
		//16000000150000000400000003h;
		//__asm {xmmword_4D6C30  xmmword 16000000150000000400000003h}
		//_mm_storeu_si128((__m128i*)nIDs, _mm_load_si128((__m128i*)&xmmword_4D6C30));
		InitListCtrl(hWndList, nIDs, (short*)width,4);

		SendMessage(ghWndListCtrlInFindDialog, LVM_SETCOLUMNWIDTH, 3, 65534);

		SetDlgItemTextW(hWnd, IDC_FINDDIALOG_VALUE_TO_FIND, gszTextToFindCopy);
		if (gcxFindDlg)
		{
			MoveWindow(hWnd, gRectFindDialog.left, gRectFindDialog.top, gcxFindDlg, gcyFindDlg, FALSE);
		}
		else
		{
			if (gWindowPlacement[FindWindowplacement].rcNormalPosition.right)
				SetWindowPlacement(hWnd, &gWindowPlacement[FindWindowplacement]);
		}
		ShowWindow(hWnd, SW_SHOWNORMAL);
		return 1;
	}
	break;
	default:
		return 0;
	}
	return 0L;
}

LRESULT CALLBACK OnFindHandleOrDll(HWND hWnd, UINT wParam1, UINT wParam2, LPARAM lParam)
{
	gbFindHandlesOrDlls = wParam1 == IDM_FIND_HANDLE_OR_DLL ? true : false;
	if (ghDlgFindDialog)
	{
		ShowWindow(ghDlgFindDialog, SW_SHOW);
		SetFocus(ghDlgFindDialog);
	}
	else
	{
		///ghDlgFindDialog = CreateDialogParam(ghInstance, MAKEINTRESOURCE(IDD_FINDDIALOG), ghWndNewOwner, FindDialogDlgProc, (LPARAM)ghDlgFindDialog);
	}
	return	0;
	return ::DefWindowProc(hWnd, WM_COMMAND, MAKEWPARAM(wParam1,wParam2), lParam);
}