#include "stdafx.h"
#include "..\include\PropSheet.h"
#include "../include/TreeList.h"

CPage::CPage()
{

}

CPage::~CPage()
{

}
//////////////////////////////////////////
CPropSheet::CPropSheet()
{
}


CPropSheet::~CPropSheet()
{
}

// 显示进程属性对话框
BOOL WINAPI ProcessPropsheetDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM pDlgProp)
{
	//HWND hTabCtrl; // eax
	//HWND hTab; // ebx
	//CPropSheet *pDlgSheet; // eax
	//PROPPAGEDATA *pPageData; // edi
	//HWND TabCount; // eax
	//LRESULT nIndex; // eax
	//CPropSheet *v11; // ebx
	//int v12; // esi
	//CPropSheet *PageData1; // edi
	//BOOL(__stdcall *v14)(HWND, UINT, WPARAM, LPARAM); // ST14_4
	//signed int MaxCount; // edi
	//PROPPAGEDATA *PageData; // esi
	//WPARAM nTab_1; // eax
	//LRESULT v18; // eax
	//WPARAM v19; // ecx
	//int *v20; // eax
	//CResize *pResizeDlg; // eax
	//LONG dwExStyle; // eax
	//LONG dwStyle; // eax
	//const wchar_t *pszName; // ecx
	//WPARAM nTab; // ebx
	//WPARAM iTab; // esi
	//WPARAM iiTab; // esi
	//TREELISTITEMPARAM *pData; // eax
	//WPARAM tab; // esi
	//WPARAM tabidx; // esi
	//HWND hTabWnd; // esi
	//WPARAM nTabCount; // ebx
	//CPropSheet *pUserData; // eax
	//int tabcount; // ebx
	//int nItemCount; // esi
	//int nCount; // esi
	//int nTabIndex; // ebx
	//CPropSheet *processHandle; // eax
	//TREELISTITEMPARAM *pTreeItemData; // ecx
	//CPropSheet *hProcessHandle; // esi
	//bool v41; // zf
	//TREELISTITEMPARAM *pTreeListItemData; // eax
	//int nIndexTab; // esi
	//int nIdxTab; // esi
	//void *v45; // esi
	//int nCurTab; // esi
	//int nCurrentTab; // esi
	//WPARAM nLastTab; // ST14_4
	//TREELISTITEMPARAM *v49; // eax
	//HWND hWndTab; // esi
	//int v51; // eax
	//WPARAM v52; // ecx
	//HCURSOR v53; // eax
	//int v54; // [esp+0h] [ebp-274h]
	//POINT Point; // [esp+10h] [ebp-264h]
	//NMHDR nmhdr; // [esp+18h] [ebp-25Ch]
	//TCITEMW item; // [esp+24h] [ebp-250h]
	//HWND hWndDlg; // [esp+40h] [ebp-234h]
	//HWND hWndTabCtrl; // [esp+44h] [ebp-230h]
	//CPropSheet *hObject; // [esp+48h] [ebp-22Ch]
	//RECT Rect; // [esp+4Ch] [ebp-228h]
	//TCHAR String[260]; // [esp+5Ch] [ebp-218h]
	//int v63; // [esp+270h] [ebp-4h]

	//hWndDlg = hDlg;
	//hTabCtrl = GetDlgItem(hDlg, IDD_PROCPROPSHEET_CTRL_TABCTRL);
	//hTab = hTabCtrl;
	//hWndTabCtrl = hTabCtrl;
	//pDlgSheet = GetWindowLongW(hDlg, GWL_USERDATA);
	//hObject = pDlgSheet;
	//if (uMsg > WM_NOTIFY)
	//{
	//	if (uMsg == WM_INITDIALOG)
	//	{
	//		ShowWindow(hDlg, SW_HIDE);

	//		pResizeDlg = new CResize(hDlg);
	//		SetWindowLongW(hDlg, GWL_USERDATA, pDlgProp);
	//		item.mask = 3;                            // TCIF_TEXT|TCIF_IMAGE
	//		item.iImage = -1;
	//		dwExStyle = GetWindowLongW(hTab, GWL_EXSTYLE);
	//		SetWindowLongW(hTab, GWL_EXSTYLE, dwExStyle | WS_EX_CONTROLPARENT);
	//		dwStyle = GetWindowLongW(hTab, GWL_STYLE);
	//		SetWindowLongW(hTab, GWL_STYLE, dwStyle | WS_CLIPCHILDREN);
	//		if (pDlgProp->TreeListItemData)
	//		{
	//			InitTreeListItemColor(pDlgProp->TreeListItemData);
	//			item.pszText = L"Image";
	//			SendMessageW(hTab, TCM_INSERTITEMW, 0, &item);
	//			wcscpy_s(pDlgProp->PageData[1].TemplateName, 0x100u, L"PROCPERF");
	//			pDlgProp->PageData[1].TabId = 2;

	//			pDlgProp->PageData[1].PageWndProc = PerfPageWndProc;
	//			item.pszText = L"Performance";
	//			SendMessageW(hTab, TCM_INSERTITEMW, 1u, &item);
	//			pszName = L"PROCCPUIO";
	//			if (!pDlgProp->TreeListItemData->field_428)
	//				pszName = L"PROCCPU";
	//			wcscpy_s(pDlgProp->PageData[2].TemplateName, 0x100u, pszName);
	//			pDlgProp->PageData[2].TabId = 4;
	//			pDlgProp->PageData[2].PageWndProc = PerfGraphPageWndProc;
	//			item.pszText = L"Performance Graph";
	//			SendMessageW(hTab, TCM_INSERTITEMW, 2u, &item);
	//			nTab = 3;
	//			if (gdwTraceEventTimeout)
	//			{
	//				wcscpy_s(pDlgProp->PageData[3].TemplateName, 0x100u, L"PROCPERFETW");
	//				pDlgProp->PageData[3].TabId = 14;
	//				pDlgProp->PageData[3].PageWndProc = ProcPerfETWWndProc;
	//				item.pszText = L"Disk and Network";
	//				SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, 3u, &item);
	//				nTab = 4;
	//			}
	//			if (gdwAdapterRuntingTime)
	//			{
	//				iTab = nTab;
	//				wcscpy_s(pDlgProp->PageData[nTab].TemplateName, 0x100u, L"PROCGPU");
	//				pDlgProp->PageData[iTab].TabId = 15;
	//				pDlgProp->PageData[iTab].PageWndProc = ProcGPUWndProc;
	//				item.pszText = L"GPU Graph";
	//				SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, nTab++, &item);
	//			}
	//			if (pDlgProp->TreeListItemData->field_28 & 0x20)
	//			{
	//				iiTab = nTab;
	//				wcscpy_s(pDlgProp->PageData[nTab].TemplateName, 0x100u, L"PROCSERVICES");
	//				pDlgProp->PageData[iiTab].TabId = 5;
	//				pDlgProp->PageData[iiTab].PageWndProc = ProcServicesWndProc;
	//				item.pszText = L"Services";
	//				SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, nTab++, &item);
	//			}
	//			pData = pDlgProp->TreeListItemData;
	//			if (pData && pData->field_364)
	//			{
	//				tab = nTab;
	//				wcscpy_s(pDlgProp->PageData[nTab].TemplateName, 0x100u, L"PROCWMIPROVIDERS");
	//				pDlgProp->PageData[tab].TabId = 16;
	//				pDlgProp->PageData[tab].PageWndProc = ProcWMIProvidersWndProc;
	//				item.pszText = L"WMI Providers";
	//				SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, nTab++, &item);
	//			}
	//			tabidx = nTab;
	//			wcscpy_s(pDlgProp->PageData[nTab].TemplateName, 0x100u, L"PROCTHREADS");
	//			pDlgProp->PageData[tabidx].TabId = 6;
	//			pDlgProp->PageData[tabidx].PageWndProc = ProcThreadsWndProc;
	//			hTabWnd = hWndTabCtrl;
	//			item.pszText = L"Threads";
	//			SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, nTab, &item);
	//			nTabCount = nTab + 1;
	//			hObject = (pDlgProp + 0x20C * nTabCount);
	//			if (AllocateAndGetUdpExTable2FromStack)
	//				wcscpy_s(hObject->PageData[0].TemplateName, 0x100u, L"PROCTCPUDPSTACK");
	//			else
	//				wcscpy_s(hObject->PageData[0].TemplateName, 0x100u, L"PROCTCPUDP");
	//			pUserData = hObject;
	//			item.pszText = L"TCP/IP";
	//			hObject->PageData[0].TabId = 7;
	//			pUserData->PageData[0].PageWndProc = ProcTcpUdpStackWndProc;
	//			SendMessageW(hTabWnd, TCM_INSERTITEMW, nTabCount, &item);
	//			tabcount = nTabCount + 1;
	//			nItemCount = tabcount;
	//			wcscpy_s(pDlgProp->PageData[tabcount].TemplateName, 0x100u, L"PROCSECURITY");
	//			pDlgProp->PageData[nItemCount].TabId = 8;
	//			pDlgProp->PageData[nItemCount].PageWndProc = ProcSecurityWndProc;
	//			item.pszText = L"Security";
	//			SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, tabcount++, &item);
	//			nCount = tabcount;
	//			wcscpy_s(pDlgProp->PageData[tabcount].TemplateName, 0x100u, L"PROCENV");
	//			pDlgProp->PageData[nCount].TabId = 9;
	//			pDlgProp->PageData[nCount].PageWndProc = ProcEnvWndProc;
	//			item.pszText = L"Environment";
	//			SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, tabcount, &item);
	//			nTabIndex = tabcount + 1;
	//			processHandle = PEOpenProcess(0x2000000u, 0, pDlgProp->TreeListItemData->ProcessId);
	//			pTreeItemData = pDlgProp->TreeListItemData;
	//			hProcessHandle = processHandle;
	//			hObject = processHandle;
	//			v41 = sub_47DA70(processHandle, pTreeItemData->ProcessId, 0, 0) == 0;
	//			pTreeListItemData = pDlgProp->TreeListItemData;
	//			if (v41)
	//			{
	//				pTreeListItemData->field_28 &= -0x801u;
	//			}
	//			else
	//			{
	//				pTreeListItemData->field_28 |= 0x800u;
	//				nIndexTab = nTabIndex;
	//				wcscpy_s(pDlgProp->PageData[nTabIndex].TemplateName, 0x100u, L"PROCJOB");
	//				pDlgProp->PageData[nIndexTab].TabId = 10;
	//				pDlgProp->PageData[nIndexTab].PageWndProc = ProcJOBWndProc;
	//				item.pszText = L"Job";
	//				SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, nTabIndex, &item);
	//				hProcessHandle = hObject;
	//				++nTabIndex;
	//			}
	//			if (hProcessHandle)
	//				CloseHandle(hProcessHandle);
	//			if (sub_440960(pDlgProp->TreeListItemData))
	//			{
	//				nIdxTab = nTabIndex;
	//				wcscpy_s(pDlgProp->PageData[nTabIndex].TemplateName, 0x100u, L"PROCCLR");
	//				pDlgProp->PageData[nIdxTab].TabId = 11;
	//				pDlgProp->PageData[nIdxTab].PageWndProc = ProcCLRWndProc;
	//				item.pszText = L".NET Assemblies";
	//				SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, nTabIndex++, &item);
	//				if (!pDlgProp->TreeListItemData->DotNetPropData)
	//				{
	//					v45 = malloc(0x88u);
	//					memset(v45, 0, 0x88u);
	//					pDlgProp->TreeListItemData->DotNetPropData = v45;
	//				}
	//			}
	//			if (pDlgProp->TreeListItemData->DotNetPropData)
	//			{
	//				CoInitialize(0);
	//				nCurTab = nTabIndex;
	//				wcscpy_s(pDlgProp->PageData[nTabIndex].TemplateName, 0x100u, L"PROCNET");
	//				pDlgProp->PageData[nCurTab].TabId = 12;
	//				pDlgProp->PageData[nCurTab].PageWndProc = ProcNetWndProc;
	//				item.pszText = L".NET Performance";
	//				SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, nTabIndex++, &item);
	//			}
	//			nCurrentTab = nTabIndex;
	//			wcscpy_s(pDlgProp->PageData[nTabIndex].TemplateName, 0x100u, L"PROCSTRINGS");
	//			pDlgProp->PageData[nCurrentTab].TabId = 13;
	//			pDlgProp->PageData[nCurrentTab].PageWndProc = ProcStringsWndProc;
	//			item.pszText = L"Strings";
	//			nLastTab = nTabIndex;
	//			hTab = hWndTabCtrl;
	//			SendMessageW(hWndTabCtrl, TCM_INSERTITEMW, nLastTab, &item);
	//			v49 = pDlgProp->TreeListItemData;
	//			if (v49->field_360)
	//			{
	//				LogToText(String, L"%s:%d (%s) Properties", v49->szImageName, v49->ProcessId, v49->field_360);
	//			}
	//			else if (v49->ProcessId < 0)
	//			{
	//				LogToText(String, L"%s Properties", v49->szImageName);
	//			}
	//			else
	//			{
	//				LogToText(String, L"%s:%d Properties", v49->szImageName, v49->ProcessId, v54);
	//			}
	//			hWndTab = hWndDlg;
	//			SetWindowTextW(hWndDlg, String);
	//			pDlgProp->PageData[0].hWnd = CreateDialogParamW(
	//				ghInstance2,
	//				L"PROCINFO",
	//				hWndTab,
	//				ProcInfoPageProc,
	//				pDlgProp->TreeListItemData);
	//			pDlgProp->PageData[0].TabId = 1;
	//		}
	//		else
	//		{
	//			hWndTab = hWndDlg;
	//		}
	//		ShowWindow(pDlgProp->PageData[0].hWnd, SW_SHOW);
	//		SetForegroundWindow(hWndTab);
	//		v51 = pDlgProp->PageData[0].TabId;
	//		v52 = 0;
	//		if (v51)
	//		{
	//			while (gdwDefaultProcPropPage != v51)
	//			{
	//				v51 = *(&pDlgProp->TabIndex + 131 * (++v52 + 1));
	//				if (!v51)
	//					goto LABEL_77;
	//			}
	//		}
	//		else
	//		{
	//		LABEL_77:
	//			v52 = 0;
	//		}
	//		SendMessageW(hTab, TCM_SETCURSEL, v52, 0);
	//		nmhdr.idFrom = IDD_PROCPROPSHEET_CTRL_TABCTRL;
	//		nmhdr.code = TCN_SELCHANGE;
	//		SendMessageW(hWndTab, WM_NOTIFY, 0, &nmhdr);
	//		if (gWindowPlacement[SysinfoWindowplacement].rcNormalPosition.right == gWindowPlacement[SysinfoWindowplacement].rcNormalPosition.left)
	//		{
	//			GetCursorPos(&Point);
	//			SetPropSheetRect(hWndTab, Point.x - 385, Point.y - 408);
	//		}
	//		else
	//		{
	//			sub_47DC50(hWndTab, pDlgProp->TreeListItemData);
	//		}
	//		ShowWindow(hWndTab, SW_SHOW);
	//		InterlockedDecrement(&Addend);
	//		v53 = LoadCursorW(0, IDC_ARROW);
	//		SetCursor(v53);
	//		return 1;
	//	}
	//	if (uMsg == 273)
	//	{
	//		if (wParam > 40092)
	//		{
	//			if (wParam == 40093)
	//			{
	//				v18 = SendMessageW(hTab, TCM_GETCURSEL, 0, 0);
	//				v19 = v18 - 1;
	//				if (!v18)
	//				{
	//					v19 = 0;
	//					v20 = &hObject->PageData[1].PageWndProc;
	//					if (hObject->PageData[1].PageWndProc)
	//					{
	//						do
	//						{
	//							v20 += 131;
	//							++v19;
	//						} while (*v20);
	//					}
	//				}
	//				SendMessageW(hTab, TCM_SETCURSEL, v19, 0);
	//				nmhdr.idFrom = IDD_PROCPROPSHEET_CTRL_TABCTRL;
	//				nmhdr.code = -0x227u;                 // TCN_SELCHANGE
	//				SendMessageW(hDlg, WM_NOTIFY, 0, &nmhdr);
	//			}
	//		}
	//		else
	//		{
	//			if (wParam == 40092)
	//			{
	//				nTab_1 = SendMessageW(hTab, TCM_GETCURSEL, 0, 0) + 1;
	//				if (!hObject->PageData[nTab_1].PageWndProc)
	//					nTab_1 = 0;
	//				SendMessageW(hTab, TCM_SETCURSEL, nTab_1, 0);
	//				nmhdr.idFrom = IDD_PROCPROPSHEET_CTRL_TABCTRL;
	//				nmhdr.code = TCN_SELCHANGE;
	//				SendMessageW(hWndDlg, WM_NOTIFY, 0, &nmhdr);
	//				return 0;
	//			}
	//			if (wParam > 0 && (wParam <= 2 || wParam == 40014))
	//			{
	//				ShowWindow(hDlg, 0);
	//				MaxCount = 32;
	//				PageData = hObject->PageData;
	//				while (1)
	//				{
	//					if (PageData->hWnd)
	//					{
	//						SendMessageW(PageData->hWnd, WM_COMMAND, wParam, 0);
	//						SendMessageW(PageData->hWnd, PMT_MSG_7F1, 0, 0);
	//					}
	//					++PageData;
	//					if (!--MaxCount)
	//					{
	//						// 通知主界面删除本TreeListItemData对象
	//						PostMessageW(ghWndNewOwner, PMT_MSG_7ED, 0, hObject->TreeListItemData);
	//						if (hObject->TreeListItemData->DotNetPropData)
	//							CoUninitialize();
	//						free(hObject);
	//						GetWindowPlacement(hWndDlg, &gWindowPlacement[SysinfoWindowplacement]);
	//						DestroyWindow(hWndDlg);
	//						_endthread();
	//					}
	//				}
	//			}
	//		}
	//	}
	//	return 0;
	//}
	//if (uMsg == WM_NOTIFY)
	//{
	//	if (pDlgProp->TreeListItemData == IDD_PROCPROPSHEET_CTRL_TABCTRL && pDlgProp->PageData[0].hWnd == -551)
	//	{
	//		nIndex = SendMessageW(hTab, TCM_GETCURSEL, 0, 0);
	//		v11 = hObject;
	//		v12 = nIndex;
	//		PageData1 = (hObject + 0x20C * nIndex);
	//		hObject = PageData1;
	//		gdwDefaultProcPropPage = PageData1->PageData[0].TabId;
	//		ShowWindow(v11->PageData[v11->TabIndex].hWnd, SW_HIDE);
	//		v11->TabIndex = v12;
	//		if (!PageData1->PageData[0].hWnd)
	//		{
	//			v14 = PageData1->PageData[0].PageWndProc;
	//			PageData1 = hObject;
	//			hObject->PageData[0].hWnd = CreateDialogParamW(
	//				ghInstance2,
	//				hObject->PageData[0].TemplateName,
	//				hWndDlg,
	//				v14,
	//				v11->TreeListItemData);
	//		}
	//		ShowWindow(PageData1->PageData[0].hWnd, SW_SHOW);
	//		return 0;
	//	}
	//	return 0;
	//}
	//if (uMsg != WM_SIZE)
	//{
	//	if (uMsg == WM_GETMINMAXINFO)
	//	{
	//		pDlgProp->PageData[0].hWnd = 2 * gScreenWidth;
	//		return 0;
	//	}
	//	return 0;
	//}
	//// Handle WM_SIZE
	//pPageData = pDlgSheet->PageData;
	//TabCount = 32;
	//hWndDlg = 32;
	//do
	//{
	//	if (pPageData->hWnd)
	//	{
	//		GetWindowRect(hTab, &Rect);
	//		SendMessageW(hTab, TCM_ADJUSTRECT, 0, &Rect);
	//		MapWindowPoints(0, hDlg, &Rect, 2u);
	//		MoveWindow(pPageData->hWnd, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, 1);
	//		InvalidateRect(pPageData->hWnd, 0, TRUE);
	//		TabCount = hWndDlg;
	//	}
	//	++pPageData;
	//	TabCount = (TabCount - 1);
	//	hWndDlg = TabCount;
	//} while (TabCount);
	return 0;
}