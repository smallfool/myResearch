#include "stdafx.h"
#include "..\include\TreeList.h"
#include "../ProcExplorer/resource.h"
#include "../ProcExplorer/ProcExplorer.h"
#include "../include/ntdefs.h"

#define SB_GetScrollPos(hWndSB, ScrollInfo) \
	ScrollInfo.cbSize = sizeof(SCROLLINFO);	\
	ScrollInfo.nTrackPos = 0;	\
	ScrollInfo.fMask = SIF_POS;	\
	GetScrollInfo(hWndSB, SB_CTL, &ScrollInfo);

#define SB_SetScrollPos(hWndSB, nPos) \
{\
	SCROLLINFO si = {sizeof(SCROLLINFO)};	\
	si.fMask = SIF_POS;	\
	si.nTrackPos = nPos; \
	SetScrollInfo(hWndSB, SB_CTL, &si);	\
}


bool gbTreeListSortable = false;
ATOM gAtomTreeListProperty = GlobalAddAtom(_TEXT("TreeListProperty"));
ATOM gAtomTreeListAtom;

WNDPROC OldListCtrlWndProc = NULL;
PTREELISTITEMPARAM gpTreeListItemParamList = NULL;

extern HWND ghWndNewOwner;
extern HWND ghWndTreeListView;
extern HWND ghWndDllsListCtrl;
extern DWORD gdwTopLastTreeList;

extern TCHAR gpszTargetName[];
extern HINSTANCE ghInstance;
extern HFONT ghDefaultFont;


extern void InitToolTips(HWND hWndTooltips);
extern ATOM RegisterTreeListCtrl();
extern LRESULT CALLBACK HandleSize(HWND, UINT, WPARAM, LPARAM);


HIMAGELIST InitSortableImageList();
__m128i *GetListCtrlColumnMagic();

int __stdcall InitListCtrl(HWND hWnd, int *uIDs, short *nWidths, LONG dwCount);

BOOL IsTreeList(HWND hWnd)
{
	return hWnd == ghWndTreeListView;
}

/////////////////////////////////////////////////////////////
//CTreeList
CTreeList::CTreeList()
{
	m_hWnd = NULL;
}

BOOL RedrawTreeList()
{
	return InvalidateRect(ghWndTreeListView, NULL, FALSE);
}
//ATOM gAtomTreeListProperty = RegisterTreeListCtrl();
CTreeList* GetTreeListCtrl(HWND hWnd)
{
	return (CTreeList*)GetProp(hWnd, (LPCTSTR)gAtomTreeListProperty);
}


CTreeList::~CTreeList()
{
}

TCHAR* CTreeList::GetThemeName()
{
	return _TEXT("treeview");
}

LRESULT CTreeList::WindProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CTreeList::Proxy_HeaderWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CTreeList* pTreeList = (CTreeList*)GetProp(hWnd, (LPCTSTR)gAtomTreeListProperty);
	ATLASSERT(pTreeList != NULL);
	return pTreeList->HeaderWndProc(hWnd, uMsg, wParam, lParam);
	return CallWindowProc(pTreeList->m_OldHeaderWndProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CTreeList::Proxy_TooltipWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CTreeList* pTreeList = (CTreeList*)GetProp(hWnd, (LPCTSTR)gAtomTreeListProperty);
	ATLASSERT(pTreeList != NULL);

	return CallWindowProc(pTreeList->m_OldTooltipWndProc, hWnd, uMsg, wParam, lParam);
}



BOOL CTreeList::Create(HWND hWndParent, HINSTANCE hInst, HMENU hMenu, DWORD dwStyle, LPRECT rc)
{
	this->m_hWnd = CreateWindowEx(0,
		(LPCTSTR)gAtomTreeListAtom,
		_TEXT(""),
		dwStyle|WS_CLIPCHILDREN,
		rc->left,
		rc->top,
		rc->right-rc->left,
		rc->bottom-rc->top,
		hWndParent,
		hMenu,
		hInst,
		(void*)this);
	///
	SetProp(this->m_hWnd, (LPCTSTR)gAtomTreeListProperty, (HANDLE)this);

	return TRUE;
}

BOOL CTreeList::Init()
{
	CRect rc;
	INITCOMMONCONTROLSEX picce;

	GetWindowRect(m_hWnd, (RECT*)&rc);
	HINSTANCE hInst = __THISMODULE__;

	if (this->m_ThemeData != NULL && gpfnOpenThemeData)
	{
		m_ThemeData = (*gpfnOpenThemeData)(m_hWnd, GetThemeName());
	}
	m_hWndHeaderLeft = CreateWindowEx(0,
		_TEXT("SysHeader32"),
		gpszTargetName,
		WS_CHILD | WS_CLIPSIBLINGS | HDS_FULLDRAG | HDS_BUTTONS,
		0, 0, 0, 0,
		m_hWnd,
		(HMENU)IDC_HEADERCTRL_LEFT,
		hInst,
		NULL);
	SetProp(m_hWndHeaderLeft, (LPCTSTR)gAtomTreeListProperty, (HANDLE)this);
	m_OldHeaderWndProc = (WNDPROC)SetWindowLong(m_hWndHeaderLeft, GWL_WNDPROC, (LONG)Proxy_HeaderWndProc);
	
	m_hWndHeaderRight = CreateWindowEx(0,
		_TEXT("SysHeader32"),
		gpszTargetName,
		WS_CHILD | WS_CLIPSIBLINGS | HDS_FULLDRAG | HDS_BUTTONS,
		0, 0, 0, 0,
		m_hWnd,
		(HMENU)IDC_HEADERCTRL_RIGHT,
		hInst,
		NULL);
	SetProp(m_hWndHeaderRight, (LPCTSTR)gAtomTreeListProperty, (HANDLE)this);
	SetWindowLong(m_hWndHeaderRight, GWL_WNDPROC, (LONG)Proxy_HeaderWndProc);

	m_hWndScrollBarRightRight = CreateWindowEx(0,
		_TEXT("ScrollBar"),
		gpszTargetName,
		WS_CHILD | WS_CLIPSIBLINGS | SBS_BOTTOMALIGN | SBS_VERT,
		0, 0, 0, 0,
		m_hWnd,
		(HMENU)IDC_SCROLLBAR_RIGHT_RIGHT,
		hInst,
		NULL);

	m_hWndScrollBarLeftHorz = CreateWindowEx(0,
		_TEXT("ScrollBar"),
		gpszTargetName,
		WS_CHILD | WS_CLIPSIBLINGS | SBS_BOTTOMALIGN | SBS_HORZ,
		0, 0, 0, 0,
		m_hWnd,
		(HMENU)IDC_SCROLLBAR_LEFT_HORZ,
		hInst,
		NULL);

	m_hWndScrollBarRightBottom = CreateWindowEx(0,
		_TEXT("ScrollBar"),
		gpszTargetName,
		WS_CHILD | WS_CLIPSIBLINGS | SBS_BOTTOMALIGN | SBS_HORZ,
		0, 0, 0, 0,
		m_hWnd,
		(HMENU)IDC_SCROLLBAR_RIGHT_BOTTOM,
		hInst,
		NULL);

	this->m_hWndStatic = CreateWindowEx(0,
		_TEXT("STATIC"),
		gpszTargetName,
		WS_CHILD | WS_CLIPSIBLINGS,
		0, 0, 0, 0,
		m_hWnd,
		(HMENU)IDC_STATIC_IN_LISTCTRL,
		hInst,
		NULL);

	picce.dwSize = sizeof(picce);
	picce.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&picce);

	m_hWndTooltipOfTreeList = CreateTooltip(hInst, m_hWnd);
	SetProp(m_hWndTooltipOfTreeList, (LPCTSTR)gAtomTreeListProperty, (HANDLE)this);
	m_OldTooltipWndProc = (WNDPROC)SetWindowLong(m_hWndTooltipOfTreeList, GWL_WNDPROC, (LONG)Proxy_TooltipWndProc);

	m_hWndTooltipOfLeftHeader = CreateTooltip(hInst, m_hWndHeaderLeft);
	m_hWndTooltipOfRightHeader = CreateTooltip(hInst, m_hWndHeaderRight);

	SetWindowFont(m_hWnd, GetStockObject(DEFAULT_GUI_FONT));

	m_hSplitterCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_SPLITTER));
		//HWND hHeaderCtrl = ListCtrl_GetHeaderCtrl(m_hWnd);
	
	return	TRUE;
}

HWND CTreeList::CreateTooltip(HINSTANCE hInst, HWND hWndParent)
{
	HWND hWnd = CreateWindowEx(0,
		_TEXT("Tooltips_class32"),
		NULL,
		WS_POPUP|TTS_NOPREFIX|TTS_ALWAYSTIP|TTS_NOANIMATE|TTS_NOFADE,
		0, 0, 0, 0,
		hWndParent,
		0,
		hInst,
		NULL);
	ATLASSERT(hWnd != NULL);

	TTTOOLINFO ToolInfo = { sizeof(ToolInfo) };
	ToolInfo.uFlags = TTF_TRANSPARENT;
	ToolInfo.hwnd = hWndParent;
	GetClientRect(hWndParent, &ToolInfo.rect);
	SendMessage(hWnd, TTM_ADDTOOL, 0, (LPARAM)&ToolInfo);

	return hWnd;
}

// 处理Windows消息
LRESULT CTreeList::HanleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		return OnKeydown(wParam, lParam);
		break;
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		if (hWnd == ghWndTreeListView
			&& ((HWND)wParam) != ghWndTreeListView)
		{
			InvalidateRect(ghWndTreeListView, NULL, FALSE);
		}
	}
	//return	CallWindowProc(gpfnOldListCtrlWndProc, hWnd, msg, wParam, lParam);
	return LRESULT();
}

//处理键盘被按下
LRESULT CTreeList::OnKeydown(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();

}

// 处理ListView消息
LRESULT CTreeList::HanleLVMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case LVM_GETITEMCOUNT:
		return this->GetItemCount();
	case LVM_DELETEITEM:
		break;
	case LVM_DELETEALLITEMS:
		break;
	case LVM_GETNEXTITEM:
		break;
	case LVM_GETITEMRECT:
		break;
	case LVM_HITTEST:
		break;
	case LVM_ENSUREVISIBLE:
		break;
	case LVM_REDRAWITEMS:
		break;
	case LVM_DELETECOLUMN:
		break;
	case LVM_GETCOLUMNWIDTH:
		break;
	case LVM_GETHEADER:
		break;
	case LVM_SETITEMSTATE:
		break;
	case LVM_SORTITEMS:
		break;
	case LVM_GETSUBITEMRECT:
		break;
	case LVM_SETCOLUMNORDERARRAY:
		break;
	case LVM_GETCOLUMNORDERARRAY:
		break;

	case LVM_GETITEM:
		break;
	case LVM_SETITEM:
		break;
	case LVM_INSERTITEM:
	{
		CTreeListData* pData = new CTreeListData();
		
	}
		break;
	case LVM_FINDITEM:
		break;
	case LVM_GETCOLUMN:
		break;
	case LVM_SETCOLUMN:
		break;
	case LVM_INSERTCOLUMN:
		break;
	case LVM_GETITEMTEXT:
		break;
	case LVM_SETITEMTEXT:
		break;
	case LVM_ISITEMVISIBLE:
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return LRESULT();
}

LRESULT CALLBACK CTreeList::ListCtrlWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SETCURSOR:
	{
		if (hWnd == ghWndHandlesListCtrl)
		{
			SetCursor(ghArrowCursor);
			return 1;
		}
		if (gnLock)
		{
			SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
		}
		else
		{
			SetCursor(ghArrowCursor);
		}
	}
	break;
	case WM_ERASEBKGND:
	{

		RECT Rect = { 0 }, rcItem = { 0 };
		RECT rc = { 0 };
		int nItemCount = ListView_GetItemCount(hWnd);
		if (!nItemCount)
			break;
		
		HDC hDC = (HDC)wParam;
		GetClientRect(hWnd, &Rect);
		ListView_GetItemRect(hWnd, nItemCount - 1, &rcItem, 0);

		rc.left = rcItem.right;
		rc = Rect;
		FillRect(hDC, &rc, (HBRUSH)GetSysColorBrush(COLOR_WINDOW));
		rc.right = Rect.right;
		rc.top = rcItem.bottom;
		rc.bottom = Rect.bottom;
		FillRect(hDC, &rc, (HBRUSH)GetSysColorBrush(COLOR_WINDOW));
		int  nTopIndex = ListView_GetTopIndex(hWnd);

		ListView_GetItemRect(hWnd, nTopIndex, &rcItem, 0);
		rc.right = Rect.right;
		rc.left = 0;
		rc.top = 0;
		rc.bottom = rcItem.top;
		FillRect(hDC, &rc, (HBRUSH)GetSysColorBrush(COLOR_WINDOW));
	}
		return	TRUE;
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		if (hWnd == ghWndTreeListView && ((HWND)wParam) != ghWndTreeListView)
		{
			RedrawTreeList();
		}
		break;
	case WM_KEYDOWN:
	{

		switch (wParam)
		{
		case VK_RETURN:
		{
			UINT nID = IDC_TREELIST;
			if (IsTreeList(hWnd))
			{

			}
			else if (hWnd == ghWndDllsListCtrl)
				nID = IDC_HANDLES_LISTCTRL;
			else
				nID = IDC_DLLS_LISTCTRL;
			int iItem = ListView_GetNextItemIndex(hWnd, -1, LVNI_SELECTED);
			if (iItem != -1)
			{
				//OnTreeListDblclks(nID);
			}
		}
		return 0;
		case VK_HOME:
		{
			ListView_SetItemState(hWnd, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			ListView_EnsureVisible(hWnd, 0, FALSE);
		}
		return 0;
		case VK_END:
		{
			int nCount = ListView_GetItemCount(hWnd);
			ListView_SetItemState(hWnd, nCount - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			ListView_EnsureVisible(hWnd, nCount - 1, FALSE);
		}
		return	0;
		case VK_TAB:
		{
			HWND hWndList = ghWndTreeListView;
			if (hWnd == ghWndTreeListView)
			{
				hWndList = ghWndHandlesListCtrl;
				if (ShowDllView)
				{
					hWndList = ghWndDllsListCtrl;
				}
			}
			int iItem = ListView_GetNextItemIndex(hWndList, -1, LVNI_SELECTED);
			if (iItem == -1)
				iItem = 0;
			SetFocus(hWndList);
			ListView_SetItemState(hWndList, iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			return	0;
		}
		case 'C':	//CTRL+C
		{
			UINT arrIds[96] = { 0 };
			if (!IsTreeList(hWnd) && GetKeyState(VK_CONTROL))
			{
				DWORD dwColumnCount = GetWindowLong(hWnd, GWL_USERDATA);
				ListView_GetColumnOrderArray(hWnd, dwColumnCount, arrIds);
				if (dwColumnCount >= 8)
				{
					DWORD dwCount = InitMMXBuffer((__m128i*)arrIds, dwColumnCount, GetListCtrlColumnMagic());
					for (;dwCount < dwColumnCount;dwCount++)
					{
						arrIds[dwCount] = dwCount;
					}
				}
				TCHAR szText[8192] = { 0 };
				CHAR* pBuffer = NULL;
				///总字节长度(无所谓宽字节与否)
				UINT nTotalLength = 0;
				///当前已分配内存长度(无所谓宽字节与否)
				UINT nAllocLength = sizeof(szText);
				HGLOBAL hGlobal = NULL;
				int nCurItem = ListView_GetNextItemIndex(hWnd, -1, LVNI_SELECTED);
				if (nCurItem == -1)
					break;
				///第一次分配内存
				if (pBuffer == NULL)
				{
					hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, nAllocLength);
					pBuffer = (CHAR*)GlobalLock(hGlobal);
					*pBuffer = 0;
				}
				CHAR* p = pBuffer;
				TCHAR chr1	= _TEXT('\t');
				TCHAR szEnd[] = _TEXT("\r\n");
				while (nCurItem != -1)
				{
					///复制一行数据到剪贴板中
					for (DWORD dwSubItem = 0; dwSubItem < dwColumnCount;dwSubItem++)
					{
						*szText = 0;
						ListView_GetItemText(hWnd, nCurItem, dwSubItem, szText, 8192);
						int nTextLen = _tcslen(szText);
						///本单元字节长度(无所谓宽字节与否)
						nTextLen = (nTextLen + 1) * sizeof(TCHAR);

						if (nAllocLength < nTotalLength+ nTextLen)
						{
							///需要重新分配内存
							nAllocLength = nTotalLength + (nTextLen < 8192 ? 8192: nTextLen)*sizeof(TCHAR);
							hGlobal = GlobalReAlloc(hGlobal, nAllocLength, GMEM_FIXED);
							pBuffer = (CHAR*)GlobalLock(hGlobal);
							///指针指导原来的偏移处，以确保后续增加的内容是正确位置
							p = pBuffer + nTotalLength;
						}
						nTotalLength += nTextLen;

						///不包括末尾结束符号
						CopyMemory(p, szText, nTextLen-sizeof(TCHAR));
						p += (nTextLen-sizeof(TCHAR));
						///每一个单元后面增加\t支付
						CopyMemory(p, &chr1, sizeof(TCHAR));
						p += sizeof(TCHAR);
						nTotalLength += sizeof(TCHAR);
					}
					///每一行的结尾增加\r\n字符本行结束
					CopyMemory(p, szEnd, sizeof(szEnd));
					p += sizeof(szEnd);
					nTotalLength += sizeof(szEnd);
					//取下一行数据
					nCurItem = ListView_GetNextItemIndex(hWnd, nCurItem, LVNI_SELECTED);
				}

				GlobalUnlock(hGlobal);

				//打开剪贴板，复制到剪贴板中
				SetFocus(hWnd);
				if (hGlobal && OpenClipboard(hWnd))
				{
					EmptyClipboard();
					SetClipboardData(CF_UNICODETEXT, hGlobal);
					CloseClipboard();
				}
			}
			SendMessage(GetParent(hWnd), WM_KEYDOWN, wParam, lParam);
		}
		break;
		default:
			SendMessage(GetParent(hWnd), WM_KEYDOWN, wParam, lParam);
			break;
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		LVHITTESTINFO HittestInfo;
		HittestInfo.pt.x = LOWORD(lParam);
		HittestInfo.pt.y = HIWORD(lParam);
		if (gdwOsType < 1)
		{
			HWND hWndTooltip = ListView_GetToolTips(hWnd);
			if (!IsWindowVisible(hWndTooltip))
			{
				int nHit = ListView_SubItemHitTest(hWnd, &HittestInfo);
				if (nHit >= 0)
				{
					UINT nRow = (UINT)GetProp(hWnd, _TEXT("hitrow"));
					if (nRow != HittestInfo.iSubItem)
					{
						SendMessage(hWndTooltip, TTM_POP, 0, 0);
					}
				}
			}
		}
		else
		{
			if (hWnd == ghWndDllsListCtrl)
			{
				GetCursorPos(&HittestInfo.pt);
				ScreenToClient(hWnd, &HittestInfo.pt);
				int nHit = ListView_SubItemHitTest(hWnd, 
					&HittestInfo);
#ifdef FUCK_HOMEPAGE
				if (nHit >= 0 && gdwIdMapForDllsListColumn[HittestInfo.iSubItem] == IDC_FUCK_HOMEPAGE)
				{
					LVITEM item = { 0 };
					item.iItem = HittestInfo.iItem;
					item.mask = LVIF_PARAM;
					ListView_GetItem(hWnd, &item);
					DLLLISTITEMPARAM* pUserParam = (DLLLISTITEMPARAM*)item.lParam;
					NavigateURL(pUserParam->HomePage);
					SetCursor(LoadCursor(NULL, IDC_HAND));
				}
#endif 
			}
		}
	}
		break;
	case WM_NOTIFY:
	{
		NMHDR* pNMhdr = (NMHDR*)lParam;
		switch (pNMhdr->code)
		{
		case TTN_GETDISPINFO:
		{
			if (gdwOsType >= 1)
				break;
			NMTTDISPINFO* pDispInfo = (NMTTDISPINFO*)lParam;
			LVHITTESTINFO HittestInfo;
			LVCOLUMN column = { 0 };
			TCHAR szText[4096] = { 0 };
			RECT rcText = { 0 };
			HDC hdc = NULL;
			int nTextWidth = 0;

			LONG res = CallWindowProc(OldListCtrlWndProc,
				hWnd, msg, wParam, lParam);
			GetCursorPos(&HittestInfo.pt);
			ScreenToClient(hWnd, &HittestInfo.pt);

			int nHit = ListView_SubItemHitTest(hWnd, &HittestInfo);
			if (nHit < 0
				|| HittestInfo.flags&HTTOPRIGHT
				|| HittestInfo.iSubItem < 0)
				return res;

			ListView_GetItemText(hWnd,
				HittestInfo.iItem,
				HittestInfo.iSubItem,
				szText,
				sizeof(szText));

			if (*szText)
			{
				pDispInfo->lpszText = NULL;
				pDispInfo->szText[0] = 0;
				return	res;
			}
			ListView_GetColumn(hWnd,
				HittestInfo.iItem,
				HittestInfo.iSubItem,
				&column);

			hdc = GetDC(hWnd);
			SelectObject(hdc, ghDefaultFont);
			DrawText(hdc, szText, -1, &rcText, DT_NOPREFIX | DT_CALCRECT | DT_CENTER);
			ReleaseDC(hWnd, hdc);

			nTextWidth = rcText.right - rcText.left + 6;
			if (nTextWidth <= 0)
			{
				pDispInfo->lpszText = NULL;
				pDispInfo->szText[0] = 0;
				return	res;
			}
			pDispInfo->lpszText = szText;
			SetProp(hWnd, _TEXT("hitrow"), (HANDLE)HittestInfo.iSubItem);
			return	res;
		}
		break;
		case TTN_SHOW:
		{
			int offset = -1;
			LVHITTESTINFO HittestInfo;
			RECT rcItem = { 0 };
			GetCursorPos(&HittestInfo.pt);
			ScreenToClient(hWnd, &HittestInfo.pt);
			int nHit = ListView_SubItemHitTest(hWnd, &HittestInfo);
			if (nHit != -1
				&& HittestInfo.iSubItem != -1)
			{
				GetListItemRect(hWnd, HittestInfo.iItem, HittestInfo.iSubItem, &rcItem);
				HittestInfo.pt.x = rcItem.left;
				HittestInfo.pt.y = rcItem.top;
				ClientToScreen(hWnd, &HittestInfo.pt);
				HWND hWndTooltip = ListView_GetToolTips(hWnd);
				SetWindowPos(hWndTooltip, HWND_DESKTOP, 
					HittestInfo.pt.x - 1, HittestInfo.pt.y - 1, 0, 0,
					SWP_NOACTIVATE | SWP_NOSIZE);
				return 1;
			}
			HWND hWndTooltip = ListView_GetToolTips(hWnd);
			SetWindowPos(hWndTooltip, HWND_DESKTOP, 
				HittestInfo.pt.x - 1, HittestInfo.pt.y - 1, 0,0,
				SWP_NOACTIVATE | SWP_NOSIZE);
		}

			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
	return CallWindowProc(OldListCtrlWndProc, hWnd, msg, wParam, lParam);
}

bool CTreeList::SetSortableColumn(int col, BOOL bAscend) 
{

	return false;
}

BOOL InitTreeList(HWND hTreeList, HFONT hFont)
{
	HWND hWndHeader; // eax
	RECT Rect; // [esp+Ch] [ebp-14h]

	//hWndHeader = (HWND)SendMessage(hTreeList, LVM_GETHEADER, 0, 0);
	hWndHeader = ListView_GetHeader(hTreeList);
	SetWindowFont(hWndHeader, hFont);
	SetWindowFont(hTreeList, hFont);
	GetClientRect(ghWndNewOwner, &Rect);
	HandleSize(
		ghWndNewOwner,
		0,
		0,
		MAKELPARAM(Rect.right - Rect.left, Rect.bottom - Rect.top));
	return InvalidateRgn(hTreeList, NULL, TRUE);
}


///创建TreeList窗口
HWND CreateTreeListView(HWND hWndParent, HINSTANCE hInst, HMENU hMenu, DWORD dwStyle, LPRECT rc)
{
	CTreeList* pTreeList = new CTreeList();
	pTreeList->Create(hWndParent, hInst, hMenu, dwStyle, rc);
	pTreeList->Init();
	return pTreeList->GetHwnd();
}

void InitSortFlagsImageList(HWND hWndList)
{
	HIMAGELIST hImg = InitSortableImageList();
	ListView_SetImageList(hWndList,	LVSIL_SMALL	, hImg);

}
// HWND CreateListCtrl(HWND hWnd, DWORD dwStyle, int *nWidths, int *nIDs, LONG dwColumnCount, HMENU uCtrlID);
HWND CreateListCtrl(HWND hWnd, DWORD dwStyle, short *nWidths, int *nIDs, LONG dwColumnCount, HMENU uCtrlID)
{
	HWND hListCtrl; // eax
	HWND hWndHeader; // eax
	HWND hToolTips; // eax
	RECT rcWnd; // [esp+Ch] [ebp-14h]

	//_mm_storeu_si128(&rcWnd, 0i64);
	InitCommonControls();
	GetClientRect(hWnd, &rcWnd);
	hListCtrl = CreateWindowEx(
		0,
		_TEXT("SysListView32"),
		gpszTargetName,
		dwStyle,
		rcWnd.left,
		rcWnd.top,
		rcWnd.right,
		rcWnd.bottom,
		hWnd,
		uCtrlID,
		ghInstance,
		NULL);
	if (!hListCtrl)
		return NULL;
	if (!InitListCtrl(hListCtrl, nIDs, nWidths, dwColumnCount))
	{
		DestroyWindow(hListCtrl);
		return NULL;
	}

	SetClassLong(hListCtrl, GCL_HCURSOR, NULL);
	OldListCtrlWndProc = (WNDPROC)SetWindowLong(hListCtrl, GWL_WNDPROC, (LONG)CTreeList::ListCtrlWndProc);
	
	DWORD dwExStyle=LVS_EX_SINGLEROW |LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT;
	
	SendMessage(hListCtrl, LVM_SETEXTENDEDLISTVIEWSTYLE, dwExStyle, dwExStyle);
//	hWndHeader =(HWND) SendMessage(hListCtrl, LVM_GETHEADER, 0, 0);
	hWndHeader = ListView_GetHeader(hListCtrl);
	SetWindowFont(hWndHeader, ghDefaultFont);
	SetWindowFont(hListCtrl, ghDefaultFont);


	hToolTips = (HWND)SendMessage(hListCtrl, LVM_GETTOOLTIPS, 0, 0);
	InitToolTips(hToolTips);
	InitSortFlagsImageList(hListCtrl);
	dwStyle = GetStyle(hListCtrl);	// GetWindowLong(hListCtrl, GWL_STYLE);
	AddStyle(hListCtrl, LVS_SHAREIMAGELISTS);

	///SetWindowLong(hListCtrl, GWL_STYLE, dwStyle | LVS_SHAREIMAGELISTS);
	return hListCtrl;
}


LRESULT CALLBACK HookListCtrlWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(OldListCtrlWndProc, hWnd, msg, wParam, lParam);

}

BOOL  RedrawHeader(HWND hWnd, int iItem, int mode)
{
	UINT fmt; // ecx
	HDITEM item; // [esp+Ch] [ebp-2Ch]

	if (mode)
	{
		if (gbTreeListSortable)
		{
			item.mask = HDI_FORMAT;
			Header_GetItem(hWnd, iItem, &item);
			fmt = HDF_SORTDOWN;
			if (mode >= 0)
				fmt = HDF_SORTUP;
			item.fmt = item.fmt & -(HDF_BITMAP | HDF_SORTUP | HDF_SORTDOWN | HDF_RIGHT | HDF_LEFT) | fmt;
		}
		else
		{
			item.mask = HDI_IMAGE | HDI_FORMAT;                         // HDI_IMAGE|HDI_FORMAT
			Header_GetItem(hWnd, iItem, &item);
			// 
			item.fmt |= HDF_BITMAP_ON_RIGHT | HDF_IMAGE | HDF_LEFT;
			item.iImage = mode < 0?0:mode;
		}
	}
	else
	{
		item.mask = HDI_FORMAT;
		Header_GetItem(hWnd, iItem, &item);
		if (gbTreeListSortable)
			item.fmt &= -(HDF_BITMAP | HDF_SORTUP | HDF_SORTDOWN | HDF_RIGHT | HDF_LEFT);
		else
			item.fmt &= -HDF_BITMAP_ON_RIGHT | HDF_IMAGE | HDF_RIGHT | HDF_LEFT;
	}
	Header_SetItem(hWnd, iItem, &item);
	return UpdateWindow(hWnd);
}

bool  InitTreeListSortableHeader(HWND hWnd, int item1, int item2, bool Ascending)
{
	HWND hWndHeader; // ebx
	HDITEMW item; // [esp+4h] [ebp-2Ch]

	CTreeList *TreeList = GetTreeListCtrl(hWnd);
	if (TreeList)
		return TreeList->SetSortableColumn(item2, Ascending != false);
	hWndHeader = ListView_GetHeader(hWnd);
	item.mask = HDI_FORMAT;
	Header_GetItem(hWndHeader, item1, &item);

	if (gbTreeListSortable)
		// 
		item.fmt &= -(HDF_BITMAP | HDF_SORTUP | HDF_SORTDOWN | HDF_RIGHT | HDF_LEFT);
	else
		item.fmt &= -(HDF_BITMAP_ON_RIGHT | HDF_IMAGE | HDF_RIGHT | HDF_LEFT);
	Header_SetItem(hWndHeader, item1, &item);
	UpdateWindow(hWndHeader);
	BOOL  ret= RedrawHeader(hWndHeader, item2, 2 * (Ascending != false) - 1);
	return ret == TRUE;
}

bool __cdecl IsColumnSortable(signed int uID)
{
	bool v1; // zf

	//if (uID > IDS_PATH2)
	//{
	//	if (uID <= IDS_AUTOSTARTLOCATION)
	//	{
	//		if (uID < IDS_PACKAGENAME)
	//		{
	//			switch (uID)
	//			{
	//			case IDS_DESKTOPINTEGRITYLEVEL:
	//			case IDS_COMMENT:
	//			case IDS_VERIFIEDSIGNER:
	//			case IDS_CPUHISTORY:
	//			case IDS_PRIVATEBYTESHISTORY:
	//			case IDS_WINDOWSTATUS:
	//			case IDS_DEP:
	//			case IDS_DECODEDACCESS:
	//			case IDS_IOHISTORY:
	//			case IDS_INTEGRITY:
	//			case IDS_VIRTUALIZED:
	//			case IDS_ASLR:
	//			case IDS_IOPRIORITY:
	//				return 0;
	//			default:
	//				return 1;
	//			}
	//			return 1;
	//		}
	//		return 0;
	//	}
	//	if (uID > IDS_UIACCESS)
	//	{
	//		v1 = uID == IDD_SELECT_COLUMN_PAGE_PROCESS_IMAGE_CONTROL_FLOW_GUARD;
	//	}
	//	else
	//	{
	//		if (uID >= IDS_PROTECTION || uID == IDS_DPIAWARENESS)
	//			return 0;
	//		v1 = uID == IDS_VIRUSTOTAL;
	//	}
	//}
	//else
	//{
	//	if (uID == IDS_PATH2)
	//		return 0;
	//	if (uID <= IDS_WINDOWTITLE)
	//	{
	//		if (uID != IDS_WINDOWTITLE)
	//		{
	//			switch (uID)
	//			{
	//			case IDS_PROCESS:
	//			case IDS_USERNAME:
	//			case IDS_TYPE:
	//			case IDS_NAME:
	//			case IDS_VERSION:
	//			case IDS_NAME2:
	//			case IDS_TIME:
	//				return 0;
	//			default:
	//				return 1;
	//			}
	//			return 1;
	//		}
	//		return 0;
	//	}
	//	if (uID > IDS_COMPANYNAME)
	//	{
	//		v1 = uID == IDS_PATH1;
	//	}
	//	else
	//	{
	//		if (uID >= IDS_COMMANDLINE || uID == IDS_DESCRIPTION1)
	//			return 0;
	//		v1 = uID == IDS_DESCRIPTION2;
	//	}
	//}
	//if (!v1)
	//	return 1;
	return 0;
}

 HIMAGELIST InitSortableImageList()
{
	HIMAGELIST hImg; // ST20_4
	HICON hDesendIcon; // edi
	HICON hAscendIcon; // ebx

	hImg = ImageList_Create(16, 16, 1, 256, 256);
	hDesendIcon = LoadIcon(ghInstance, (MAKEINTRESOURCE(IDR_ICON_TRIANGLE)));
	hAscendIcon = LoadIcon(ghInstance, MAKEINTRESOURCE(IDR_ICON_TRIANGLE_1));
	ImageList_ReplaceIcon(hImg, -1, hDesendIcon);
	ImageList_ReplaceIcon(hImg, -1, hAscendIcon);
	return hImg;
}
 
 __m128i *GetListCtrlColumnMagic()
 {
	 static bool bInited = false;
	 static __m128i xmmword_4CEA50;
	 if (!bInited)
	 {
		 xmmword_4CEA50.m128i_u32[0] = 3;
		 xmmword_4CEA50.m128i_u32[1] = 2;
		 xmmword_4CEA50.m128i_u32[2] = 1;
		 xmmword_4CEA50.m128i_u32[3] = 0;
		 bInited = true;
	 }
	 return &xmmword_4CEA50;
 }
 extern int InitMMXBuffer(__m128i* pFixedBuffer,int nCount, const __m128i* Value);

int __stdcall InitListCtrl(HWND hWnd, int *uIDs, short *nWidths, LONG dwCount)
{
	int item; // esi
	bool bSortable; // al
	LVCOLUMNW lvcolumn = { 0 }; 
	__m128i arrIds[24] = { 0 };
	TCHAR szName[64] = { 0 }; 



	while (ListView_DeleteColumn(hWnd, 0));

	item = 0;
	// LVCF_FMT|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM|LVCF_IMAGE
	lvcolumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_IMAGE;
	lvcolumn.pszText = szName;
	if (dwCount)
	{
		while (item< dwCount)
		{
			lvcolumn.fmt = LVCFMT_LEFT;
			lvcolumn.iSubItem = item;
			lvcolumn.cx = abs(nWidths[item]);
			if (GetParent(hWnd) != ghWndNewOwner)
				break;
			if (nWidths[ item]< 0)
				lvcolumn.fmt = LVCFMT_RIGHT;

			bSortable = IsColumnSortable(uIDs[ item]);
			if (bSortable)
				lvcolumn.fmt = LVCFMT_RIGHT;
			LoadString(ghInstance, uIDs[item], szName, 64);
			if (ListView_InsertColumn(hWnd, item, &lvcolumn) == -1)
				return FALSE;
			item++;
		}
	}
	if (GetParent(hWnd) == ghWndNewOwner 
		&& IsColumnSortable(uIDs[0]))
	{
		lvcolumn.mask = LVIF_TEXT;
		SendMessage(hWnd, LVM_GETCOLUMN, 0, (LPARAM)&lvcolumn);
		lvcolumn.fmt |= LVCFMT_RIGHT;
		lvcolumn.mask = LVIF_TEXT;
		SendMessage(hWnd, LVM_SETCOLUMN, 0, (LPARAM)&lvcolumn);
	}
	// 用户自定义数据记录的当前列总数
	SetWindowLong(hWnd, GWL_USERDATA, dwCount);
	// 对一个128位双精度浮点数对数组进行操作

	////计算数组实际数据
	InitMMXBuffer(arrIds, sizeof(arrIds)/4, GetListCtrlColumnMagic());
	

	///Pointer to an array that specifies the order in which columns 
	///should be displayed, from left to right.For example, 
	///if the contents of the array are{ 2,0,1 }, the control displays 
	///column 2, column 0, and column 1 in that order.
	SendMessage(hWnd, LVM_SETCOLUMNORDERARRAY, dwCount, (LPARAM)arrIds);
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

void GetListColor(HWND hListCtrl,void* pData, BOOL bFocused, COLORREF* clrText, COLORREF* clrBack)
{

}
BOOL __stdcall DrawItemText(HDC hDC, LPCTSTR lpszText, const RECT *lprc, int fRightAlign);

bool ListCtrlDrawItem(LPDRAWITEMSTRUCT lpdis)
{
	__m128i arrIds[24] = { 0 };
	__m128i arrIDs[24] = { 0 };
	HWND hWndItem = lpdis->hwndItem;
	int nCount = GetWindowLong(hWndItem, GWL_USERDATA);
	int itemID = lpdis->itemID;


	LVITEM item = { 0 };
	item.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE | LVIF_STATE; 0xE;
	item.iItem = itemID;
	item.iSubItem = 0;
	ListView_GetItem(hWndItem, &item);


	BOOL bFocused = lpdis->itemState & LVIS_FOCUSED;
	COLORREF clrText, clrBack;
	GetListColor(hWndItem, (void*)lpdis->itemData, bFocused, &clrText, &clrBack);
	
	SetTextColor(lpdis->hDC, clrText);
	SetBkMode(lpdis->hDC, OPAQUE);
	SetBkColor(lpdis->hDC, clrBack);

	if (IsTreeList(hWndItem))
	{

		return	true;
	}

	if (!ListView_GetColumnOrderArray(hWndItem, nCount, arrIds))
	{
		int idx = InitMMXBuffer(arrIds, COUNTALIGN(nCount), GetListCtrlColumnMagic());
		
		for (;idx < nCount;idx++)
		{
			arrIds[0].m128i_i32[idx] = idx;
		}
	}

	int index = 0;
	int width = 0;
	RECT rcItem;
	TCHAR szText[8192] = { 0 };
	LVCOLUMN column = { 0 };
	if (nCount > 0)
	{
		do
		{
			column.mask = LVIF_TEXT | LVIF_IMAGE;
			ListView_GetColumn(hWndItem, arrIds[0].m128i_i32[index], &column);
			int left = lpdis->rcItem.left;
			int cx = column.cx;
			rcItem.left = left + width;
			rcItem.top = lpdis->rcItem.top;
			rcItem.bottom = lpdis->rcItem.bottom;
			rcItem.right = left + width + column.cx;
			if (hWndItem == ghWndDllsListCtrl)
			{
				int nColumnCount = GetWindowLong(hWndItem, GWL_USERDATA);
				if (!ListView_GetColumnOrderArray(hWndItem, nColumnCount, arrIDs))
				{
					int index = InitMMXBuffer(&arrIDs[1],COUNTALIGN(nColumnCount), GetListCtrlColumnMagic());

					for (;index < nColumnCount;index++)
					{
						arrIDs[0].m128i_i32[index] = index;
					}
				}
				//if (gpDllColumnMap[*(arrIDs + index)] == IDC_DLLCOLUMN_CHECKBOX_VIRUSTOTAL)
				//{
				//	void* pUserData = (void*)lpdis->itemData;
				//}
				//else
				{
					SetTextColor(lpdis->hDC, clrText);
					SelectObject(lpdis->hDC, ghDefaultFont);
				}
			}
			
			ListView_GetItemText(hWndItem, lpdis->itemID, ((int*)arrIds)[index], szText, sizeof(szText)-1);
			DrawItemText(lpdis->hDC, szText, &rcItem, column.fmt | LVCFMT_RIGHT);
			index++;
			width += column.cx;
		} while (index < nCount);
	}

	if (lpdis->itemState& LVIS_FOCUSED)
	{
		SetTextColor(lpdis->hDC, GetSysColor(COLOR_WINDOWTEXT));
		SetBkColor(lpdis->hDC, GetSysColor(COLOR_WINDOW));
	}
	if (IsTreeList(hWndItem))
	{
		ListView_GetItemRect(hWndItem, 0, &rcItem,0);
		if (rcItem.top != gdwTopLastTreeList)
		{
			gdwTopLastTreeList = rcItem.top;
			InvalidateRect(ghWndTreeListView, NULL, FALSE);
		}
	}
	return	true;
}

BOOL __stdcall DrawItemText(HDC hDC, LPCTSTR lpszText, const RECT *lprc, int fRightAlign)
{
	signed int delta; 
	int left; 
	TCHAR szText[16384] = { 0 }; 

	if (lstrlen(lpszText) > 8191)
	{
		///只保留前8K字节内容
		_tcsncpy_s(szText, 8192, lpszText, -1);
	}
	_tcscpy_s(szText, 8192, lpszText);
	delta = 2;
	if (fRightAlign)
		delta = 8;
	///sub_444D00(hDC, szText, 8192, lprc->right - delta - lprc->left);
	if (fRightAlign)
	{
		SetTextAlign(hDC, TA_RIGHT);
		left = lprc->right - 10;
	}
	else
	{
		SetTextAlign(hDC, 0);
		left = lprc->left + 2;
	}
	return ExtTextOut(hDC, left, lprc->top + 1, ETO_CLIPPED | ETO_OPAQUE, lprc, szText, lstrlen(szText), 0);
}

BOOL ListCtrlMeasureItem(HWND hWnd, UINT msg, WPARAM wParam, LPMEASUREITEMSTRUCT lpmis)
{
	HDC hdc; // ebx
	BOOL result; // eax
	int height; // edi
	TEXTMETRICW tm; // [esp+8h] [ebp-40h]

	if (lpmis->CtlType == IDC_HANDLES_LISTCTRL || lpmis->CtlID == IDC_TREELIST)
	{
		hdc = GetDC(ghWndTreeListView);
		SelectObject(hdc, (HGDIOBJ)ghDefaultFont);
		result = GetTextMetricsW(hdc, &tm);
		if (!result)
			return result;
		if (lpmis->CtlID == IDC_TREELIST)
		{
			height = tm.tmHeight + 2;
			if (height <= GetSystemMetrics(SM_CYSMICON))
				height = GetSystemMetrics(SM_CYSMICON);
			lpmis->itemHeight = height;
		}
		else
		{
			lpmis->itemHeight = tm.tmHeight + 2;
		}
		ReleaseDC(ghWndTreeListView, hdc);
	}
	else if (lpmis->CtlType == IDOK)
	{
		lpmis->itemWidth = 16;
		lpmis->itemHeight = 16;
	}
	return	TRUE;
}

////
int CALLBACK SortInFindDialogCallback(LPARAM, LPARAM, LPARAM)
{
	return	0;
}

////处理WM_NOTIFY消息
BOOL ListCtrlNotifyInFindDlg(NMLISTVIEW* pNMListView)
{
	switch (pNMListView->hdr.code)
	{
	case LVN_COLUMNCLICK:
	{
		bool ascending = gbFINDDLGLISTAscending;
		if (pNMListView->iSubItem == gnItemSortedInFindDialog)
		{
			gbFINDDLGLISTAscending = !gbFINDDLGLISTAscending;
		}
		InitTreeListSortableHeader(pNMListView->hdr.hwndFrom, gnItemSortedInFindDialog, pNMListView->iSubItem, ascending);
		gnItemSortedInFindDialog = pNMListView->iSubItem;
		ListView_SortItems(pNMListView->hdr.hwndFrom, gnItemSortedInFindDialog, SortInFindDialogCallback);
		return	FALSE;
	}
		break;
	case LVN_DELETEITEM:
	{
		LVITEM item = { 0 };
		item.mask = LVIF_PARAM;
		item.iItem = pNMListView->iItem;
		ListView_GetItem(pNMListView->hdr.hwndFrom, &item);
		void* pBuffer = (void*)item.lParam;
		ListView_DeleteItem(pNMListView->hdr.hwndFrom, pNMListView->iItem);
		//释放内存
	}
	return	FALSE;
	case LVN_ITEMCHANGED:
	case LVN_INSERTITEM:
	case LVN_ITEMCHANGING:
	{
		int nItem = ListView_GetNextItem(pNMListView->hdr.hwndFrom, -1, LVIS_SELECTED);
		if (gdwSelectedItemInFindDialog == nItem)
			return 0;
		gdwSelectedItemInFindDialog = nItem;
		if (gdwSelectedItemInFindDialog == -1)
			return 0;
	}
		break;
	default:
		break;
	}
	return	TRUE;
}

BOOL SetPropSheetRect(HWND hWnd, int x, int y)
{

	WINDOWPLACEMENT wndpl; // [esp+10h] [ebp-40h]
	RECT Rect;

	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int VirtualScreenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int VirtualScreenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	if (x < 0)
		x = 10;
	if (y < 0)
		y = 10;

	GetClientRect(hWnd, &Rect);
	if (VirtualScreenWidth)
	{
		if (VirtualScreenWidth - Rect.right < y)
			y = VirtualScreenWidth - Rect.right;
		if (VirtualScreenHeight - Rect.bottom < y)
		{
			y = VirtualScreenHeight - Rect.bottom;
		}
	}
	else
	{
		if (ScreenWidth - Rect.right < x)
			x = ScreenWidth - Rect.right;
		if (ScreenHeight - Rect.bottom < y)
		{
			y = ScreenHeight - Rect.bottom;
		}
	}
	wndpl.flags = 1;
	wndpl.rcNormalPosition.right = x + Rect.right;
	wndpl.rcNormalPosition.bottom = y + Rect.bottom;
	wndpl.rcNormalPosition.left = x;
	wndpl.rcNormalPosition.top = y;
	return SetWindowPlacement(hWnd, &wndpl);
}

////CTreeListData
CTreeListData::CTreeListData()
{

}
CTreeListData::~CTreeListData()
{

}

// 初始化滚动条位置
void CTreeList::RestoreScrollbars(int flags)
{
}


// 调整窗口大小尺寸
bool CTreeList::AutoSize(HWND hWndScrollbar, bool bFlags1, bool bFlags)
{
	return false;
}


// 让滚动条翻滚到指定的位置
void CTreeList::Scroll(HWND hWndScrollbar, int delta)
{
}


void CTreeList::SetSplitterCursor(HCURSOR hCursor)
{
	HCURSOR h = m_hSplitterCursor;
	this->m_hSplitterCursor = hCursor;
	if (h != NULL)
		DestroyCursor(h);
}

//#include <prsht.h>
// 自定义的HeaderCtrl消息处理函数
LRESULT CTreeList::HeaderWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hWndTooltip = hWnd == m_hWndHeaderLeft ? m_hWndTooltipOfLeftHeader : m_hWndTooltipOfRightHeader;

	WM_MOUSEFIRST;
	if (uMsg>=WM_MOUSEFIRST && uMsg <= WM_XBUTTONDBLCLK)
	{

	}
	if (uMsg == WM_PAINT)
	{

	}

	if (uMsg == WM_NOTIFY)
	{
		NMTTDISPINFO *pNMDispInfo = (NMTTDISPINFO*)lParam;
		switch (pNMDispInfo->hdr.code)
		{
		case TTN_GETDISPINFO:
		{
			HDITEM item = { 0 };
			item.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
			item.pszText = pNMDispInfo->szText;
			item.cchTextMax = 80;
			int iItem = this->m_nHeaderItem;
			if (iItem == -1)
			{
				pNMDispInfo->lpszText = gpszTargetName;
				break;
			}
			if (hWndTooltip != m_hWndTooltipOfLeftHeader)
				iItem--;
			Header_GetItem(hWnd, iItem, &item);
			

			int cxWidth = Header_GetBitmapMargin(hWnd);
			CRect rcItem;
			Header_GetItemRect(hWnd, iItem, &rcItem);
			if (rcItem.left >= 0)
			{
				CRect rc;
				rc.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
				rc.bottom = GetHeaderHeight();
				HDC hdc = GetDC(m_hWnd);
				HFONT hOldFont = (HFONT)SelectObject(hdc, m_Font);
				UINT uFormat = this->m_HeaderFormat;
				uFormat &= ~((HDF_STRING|HDF_OWNERDRAW)|0x40000);
				uFormat |= HDF_SORTUP;

				DrawText(hdc, pNMDispInfo->lpszText, -1, (RECT*)&rc, uFormat);
				SelectObject(hdc, hOldFont);
				ReleaseDC(m_hWnd, hdc);

				if (rc.Width() <= rcItem.Width())
				{
					pNMDispInfo->lpszText = gpszTargetName;
				}
			}
			else
			{
				pNMDispInfo->lpszText = gpszTargetName;
			}
			break;
		}
		case TTN_SHOW:
		{
			CRect rcItem;
			int nHeaderItem = this->m_nHeaderItem;
			if (hWndTooltip != this->m_hWndTooltipOfLeftHeader)
				--nHeaderItem;
			Header_GetItemRect(hWnd, nHeaderItem, &rcItem);
			MapWindowPoints(hWnd, NULL, (LPPOINT)&rcItem, 2);
			rcItem.top = rcItem.bottom + rcItem.Height();
			rcItem.left = rcItem.Height() + rcItem.left;
			SetWindowPos(hWndTooltip, 0, rcItem.left, rcItem.top, 0, 0, SWP_NOACTIVATE |SWP_NOZORDER|SWP_NOSIZE);

			SetWindowFont(hWndTooltip, m_Font);
		}
		return 1;
		default:
			break;
		}
	}
	return CallWindowProc(m_OldTooltipWndProc, hWnd, uMsg, wParam, lParam);
}


// 返回子窗口HeaderCtrl的高度
int CTreeList::GetHeaderHeight()
{
	TEXTMETRIC tm = { 0 };
	HDC hDC = GetDC(m_hWnd);
	SelectObject(hDC, m_Font);
	GetTextMetrics(hDC, &tm);
	ReleaseDC(m_hWnd, hDC);
	int nHeight = tm.tmHeight;
	if (m_ImageList != NULL)
	{
		int cy = GetSystemMetrics(SM_CYSMICON);
		if (nHeight < cy)
			nHeight = cy;
	}
	return nHeight + m_Height;
}


// 返回垂直滚动条当前的位置
int CTreeList::GetVertScrollPosition()
{
	SCROLLINFO vi = { sizeof(vi) };
	vi.fMask = SIF_POS;
	GetScrollInfo(m_hWndScrollBarRightRight, SB_CTL, &vi);
	return vi.nPos;
}

int CTreeList::GetItemCount()
{
	return	0;
}

// 关闭
void CTreeList::Close()
{
	InvalidateRect(m_hWnd, NULL, FALSE);
	InvalidateRect(this->m_hWndHeaderLeft, NULL, FALSE);
	InvalidateRect(this->m_hWndHeaderRight, NULL, FALSE);

}





// 返回指定行的尺寸
void CTreeList::GetItemRect(int nPos, RECT* lprc)
{
	TEXTMETRIC tm;
	RECT Rect;
	SCROLLINFO ScrollInfo = { sizeof(ScrollInfo) };

	HWND hWndSB = this->m_hWndScrollBarRightRight;
	SB_GetScrollPos(hWndSB, ScrollInfo);

	//ScrollInfo.nTrackPos = 0;
	//ScrollInfo.fMask = SIF_POS;
	//GetScrollInfo(hWndSB, SB_CTL, &ScrollInfo);

	int nHeaderHeight = GetHeadHeight();

	GetClientRect(this->m_hWnd, lprc);
	GetWindowRect(this->m_hWndHeaderLeft, &Rect);
	lprc->top = nHeaderHeight * (nPos - ScrollInfo.nPos) - Rect.top + Rect.bottom;
	lprc->bottom = nHeaderHeight + lprc->top;
}


int CTreeList::GetHeadHeight()
{
	TEXTMETRIC tm;
	HDC hDC = GetDC(this->m_hWnd);
	SelectObject(hDC, this->m_Font);
	GetTextMetrics(hDC, &tm);
	ReleaseDC(this->m_hWnd, hDC);
	int nTextHeight = tm.tmHeight;
	if (this->m_ImageList)
	{
		int nIconHeight = GetSystemMetrics(SM_CYSMICON);
		if (nTextHeight < nIconHeight)
			nTextHeight = nIconHeight;
	}
	return nTextHeight + this->m_Height;
}


LRESULT CTreeList::OnTreeListColumnClick(WPARAM wParam, NMLISTVIEW* pNMListView)
{
	return LRESULT();
}


LRESULT CTreeList::DeleteItem(int nItem, NMLISTVIEW* pNMListView)
{
	return LRESULT();
}

LRESULT CTreeList::OnItemChanged(int nItem)
{
	return LRESULT();
}

LRESULT CTreeList::OnDblClick(int nItem)
{
	return LRESULT();
}

LRESULT CTreeList::OnRClick(int nItem, NMCLICK *pNMClick)
{
	return LRESULT();
}

LRESULT CTreeList::OnTreeListColumnClick(int nItem, NMCLICK *pNMClick)
{
	return LRESULT();
}



// 处理NM_KEYDOWN消息
BOOL CTreeList::OnKeydown(UINT nID, NMLVKEYDOWN* pNMKeydown)
{
	return 0;
}


// 处理LVN_GETDISPINFO消息
BOOL CTreeList::OnGetDispInfo(UINT nID, NMLVDISPINFO* pNMGetDispInfo)
{
	return 0;
}


// 处理TTN_GETDISPINFO消息
BOOL CTreeList::GetTooltipGetDispInfo(UINT nID, NMTTDISPINFO* pNMDispInfo)
{
	return 0;
}


// 处理LVN_ODFINDITEM
BOOL CTreeList::OnFindItem(UINT nItem)
{
	return 0;
}

void GetListItemRect(HWND hWnd, int nItem, int nSubItem, LPRECT lprc)
{
	UINT arrIds[96] = { 0 };
	LONG res;

	res = ListView_GetColumnOrderArray(hWnd, gdwProcessColumnCount, arrIds);
	if (!res)
	{
		DWORD dwColumnCount = gdwProcessColumnCount - gdwProcessColumnCount % 8;
		DWORD dwCount = InitMMXBuffer((__m128i*)arrIds, dwColumnCount, GetListCtrlColumnMagic());
		for (;dwCount < dwColumnCount;dwCount++)
		{
			arrIds[dwCount] = dwCount;
		}
	}

	if (nItem == -2)
	{
		CTreeList* pTreeList = GetTreeListCtrl(hWnd);
		pTreeList->GetItemRect(nSubItem, lprc);
		lprc->bottom -= lprc->top;
		lprc->right -= lprc->left;
	}
	else if (nSubItem)
	{
		if (lprc)
		{
			lprc->top = nSubItem;
			lprc->left = 0;
		}
		else
		{

		}
		//ListView_GetItemRect(hWnd, nItem, nSubItem, lprc);
		lprc->bottom -= lprc->top;
		lprc->right -= lprc->left;
	}
	else
	{

	}
}
