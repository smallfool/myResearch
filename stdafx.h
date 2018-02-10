// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

#include <winsock2.h>
// Windows ͷ�ļ�: 
#include <windows.h>

#define USE_PE

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <atlbase.h>
#include <atlstr.h>
#include <atlcomcli.h>	//CComVariant
#include <comutil.h>  //_bstr_t  ;
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <string>
#include <list>
#include <vector>
#include <map>
#include <deque>
using namespace std;

#include <xmmintrin.h>
#include "..\include\ntdefs.h"

//#include <Winternl.h>
#include <TlHelp32.h>  
#include <Psapi.h>
#include <Shellapi.h>
#include <dbgeng.h>
#include <dbghelp.h>
#include <WinCred.h>
#include <SetupAPI.h>
#include <VdmDbg.h>
#include <Uxtheme.h>
#include <CommCtrl.h>
#include <Taskschd.h>	
#include <WtsApi32.h>

#include <CorPub.h>
#include <Wintrust.h>
#include <Vsstyle.h>
#include <Mscat.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"CorGuids.lib")
#pragma comment(lib,"WtsApi32.lib")
#pragma comment(lib,"VdmDbg.lib")
#pragma comment(lib,"SetupAPI.lib")
#pragma comment(lib,"ntdll")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"dbgeng.lib")
#pragma comment(lib,"Shell32")
#pragma comment(lib,"UxTheme")
#pragma comment(lib,"Comctl32")
#pragma comment(lib,"Taskschd.lib")



#include "../include/Common.h"


#include "../include/Alloc.h"
#include "../include/AutoPtr.h"
#include "../include/FileObj.h"
#include "../include/Graph.h"
#include "../include/Lock.h"
#include "../include/PE.h"
#include "../include/Process.h"
#include "../include/Reg.h"
#include "../include/Res.h"
#include "../include/Str.h"
#include "../include/System.h"
#include "../include/TreeList.h"
#include "..\include\Security.h"
#include "..\include\Log.h"
