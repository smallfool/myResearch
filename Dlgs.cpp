#include "stdafx.h"
#include "../include/Common.h"
#include "../ProcExplorer/ProcExplorer.h"


SYSINFOPAGEFUNCITEM gSystemInfoPageItem[] =
{
	{ TEXT("Summary"),	SummaryWndProc,IDD_PAGE_SUMMARY },
	{ TEXT("CPU"),		CPUWndProc,IDD_PAGE_CPUINFO },
	{ TEXT("Memory"),	MemoryWndProc,IDD_PAGE_MEMORY_INFO },
	//{ TEXT("I/O"),		IOWndProc,IDD_PAGE_IOINFO },
	//{ TEXT("GPU"),		GPUWndProc,IDD_PAGE_IGPUINFO },
};

///System系统属性窗口回调函数
LRESULT CALLBACK SysInfoPropertiesDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	NMHDR* pnmhdr = NULL;
	HWND hWndTab = NULL;
	HWND hBtnOK = NULL;
	int nCount = 0;
	int i = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		hWndTab = GetDlgItem(hWnd, IDC_TAB_SYSTEMINFO);
		hBtnOK = GetDlgItem(hWnd, IDOK);
		break;
	case WM_COMMAND:
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_TIMER:
		hWndTab = GetDlgItem(hWnd, IDC_TAB_SYSTEMINFO);
		hBtnOK = GetDlgItem(hWnd, IDOK);
		ATLASSERT(hWndTab != NULL);
		nCount = TabCtrl_GetItemCount(hWnd);
		for (i = 0; i < nCount;i++)
		{
			HWND hWndPage = (HWND)TabCtrl_GetItemParam(hWndTab, i);
			if (hWndPage != NULL)
				SendMessage(hWndPage, WM_TIMER, wParam, lParam);
		}
		break;
	case WM_NOTIFY:
		break;
	}
	return 0L;
}

///-----------------------------------------------------------------
///
LRESULT CALLBACK ProcJobWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
		break;
	default:
		break;
	}
	return 0L;
}

INT_PTR CALLBACK ServiceControlDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{

	}
		break;
	case WM_TIMER:
		break;
	case WM_COMMAND:
	{
		SC_HANDLE Service = (SC_HANDLE)::GetProp(hWnd, _TEXT("Service"));
		if (Service != NULL)
		{
			CloseServiceHandle(Service);
		}
		SetProp(hWnd, _TEXT("Service"), (HANDLE)0);
		EndDialog(hWnd, 0);
		return	0;
	}
	default:
		break;
	}
	return	0;
}

///打开服务管理对话框
BOOL OpenServiceControlDlg(HWND hWndParent, void* pUserData)
{
	INT_PTR nRet = DialogBoxParam(ghInstance, MAKEINTRESOURCE(IDD_SERVICE_CONTROL), hWndParent, ServiceControlDlgProc, (LPARAM)pUserData);
	if (nRet == IDOK)
	{
		return	FALSE;
	}
	return	TRUE;
}

/*
int __stdcall ProcStringsWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG v4; // esi
	HWND v5; // ebx
	CResize *v7; // eax
	LONG v8; // eax
	HWND v9; // ST14_4
	HWND hWndParent; // eax
	LRESULT v11; // ST14_4
	HWND v12; // eax
	LRESULT v13; // ST14_4
	HWND v14; // eax
	void *v15; // eax
	LONG v16; // ecx
	HANDLE v17; // eax
	char v18; // al
	HANDLE v19; // esi
	void(__stdcall *v20)(HWND, UINT, WPARAM, LPARAM); // esi
	LRESULT v21; // ST14_4
	HWND v22; // eax
	LRESULT v23; // ST14_4
	HWND v24; // eax
	wchar_t *v25; // eax
	HWND v26; // eax
	HANDLE v27; // esi
	LRESULT v28; // eax
	HGLOBAL v29; // edi
	_WORD *v30; // eax
	HGLOBAL v31; // eax
	int v32; // esi
	rsize_t v33; // ST04_4
	LRESULT i; // esi
	HCURSOR v35; // esi
	int v36; // eax
	HCURSOR v37; // esi
	char v38; // bl
	HWND v39; // esi
	HWND v40; // eax
	HWND v41; // eax
	HWND v42; // eax
	WPARAM v43; // [esp-8h] [ebp-29Ch]
	LPARAM v44; // [esp+Ch] [ebp-288h]
	int v45; // [esp+14h] [ebp-280h]
	int v46; // [esp+18h] [ebp-27Ch]
	int v47; // [esp+1Ch] [ebp-278h]
	WCHAR *v48; // [esp+20h] [ebp-274h]
	int v49; // [esp+24h] [ebp-270h]
	LPARAM lParama; // [esp+40h] [ebp-254h]
	WPARAM v51; // [esp+44h] [ebp-250h]
	const wchar_t *v52; // [esp+54h] [ebp-240h]
	int v53; // [esp+60h] [ebp-234h]
	LPCVOID lpBaseAddress; // [esp+74h] [ebp-220h]
	int dwBytesToRead; // [esp+78h] [ebp-21Ch]
	int Indent; // [esp+7Ch] [ebp-218h]
	int a3; // [esp+80h] [ebp-214h]
	HANDLE hMem; // [esp+84h] [ebp-210h]
	HANDLE hObject; // [esp+88h] [ebp-20Ch]
	LONG dwNewLong; // [esp+8Ch] [ebp-208h]
	WCHAR String; // [esp+90h] [ebp-204h]

	hObject = hWnd;
	dwNewLong = lParam;
	v4 = GetWindowLongW(hWnd, GWL_USERDATA);
	lParama = 0;
	memset(&v51, 0, 0x30u);
	v5 = GetDlgItem(hWnd, 1198);
	Indent = 1058;
	LOWORD(a3) = MulDiv(100, gLogPixelsX, 96);
	if (uMsg > 0x110)
	{
		switch (uMsg)
		{
		case WM_COMMAND:
			if (wParam > IDS_PROCEXPLORERMENU_SAVE)
			{
				if (wParam > 40631)
				{
					if (wParam == 40634 && byte_13C10E0)
					{
						v35 = SetCursor(ghArrowCursor);
						sub_12F8E30(hObject, v5, &ofn, &word_13BD0B8, &byte_13C10E0, &word_13C10E8);
						SetCursor(v35);
						ReleaseCapture();
					}
				}
				else if (wParam == 40631)
				{
					for (i = 0; i < SendMessageW(v5, LVM_GETITEMCOUNT, 0, 0); ++i)
					{
						v47 = 2;
						v46 = 2;
						SendMessageW(v5, LVM_SETITEMSTATE, i, &v44);
					}
				}
				else
				{
					if (wParam != 40083)
					{
						if (wParam - 40092 > 1)
							return 0;
						v43 = wParam;
						goto LABEL_50;
					}
					v27 = 0;
					dwNewLong = 0;
					v28 = SendMessageW(v5, LVM_GETNEXTITEM, v51, 2);
					v51 = v28;
					if (v28 != -1)
					{
						v29 = 0;
						do
						{
							v45 = 0;
							v48 = &String;
							v49 = 256;
							SendMessageW(v5, LVM_GETITEMTEXTW, v28, &v44);
							dwNewLong += wcslen(&String) + 2;
							if (v27)
							{
								v29 = GlobalReAlloc(v29, 2 * dwNewLong + 2, 0);
								v30 = GlobalLock(v29);
							}
							else
							{
								v31 = GlobalAlloc(0x2002u, 2 * dwNewLong + 2);
								v29 = v31;
								v30 = GlobalLock(v31);
								*v30 = 0;
							}
							v32 = dwNewLong + 1;
							hMem = v30;
							wcscat_s(v30, dwNewLong + 1, &String);
							v33 = v32;
							v27 = hMem;
							wcscat_s(hMem, v33, L"\r\n");
							GlobalUnlock(v29);
							v28 = SendMessageW(v5, 0x100Cu, v51, 2);
							v51 = v28;
						} while (v28 != -1);
						hMem = v29;
						if (v29)
						{
							if (OpenClipboard(hObject))
							{
								EmptyClipboard();
								SetClipboardData(CF_UNICODETEXT, hMem);
								CloseClipboard();
							}
						}
					}
				}
			}
			else if (wParam == IDS_PROCEXPLORERMENU_SAVE)
			{
				if (*(v4 + 4))
				{
					v25 = wcsrchr(*(v4 + 344), '\\');
					SaveStringToFile(v5, (v25 + 1));
				}
				else
				{
					SaveStringToFile(v5, *(v4 + 60));
				}
			}
			else
			{
				if (wParam <= 1304)
				{
					switch (wParam)
					{
					case 0x518u:
						SendMessageW(v5, WM_SETREDRAW, 0, 0);
						SendMessageW(v5, LVM_DELETEALLITEMS, 0, 0);
						if (*(v4 + 4))
							sub_13064C0(v5, *(v4 + 344));
						else
							sub_13064C0(v5, *(v4 + 792));
						SendMessageW(v5, 0x101Eu, 0, 0xFFFF);
						SendMessageW(v5, WM_SETREDRAW, 1u, 0);
						v11 = SendMessageW(v5, LVM_GETITEMCOUNT, 0, 0);
						v12 = GetDlgItem(hWnd, 40009);
						EnableWindow(v12, v11);
						v13 = SendMessageW(v5, 0x1004u, 0, 0);
						v14 = GetDlgItem(hWnd, 1210);
						EnableWindow(v14, v13);
						break;
					case 2u:
						EndDialog(hWnd, 0);
						return 1;
					case 0x4BAu:
						tcscpy_s(&word_13BD0B8, 0x2000u, &word_13C10E8);
						ofn.hInstance = ghInstance;
						ofn.lStructSize = 40;
						ofn.hwndOwner = hWnd;
						ofn.lpstrFindWhat = &word_13BD0B8;
						ofn.lpstrReplaceWith = 0;
						*&ofn.wFindWhatLen = 0x2000;
						ofn.lCustData = 0;
						ofn.Flags = dword_13A10E0;
						ofn.lpfnHook = 0;
						ofn.lpTemplateName = 0;
						v9 = FindTextW(&ofn);
						hWndParent = GetParent(hWnd);
						SetPropW(hWndParent, L"FindWindow", v9);
						break;
					}
					return 0;
				}
				if (wParam != IDD_PROCSTRINGS_RADIO_MEMORY)
				{
					if (wParam != IDS_PROCEXPLORERMENU_EXIT)
						return 0;
					v43 = 40014;
				LABEL_50:
					v26 = GetParent(hWnd);
					PostMessageW(v26, WM_COMMAND, v43, 0);
					return 0;
				}
				SendMessageW(v5, WM_SETREDRAW, 0, 0);
				SendMessageW(v5, LVM_DELETEALLITEMS, 0, 0);
				if (*(v4 + 4))
				{
					v15 = *(v4 + 40);
					v16 = *(v4 + 344);
				}
				else
				{
					v15 = *(v4 + 68);
					v16 = *(v4 + 792);
				}
				dwNewLong = v16;
				hMem = v15;
				v17 = OpenProcess(0x410u, 0, v15);
				hObject = v17;
				if (v17)
				{
					if (*(v4 + 4))
					{
						sub_1306620(v5, v17, *(v4 + 36), *(v4 + 56));
						v19 = hObject;
					}
					else
					{
						v18 = sub_13203E0(v17, hMem, dwNewLong, &lpBaseAddress, &dwBytesToRead);
						v19 = hObject;
						if (v18)
							sub_1306620(v5, hObject, lpBaseAddress, dwBytesToRead);
					}
					CloseHandle(v19);
					v20 = SendMessageW;
				}
				else
				{
					v20 = SendMessageW;
					lParama = 5;
					v51 = 0x7FFFFFFF;
					v52 = L"<Error opening process>";
					v53 = 1;
					SendMessageW(v5, 0x104Du, 0, &lParama);
				}
				v20(v5, 0x101Eu, 0, 0xFFFF);
				v20(v5, 0xBu, 1u, 0);
				v21 = (v20)(v5, 0x1004u, 0, 0);
				v22 = GetDlgItem(hWnd, 40009);
				EnableWindow(v22, v21);
				v23 = SendMessageW(v5, 0x1004u, 0, 0);
				v24 = GetDlgItem(hWnd, 1210);
				EnableWindow(v24, v23);
			}
			return 0;
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSTATIC:
			return sub_1306710(hWnd, wParam);
		default:
			goto LABEL_67;
		}
	}
	if (uMsg != 272)
	{
		switch (uMsg)
		{
		case 0x2Bu:
			FindDlg_DrawItem(dwNewLong);
			return 0;
		case 0x2Cu:
			return HandleMeasureItem(hWnd, uMsg, wParam, dwNewLong);
		case 0x101u:
			if (wParam == 114)
				SendMessageW(hWnd, WM_COMMAND, 0x9EBAu, 0);
			return 0;
		case 0x102u:
			if (wParam == 70 || wParam == 102)
			{
				if (GetKeyState(17))
					SendMessageW(hWnd, WM_COMMAND, 0x4BAu, 0);
			}
			return 0;
		default:
			break;
		}
	LABEL_67:
		if (uMsg == gMSGCommdlg_FindReplace)
		{
			v36 = *(dwNewLong + 12);
			if (v36 & 0x40)
			{
				dword_13A10E0 = ofn.Flags & 7;
			}
			else if (v36 & 8)
			{
				v37 = SetCursor(ghArrowCursor);
				v38 = sub_12F8E30(hObject, v5, dwNewLong, &word_13BD0B8, &byte_13C10E0, &word_13C10E8);
				SetCursor(v37);
				ReleaseCapture();
				v39 = hObject;
				v40 = GetParent(hObject);
				v41 = GetPropW(v40, L"FindWindow");
				if (v38 && v41)
				{
					PostMessageW(v41, 0x111u, 3u, 0);
					v42 = GetParent(v39);
					SetPropW(v42, L"FindWindow", 0);
				}
				else
				{
					SetFocus(v41);
				}
			}
		}
		return 0;
	}
	v7 = operator new(0x40u);
	if (v7)
		CResize::CResize(v7, hWnd);
	GetWindowTextW(hWnd, &String, 256);
	ResizeTab(hWnd);
	SetWindowLongW(hWnd, -21, dwNewLong);
	CheckRadioButton(hWnd, 1304, 1305, 1304);
	v8 = GetWindowLongW(v5, GWL_STYLE);
	SetWindowLongW(v5, GWL_STYLE, v8 & 0xFFFFFFFD | 1);
	SetWindowLongW(v5, GWL_WNDPROC, PEListCtrlWndProc);
	SendMessageW(v5, 0x30u, ghDefaultFont, 0);
	InitListCtrl(v5, &Indent, &a3, 1);
	PostMessageW(hWnd, 0x111u, 0x518u, 0);
	return 1;
}


*/