#include "stdafx.h"
#include "..\include\FileObj.h"


CFileObj::CFileObj()
{
	this->m_Handle = NULL;
}


CFileObj::~CFileObj()
{
	this->Close();
}

BOOL CFileObj::Open(LPCTSTR lpszFileName, BOOL bReadOnly/*=TRUE*/)
{
	this->m_Handle = CreateFile(
		lpszFileName,
		bReadOnly ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE),
		bReadOnly ? FILE_SHARE_READ | FILE_SHARE_WRITE : (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE),
		NULL,
		bReadOnly ? OPEN_EXISTING : (CREATE_NEW | CREATE_ALWAYS),
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_Handle == NULL || m_Handle == INVALID_HANDLE_VALUE)
		return FALSE;
	return TRUE;
}

BOOL CFileObj::Read(void* pBuffer, DWORD dwSize)
{
	if (dwSize == -1)
		dwSize = GetSize();
	DWORD dwRet = 0L;
	BOOL ret = ::ReadFile(m_Handle, pBuffer, dwSize, &dwRet,NULL);
	if (!ret || dwRet != dwSize)
		return	FALSE;
	return	TRUE;
}

void CFileObj::Close()
{
	if (m_Handle != INVALID_HANDLE_VALUE || m_Handle != NULL)
	{
		CloseHandle(m_Handle);
		m_Handle = NULL;
	}
}

DWORD CFileObj::GetSize()
{
	return ::GetFileSize(m_Handle, NULL);
}