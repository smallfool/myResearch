#include "stdafx.h"
#include "..\include\Res.h"

#include "..\include\FileObj.h"
#include "../include/Alloc.h"

CRes::CRes()
{
	m_Buffer = NULL;
}


CRes::~CRes()
{
	if (m_Buffer != NULL)
	{
		TAlloc<CGlobalAlloc> alloc;
		alloc.Free(m_Buffer);
	}
}

BOOL CRes::Load(const void* pBuffer, UINT nLength)
{
	CoInitialize(NULL);
	IStream* pStream = NULL;
	HRESULT res = CreateStreamOnHGlobal((HGLOBAL)pBuffer, FALSE, &pStream);
	if (FAILED(res))
		return	FALSE;
	m_Stream.Set(pStream);
	return	TRUE;
}

BOOL CRes::Load(LPCTSTR lpszFileName)
{
	TAlloc<CGlobalAlloc> alloc;

	CFileObj file;
	if (!file.Open(lpszFileName))
	{
		return FALSE;
	}
	DWORD dwSize = file.GetSize();
	void* pBuffer = alloc.Allocate(dwSize);
	if (file.Read(pBuffer, dwSize))
	{
		alloc.Free(pBuffer);
		return	FALSE;
	}
	return	Load(pBuffer, dwSize);
}

BOOL CRes::Save(LPCTSTR lpszFileName)
{
	return	FALSE;
}

//////
/////////////////////////////////////////////////

CBmpRes::CBmpRes()
{

}

CBmpRes::~CBmpRes()
{

}

BOOL CBmpRes::Load(const void* pBuffer, UINT nLength)
{
	if (!CRes::Load(pBuffer, nLength))
		return	FALSE;
	IPicture* pPicture = NULL;
	HRESULT hr = OleLoadPicture(m_Stream.Get(), 0, FALSE, __uuidof(IPicture), (LPVOID*)&pPicture);//用leLoadPicture获得IPicture接口指针   
	if (FAILED(hr))
	{
		return	FALSE;
	}
	m_Picture.Set(pPicture);

	return	TRUE;
}
