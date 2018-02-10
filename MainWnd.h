#pragma once
///主窗口
class CMainWnd
{
public:
	CMainWnd();
	~CMainWnd();

public:
	HWND m_hWnd;
	// 创建主窗口
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	// 0x7D1的处理函数
	static BOOL OnNotify7D1();
	// 0x7D2的处理函数
	static BOOL OnNotify7D2();
	// 0x7D4的处理函数
	static BOOL OnNotify7D4(WPARAM wParam, LPARAM lParam);
	// 0x7D5的处理函数
	static BOOL OnNotify7D5(WPARAM wParam, LPARAM lParam);
	// 消息处理回调函数
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	// 处理右下角图标响应函数
	static BOOL HandleShellIconMsg();
public:
	// 创建工具条
	static HWND CreateRebar(HWND hWnd, UINT nID);
};

