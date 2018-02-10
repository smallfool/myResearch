#pragma once
///������
class CMainWnd
{
public:
	CMainWnd();
	~CMainWnd();

public:
	HWND m_hWnd;
	// ����������
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	// 0x7D1�Ĵ�����
	static BOOL OnNotify7D1();
	// 0x7D2�Ĵ�����
	static BOOL OnNotify7D2();
	// 0x7D4�Ĵ�����
	static BOOL OnNotify7D4(WPARAM wParam, LPARAM lParam);
	// 0x7D5�Ĵ�����
	static BOOL OnNotify7D5(WPARAM wParam, LPARAM lParam);
	// ��Ϣ����ص�����
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	// �������½�ͼ����Ӧ����
	static BOOL HandleShellIconMsg();
public:
	// ����������
	static HWND CreateRebar(HWND hWnd, UINT nID);
};

