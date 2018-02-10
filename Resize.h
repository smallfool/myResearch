#pragma once
struct CResizeBuffer
{
	__m128d value1;
	__m128d value2;
	RECT   Rect;
	HWND   hWnd;
	DWORD  dwValue;
};

class CThemedWindow
{
public:
	CThemedWindow() {}
	virtual TCHAR* GetName() = 0;
	virtual LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

class CResizer : public CThemedWindow
{
public:
	CResizer();
	CResizer(HWND hWnd);
	~CResizer();
	virtual TCHAR* GetName() { return TEXT("status"); }
	virtual LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	CResizeBuffer* InitFromHwnd(HWND hWnd, int nCount);
	void Update(HWND* phWnds, int nCount);
	int HookHwnd(HWND hWnd);
	int Refresh();
	BOOL WalkallWindows(HWND hWnd);
private:
	static TCHAR* SZPROPCLASS;
private:
	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	//04
	HTHEME	m_Theme;
	WNDPROC m_WndProc;
	HWND	m_hWnd;
	//10
	int		m_nWidth2;
	int		m_nHeight2;
	int		m_nWidth;
	int		m_nHeight;
	int		m_nWidth1;
	int		m_nHeight1;
	//20

	

	HDWP	m_hDWP;
	int		m_WidthScale;
	int		m_HeightScale;
	
	bool    m_FixedBorder;

	CResizeBuffer*	m_Buffer;
	DWORD	m_BufCount;
};

