// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#include <winsock2.h>
// Windows 头文件: 
#include <windows.h>

#define USE_PE

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <atlbase.h>
#include <atlstr.h>
#include <atlcomcli.h>	//CComVariant
#include <comutil.h>  //_bstr_t  ;
// TODO:  在此处引用程序需要的其他头文件
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
