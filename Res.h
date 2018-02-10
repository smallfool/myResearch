#pragma once
#include <OleCtl.h>
#pragma comment(lib, "OleAut32")

#include "AutoPtr.h"

enum EResType
{
	RS_CURSOR,
	RS_ICON,
	RS_BITMAP,
	RS_MENU,
	RS_DIALOG,
	RS_STRING,
	RS_VERSION,
	RS_MANIFEST,
	RS_HTML,
};

class CRes
{
public:
	CRes();
	virtual ~CRes();
	virtual int GetType()const = 0;

	virtual void Render(HDC hDC) {}
	virtual BOOL Load(const void* pBuffer, UINT nLength);
	virtual BOOL Load(LPCTSTR lpszFileName);
	virtual BOOL Save(LPCTSTR lpszFileName);
private:
	void* m_Buffer;
protected:
	TAutoRelease<IStream> m_Stream;
};

class CBmpRes :public CRes
{
public:
	CBmpRes();
	~CBmpRes();
	int GetType() { return RS_BITMAP; }
	virtual BOOL Load(const void* pBuffer, UINT nLength);
protected:
	TAutoRelease<IPicture> m_Picture;
};
