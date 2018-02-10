#include "stdafx.h"
#include "..\include\MainWnd.h"
#include "../include/Common.h"
#include "../ProcExplorer/ProcExplorer.h"


CMainWnd::CMainWnd()
{
	this->m_hWnd = NULL;
}


CMainWnd::~CMainWnd()
{
}


// ////����������,��������Ϣѭ��
BOOL CMainWnd::Create(HINSTANCE hInstance, int nShowCmd)
{
	HANDLE hProcessExplorerElevatingEvent; // eax
	int cyy; 
	int cxx;
	HWND hWndMain; 
	UINT bMinimized; 
	int nWidth;
	int nHeight;
	LONG left; 
	LONG right;
	LONG top;
	int nShowCommand; 
	WNDCLASSEX wc = { sizeof(wc) };

	LoadString(hInstance, IDS_PROCESSEXPLORER_CLASSNAME, PROCESSEXPLORER_CLASSNAME, 16);
	LoadString(hInstance, IDS_PROCESSEXPLORER_WINDOWNAME, PROCESSEXPLORER_WINDOWNAME, 40);

	ghInstance = hInstance;
	InitDrawEngine();
	if (gbAllOneInstance)
	{
		///!!!!!
		///�ò���δ֪����Ҫ����OSȥ��ѯ��Ŀǰ��û��ʵ��
		///
		///IDA��ʾ��EVENT_TRACE_MODE_RESERVED
		///EVENT_TRACE_FLAG_ALPC
		/// Ӧ����EVENT_TRACE_PROPERTIES�ṹ�й�ϵ
		hProcessExplorerElevatingEvent = OpenEvent(SYNCHRONIZE, 0, L"ProcessExplorerElevating");
		if (hProcessExplorerElevatingEvent)
		{
			CloseHandle(hProcessExplorerElevatingEvent);
		}
		else
		{
			HWND hMainWnd = FindWindow(PROCESSEXPLORER_CLASSNAME, 0);
			if (hMainWnd)
			{
				PostMessage(hMainWnd, PMT_MSG_7F2, 0, 0);
				return FALSE;
			}
		}
	}
	//if (!sub_487A70())
	//	return 0;
	////ע��������
	ghArrowCursor = LoadCursor(NULL, IDC_WAIT);
	wc.lpfnWndProc = CMainWnd::MainWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_PROCESSEXPLORER));
	wc.lpszMenuName = MAKEINTRESOURCE(IDC_PROCEXPLORER);////_TEXT("PROCEXPLORER");
	wc.lpszClassName = PROCESSEXPLORER_CLASSNAME;
	cyy = GetSystemMetrics(SM_CYSMICON);
	cxx = GetSystemMetrics(SM_CXSMICON);
	wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDR_PROCESSEXPLORER), IMAGE_ICON, cxx, cyy, 0);
	if (!RegisterClassEx(&wc) && !RegisterClass((WNDCLASS*)&wc.style))
		return FALSE;

	///ע������ͼ
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = CGraph::GraphWndProc;
	wc.hInstance = ghInstance;
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	///ͼ������ͼ
	wc.lpszClassName = _TEXT("CpuGraphClassChart");
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&wc);

	///ע�����б�����ͼ
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = CGraph::GraphWndProc;
	wc.hInstance = ghInstance;
	wc.hbrBackground = CreateSolidBrush(gColorGraphBackground);
	///ͼ�����б�
	wc.lpszClassName = TEXT("CpuGraphClassGraph");
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&wc);


	////����������
	hWndMain = CreateWindowEx(
		NULL,
		PROCESSEXPLORER_CLASSNAME,
		TEXT("FAKE Process Explorer - SMALLFOOL,2018"),
		WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_GROUP | WS_TABSTOP,
		0,
		0,
		780,
		550,
		0,
		0,
		hInstance,
		NULL);

	if (!hWndMain)
		return FALSE;

	////��ʼ�������ھ���λ����Ϣ
	if (gWindowPlacement[Windowplacement].length)
	{
		bMinimized = gWindowPlacement[Windowplacement].showCmd;
		if (gWindowPlacement[Windowplacement].showCmd == SW_MINIMIZE)
			bMinimized = 1;
		if (gbHide1)
			bMinimized = 0;
		gWindowPlacement[Windowplacement].showCmd = bMinimized;
		nWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		if (!nWidth)
			nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
		nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		if (!nHeight)
			nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
		left = gWindowPlacement[Windowplacement].rcNormalPosition.left;
		right = gWindowPlacement[Windowplacement].rcNormalPosition.right;
		if (gWindowPlacement[Windowplacement].rcNormalPosition.left >= nWidth - 10)
			left = 100;
		gWindowPlacement[Windowplacement].rcNormalPosition.left = left;
		top = gWindowPlacement[Windowplacement].rcNormalPosition.top;
		if (gWindowPlacement[Windowplacement].rcNormalPosition.right > nWidth)
			right = nWidth;
		gWindowPlacement[Windowplacement].rcNormalPosition.right = right;
		if (gWindowPlacement[Windowplacement].rcNormalPosition.top >= nHeight - 10)
			top = 100;
		gWindowPlacement[Windowplacement].rcNormalPosition.top = top;
		if (nShowCmd == SW_SHOWMINNOACTIVE)
		{
			if (gbHideWhenMinimized)
				gWindowPlacement[Windowplacement].showCmd = SW_SHOW;
		}
		SetWindowPlacement(hWndMain, &gWindowPlacement[Windowplacement]);
	}
	else
	{
		nShowCommand = SW_SHOW;
		if (gbHide1)
			nShowCommand = SW_HIDE;
		ShowWindow(hWndMain, nShowCommand);
	}
	if (gbHide1)
		gbHide2 = true;
	UpdateWindow(hWndMain);
	return TRUE;
}


// 0x7D1�Ĵ�����
BOOL CMainWnd::OnNotify7D1()
{
	return 0;
}


// 0x7D2�Ĵ�����
BOOL CMainWnd::OnNotify7D2()
{
	return 0;
}


// 0x7D4�Ĵ�����
BOOL CMainWnd::OnNotify7D4(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


// 0x7D5�Ĵ�����
BOOL CMainWnd::OnNotify7D5(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


// ��Ϣ����ص�����
LRESULT CALLBACK CMainWnd::MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	///��������½�ͼ����ʾ��Ϣ
	if (message == gShellIconMsg)
		HandleShellIconMsg();
	///ת�����������Ϣ������
	return MSGHANDLER::HandleMessage(&gNormalWindowMsgHandler, hWnd, message, wParam, lParam);
}


// �������½�ͼ����Ӧ����
BOOL CMainWnd::HandleShellIconMsg()
{
	return 0;
}

////������ɫ��ProcExplorer������Ϣ������
LRESULT MSGHANDLER::HandleMessage(MSGHANDLER* pMsgHandler, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!pMsgHandler)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	if (pMsgHandler->Count <= 0)
	{
		///Ĭ�ϴ���
		switch (pMsgHandler->Type)
		{
		case WINDOW_NORMAL:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		case WINDOW_DIALOG:
			return DefDlgProc(hWnd, msg, wParam, lParam);
		case WINDOW_MDICHILD:
			return DefMDIChildProc(hWnd, msg, wParam, lParam);
		case WINDOW_FRAME:
			return DefFrameProc(hWnd, NULL, msg, wParam, lParam);
		default:
			return	0;
		}
	}
	///���Ƚ����Զ������Ϣ������
	for (DWORD i = 0;i < pMsgHandler->Count;i++)
	{
		if (pMsgHandler->pFuncs[i].dwMsgId == msg)
			return (*(pMsgHandler->pFuncs[i].pfnCallback))(hWnd, msg, wParam, lParam);
	}
	///Ĭ�ϴ���
	switch (pMsgHandler->Type)
	{
	case WINDOW_NORMAL:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	case WINDOW_DIALOG:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	case WINDOW_MDICHILD:
		return DefMDIChildProc(hWnd, msg, wParam, lParam);
	case WINDOW_FRAME:
		return DefFrameProc(hWnd, NULL, msg, wParam, lParam);
	default:
		return	0;
	}
	return	0;
}

///������ɫ��ProcExplorer��WM_COMMAND��Ϣ���ɷ�����
LRESULT MSGHANDLER::HandleCommand(MSGHANDLER* pMsgHandler, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!pMsgHandler)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	if (pMsgHandler->Count <= 0)
	{
		///Ĭ�ϴ���
		switch (pMsgHandler->Type)
		{
		case WINDOW_NORMAL:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		case WINDOW_DIALOG:
			return DefDlgProc(hWnd, msg, wParam, lParam);
		case WINDOW_MDICHILD:
			return DefMDIChildProc(hWnd, msg, wParam, lParam);
		case WINDOW_FRAME:
			return DefFrameProc(hWnd, NULL, msg, wParam, lParam);
		default:
			return	0;
		}
	}
	for (DWORD i = 0;i < pMsgHandler->Count;i++)
	{
		if (pMsgHandler->pFuncs[i].dwMsgId == wParam)
			return (*(pMsgHandler->pFuncs[i].pfnCallback))(hWnd, LOWORD(wParam), HIWORD(wParam), lParam);
	}
	///Ĭ�ϴ���
	switch (pMsgHandler->Type)
	{
	case WINDOW_NORMAL:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	case WINDOW_DIALOG:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	case WINDOW_MDICHILD:
		return DefMDIChildProc(hWnd, msg, wParam, lParam);
	case WINDOW_FRAME:
		return DefFrameProc(hWnd, NULL, msg, wParam, lParam);
	default:
		return	0;
	}
	return	0;
}


// ����������
HWND CMainWnd::CreateRebar(HWND hWndParent, UINT nID)
{
	UINT nToolbarHeight = 0;
	REBARINFO rbInfo = { 0 };
	INITCOMMONCONTROLSEX picce;
	REBARBANDINFO rbbi = { 0 };
	RECT rcSrc1; // 
	RECT rcSrc2; // 
	RECT rcDst; // 
	TBBUTTON tbBtns[14] = { 0 };

	///��ʼ����ť
	tbBtns[0].iBitmap = 3;
	tbBtns[0].idCommand = IDS_PROCEXPLORERMENU_SAVE;
	tbBtns[0].fsState = TBSTATE_ENABLED;
	tbBtns[1].fsState = TBSTATE_ENABLED;
	tbBtns[2].iBitmap = 7;
	tbBtns[2].idCommand = IDM_VIEW_REFRESH_NOW;
	tbBtns[2].fsState = TBSTATE_ENABLED;
	tbBtns[3].iBitmap = 0;
	tbBtns[3].fsState = TBSTYLE_TOOLTIPS | TBSTATE_ENABLED;
	tbBtns[4].iBitmap = 13;
	tbBtns[4].idCommand = IDM_VIEW_SYSTEM_INFORMATION;
	tbBtns[4].fsState = 4;
	tbBtns[5].iBitmap = 8;
	tbBtns[5].idCommand = IDM_VIEW_SHOW_PROCESS_TREE;
	tbBtns[5].fsState = TBSTATE_ENABLED;
	tbBtns[6].iBitmap = 9;
	tbBtns[6].idCommand = IDM_VIEW_SHOW_LOWER_PANE;
	tbBtns[6].fsState = TBSTATE_ENABLED;
	tbBtns[7].iBitmap = 4;
	tbBtns[7].idCommand = IDM_POPUP_DLL_PROPERTIES;
	tbBtns[7].fsState = TBSTATE_ENABLED;
	tbBtns[8].iBitmap = 0;
	tbBtns[8].idCommand = 0;
	tbBtns[8].fsState = TBSTYLE_TOOLTIPS | TBSTATE_ENABLED;
	tbBtns[9].iBitmap = 6;
	tbBtns[9].idCommand = IDM_PROCESS_PROPERTIES;
	tbBtns[9].fsState = TBSTATE_ENABLED;
	tbBtns[10].iBitmap = 5;
	tbBtns[10].idCommand = IDM_PROCESS_KILL_PROCESS;
	tbBtns[10].fsState = TBSTATE_ENABLED;
	tbBtns[11].iBitmap = 0;
	tbBtns[11].idCommand = 0;
	tbBtns[11].fsState = TBSTYLE_TOOLTIPS | TBSTATE_ENABLED;
	tbBtns[12].iBitmap = 2;
	tbBtns[12].idCommand = IDM_FIND_HANDLE_OR_DLL;
	tbBtns[12].fsState = TBSTATE_ENABLED;
	tbBtns[13].iBitmap = 12;
	tbBtns[13].idCommand = IDM_FIND_WINDOW;
	tbBtns[13].fsState = TBSTATE_ENABLED;
	////
	///�ǵò�Ҫ�����������
	picce.dwSize = sizeof(picce);
	picce.dwICC = ICC_USEREX_CLASSES | ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	InitCommonControlsEx(&picce);

	///����rebar
	ghWndRebar = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		_TEXT("ReBarWindow32"),
		0,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | RBS_BANDBORDERS | CCS_NODIVIDER | CCS_NOPARENTALIGN,
		//0x56800448u,
		0,
		0,
		400,
		275,
		hWndParent,
		0,
		ghInstance,
		NULL);
	rbInfo.cbSize = sizeof(rbInfo);
	rbInfo.fMask = 0;

	if (!SendMessage(ghWndRebar, RB_SETBARINFO, 0, (LPARAM)&rbInfo))
		return NULL;

	///����������
	ghWndToolbar = CreateToolbarEx(
		ghWndRebar,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | BTNS_SHOWTEXT | TBSTYLE_GROUP,
		//0x56000944,
		nID,
		12,                          // idBitmap
		ghInstance,
		IDC_TOOLBAR,
		tbBtns,
		14,
		NULL,
		0,
		0,
		0,
		20);                        // structsize
	if (!ghWndToolbar)
		return NULL;

	///���㹤�����ĸ߶�
	nToolbarHeight = GetToolbarButtonHeight();

	SendMessage(ghWndToolbar, TB_GETRECT, IDS_PROCEXPLORERMENU_SAVE, (LPARAM)&rcSrc1);
	SendMessage(ghWndToolbar, TB_GETRECT, IDM_FIND_WINDOW, (LPARAM)&rcSrc2);
	UnionRect(&rcDst, &rcSrc1, &rcSrc2);

	rbbi.cbSize = sizeof(rbbi);
	///��һ����BANRDINFO
	// RBBIM_ID|RBBIM_CHILD|RBBIM_CHILDSIZE|RBBIM_SIZE|RBBIM_STYLE|RBBIM_COLORS
	rbbi.fMask = RBBIM_ID | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE | RBBIM_STYLE | RBBIM_COLORS;
	rbbi.fStyle = RBBS_GRIPPERALWAYS;
	rbbi.hwndChild = ghWndToolbar;
	rbbi.cxMinChild = 0;
	rbbi.clrFore = GetSysColor(COLOR_BTNTEXT);
	rbbi.clrBack = GetSysColor(COLOR_BTNFACE);
	rbbi.cx = rcDst.right - rcDst.left;
	rbbi.cxMinChild = rcDst.right - rcDst.left;
	rbbi.cyMinChild = HIWORD(nToolbarHeight);
	rbbi.wID = 0;
	SendMessage(ghWndRebar, RB_INSERTBANDW, 0xFFFFFFFF, (LPARAM)&rbbi);

	///�ڶ���BANDINFO
	ghWndGraphCPUUsage = CreateWindowEx(
		0,
		_TEXT("CpuGraphClassGraph"),
		gpszTargetName,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | WS_CLIPSIBLINGS | RBBS_CHILDEDGE,
		//0x56010004,
		0,
		0,
		0,
		0,
		ghWndRebar,
		(HMENU)IDC_GRAPHCTRL,
		ghInstance,
		(void*)&gpGraphClassData[1]);
	rbbi.hwndChild = ghWndGraphCPUUsage;
	rbbi.cx = MulDiv(75, gLogPixelsX, 96);
	rbbi.cxMinChild = 0;
	rbbi.wID = 1;
	SendMessage(ghWndRebar, RB_INSERTBANDW, 0xFFFFFFFF, (LPARAM)&rbbi);

	///������BANDINFO
	ghWndGraphMemory = CreateWindowEx(
		0,
		_TEXT("CpuGraphClassGraph"),
		gpszTargetName,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | WS_CLIPSIBLINGS | RBBS_CHILDEDGE,
		//0x56010004,
		0,
		0,
		0,
		0,
		ghWndRebar,
		(HMENU)IDC_GRAPHCTRL,
		ghInstance,
		(void*)&gpGraphClassData[0]);
	rbbi.hwndChild = ghWndGraphMemory;
	rbbi.cx = MulDiv(75, gLogPixelsX, 96);
	rbbi.cxMinChild = 0;
	rbbi.wID = 2;
	SendMessage(ghWndRebar, RB_INSERTBANDW, 0xFFFFFFFF, (LPARAM)&rbbi);

	///���ĸ�BANDINFO
	ghWndGraphPhysicalMemory = CreateWindowEx(
		0,
		_TEXT("CpuGraphClassGraph"),
		gpszTargetName,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | WS_CLIPSIBLINGS | RBBS_CHILDEDGE,
		//0x56010004,
		0,
		0,
		0,
		0,
		ghWndRebar,
		(HMENU)IDC_GRAPHCTRL,
		ghInstance,
		(void*)&gpGraphClassData[5]);
	rbbi.hwndChild = ghWndGraphPhysicalMemory;
	rbbi.cx = MulDiv(75, gLogPixelsX, 96);
	rbbi.cxMinChild = 0;
	rbbi.wID = 3;
	SendMessage(ghWndRebar, RB_INSERTBANDW, 0xFFFFFFFF, (LPARAM)&rbbi);

	///�����BANDINFO
	if (LOWORD(gptrIOGraphData.GraphData))
	{
		ghWndIOGraphCtrl = CreateWindowEx(
			0,
			_TEXT("CpuGraphClassGraph"),
			gpszTargetName,
			WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | RBBS_CHILDEDGE,
			//0x56010004,
			0,
			0,
			0,
			0,
			ghWndRebar,
			(HMENU)IDC_GRAPHCTRL,
			ghInstance,
			(void*)(&gptrIOGraphData.GraphData));
		rbbi.hwndChild = ghWndIOGraphCtrl;
		rbbi.cx = MulDiv(75, gLogPixelsX, 96);
		rbbi.cxMinChild = 0;
		rbbi.wID = 4;
		SendMessage(ghWndRebar, RB_INSERTBANDW, 0xFFFFFFFF, (LPARAM)&rbbi);
	}

	///������BANDINFO
	///	if (IsBuiltinAndAnministrative() && LOWORD(gptrNetGraphData.GraphData))
	{
		ghWndNetworkGraphCtrl = CreateWindowEx(
			0,
			_TEXT("CpuGraphClassGraph"),
			gpszTargetName,
			WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | WS_CLIPSIBLINGS | RBBS_CHILDEDGE,
			//0x56010004,
			0,
			0,
			0,
			0,
			ghWndRebar,
			(HMENU)IDC_GRAPHCTRL,
			ghInstance,
			(void*)&gptrNetGraphData.GraphData);
		rbbi.hwndChild = ghWndNetworkGraphCtrl;
		rbbi.cx = MulDiv(75, gLogPixelsX, 96);
		rbbi.cxMinChild = 0;
		rbbi.wID = 5;
		SendMessage(ghWndRebar, RB_INSERTBANDW, 0xFFFFFFFF, (LPARAM)&rbbi);
	}

	///���߸�BANDINFO
	if (IsBuiltinAndAnministrative() && LOWORD(gptrDiskGraphData.GraphData))
	{
		ghWndDiskGraphCtrl = CreateWindowEx(
			0,
			_TEXT("CpuGraphClassGraph"),
			gpszTargetName,
			WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | WS_CLIPSIBLINGS | RBBS_CHILDEDGE,
			//0x56010004,
			0,
			0,
			0,
			0,
			ghWndRebar,
			(HMENU)IDC_GRAPHCTRL,
			ghInstance,
			(void*)&gptrDiskGraphData.GraphData);
		rbbi.hwndChild = ghWndDiskGraphCtrl;
		rbbi.cx = MulDiv(75, gLogPixelsX, 96);
		rbbi.cxMinChild = 0;
		rbbi.wID = 6;
		SendMessage(ghWndRebar, RB_INSERTBANDW, 0xFFFFFFFF, (LPARAM)&rbbi);
	}

	///�ڰ˸�BANDINFO
	if (gdwAdapterRuntingTime)
	{
		ghWndGPUGraphCtrl = CreateWindowEx(
			0,
			_TEXT("CpuGraphClassGraph"),
			gpszTargetName,
			WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | WS_CLIPSIBLINGS | RBBS_CHILDEDGE,
			//0x56010004,
			0,
			0,
			0,
			0,
			ghWndRebar,
			(HMENU)IDC_GRAPHCTRL,
			ghInstance,
			(void*)&gpGraphClassData[2]);
		rbbi.hwndChild = ghWndGPUGraphCtrl;
		rbbi.cx = MulDiv(75, gLogPixelsX, 96);
		rbbi.cxMinChild = 0;
		rbbi.wID = 7;
		SendMessage(ghWndRebar, RB_INSERTBANDW, 0xFFFFFFFF, (LPARAM)&rbbi);
	}


	if (gdwToolbarBands[0] != -1)
	{
		int nItem = 0;
		int idx = 0;
		do
		{
			int nIndex = SendMessage(ghWndRebar, RB_IDTOINDEX, gGraphReBarCtrlIds[idx], 0);
			SendMessage(ghWndRebar, RB_MOVEBAND, nIndex, nItem);
			SendMessage(ghWndRebar, RB_GETBANDINFOW, nItem, (LPARAM)&rbbi);
			rbbi.fMask = RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
			if (gdwSortableColumnVector[idx] == 0)
				rbbi.fStyle &= -RBBS_FIXEDSIZE;
			else
				rbbi.fStyle |= RBBS_BREAK;
			rbbi.cx = MulDiv(gdwToolbarBands[idx], gLogPixelsX, 96);
			rbbi.wID = gGraphReBarCtrlIds[idx];
			SendMessage(ghWndRebar, RB_SETBANDINFOW, nItem, (LPARAM)&rbbi);
			idx += 3;
			++nItem;
		} while (idx < 24);
	}
	return ghWndToolbar;
}
