#include "stdafx.h"
#include "..\include\Alloc.h"


CAlloc::CAlloc()
{
}


CAlloc::~CAlloc()
{
}

void* CAlloc::Allocate(DWORD dwSize, bool bZero)
{
	void* p = ::malloc(dwSize);
	if (bZero)
	{
		ZeroMemory(p, dwSize);
	}
	return p;
}
void CAlloc::Free(void* pBuffer)
{
	::free(pBuffer);
}

DWORD CAlloc::GetSize(void* pBuffer)
{
	return 0L;
}

CGlobalAlloc::CGlobalAlloc()
{
	m_Global = NULL;
}

CGlobalAlloc::~CGlobalAlloc()
{
	if (m_Global)
	{
		GlobalUnlock(m_Global);
		GlobalFree(m_Global);
		m_Global = NULL;
	}
}

void* CGlobalAlloc::Allocate(DWORD dwSize, bool bZeroMemory/* = FALSE*/)
{
	m_Global = ::GlobalAlloc(bZeroMemory ? GMEM_FIXED | GMEM_ZEROINIT:GMEM_FIXED, dwSize);
	if (!m_Global)
	{
		AtlThrow(ERROR_OUTOFMEMORY);
	}
	return ::GlobalLock(m_Global);
}

void CGlobalAlloc::Free(void* pBuffer)
{
	if (m_Global)
	{
		GlobalUnlock(m_Global);
		GlobalFree(m_Global);
		m_Global = NULL;
	}
	pBuffer = NULL;
}

DWORD CGlobalAlloc::GetSize(void* pBuffer)
{
	return ::GlobalSize((HGLOBAL)pBuffer);
}

///
CHeapAlloc::CHeapAlloc()
{
	m_Heap = ::GetProcessHeap();
}

CHeapAlloc::~CHeapAlloc()
{

}

void* CHeapAlloc::Allocate(DWORD dwSize, bool bZeroMemory/* = FALSE*/)
{
	return ::HeapAlloc(m_Heap, bZeroMemory ? HEAP_ZERO_MEMORY : 0, dwSize);
}

void CHeapAlloc::Free(void* pBuffer)
{
	HeapFree(m_Heap, 0, pBuffer);
}

DWORD CHeapAlloc::GetSize(void* pBuffer)
{
	return ::HeapSize(m_Heap, 0, pBuffer);
}

CVirtualAlloc::CVirtualAlloc()
{

}

CVirtualAlloc::~CVirtualAlloc()
{

}

void* CVirtualAlloc::Allocate(void* pBase, DWORD dwSize, bool bZero)
{
	void* res = ::VirtualAlloc(pBase, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (res == NULL || ERROR_INVALID_ADDRESS == __ERROR__)
	{
		return ::VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}
	return	res;
}


void* CVirtualAlloc::Allocate(DWORD dwSize, bool bZeroMemory/*=FALSE*/)
{
	return  ::VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void CVirtualAlloc::Free(void* pBuffer)
{
	::VirtualFree(pBuffer, 0, MEM_RELEASE);
}

DWORD CVirtualAlloc::GetSize(void* pBuffer)
{
	return 0L;
}

CLocalAlloc::CLocalAlloc()
{

}

CLocalAlloc::~CLocalAlloc()
{

}

void* CLocalAlloc::Allocate(DWORD dwSize, bool bZero)
{
	return ::LocalAlloc(bZero ? LPTR : LMEM_FIXED, dwSize);
}
void CLocalAlloc::Free(void* pBuffer)
{
	::LocalFree(pBuffer);
}

DWORD CLocalAlloc::GetSize(void* pBuffer)
{
	return ::LocalSize((HLOCAL)pBuffer);
}