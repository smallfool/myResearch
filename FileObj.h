#pragma once
class CFileObj
{
public:
	CFileObj();
	~CFileObj();

	BOOL Open(LPCTSTR lpszFileName, BOOL bReadOnly = TRUE);
	BOOL Save(LPCTSTR lpszFileName);
	BOOL Append(LPCTSTR lpszFileName, BOOL bAppendTail = TRUE);
	DWORD GetSize();
	void Close();
	BOOL Read(void* pBuffer, DWORD dwSize = -1);
protected:
	HANDLE m_Handle;
};

