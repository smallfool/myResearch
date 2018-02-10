#pragma once

class CPage
{
public:
	CPage();
	~CPage();

	HWND	m_hWnd;
	TCHAR	m_TemplateName[256];
	UINT	m_TabId;
	WNDPROC m_PageWndProc;
};

class CPropSheet
{
public:
	enum { MAX_PAGECOUNT  = 32,};
	CPropSheet();
	~CPropSheet();

public:
	ULONG m_TabIndex;
	TreeListItemData* m_TreeListItemData;
	CPage m_Page[MAX_PAGECOUNT];
};

