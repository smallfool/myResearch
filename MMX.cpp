#include "stdafx.h"
#include "../include/Common.h"
#include "../ProcExplorer/ProcExplorer.h"

struct MMXBuffer
{
	union
	{
		__m128i Buf[24];
		char	sBuf[384];
		int		nBuf[96];
	};
};

////
////请注意pFixedBuffer缓冲区的长度一定要在nCount大。
///
int InitMMXBuffer(__m128i* pFixedBuffer,int nCount, const __m128i* Value)
{
	////计算数组实际数据
	__m128i  __v1 = _mm_load_si128(Value);//GetListCtrlColumnMagic());
	__m128i* __Id128 = &pFixedBuffer[1];
	int idx = 0;
	do
	{
		__Id128 += 2;
		_mm_storeu_si128(__Id128 - 3, _mm_add_epi32(_mm_shuffle_epi32(_mm_cvtsi32_si128(idx), 0), __v1));
		_mm_storeu_si128(__Id128 - 2, _mm_add_epi32(_mm_shuffle_epi32(_mm_cvtsi32_si128(idx + 4), 0), __v1));
		idx += 8;
	} while (idx < nCount);
	return	nCount;
}
