#pragma once

struct ERRMSG
{
	DWORD	dwError;
	CHAR*	szMsg;
};

extern CHAR* GetErrMsg(DWORD dwErr);
extern ATL::CString GetErrorText(DWORD dwError);


class CLog
{
public:
	CLog();
	~CLog();
};

//日志钩子函数
extern LPFN_DbgTraceFunction g_DbgTraceFunction;
//日志输出函数
void DbgPrintMessage(int TraceLevel, const char *lpszFormat, ...);
//日志深度配置
extern UINT g_DebugSpewTraceLevel;
//输出前缀信息
extern CHAR* _TRACE_PREFIX;
//输出内容
extern CHAR g_DbgTrace[10240];
extern LONG g_DbgTracePos;
extern int g_bDebugSpew;