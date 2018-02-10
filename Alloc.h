#pragma once
class CAlloc
{
public:
	CAlloc();
	~CAlloc();
	void* Allocate(DWORD dwSize, bool bZero = FALSE);
	void Free(void* pBuffer);
	DWORD GetSize(void* pBuffer);
};

class CGlobalAlloc
{
public:
	CGlobalAlloc();
	~CGlobalAlloc();

	void* Allocate(DWORD dwSize, bool bZero = FALSE);
	void Free(void* pBuffer);
	DWORD GetSize(void* pBuffer);
private:
	HGLOBAL m_Global;
};

class CVirtualAlloc
{
public:
	CVirtualAlloc();
	~CVirtualAlloc();
	void* Allocate(DWORD dwSize, bool bZero = FALSE);
	void* Allocate(void* pBase, DWORD dwSize, bool bZero = FALSE);
	void Free(void* pBuffer);
	DWORD GetSize(void* pBuffer);
protected:

};

class CHeapAlloc
{
public:
	CHeapAlloc();
	~CHeapAlloc();

	void* Allocate(DWORD dwSize, bool bZero = FALSE);
	void Free(void* pBuffer);
	DWORD GetSize(void* pBuffer);
private:
	HANDLE m_Heap;
};

class CLocalAlloc
{
public:
	CLocalAlloc();
	~CLocalAlloc();
	void* Allocate(DWORD dwSize, bool bZero = FALSE);
	void Free(void* pBuffer);
	DWORD GetSize(void* pBuffer);
};

template<typename ALLOC>
class TAlloc : public ALLOC
{
public:
	TAlloc() {}
	~TAlloc() {}
	void* Allocate(void* pBase,DWORD dwSize, bool bZeroMemory = FALSE)
	{
		return ALLOC::Allocate(pBase, dwSize, bZeroMemory);
	}
	void* Allocate(DWORD dwSize, bool bZeroMemory = FALSE)
	{
		return ALLOC::Allocate(dwSize, bZeroMemory);
	}
	void Free(void* p)
	{
		ALLOC::Free(p);
	}
	DWORD GetSize(void *p)
	{
		return ALLOC::GetSize(p);
	}
};

//template<typename ALLOC>
//class TAutoFreeAlloc 
//{
//public:
//	TAutoFreeAlloc(void*& p)
//		:m_pBuffer(p)
//	{
//
//	}
//
//	~TAutoFreeAlloc()
//	{
//
//	}
//private:
//
//};

template<typename T>
class TRect 
{
public:
	TRect()
	{
		ZeroMemory(this, sizeof(*this));
	}
	TRect(TRect& r)
		:left(r.left)
		, top(r.top)
		, right(r.right)
		, bottom(r.bottom)
	{
	}

	TRect(T l, T t, T r, T b)
		:left(l), top(t), right(r), bottom(b)
	{

	}
	T Width()	{ return right -left; }
	T Height()	{ return bottom - top; }

	T left, top, right, bottom;
};

struct CRect : public TRect<LONG>
{
public:
	operator RECT() { return *this; }
	RECT* GetRect() { return (RECT*)this; }
	const RECT* GetRect() const { return (RECT*)this; }
};

//typedef TRect<double>	CRectd;