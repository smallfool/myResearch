#include "stdafx.h"
#include "..\include\QQInject.h"

typedef struct _FUCKWINDOWINFO
{
	TCHAR* Name;
	TCHAR* Class;
}FUCKWINDOWINFO, *PFUCKWINDOWINFO, *LPFUCKWINDOWINFO;

static FUCKWINDOWINFO  gWindowNames[] = {
	{_TEXT("��Ѷ����������"),_TEXT("TXGuiFoundation")},
	//s{_TEXT("����������ܴ�-Ƿ��12��̿�Ҹ�"),_TEXT("TXGuiFoundation")},
	
	//{_TEXT("����Ự"),NULL} ,//#32770 (�Ի���)")},
	///{_TEXT("TeamViewer Panel"),_TEXT("TV_ControlWin")},
	{_TEXT("�������� - ��������"),_TEXT("StandardWindow")},
};



CQQInject::CQQInject()
{
	m_Exiting = false;
	m_KillProcess = false;
	m_ThreadId = 0L;
	this->m_ThreadHandle = NULL;
}


CQQInject::~CQQInject()
{
	PostThreadMessage(m_ThreadId, WM_CLOSE, 0, 0);
	m_Exiting = true;
	Sleep(50);
	CloseHandle(m_ThreadHandle);
	m_ThreadId = 0;
	m_ThreadHandle = NULL;
}


// ��������QQ���Ŵ��ڵ����ش���
BOOL CQQInject::Start()
{
	this->m_ThreadHandle = CreateThread(NULL, 0, ThreadProc, (void*)this, 0, &m_ThreadId);
	if (!m_ThreadHandle)
		return	FALSE;
	//WaitForSingleObject(m_ThreadHandle, INFINITE);
	return TRUE;
}

DWORD WINAPI CQQInject::ThreadProc(void* p)
{
	CQQInject* pInject = (CQQInject*)p;
	CoInitialize(NULL);
	INITCOMMONCONTROLSEX picce;
	///�ǵò�Ҫ�����������
	picce.dwSize = sizeof(picce);
	picce.dwICC = ICC_USEREX_CLASSES | ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	InitCommonControlsEx(&picce);
	while (1)
	{
		if (pInject->IsExit())
			break;
		pInject->Run();
	}
	////MSG msg;
	////while (GetMessage(&msg,0,0,0))
	////{
	////	if (msg.message == WM_CLOSE)
	////		break;
	////	TranslateMessage(&msg);
	////	DispatchMessage(&msg);
	////	if(pInject->IsExit())
	////		break;
	////	pInject->Run();
	////}
	return 0L;
}

void CQQInject::Run()
{
	TCHAR szClassName[256] = { 0 };
	for (int i = 0; i < sizeof(gWindowNames) / sizeof(*gWindowNames);i++)
	{
		HWND hWnd = FindWindowEx(NULL, NULL, NULL/*gWindowNames[i].Class*/, gWindowNames[i].Name);
		if (hWnd != NULL && IsWindow(hWnd))
		{
			GetClassName(hWnd, szClassName, 256);
			BOOL bDlg = FALSE;
			//����Ի�����ĵ���ʽ����
			{
				szClassName[6] = 0;
				if (_tcsicmp(szClassName, _TEXT("#32770")) == 0
					|| _tcsicmp(szClassName, _TEXT("#32771")) == 0
					|| _tcsicmp(szClassName, _TEXT("#32772")) == 0
					)
				{
					bDlg = TRUE;
				}
			}
			//��������ǧţ������


			//DbgPrintMessage(TRACE_WARNING, "FindWindowEx(\"%ls\",\"%ls\") OK!", szClassName, gWindowNames[i].Name);

			DWORD dwProcessId = 0L;
			if (m_KillProcess)
			{
				DWORD dwTheadId = ::GetWindowThreadProcessId(hWnd, &dwProcessId);
			}
			if (bDlg)
			{
				//	TeamViewer Panel
				//if (i == 3)	//
				//{
				//	HWND hBtn = GetDlgItem(hWnd, 12);
				//	//SetButtonState(hBtn, BTNS_CHECK);

				//}
				//if (IsWindowVisible(hWnd))
				//{
				//	ShowWindow(hWnd, SW_HIDE);
				//}
//				PostMessage(hWnd, WM_DESTROY, 0, 0);//, MAKEWPARAM(IDOK, 0), 0);
			}
			else
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			if (m_KillProcess)
			{
				KillProcess(dwProcessId);
			}
		}
	}

	Sleep(5);
	
}

BOOL CQQInject::KillProcess(DWORD dwPId)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPId);
	if (hProcess != INVALID_HANDLE_VALUE)
	{
		BOOL ret = TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		return ret;
	}
	return	FALSE;
}