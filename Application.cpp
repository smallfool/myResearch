#include "stdafx.h"
#include "../include/Common.h"
#include "../ProcExplorer/ProcExplorer.h"
#include "../include/Lock.h"

//定义一个枚举服务名称的锁
CLock gEnumServiceLock;
//
SC_HANDLE ghSCManager = NULL;
//
TCHAR* gstrServiceNameBuffer = NULL;
DWORD gdwServiceNameBufferSize = 0L;
DWORD gdwServiceNameRealSize = 0L;

//#include<Winternl.h>
TCHAR** gstrProcessComments = NULL;

extern LOGFONT gDefaultLogFont;
extern LRESULT CALLBACK TreeListWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


///全局变量初始化
static GlobalVariable gGlobalVariables[] =
{
	{ TEXT("Windowplacement"),			7, sizeof(WINDOWPLACEMENT), &gWindowPlacement[0], 0 },
	{ TEXT("FindWindowplacement"),		7, sizeof(WINDOWPLACEMENT), &gWindowPlacement[1], 0 },
	{ TEXT("SysinfoWindowplacement"),	7, sizeof(WINDOWPLACEMENT), &gWindowPlacement[2], 0 },
	{ TEXT("PropWindowplacement"),		7, sizeof(WINDOWPLACEMENT), &gWindowPlacement[3], 0 },
	{ TEXT("DllPropWindowplacement"),	7, sizeof(WINDOWPLACEMENT), &gWindowPlacement[4], 0 },

	{ TEXT("UnicodeFont"),			7,sizeof(LOGFONT), &gDefaultLogFont, 0 },
	{ TEXT("Divider"),			2, sizeof(double), NULL, 0 },
	{ TEXT(""),			0, 0,NULL, 0 },
	{ TEXT(""),			0, 0,NULL, 0 },

};

int  PE_GetDeviceCaps(int idx)
{
	HDC hDC = CreateCompatibleDC(NULL);
	int ret = GetDeviceCaps(hDC, idx);
	ReleaseDC(NULL, hDC);
	return ret;
}

LONG RetrieveProcessComments()
{
	DWORD idx; // esi
	LONG result; // eax
	DWORD Type; // [esp+4h] [ebp-814h]
	DWORD cbData; // [esp+8h] [ebp-810h]
	HKEY phkResult; // [esp+Ch] [ebp-80Ch]
	DWORD cchValueName; // [esp+10h] [ebp-808h]
	TCHAR ValueName[1024] = { 0 };; // [esp+14h] [ebp-804h]

	idx = 0;
	result = RegOpenKeyEx(
		HKEY_CURRENT_USER,
		_TEXT("Software\\Sysinternals\\Process Explorer\\ProcessComments"),
		0,
		0,//REGSAM_EXECUTE,
		&phkResult);
	if (!result)
	{
		cchValueName = 1024;
		if (!RegEnumValue(phkResult, 0, ValueName, &cchValueName, 0, &Type, 0, &cbData))
		{
			do
			{
				gstrProcessComments = (TCHAR**)realloc(gstrProcessComments, 8 * idx + 16);
				gstrProcessComments[2 * idx] = _tcsdup(ValueName);
				gstrProcessComments[2 * idx + 1] = (TCHAR*)malloc(cbData);
				cchValueName += 2;
				RegEnumValue(phkResult, idx, ValueName, &cchValueName, 0, &Type, (BYTE*)gstrProcessComments[2 * idx + 1], &cbData);
				gstrProcessComments[2 * idx + 2] = 0;
				cchValueName = 1024;
				++idx;
			} while (!RegEnumValue(phkResult, idx, ValueName, &cchValueName, 0, &Type, 0, &cbData));
		}
		result = RegCloseKey(phkResult);
	}
	return (LONG)result;
}
BOOL LoadProcessCommentsFromReg()
{
	return RetrieveProcessComments();
}


bool GetProcessDEPStatus()
{
	//HANDLE hProcess; // eax
	//int status; // edi
	//int nProcessFlags; // [esp+0h] [ebp-8h]
	//int dwRet; // [esp+4h] [ebp-4h]

	//if (gdwOsType >= WINDOWS_8 || gdwOsType >= WINDOWS_7)
	//	return true;
	//if (gdwOsType >= WINDOWS_VISTA)
		return false;
		ObjectBasicInformation;

	//hProcess2 = OpenProcess(PROCESS_QUERY_INFORMATION, 0, GetCurrentProcessId());
	//if (!hProcess2)
	//	return false;
	//hProcess = GetCurrentProcess();
	//status = NtQueryInformationProcess(hProcess, ProcessDEPStatus, &nProcessFlags, 4, &dwRet);
	//CloseHandle(hProcess2);
	//return !status || status == STATUS_INVALID_PARAMETER;
}



LONG LoadHiddenProcsFromRegistry()
{
	void* pBuffer = NULL;
	DWORD dwLength = 0L;
	CProcExpReg reg;
	if (!reg.Open())
		return	FALSE;
	gpHiddenProcs = (TCHAR**)malloc(sizeof(TCHAR**));

	ATLASSERT(gpHiddenProcs != NULL);

	if (!reg.GetValue(pBuffer, dwLength, TEXT("HiddenProcs")))
		return	FALSE;
	PTCHAR pszName = (PTCHAR)pBuffer;
	DWORD idx = 0;
	DWORD k = 0;
	while (pszName < ((PTCHAR)pBuffer + dwLength))
	{
		if (!*pszName)
			break;
		gpHiddenProcs = (TCHAR**)::realloc(gpHiddenProcs, idx * 4 + 8);
		gpHiddenProcs[idx] = _tcsdup(pszName);
		gpHiddenProcs[idx + 1] = 0;
		pszName += _tcslen(pszName) + 1;
		idx++;
	}
	return	TRUE;
}

int LoadGlobalVariablesFromRegistry(CRegStr* pString, GlobalVariable *pVariables);

BOOL IsX64()
{
	BOOL bWow64Process = FALSE;
	IsWow64Process(GetCurrentProcess(), &bWow64Process);
	return bWow64Process;
}

BOOL LoadDbgHelp(LPCTSTR lpszModuleName)
{
	return	FALSE;
}

BOOL LoadDriverFromRes(LPCTSTR lpszResId, LPCTSTR lpszFileName)
{
	HGLOBAL hResGlb;
	DWORD dwSize;
	void* pBuffer;
	FILE* pFile = NULL;
	HRSRC hRsrc = FindResource(NULL, lpszResId, TEXT("BINRES"));
	if (!hRsrc)
		return	FALSE;
	hResGlb = LoadResource(NULL, hRsrc);
	dwSize = SizeofResource(NULL, hRsrc);
	pBuffer = LockResource(hResGlb);
	if (_tfopen_s(&pFile, lpszFileName, TEXT("wb")))
	{
		return	FALSE;
	}
	fwrite(pBuffer, 1, dwSize, pFile);
	fclose(pFile);
	return	TRUE;
}

BOOL StartProcExp64(BOOL fWaiting, int nShowCmd)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = { 0 };
	TCHAR szFileName[_MAX_PATH] = { 0 };
	TCHAR szEnvCmd[_MAX_PATH] = { 0 };
	HKEY hKey = NULL;
	UINT nResId = 0;
	TCHAR* pszCmd;

	DWORD dwRet = GetModuleFileName(NULL, szFileName, _MAX_PATH - 1);
	if (!dwRet)
	{
		return	FALSE;
	}
	if (!RegCreateKey(HKEY_CURRENT_USER, gszMainProgramRegKeyName, &hKey))
	{
		RegSetValueEx(hKey, TEXT("OriginalPath"), 0, REG_SZ, (BYTE*)szFileName, 2 * _tcslen(szFileName));
		RegCloseKey(hKey);
	}
	*_tcsrchr(szFileName, TEXT('.')) = 0;
	_tcscat_s(szFileName, _MAX_PATH, TEXT("64.exe"));
	if (gSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		nResId = IDR_DRIVER_FOR_IA64;
	}
	else
	{
		if (gSystemInfo.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64)
		{
			return	FALSE;
		}
		nResId = IDR_DRIVER_FOR_AMD64;
	}
	ExpandEnvironmentStrings(TEXT("%TEMP%"), szEnvCmd, _MAX_PATH);

	pszCmd = _tcsrchr(szFileName, TEXT('\\'));
	_tcscat_s(szEnvCmd, _MAX_PATH, pszCmd);
	_tcscpy_s(szFileName, _MAX_PATH, szEnvCmd);

	if (!LoadDriverFromRes(MAKEINTRESOURCE(nResId), szFileName)
		&& GetFileAttributes(szFileName) == INVALID_FILE_ATTRIBUTES)
	{
		return	FALSE;
	}
	if (!RegOpenKey(HKEY_CURRENT_USER, gszMainProgramRegKeyName, &hKey))
	{
		RegSetValueEx(hKey, TEXT("Path"), 0, REG_SZ, (BYTE*)szFileName, 2 * _tcslen(szFileName));
		RegCloseKey(hKey);
	}
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = nShowCmd;

	///执行64位的可执行文件
	if (!CreateProcess(szFileName,
		GetCommandLine(),
		NULL,
		NULL,
		FALSE,
		0,
		0,
		0,
		&si,
		&pi))
	{
		return	FALSE;
	}

	if (fWaiting)
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		DeleteFile(szFileName);
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return	TRUE;
}

BOOL DumpProcessToFile(HANDLE hFile, HANDLE hProcess, BOOL bDumpFullMemory)
{
	DWORD dwDefaultType = 0L;
	MINIDUMP_TYPE Type;

	//EnumDirTreeW:
	//Enumerates all occurrences of the specified file in the specified directory tree.
	//枚举所有在指定文件夹中的指定文件.
	if (GetProcAddress(LoadLibraryW(L"DbgHelp.dll"), "EnumDirTreeW"))
	{
		dwDefaultType = MiniDumpWithThreadInfo | MiniDumpWithProcessThreadData | MiniDumpWithDataSegs | MiniDumpWithHandleData;
		if (bDumpFullMemory)
			dwDefaultType |= (MiniDumpWithFullMemoryInfo | MiniDumpWithFullMemory);
	}
	else
	{
		dwDefaultType = MiniDumpWithProcessThreadData | MiniDumpWithDataSegs | MiniDumpWithHandleData;
		if (bDumpFullMemory)
			dwDefaultType |= MiniDumpWithFullMemory;
	}
	Type = (MINIDUMP_TYPE)dwDefaultType;
	return	MiniDumpWriteDump(hProcess,
		GetProcessId(hProcess),
		hFile,
		Type,
		NULL,//No exception
		NULL,//No user stream information
		NULL);//No callback 

}

static void DumpProcessThreadProc(void* p)
{
	HANDLE hProcexp32bitServerEvent = NULL;
	HANDLE hProcexp32bitClientEvent = NULL;
	HANDLE hMapping = NULL;
	void* pMapBuf = NULL;
	MapInfo *pMapInfo = NULL;
	DWORD dwRet = 0L;
	HANDLE hFile = NULL;
	HANDLE hProcess = NULL;

	LoadDbgHelp(TEXT("DbgHelp.dll"));
	hProcexp32bitServerEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("Procexp32bitServerEvent"));
	hProcexp32bitClientEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("Procexp32bitClientEvent"));
	if (!hProcexp32bitServerEvent)
		return;
	hMapping = CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE | PAGE_REVERT_TO_FILE_MAP,
		0,
		gSystemInfo.dwPageSize,
		TEXT("Procexp32bitSection"));
	pMapBuf = MapViewOfFile(hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, gSystemInfo.dwPageSize);
	pMapInfo = (MapInfo*)pMapBuf;

	dwRet = WaitForSingleObject(hProcexp32bitServerEvent, INFINITE);
	if (!dwRet)
	{
		TCHAR* pszFileName = pMapInfo->szFileName;
		do
		{
			hFile = CreateFile(pszFileName,
				GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				pMapInfo->dwError = GetLastError();
			else
			{
				hProcess = OpenProcess(PROCESS_TRACE_MODE_EVENT_RECORD,
					FALSE,
					pMapInfo->dwProcessId);
				if (hProcess)
				{
					if (!DumpProcessToFile(hFile, hProcess, pMapInfo->bDumpFullMemory))
					{
						pMapInfo->dwError = GetLastError();
					}
					CloseHandle(hProcess);
				}
				else
				{
					pMapInfo->dwError = GetLastError();
				}
				CloseHandle(hFile);
			}

			SetEvent(hProcexp32bitClientEvent);
			dwRet = WaitForSingleObject(hProcexp32bitServerEvent, INFINITE);
			pszFileName = pMapInfo->szFileName;
		} while (!dwRet);
	}
}


///注册TreeList窗口类型
ATOM RegisterTreeListCtrl()
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) }; // [esp+0h] [ebp-30h]


	wc.lpfnWndProc = TreeListWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.style = LVS_SHOWSELALWAYS;
	wc.lpszClassName = _TEXT("TreeListWindowClass");
	return RegisterClassEx(&wc);
}

///从系统注册表中加载全局配置变量信息
int LoadGlobalVariablesFromRegistry(CRegStr* pReg, GlobalVariable* pVariables)
{
	GlobalVariable* pVar = pVariables;
	pReg->Open();

	while (pVar->Name != NULL)
	{
		switch (pVar->Type)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;

		default:
			break;
		}
		pVar++;
	}

	return	FALSE;
}

///初始化绘制环境
void InitDrawEngine()
{

	CRegStr str;
	LOGFONT lf;
	RegDeleteKey(HKEY_CURRENT_USER, gszMainProgramInnerRegKeyName);
	////这个比较狠，目前还不敢放进来
	///*/*memset(gWindowPlacement, 0, 18400u);*/*/
	gWindowPlacement[Windowplacement].showCmd = SW_SHOWNORMAL;
	gWindowPlacement[Windowplacement].rcNormalPosition.top = MulDiv(50, gLogPixelsY, 96);
	gWindowPlacement[Windowplacement].rcNormalPosition.bottom = MulDiv(650, gLogPixelsY, 96);
	gWindowPlacement[Windowplacement].rcNormalPosition.left = MulDiv(100, gLogPixelsX, 96);
	gWindowPlacement[Windowplacement].rcNormalPosition.right = MulDiv(900, gLogPixelsX, 96);
	gWindowPlacement[Windowplacement].length = sizeof(WINDOWPLACEMENT);
	gWindowPlacement[FindWindowplacement].rcNormalPosition.top = MulDiv(150, gLogPixelsY, 96);
	gWindowPlacement[FindWindowplacement].rcNormalPosition.left = MulDiv(150, gLogPixelsX, 96);
	gWindowPlacement[FindWindowplacement].length = sizeof(WINDOWPLACEMENT);
	gWindowPlacement[SysinfoWindowplacement].rcNormalPosition.top = MulDiv(40, gLogPixelsY, 96);
	gWindowPlacement[SysinfoWindowplacement].rcNormalPosition.left = MulDiv(40, gLogPixelsX, 96);
	gWindowPlacement[SysinfoWindowplacement].length = sizeof(WINDOWPLACEMENT);
	gWindowPlacement[PropWindowplacement].rcNormalPosition.top = MulDiv(40, gLogPixelsY, 96);
	gWindowPlacement[PropWindowplacement].rcNormalPosition.left = MulDiv(40, gLogPixelsX, 96);
	gWindowPlacement[PropWindowplacement].length = sizeof(WINDOWPLACEMENT);
	gWindowPlacement[DllPropWindowplacement].rcNormalPosition.top = MulDiv(40, gLogPixelsY, 96);
	gWindowPlacement[DllPropWindowplacement].rcNormalPosition.left = MulDiv(40, gLogPixelsX, 96);
	gWindowPlacement[DllPropWindowplacement].length = sizeof(WINDOWPLACEMENT);
	DWORD i;

	gdwProcessColumnCount = sizeof(gdwNumberOfProcessColumn) / sizeof(*gdwNumberOfProcessColumn);
	for (i = 0;i < gdwProcessColumnCount;i++)
	{
		gdwIDMapForSelectColumn[i] = LOWORD(gdwNumberOfProcessColumn[i]);
		gwProcessColumns[i] = MulDiv(HIWORD(gdwIDMapForSelectColumn[i]), gLogPixelsX, 96);
		ATLTRACE("Map: %x, Column: %x\n", LOWORD(gdwNumberOfProcessColumn[i]), HIWORD(gdwNumberOfProcessColumn[i]));
	}
	gdwHandleColumnCount = sizeof(gdwNumberOfHandleColumnMap) / sizeof(*gdwNumberOfHandleColumnMap);
	for (i = 0; i < gdwHandleColumnCount;i++)
	{
		gdwHandleColumnMap[i] = LOWORD(gdwNumberOfHandleColumnMap[i]);
		HandleColumns[i] = MulDiv(HIWORD(gdwNumberOfHandleColumnMap[i]), gLogPixelsX, 96);
		ATLTRACE("Map: %x, Column: %x\n", LOWORD(gdwNumberOfHandleColumnMap[i]), HIWORD(gdwNumberOfHandleColumnMap[i]));
	}
	gdwDllColumnCount = sizeof(gdwNumberODllColumnMap) / sizeof(*gdwNumberODllColumnMap);
	for (i = 0; i < gdwDllColumnCount;i++)
	{
		gpDllColumnMap[i] = LOWORD(gdwNumberODllColumnMap[i]);
		DllColumns[i] = MulDiv(HIWORD(gdwNumberODllColumnMap[i]), gLogPixelsX, 96);
		ATLTRACE("Map: %x, Column: %x\n", LOWORD(gdwNumberODllColumnMap[i]), HIWORD(gdwNumberODllColumnMap[i]));
	}

	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	lf.lfWeight = FW_NORMAL;
	lf.lfWidth = 0;
	lf.lfHeight = -MulDiv(8, gLogPixelsY, 72);
	memcpy(&gUnicodeFont, &lf, sizeof(LOGFONT));

	str.m_strName = _tcsdup(gszMainProgramRegKeyName);

	LoadGlobalVariablesFromRegistry(&str, gGlobalVariables);

	if (gUnicodeFont.lfHeight > 0)
	{
		gUnicodeFont.lfHeight = -MulDiv(gUnicodeFont.lfHeight, gLogPixelsY, 72);
	}
	//	WinStationGetProcessSid;
	//VARIANT vtServerName, vtUser, vtDomain, vtPassword;
	//ITaskService* pTaskService;
	//pTaskService->Connect(vtServerName, vtUser, vtDomain, vtPassword);
	TCHAR szSettings[892] = { 0 };
	//W223;

	////从注册表加载以前保存的Columnset信息
	CProcExpReg Reg;
	if (Reg.Open())
	{

	}

	i = 0;
	while (i < gdwProcessColumnCount)
	{
		if (gdwIDMapForSelectColumn[i] == 3)
		{
			memmove(&gdwIDMapForSelectColumn[i], &gdwIDMapForSelectColumn[i + 1], sizeof(DWORD)*(gdwProcessColumnCount - i));
			memmove(&gwProcessColumns[i], &gwProcessColumns[i + 1], sizeof(WORD)*(gdwProcessColumnCount - i));
			gdwProcessColumnCount--;
		}
		i++;
	}
	if (gdwProcessColumnCount > 0)
	{

	}

	i = 0;
	if (gdwNumColumnsets > 0)
	{
		do
		{
			//sub_446F30(0, i++, gstrMenuCulumnSet[i]);
			i++;
		} while (i < gdwNumColumnsets);
	}
	///加载系统调试符号路径信息
	if (!gszSymbolPath[0])
		GetEnvironmentVariable(TEXT("_NT_SYMBOL_PATH"), gszSymbolPath, _MAX_PATH);

	///初始化字体信息
	ghDefaultUnicodeFont = CreateFontIndirect(&gUnicodeFont);
	memcpy(&lf, &gUnicodeFont, sizeof(lf));
	lf.lfUnderline = TRUE;
	ghDefaultFontUnderline = CreateFontIndirect(&lf);
}

BOOL SearchDlls(HWND hWndListCtrl, DWORD dwProcessId, BOOL *pbCancelFindInFindDlg)
{
	return	FALSE;
}

///插入一行数据到DLL列表中
BOOL InsertRowToDllList(HWND hWnd, HANDLESANDDLLSLISTUSERPARAM* pUserParam)
{
	TCHAR szText[256] = { 0 };
	LVITEM item = { 0 };
	item.iItem = -1;	//最后一行
	item.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
	item.pszText = pUserParam->ImgName;
	item.cchTextMax = _tcslen(pUserParam->ImgName) + 1;
	HANDLESANDDLLSLISTUSERPARAM* p = new HANDLESANDDLLSLISTUSERPARAM();
	item.lParam = (LPARAM)p;
	CopyMemory(p, pUserParam, sizeof(HANDLESANDDLLSLISTUSERPARAM));
	int nItem = ListView_InsertItem(hWnd, &item);
	if (nItem == -1)
		return FALSE;
	_stprintf_s(szText, 256, _TEXT("%d"), pUserParam->ProcessId);
	ListView_SetItemText(hWnd, nItem, 1, szText);
	ListView_SetItemText(hWnd, nItem, 2, pUserParam->szText);
	ListView_SetItemText(hWnd, nItem, 3, pUserParam->szFileName);

	SendMessage(GetParent(hWnd), FDM_STARTSORT, 0, 0);
	return TRUE;
}

BOOL UpdateSysIconInMenu(HMENU hMenu, UINT nId)
{
	SHSTOCKICONINFO ssii = { sizeof(ssii) };
	HICON hIcon = ghSheildIcon;
	if (!ghSheildIcon)
	{
		SHGetStockIconInfo(SIID_SHIELD, SHGSI_ICONLOCATION|SHGSI_ICON| SHGFI_SMALLICON, &ssii);
		ghSheildIcon = ssii.hIcon;
	}
	MENUITEMINFO mii = { sizeof(mii) };
	mii.fMask = MIIM_DATA | MIIM_BITMAP;
	mii.hbmpItem = (HBITMAP)-1;
	mii.dwItemData = (DWORD)ghSheildIcon;
	return SetMenuItemInfo(hMenu,nId,FALSE,&mii);
}

void OnAlwaysOnTop(HWND hWnd)
{
	gbAlwaysOntop = gbAlwaysOntop == false;
	CheckMenuItem(GetMenu(hWnd), IDM_OPTIONS_ALWAYS_ON_TOP, gbAlwaysOntop ? MF_CHECKED : MF_UNCHECKED);
	if (gbAlwaysOntop)
	{
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE| SWP_NOSIZE);
		if (ghSysinfoPropsheetDlg)
		{
			SetWindowPos(ghSysinfoPropsheetDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
	}
	else
	{
		if (ghSysinfoPropsheetDlg)
		{
			SetWindowPos(ghSysinfoPropsheetDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

BOOL Init3DStatistics(ULONG64* a, ULONG64* b)
{
	return	FALSE;
}

///启动进程属性
void StartProcessPropertiesThread(void* pInfo)
{
	TREELISTITEMPARAM* pTreeListItemParam
		= (TREELISTITEMPARAM*)pInfo;

	InterlockedIncrement(&gnLock);

}

///从指定的地址提取出字符串
BOOL GetStringFromMemory(HWND hList, DWORD dwSize, PTCHAR pBuffer, DWORD dwLength)
{
	char szTextA[65536] = { 0 };
	TCHAR szText[65536] = { 0 };

	PTCHAR pbuffer = pBuffer;
	PTCHAR pBuf = pBuffer;
	LVITEM item = { 0 };

	item.mask = LVIF_PARAM;
	BOOL bPrintable = FALSE;
	DWORD nSize = dwSize;
	if (dwLength >= dwSize)
	{
		int iCharStart = -1;
		int iChar = 0;
		if (dwLength)
		{
			while (1)
			{
				TCHAR chBuffer = pBuffer[iChar];
				bPrintable = _ismbcprint(chBuffer);
				if (!bPrintable && chBuffer != '~')
				{
					//
					if (iCharStart != (-1))
					{
						if (iChar - iCharStart >= sizeof(TCHAR) * dwSize)
						{
							int nTarget = 0;
							int nIndex = 0;
							if (iCharStart < iChar)
							{
								DWORD dwLeft = (dwLength - dwSize + 1);
								do
								{
									if (iCharStart >= dwLeft)
										break;
									if (iswalpha(pBuffer[iCharStart]))
									{
										nIndex++;
									}
									szText[sizeof(TCHAR) * nTarget++] = pBuffer[iCharStart];

									iCharStart += sizeof(TCHAR);
								} while (iCharStart < iChar);
							}
							DWORD nTextLen = sizeof(TCHAR) * nTarget;
							////if (nTextLen >= 0x20000)
							////{
							////__ERRORREPORT:
							////	//__report_rangecheckfailure();
							////	//JUMPOUT(*algn_12F2FF0);
							////}
							bPrintable = FALSE;
							szText[nTextLen] = 0;
							if (nIndex >= dwSize)
							{
								item.pszText = szText;
								item.mask = LVIF_TEXT | LVIF_PARAM;
								item.iItem = 0x7FFFFFFF;
								item.lParam = 1;
								bPrintable = ListView_InsertItem(hList, &item);
							}
						}
						iCharStart = -1;
					}
					goto __nextchar;
				}
				if (chBuffer == ' ')
				{
					if (iCharStart == -1)
						goto __nextchar;
				}
				else if (iCharStart == -1)
				{
					iCharStart = iChar;
				}
				++iChar;
__nextchar:
				++iChar;
				if (iChar >= dwLength)
				{
					nSize = dwSize;
					break;
				}
			}
		}


		//v18 = -1;
		//v19 = 0;
		//if (dwLengh)
		//{
		//	do
		//	{
		//		LOBYTE(bPrintable) = pbuffer[v19];
		//		if ((bPrintable < ' ' || bPrintable >= '~') && bPrintable != '~')
		//		{
		//			if (v18 != -1)
		//			{
		//				LOBYTE(bPrintable) = v19 - v18;
		//				if (v19 - v18 >= nSize)
		//				{
		//					v20 = 0;
		//					v21 = 0;
		//					v31 = 0;
		//					if (v18 < v19)
		//					{
		//						v22 = dwLength - nSize + 1;
		//						v27 = v22;
		//						bPrintable = &szTextA[-v18];
		//						v28 = &szTextA[-v18];
		//						do
		//						{
		//							if (v18 >= v22)
		//								break;
		//							v23 = isalpha(pBuf[v18]);
		//							v20 = v31;
		//							if (v23)
		//								v20 = v31++ + 1;
		//							++v21;
		//							LOBYTE(bPrintable) = pBuf[v18];
		//							v28[v18++] = bPrintable;
		//							v22 = v27;
		//						} while (v18 < v19);
		//						if (v21 >= 0x10000)
		//							goto LABEL_51;
		//						nSize = dwSize;
		//					}
		//					szTextA[v21] = 0;
		//					if (v20 >= nSize)
		//					{
		//						CString::LoadString(szTextA);
		//						item.pszText = str;
		//						item.mask = 5;
		//						item.iItem = 0x7FFFFFFF;
		//						szTextA[1024] = 0;
		//						item.lParam = 1;
		//						SendMessageW(hWnd, LVM_INSERTITEMW, 0, &item);
		//						strData = &str[-4];
		//						bPrintable = &str[-1];
		//						if (_InterlockedDecrement(bPrintable) <= 0)
		//							LOBYTE(bPrintable) = (strData->pStringMgr->vtptr->Free)(strData);
		//						nSize = dwSize;
		//					}
		//					pbuffer = pBuf;
		//					dwLen = dwLength;
		//				}
		//				v18 = -1;
		//			}
		//		}
		//		else if (bPrintable != ' ' && v18 == -1)
		//		{
		//			v18 = v19;
		//		}
		//		++v19;
		//	} while (v19 < dwLen);
		//}
	}
	return TRUE;
}

TCHAR* envVarStrings[] =
{
	TEXT("OS         = %OS%"),
	TEXT("PATH       = %PATH%"),
	TEXT("HOMEPATH   = %HOMEPATH%"),
	TEXT("TEMP       = %TEMP%")
};
#define  ENV_VAR_STRING_COUNT  (sizeof(envVarStrings)/sizeof(TCHAR*))
#define INFO_BUFFER_SIZE 32767
void printError(TCHAR* msg);

void TestD()
{
	DWORD i;
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	// Get and display the name of the computer. 
	bufCharCount = INFO_BUFFER_SIZE;
	if (!GetComputerName(infoBuf, &bufCharCount))
		printError(TEXT("GetComputerName"));
	_tprintf(TEXT("\nComputer name:      %s"), infoBuf);

	// Get and display the user name. 
	bufCharCount = INFO_BUFFER_SIZE;
	if (!GetUserName(infoBuf, &bufCharCount))
		printError(TEXT("GetUserName"));
	_tprintf(TEXT("\nUser name:          %s"), infoBuf);

	// Get and display the system directory. 
	if (!GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE))
		printError(TEXT("GetSystemDirectory"));
	_tprintf(TEXT("\nSystem Directory:   %s"), infoBuf);

	// Get and display the Windows directory. 
	if (!GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE))
		printError(TEXT("GetWindowsDirectory"));
	_tprintf(TEXT("\nWindows Directory:  %s"), infoBuf);

	// Expand and display a few environment variables. 
	_tprintf(TEXT("\n\nSmall selection of Environment Variables:"));
	for (i = 0; i < ENV_VAR_STRING_COUNT; ++i)
	{
		bufCharCount = ExpandEnvironmentStrings(envVarStrings[i], infoBuf,
			INFO_BUFFER_SIZE);
		if (bufCharCount > INFO_BUFFER_SIZE)
			_tprintf(TEXT("\n\t(Buffer too small to expand: \"%s\")"),
				envVarStrings[i]);
		else if (!bufCharCount)
			printError(TEXT("ExpandEnvironmentStrings"));
		else
			_tprintf(TEXT("\n   %s"), infoBuf);
	}
	_tprintf(TEXT("\n\n"));
}

void printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n\t%s failed with error %d (%s)"), msg, eNum, sysMsg);
}

PSID InitializeSidFromText(LPCTSTR lpszText)
{
	TCHAR szText[1024] = { 0 };
	SID_IDENTIFIER_AUTHORITY IdentityAuth = { 0 };
	int nSubAuthority[8] = { 0 };

	_tcscpy_s(szText, 1024, lpszText);
	TCHAR* psz = _tcschr(szText, _TEXT('-'));
	if (!psz)
		return NULL;
	psz++;
	psz = _tcschr(psz, _TEXT('-'));
	if (!psz)
		return FALSE;
	psz++;
	TCHAR* pszText = psz + 1;

	if (!psz)
		return NULL;
	*psz = 0;	///强行截断

	if (*pszText != _TEXT('0') || *(pszText + 1) != _TEXT('x'))
	{
		int nValue = 0;
		_tscanf_s(pszText, 1024,  _TEXT("%lu"), &nValue);
		*(int*)(&IdentityAuth.Value[2]) = nValue;
	}
	else
	{
		_tscanf_s(pszText, 1024, _TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
			&IdentityAuth.Value[0], &IdentityAuth.Value[1],
			&IdentityAuth.Value[2], &IdentityAuth.Value[3],
			&IdentityAuth.Value[4], &IdentityAuth.Value[5]);
	}
	*psz = _TEXT('-');	///恢复到原来的状态

	pszText = psz + 2;
	int index = 0;
	BYTE byCount = 0;
	do
	{
		pszText = _tcschr(pszText, _TEXT('-'));
		if (!pszText)
			break;
		++index;
		*pszText = 0;
		byCount++;
		pszText++;
	} while (index < 8);


	if (byCount > 0)
	{
		index = byCount;
		do
		{
			_tscanf_s(pszText, 1024, _TEXT("%lu"), &nSubAuthority[index]);
			pszText += (_tcslen(pszText) + 1);
			--index;
		} while (index);
	}
	PSID pSID = NULL;

	BOOL ret= AllocateAndInitializeSid(&IdentityAuth,
		byCount,
		nSubAuthority[0],
		nSubAuthority[1],
		nSubAuthority[2],
		nSubAuthority[3],
		nSubAuthority[4],
		nSubAuthority[5],
		nSubAuthority[6],
		nSubAuthority[7],
		&pSID);

	return ret ? pSID : NULL;
}

typedef BOOL (__stdcall* LPFN_QueryServiceConfig2W)(SC_HANDLE hService, DWORD dwInfoLevel, LPBYTE lpBuffer, DWORD cbBufSize, LPDWORD pcbBytesNeeded);
typedef BOOL (__stdcall* LPFN_EnumServicesStatusExW)(SC_HANDLE hSCManager, SC_ENUM_TYPE InfoLevel, DWORD dwServiceType, DWORD dwServiceState, LPBYTE lpServices, DWORD cbBufSize, LPDWORD pcbBytesNeeded, LPDWORD lpServicesReturned, LPDWORD lpResumeHandle, LPCWSTR pszGroupName);

LPFN_EnumServicesStatusExW gpfnEnumServicesStatusExW = NULL;
LPFN_QueryServiceConfig2W gpfnQueryServiceConfig2W = NULL;

BOOL InitServices()
{
	ghSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
	if (!ghSCManager)
		return	FALSE;
	gpfnEnumServicesStatusExW = (LPFN_EnumServicesStatusExW)GetProcAddress(LoadLibraryW(L"ADVAPI32"), "EnumServicesStatusExW");
	gpfnQueryServiceConfig2W = (LPFN_QueryServiceConfig2W)GetProcAddress(LoadLibraryW(L"ADVAPI32"), "QueryServiceConfig2W");
	return	TRUE;
}
////获取所有的服务的名称
void GetAllServicesName()
{
	DWORD cbSized = 0L;
	DWORD dwHandle = 0L;
	if (gpfnEnumServicesStatusExW)
	{
		TAutoLock<CLock> lock(gEnumServiceLock);
		BOOL ret = (*gpfnEnumServicesStatusExW)(ghSCManager,
			SC_ENUM_PROCESS_INFO,
			SERVICE_WIN32,
			SERVICE_ACTIVE,
			(LPBYTE)gstrServiceNameBuffer,
			gdwServiceNameBufferSize,
			&cbSized,
			&gdwServiceNameRealSize,
			&dwHandle,
			NULL);
		if (!ret)
		{
			do
			{
				if (ERROR_MORE_DATA != GetLastError())
					break;
				if (gstrServiceNameBuffer)
				{
					free(gstrServiceNameBuffer);
				}
				gdwServiceNameBufferSize += 16384;

				gstrServiceNameBuffer = (TCHAR*)malloc(gdwServiceNameBufferSize);

				ret = (*gpfnEnumServicesStatusExW)(ghSCManager,
					SC_ENUM_PROCESS_INFO,
					SERVICE_WIN32,
					SERVICE_ACTIVE,
					(LPBYTE)gstrServiceNameBuffer,
					gdwServiceNameBufferSize,
					&cbSized,
					&gdwServiceNameRealSize,
					&dwHandle,
					NULL);
			} while (!ret);
		}
		return;
	}
}