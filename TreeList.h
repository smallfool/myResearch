#pragma once

class CTreeListData;

class CTreeList
{
public:
	CTreeList();
	~CTreeList();
	virtual TCHAR* GetThemeName();
	virtual LRESULT WindProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	HWND GetHwnd() { return m_hWnd; }
	BOOL Create(HWND hWndParent, HINSTANCE hInst, HMENU hMenu, DWORD dwStyle, LPRECT rc);
	BOOL Init();
	HWND CreateTooltip(HINSTANCE hInst,HWND hWndParent);
protected:
	LRESULT OnKeydown(WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK Proxy_HeaderWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proxy_TooltipWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	HANDLE m_ThemeData;
	HWND m_hWnd;
	HWND m_hWndHeaderLeft;
	HWND m_hWndTooltipOfLeftHeader;
	HWND m_hWndHeaderRight;
	HWND m_hWndTooltipOfRightHeader;
	HWND m_hWndScrollBarRightRight;
	HWND m_hWndScrollBarLeftHorz;
	HWND m_hWndScrollBarRightBottom;
	HWND m_hWndStatic;
	HWND m_hWndTooltipOfTreeList;
	HFONT m_Font;
	HCURSOR m_hSplitterCursor;
	HIMAGELIST m_ImageList;

	UINT	m_HeaderFormat;
	int		m_nHeaderItem;


	//	HeaderCtrl除ICON之外的高度
	int		m_Height;	

	WNDPROC m_OldTooltipWndProc;

	WNDPROC m_OldHeaderWndProc;

	HCURSOR m_HandCursor;

	// 处理Windows消息
	LRESULT HanleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HanleLVMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 初始化滚动条位置
	void RestoreScrollbars(int flags);
	// 调整窗口大小尺寸
	bool AutoSize(HWND hWndScrollbar, bool bFlags1, bool bFlags);
	// 让滚动条翻滚到指定的位置
	void Scroll(HWND hWndScrollbar, int delta);
	void SetSplitterCursor(HCURSOR hCursor);

	bool SetSortableColumn(int col, BOOL bAscend = TRUE);
public:
	static LRESULT CALLBACK ListCtrlWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 自定义的HeaderCtrl消息处理函数
	LRESULT HeaderWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 返回子窗口HeaderCtrl的高度
	int GetHeaderHeight();
	// 返回垂直滚动条当前的位置
	int GetVertScrollPosition();
	int GetItemCount();
	// 关闭
	void Close();
	// 返回指定行的尺寸
	void GetItemRect(int iItem, RECT* lprc);
private:
	int GetHeadHeight();
public:
	static LRESULT OnTreeListColumnClick(WPARAM wParam, NMLISTVIEW* pNMListView);
	static LRESULT DeleteItem(int nItem, NMLISTVIEW* pNMListView);
	static LRESULT OnItemChanged(int nItem);
	static LRESULT OnDblClick(int nItem);
	static LRESULT OnRClick(int nItem,NMCLICK *pNMClick);
	static LRESULT OnTreeListColumnClick(int nItem, NMCLICK *pNMClick);
	// 处理NM_KEYDOWN消息
	static BOOL OnKeydown(UINT nID, NMLVKEYDOWN* pNMKeydown);
	// 处理LVN_GETDISPINFO消息
	static BOOL OnGetDispInfo(UINT nID, NMLVDISPINFO* pNMGetDispInfo);
	// 处理TTN_GETDISPINFO消息
	static BOOL GetTooltipGetDispInfo(UINT nID, NMTTDISPINFO* pNMDispInfo);
	// 处理LVN_ODFINDITEM
	static BOOL OnFindItem(UINT nItem);
};

class CTreeListData
{
public:
	CTreeListData();
	virtual ~CTreeListData();
};

////0x5BC
struct TreeListItemData
{
	HWND hWnd;

	ULONG RefCount;
	DWORD Style;
	HANDLE ProcessHandle;

	TCHAR* szImageName;
	
	DWORD  ProcessId;

	DWORD dwPid;

	HANDLE m_ProcessHandle;
	PVOID  m_BaseAddress;

	///0x5B0
	TreeListItemData* Prev;
	///0x5B4
	TreeListItemData* Next;
	
};


typedef TreeListItemData TREELISTITEMPARAM;
typedef TREELISTITEMPARAM *PTREELISTITEMPARAM;

extern PTREELISTITEMPARAM gpTreeListItemParamList;

extern ATOM gAtomTreeListProperty;
extern CTreeList* GetTreeListCtrl(HWND hWnd);
extern HWND CreateListCtrl(HWND hWnd, DWORD dwStyle, short *nWidths, int *nIDs, LONG dwColumnCount, HMENU uCtrlID);
extern void GetListItemRect(HWND hWnd, int nItem, int nSubItem, LPRECT lprc);




inline int ListCtrl_GetSelectedItem(HWND hListCtrl)
{
	return SendMessage(hListCtrl, LVM_GETNEXTITEM, (WPARAM)-1, (LPARAM)LVNI_SELECTED);
}

inline int ListCtrl_GetItemCount(HWND hListCtrl)
{
	return SendMessage(hListCtrl, LVM_GETITEMCOUNT, 0, 0);
}

inline BOOL ListCtrl_GetItem(HWND hListCtrl, int nItem, LVITEM* pItem)
{
	return (BOOL)SendMessage(hListCtrl, LVM_GETITEM, (WPARAM)nItem, (LPARAM)pItem);
}

inline BOOL ListCtrl_InsertIteem(HWND hListCtrl, int nItem, LPCTSTR lpszText)
{
	LVITEM item = { 0 };
	item.mask = LVIF_TEXT;
	item.iItem = nItem;
	item.cchTextMax = _tcslen(lpszText) + 1;
	return (BOOL)SendMessage(hListCtrl, LVM_INSERTITEM, (WPARAM)nItem, (LPARAM)&item);
}

inline HWND ListCtrl_GetHeaderCtrl(HWND hListCtrl)
{
	return (HWND)SendMessage(hListCtrl, LVM_GETHEADER, 0, 0);
}