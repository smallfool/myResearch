#include "stdafx.h"
#include "../ProcExplorer/ProcExplorer.h"

const TCHAR *CPtoLocaleName(int uACP)
{
	//switch (uACP)
	//{
	//case CP_932:
	//	return _TEXT("ja-JP");
	//case CP_936:
	//	return _TEXT("zh-CN");
	//case CP_949:
	//	return _TEXT("ko-KR");
	//case CP_950:
	//	return _TEXT("zh-TW");
	//}
	return _TEXT("");
}



LPFN_WinVerifyTrust    gpfnWinVerifyTrust = NULL;
LPFN_WTHelperGetProvSignerFromChain    gpfnWTHelperGetProvSignerFromChain = NULL;
LPFN_WTHelperProvDataFromStateData    gpfnWTHelperProvDataFromStateData = NULL;
LPFN_CryptCATAdminReleaseContext    gpfnCryptCATAdminReleaseContext = NULL;
LPFN_CryptCATAdminReleaseCatalogContext    gpfnCryptCATAdminReleaseCatalogContext = NULL;
LPFN_CryptCATCatalogInfoFromContext    gpfnCryptCATCatalogInfoFromContext = NULL;
LPFN_CryptCATAdminEnumCatalogFromHash    gpfnCryptCATAdminEnumCatalogFromHash = NULL;
LPFN_CryptCATAdminCalcHashFromFileHandle    gpfnCryptCATAdminCalcHashFromFileHandle = NULL;
LPFN_CryptCATAdminAcquireContext    gpfnCryptCATAdminAcquireContext = NULL;
LPFN_CertNameToStrW    gpfnCertNameToStrW = NULL;
LPFN_CryptCATAdminAddCatalog    gpfnCryptCATAdminAddCatalog = NULL;
LPFN_CryptCATAdminRemoveCatalog    gpfnCryptCATAdminRemoveCatalog = NULL;
LPFN_IsCatalogFile    gpfnIsCatalogFile = NULL;


bool LoadWintrust()
{
	HMODULE hModule; // edi
	static bool sbinited = false;
	if (!sbinited)
	{
		sbinited = 1;
		hModule = LoadLibraryW(L"Wintrust.dll");
		if (!hModule)
			return 0;
		gpfnWinVerifyTrust = (LPFN_WinVerifyTrust)GetProcAddress(hModule, "WinVerifyTrust");
		gpfnWTHelperGetProvSignerFromChain = (LPFN_WTHelperGetProvSignerFromChain)GetProcAddress(hModule, "WTHelperGetProvSignerFromChain");
		gpfnWTHelperProvDataFromStateData = (LPFN_WTHelperProvDataFromStateData)GetProcAddress(hModule, "WTHelperProvDataFromStateData");
		gpfnCryptCATAdminReleaseContext = (LPFN_CryptCATAdminReleaseContext)GetProcAddress(hModule, "CryptCATAdminReleaseContext");
		gpfnCryptCATAdminReleaseCatalogContext = (LPFN_CryptCATAdminReleaseCatalogContext)GetProcAddress(hModule, "CryptCATAdminReleaseCatalogContext");
		gpfnCryptCATCatalogInfoFromContext = (LPFN_CryptCATCatalogInfoFromContext)GetProcAddress(hModule, "CryptCATCatalogInfoFromContext");
		gpfnCryptCATAdminEnumCatalogFromHash = (LPFN_CryptCATAdminEnumCatalogFromHash)GetProcAddress(hModule, "CryptCATAdminEnumCatalogFromHash");
		gpfnCryptCATAdminCalcHashFromFileHandle = (LPFN_CryptCATAdminCalcHashFromFileHandle)GetProcAddress(hModule, "CryptCATAdminCalcHashFromFileHandle");
		gpfnCryptCATAdminAcquireContext = (LPFN_CryptCATAdminAcquireContext)GetProcAddress(hModule, "CryptCATAdminAcquireContext");
		gpfnCertNameToStrW = (LPFN_CertNameToStrW)GetProcAddress(hModule, "CertNameToStrW");
		gpfnCryptCATAdminAddCatalog = (LPFN_CryptCATAdminAddCatalog)GetProcAddress(hModule, "CryptCATAdminAddCatalog");
		gpfnCryptCATAdminRemoveCatalog = (LPFN_CryptCATAdminRemoveCatalog)GetProcAddress(hModule, "CryptCATAdminRemoveCatalog");
		hModule = LoadLibraryW(L"crypt32.dll");
		gpfnIsCatalogFile = (LPFN_IsCatalogFile)GetProcAddress(hModule, "IsCatalogFile");


	}
	return gpfnCryptCATAdminAcquireContext != 0;
}

//NTSTATUS __stdcall AnsiToUnicode(PWCH UnicodeString, 
//	ULONG MaxBytesInUnicodeString, ULONG BytesInUnicodeString)
//{
//	return RtlMultiByteToUnicodeN(
//		UnicodeString,
//		MaxBytesInUnicodeString,
//		&BytesInUnicodeString,
//		(const CHAR *)BytesInUnicodeString,
//		strlen((const char *)BytesInUnicodeString) + 1);
//}
//
//
//
//
//int __stdcall ConvertHRESULT2WIN32(int hres)
//{
//	int result; // eax
//	unsigned int res; // ecx
//
//	result = hres;
//	res = (unsigned __int16)hres;
//	if ((signed int)(unsigned __int16)hres > 0)
//		res = (unsigned __int16)hres | 0x80070000;
//	if (res == hres)
//		return (unsigned __int16)hres;
//	if ((hres | 0x10000000) == hres)
//		return RtlNtStatusToDosError(hres & 0xEFFFFFFF);
//	switch (hres)
//	{
//	case E_NOTIMPL:
//		return ERROR_CALL_NOT_IMPLEMENTED;
//	case E_NOINTERFACE:
//		return ERROR_INVALID_FUNCTION;
//	case E_UNEXPECTED:
//		return ERROR_INVALID_DATA;
//	}
//	if (hres >= 0)
//		result = NO_ERROR;
//	return result;
//}
//
//BOOL __stdcall ConvertUnicodeToAnsi(char **ppszText, LPCWSTR lpString)
//{
//	int len; // eax
//	NTSTATUS status; // eax
//	char *buf; // eax
//	DWORD err; // eax
//	ULONG ResultSize; // [esp+8h] [ebp-8h]
//	ULONG nSizeNeeded; // [esp+Ch] [ebp-4h]
//
//	len = lstrlenW(lpString);
//	status = RtlUnicodeToMultiByteSize(&nSizeNeeded, lpString, 2 * len);
//	if (status)
//	{
//		err = RtlNtStatusToDosError(status);
//		SetLastError(err);
//		return FALSE;
//	}
//	if (nSizeNeeded + 1 > nSizeNeeded)
//	{
//		buf = (char *)LocalAlloc(64, ++nSizeNeeded);
//		*ppszText = buf;
//		if (!buf)
//		{
//			SetLastError(ERROR_OUTOFMEMORY);
//			return FALSE;
//		}
//		status = RtlUnicodeToMultiByteN(buf, nSizeNeeded,
//			&ResultSize, lpString, len*2);
//		if (!status)
//			return TRUE;
//		LocalFree(*ppszText);
//		*ppszText = NULL;
//		err = RtlNtStatusToDosError(status);
//		SetLastError(err);
//		return FALSE;
//	}
//	SetLastError(ERROR_BUFFER_OVERFLOW);
//	return FALSE;
//}
__m128i  xmmword_4CEA50;// = { 0,0,0,0 };//(__m128i) 30000000 20000000 10000000 0;
void MMXtest()
{
	xmmword_4CEA50.m128i_i32[0] = 0;
	xmmword_4CEA50.m128i_i32[1] = 0x10000000;
	xmmword_4CEA50.m128i_i32[2] = 0x20000000;
	xmmword_4CEA50.m128i_i32[3] = 0x30000000;
	char Buffer[368];
	__m128i v7; // xmm1
	char *pBuffer; // edx
	unsigned int idx; // ecx
	unsigned int v10; // eax
	__m128i v11; // xmm0

	// 对一个128位双精度浮点数进行某种操作
	v7 = _mm_load_si128(&xmmword_4CEA50);
	pBuffer = Buffer;
	idx = 0;
	do
	{
		v10 = idx + 4;
		pBuffer += 32;
		v11 = _mm_add_epi32(_mm_shuffle_epi32(_mm_cvtsi32_si128(idx), 0), v7);
		idx += 8;
		_mm_storeu_si128((__m128i *)(pBuffer - 3), v11);
		_mm_storeu_si128((__m128i *)(pBuffer - 2), _mm_add_epi32(_mm_shuffle_epi32(_mm_cvtsi32_si128(v10), 0), v7));
	} while (idx < 96);
}

