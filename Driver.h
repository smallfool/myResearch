#pragma once

enum EDriverCmd
{
	CMD_VerifyVersion = 0x83350008,
	CMD_OpenThread = 0x8335003C,
};

enum EDriverID
{
	EDriverID_WIN32 = 150,
	EDriverID_WIN64 = 152,
};

class CDriver
{
public:
	CDriver();
	~CDriver();
	BOOL IsValid() { return m_Driver != INVALID_HANDLE_VALUE; }
	operator HANDLE() { return m_Driver; }
	HANDLE GetHandle() { return m_Driver;  }

	BOOL Close();
	BOOL Load();
	BOOL Open(LPCTSTR lpszName,HANDLE* phHandle);
	BOOL Open(LPCTSTR lpszName,LPCTSTR lpszPathName, HANDLE* phHandle);
	BOOL IsIllegalVersion(DWORD dwVersion);
	HANDLE OpenProcess(DWORD dwDesiredAccess, DWORD dwProcesId);
	BOOL LoadDriverFromRes(UINT nID, LPCTSTR lpszFileName);
public:
	HANDLE m_Driver;
};

extern BOOL TmAdjustPrivilege(LPCTSTR lpName);
extern CDriver theProcExpDriver;

extern LPFN_RtlNtStatusToDosError gpfnRtlNtStatusToDosError;
extern LPFN_RtlInitUnicodeString  gpfnRtlInitUnicodeString;
extern LPFN_NtLoadDriver gpfnNtLoadDriver;
extern HMODULE ghNTDLLModule;
