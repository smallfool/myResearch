#include "stdafx.h"
#include "..\include\Resize.h"

LRESULT CThemedWindow::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


////////////////////////////////////////////////////////////////
///
CResizer::CResizer()
{
	m_hWnd = NULL;
	m_Buffer = NULL;
	m_BufCount = 0L;

	m_FixedBorder = true;
	m_WidthScale = 50;
	m_HeightScale = 70;
}

CResizer::CResizer(HWND hWnd)
	:m_hWnd(hWnd)
{
	m_FixedBorder = true;
	m_WidthScale = 50;
	m_HeightScale = 70;
	this->HookHwnd(hWnd);
}


CResizer::~CResizer()
{
	if(m_Buffer)
	{
		::free(m_Buffer);
		m_Buffer = NULL;
	}
}

LRESULT CResizer::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_WndProc, hWnd, msg, wParam, lParam);
}

CResizeBuffer* CResizer::InitFromHwnd(HWND hWnd, int nCount)
{
	///双精度的浮点数。-1.0000000
	////对于MMX再来说就是 __m128 X = -1.0000000;
	static DWORD value[4] = { 0, 0xBFF00000, 0, 0xBFF0000 };

	if (hWnd == NULL)
		return NULL;
	CResizeBuffer* pBuf = m_Buffer;
	if (m_BufCount > 0)
	{
		int i = 0;
		while (pBuf->hWnd != hWnd)
		{
			i++;
			pBuf++;
			if (i >= nCount)
				break;
		}
		if (i < nCount)
			return pBuf;
	}

	if (nCount <= 0)
		return	NULL;
	m_BufCount++;

	pBuf = (CResizeBuffer*)::realloc((void*)m_Buffer, sizeof(CResizeBuffer)*m_BufCount);

	if (!pBuf)
	{
		return NULL;
	}
	pBuf += (m_BufCount - 1);
	pBuf->hWnd = hWnd;
	///初始化这个双精度
	CopyMemory(&pBuf->value1, value, 16);
	CopyMemory(&pBuf->value2, value, 16);
	GetWindowRect(hWnd, &pBuf->Rect);
	ScreenToClient(this->m_hWnd, (LPPOINT)&pBuf->Rect.left);
	ScreenToClient(this->m_hWnd, (LPPOINT)&pBuf->Rect.right);
	return pBuf;
}

void CResizer::Update(HWND* phWnds, int nCount)
{
	RECT rect;
	double dbHeight = 0.0;
	double dbLeft = 0.0;
	double res = 0.0;

	for (int i = 0; i < nCount;i++)
	{
		GetWindowRect(phWnds[i], &rect);
		dbHeight += (rect.bottom - rect.top);
	}

	for (int j = 0; j < nCount;j++)
	{
		GetWindowRect(phWnds[j], &rect);
		dbLeft += (rect.bottom - rect.top);
		dbHeight = dbHeight / 1000.0;
		dbLeft += dbHeight;
		rect.right = (long)dbLeft;
		res = dbLeft;
		if (dbLeft > 0.9999)
		{
			res = 1.0;
		}
		CResizeBuffer* pBuf = InitFromHwnd(phWnds[j], 1);
		pBuf->value2.m128d_f64[0] = dbLeft;
		pBuf->value2.m128d_f64[1] = res;

	}
}

TCHAR* CResizer::SZPROPCLASS = TEXT("ResizerClass");

LRESULT CALLBACK CResizer::HookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	DWORD dwStyle;
	WNDPROC pfn = NULL;
	CResizer *pResizer = (CResizer*)::GetProp(hWnd, CResizer::SZPROPCLASS);
	switch (msg)
	{
	case WM_DESTROY:
		pfn = pResizer->m_WndProc;
		SetWindowLong(hWnd, GWL_WNDPROC, (LONG)pResizer->m_WndProc);
		delete pResizer;
		return CallWindowProc(pfn, hWnd, msg, wParam, lParam);
	case WM_PAINT:
		dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		if ( (!(dwStyle & WS_CHILD))
			&& (!IsZoomed(hWnd)))
		{
			PAINTSTRUCT ps = { 0 };
			HDC hDC = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			rect.left = rect.right - GetSystemMetrics(SM_CXHSCROLL);
			rect.top = rect.bottom - GetSystemMetrics(SM_CYVSCROLL);
			if (pResizer->m_Theme)
			{
				DrawThemeBackground(pResizer->m_Theme, hDC, SBP_THUMBBTNVERT, 0, &rect, NULL);
			}
			else
			{
				DrawFrameControl(hDC, &rect, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
			}
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_GETMINMAXINFO:
	{

		MINMAXINFO *pInfo = (MINMAXINFO*)lParam;
		pInfo->ptMinTrackSize.x = pResizer->m_nWidth2;
		pInfo->ptMinTrackSize.y = pResizer->m_nHeight2;
	}
	return 0L;
	case WM_NCHITTEST:
	{
		int res = CallWindowProcW(pResizer->m_WndProc, hWnd, msg, wParam, lParam);
		dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		if ((dwStyle & WS_CHILD)
			|| res != HTCLIENT
			|| IsZoomed(hWnd))
		{
			GetWindowRect(hWnd, &rect);
			rect.left = rect.right - GetSystemMetrics(SM_CXHSCROLL);
			rect.top = rect.bottom - GetSystemMetrics(SM_CYVSCROLL);
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			if (!PtInRect(&rect, pt))
				return res;
		}
		return HTBOTTOMRIGHT;
	}
	case WM_SIZE:
	{
		if (!pResizer->m_FixedBorder || (wParam == SIZE_MAXIMIZED && wParam!=SIZE_RESTORED))
		{
		}
		else
		{
			if (GetWindowRect(hWnd, &rect))
			{
				int Width = rect.right - rect.left;
				int Height = rect.bottom - rect.top;
				if (rect.right - rect.left < pResizer->m_nWidth2)
					Width = pResizer->m_nWidth2;
				//v13 = __OFSUB__(Height, pResizer->m_nHeight2);
				//v12 = Height - pResizer->m_nHeight2 < 0;
				pResizer->m_nWidth1 = Width;
	//			if (v12 ^ v13)
					Height = pResizer->m_nHeight2;
				pResizer->m_nHeight1 = Height;
				//sub_43FD80(pResizer);
			}
		}
	}
	break;
	default:
		if (msg == WM_THEMECHANGED)
		{
			if (pResizer->m_Theme)
				CloseThemeData(pResizer->m_Theme);
			pResizer->m_Theme = 0;
			if (IsThemeActive())
			{

				pResizer->m_Theme = OpenThemeData(hWnd, pResizer->GetName());
			}
		}
		return pResizer->HandleMessage( hWnd, msg, wParam, lParam);
	}
	return pResizer->HandleMessage(hWnd, msg, wParam, lParam);
}

int CResizer::HookHwnd(HWND hWnd)
{
	RECT Rect;
	this->m_hWnd = hWnd;
	SetProp(hWnd, CResizer::SZPROPCLASS, (HANDLE)this);
	this->m_WndProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);
	SetWindowLong(hWnd, GWL_WNDPROC,(LONG) CResizer::HookWndProc);

	if (this->m_Theme)
		CloseThemeData(this->m_Theme);
	this->m_Theme = 0;
	if ( IsThemeActive())
	{
		this->m_Theme = OpenThemeData(hWnd, GetName());
	}
	GetWindowRect(this->m_hWnd, &Rect);
	this->m_nWidth1 = Rect.right - Rect.left;
	this->m_nHeight1 = Rect.bottom - Rect.top;
	this->m_nWidth2 = this->m_nWidth1;
	this->m_nHeight2 = this->m_nHeight1;
	return	TRUE;
}

BOOL __stdcall EnumFunc(HWND hWnd, LPARAM lParam)
{
	CResizer* pResizer = (CResizer*)lParam;
	return pResizer->WalkallWindows(hWnd);
}

BOOL CResizer::WalkallWindows(HWND hWnd)
{
	TCHAR szClassName[_MAX_PATH] = { 0 };
	DWORD dwStyle ;
	RECT rect;
	int Height, Width;
	int nHeight, nWidth = 0;
	//int nHeight1;

	bool NoSysBtn = true;
	double dbYYRatio = 0.0;
	double dbXXRatio = 0.0;
	double dbXRatio = 0.0;
	double dbYRatio = 0.0;
	double dyRatio1, dyRatio2;
	if (GetParent(hWnd) == this->m_hWnd)
	{
		GetWindowRect(hWnd, &rect);
		ScreenToClient(this->m_hWnd, (POINT*)&rect.left);
		ScreenToClient(this->m_hWnd, (POINT*)&rect.right);
		dbYYRatio = 0.0;
		Height = this->m_nHeight * this->m_HeightScale;
		Width = this->m_nWidth * this->m_WidthScale / 100;
		//Width1 = Width;
		nHeight = Height / 100;
		//nHeight1 = Height / 100;

		if (rect.left >= Width)
			dbXRatio = 1.0;
		else
			dbXRatio = 0.0;

		if (rect.right >= Width)
			dbXXRatio = 1.0;
		else
			dbXXRatio = 0.0;

		if (rect.top >= nHeight)
			dbYRatio = 1.0;
		else
			dbYRatio = 0.0;
		if (rect.bottom >= nHeight)
			dbYYRatio = 1.0;

		GetClassName(hWnd, szClassName, _MAX_PATH);

		if (!_tcsicmp(szClassName, _TEXT("BUTTON")))
		{
			dwStyle = dwStyle & 0xD;
			if (dwStyle == BS_PUSHBUTTON
				|| dwStyle == BS_DEFPUSHBUTTON
				|| dwStyle == BS_OWNERDRAW
				|| dwStyle == BS_USERBUTTON)
			{
				NoSysBtn = false;
			}
		}

		if (!_tcsicmp(szClassName, _TEXT("STATIC")))
			InvalidateRect(hWnd, NULL, FALSE);



		//bottom = rect.bottom;
		//top = rect.top;
		//right = rect.right;

		if (NoSysBtn)
		{
			dyRatio1 = dbYYRatio;
			dyRatio2 = dbYRatio;
		}
		else
		{
			if (Width - rect.left < rect.right - Width)
				dbXRatio = dbXXRatio;
			else
				dbXXRatio = dbXRatio;
			if (nWidth - rect.top < rect.bottom - nWidth)
			{
				dyRatio1 = dbYYRatio;
				dyRatio2 = dbYYRatio;
			}
			else
			{
				dyRatio2 = dbYRatio;
				dyRatio1 = dbYRatio;
			}
		}

	}

	DWORD dwBufferCount = this->m_BufCount;
	if (hWnd)
	{
		if (this->m_BufCount <= 0)
			dwBufferCount = 0;
	}
	else
	{
		DWORD dwIndexFound = -1;
		for (DWORD i = 0; i < dwBufferCount; i++)
		{
			if (m_Buffer[i].hWnd == hWnd)
			{
				dwIndexFound = i;
				break;
			}
		}
		if (dwIndexFound)
		{
			if (m_Buffer[dwIndexFound].value1.m128d_f64[0] > 0.0)
			{
				dbXRatio = m_Buffer[dwIndexFound].value1.m128d_f64[0];
			}
			if (m_Buffer[dwIndexFound].value1.m128d_f64[1] > 0.0)
			{
				dbXXRatio = m_Buffer[dwIndexFound].value1.m128d_f64[1];
			}
		}
	}
	return	FALSE;
}

int  CResizer::Refresh()
{
	int result; // eax

	int nHeight; // ST08_4
	int nWidth; // eax
	RECT rcDst; // [esp+4h] [ebp-44h]
	RECT rc; // [esp+14h] [ebp-34h]
	RECT rect; // [esp+24h] [ebp-24h]
	RECT rcSrc; // [esp+34h] [ebp-14h]


	if (this->m_nHeight != this->m_nHeight1
		|| this->m_nWidth!= this->m_nWidth1)
	{
		this->m_hDWP = BeginDeferWindowPos(SWP_NOREPOSITION);
		EnumChildWindows(this->m_hWnd, EnumFunc, (LPARAM)this);
		EndDeferWindowPos(this->m_hDWP);
		GetClientRect(this->m_hWnd, &rect);

		rcSrc.left	= rect.right - GetSystemMetrics(SM_CXHSCROLL);
		rcSrc.top	= rect.bottom - GetSystemMetrics(SM_CYVSCROLL);
		rcSrc.right = rect.right;
		rcSrc.bottom = rect.bottom;
		nHeight = this->m_nHeight - this->m_nHeight1;
		nWidth = this->m_nWidth - this->m_nWidth1;
		_mm_storeu_si128((__m128i*)&rc, _mm_loadu_si128((__m128i*)&rcSrc));
		OffsetRect(&rc, nWidth, nHeight);
		UnionRect(&rcDst, &rc, &rcSrc);
		InvalidateRect(this->m_hWnd, &rcDst, TRUE);
		this->m_nWidth = this->m_nWidth1;
		this->m_nHeight = this->m_nHeight1;
		result = InvalidateRect(this->m_hWnd, 0, TRUE);
	}
	return result;
}