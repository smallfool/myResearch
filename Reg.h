#pragma once
class CReg
{
public:
	CReg();
	~CReg();

	virtual ATL::CString GetKeyName() { return _TEXT(""); }

	int GetValue(void*& pBuffer, DWORD& dwLength, LPCTSTR lpszKeyName);

	ULONG GetValueLength(LPCTSTR lpszKeyName);
	int Open();
	int Open(HKEY hKey, LPCTSTR lpszKeyName, BOOL bReadOnly);
	void Close();
	ATL::CString GetStrValue(LPCTSTR lpszValueName);
protected:
	HKEY m_Key;
public:
	// 从注册表中读取一组整形数据到内存中来
	LONG ReadInts(LPCTSTR lpszKeyName, int nSize, LPBYTE lpData);
	// 从注册表中读取一个整形数据
	BOOL ReadInt(LPCTSTR lpszName, int* pnValue, int nDefValue = 0);
	//	从注册表中读一个整形宽度数据
	int ReadWidth(LPCTSTR lpszName, int* pnWidth, int nDefValue = 0);
	//	从注册表中读一组整形宽度数据
	int ReadWidths(LPCTSTR lpszName,int nSize, int* pnWidths);

};

class CProcExpReg : public CReg
{
public:
	CProcExpReg();
	virtual ATL::CString GetKeyName() {
		return m_KeyName;
	}

public:
	ATL::CString m_KeyName;
};

