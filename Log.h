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

//��־���Ӻ���
extern LPFN_DbgTraceFunction g_DbgTraceFunction;
//��־�������
void DbgPrintMessage(int TraceLevel, const char *lpszFormat, ...);
//��־�������
extern UINT g_DebugSpewTraceLevel;
//���ǰ׺��Ϣ
extern CHAR* _TRACE_PREFIX;
//�������
extern CHAR g_DbgTrace[10240];
extern LONG g_DbgTracePos;
extern int g_bDebugSpew;