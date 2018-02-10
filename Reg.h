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
	// ��ע����ж�ȡһ���������ݵ��ڴ�����
	LONG ReadInts(LPCTSTR lpszKeyName, int nSize, LPBYTE lpData);
	// ��ע����ж�ȡһ����������
	BOOL ReadInt(LPCTSTR lpszName, int* pnValue, int nDefValue = 0);
	//	��ע����ж�һ�����ο������
	int ReadWidth(LPCTSTR lpszName, int* pnWidth, int nDefValue = 0);
	//	��ע����ж�һ�����ο������
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

