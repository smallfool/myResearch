#pragma once
class CQQInject
{
public:
	CQQInject();
	~CQQInject();

	static CQQInject& Instance()
	{
		static CQQInject theQQ;
		return theQQ;
	}
	// ��������QQ���Ŵ��ڵ����ش���
	BOOL Start();

private:
	bool m_Exiting;
	bool m_KillProcess;
	DWORD m_ThreadId;
	HANDLE m_ThreadHandle;
	static DWORD WINAPI ThreadProc(void* p);
	void Run();
	bool IsExit() { return m_Exiting; }
	BOOL KillProcess(DWORD dwPid);
};

