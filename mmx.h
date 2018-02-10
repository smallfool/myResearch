#pragma once


typedef union __declspec(intrin_type)_CRT_ALIGN(8) __m64
{
	unsigned __int64    m64_u64;
	float               m64_f32[2];
	__int8              m64_i8[8];
	__int16             m64_i16[4];
	__int32             m64_i32[2];
	__int64             m64_i64;
	unsigned __int8     m64_u8[8];
	unsigned __int16    m64_u16[4];
	unsigned __int32    m64_u32[2];
} __m64;

typedef union __declspec(intrin_type)_CRT_ALIGN(16) __m128 {
	float               m128_f32[4];
	unsigned __int64    m128_u64[2];
	__int8              m128_i8[16];
	__int16             m128_i16[8];
	__int32             m128_i32[4];
	__int64             m128_i64[2];
	unsigned __int8     m128_u8[16];
	unsigned __int16    m128_u16[8];
	unsigned __int32    m128_u32[4];
} __m128;

typedef union __declspec(intrin_type)_CRT_ALIGN(16) __m128i {
	__int8              m128i_i8[16];
	__int16             m128i_i16[8];
	__int32             m128i_i32[4];
	__int64             m128i_i64[2];
	unsigned __int8     m128i_u8[16];
	unsigned __int16    m128i_u16[8];
	unsigned __int32    m128i_u32[4];
	unsigned __int64    m128i_u64[2];
} __m128i;

typedef struct __declspec(intrin_type)_CRT_ALIGN(16) __m128d {
	double              m128d_f64[2];
} __m128d;


//����һ��__m128d�ļĴ�����r0=_A0+_B0, r1=_A1  
extern __m128d _mm_add_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0+_B0, r1=_A1+_B1  
extern __m128d _mm_add_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0-_B0, r1=_A1  
extern __m128d _mm_sub_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0-_B0, r1=_A1-_B1  
extern __m128d _mm_sub_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0*_B0, r1=_A1  
extern __m128d _mm_mul_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0*_B0, r1=_A1*_B1  
extern __m128d _mm_mul_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=sqrt(_B0), r1=_A1  
extern __m128d _mm_sqrt_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=sqrt(_A0), r1=sqrt(_A1)  
extern __m128d _mm_sqrt_pd(__m128d _A);
//����һ��__m128d�ļĴ�����r0=_A0/_B0, r1=_A1  
extern __m128d _mm_div_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0/_B0, r1=_A1/_B1  
extern __m128d _mm_div_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=min(_A0,_B0), r1=_A1  
extern __m128d _mm_min_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=min(_A0,_B0), r1=min(_A1,_B1)  
extern __m128d _mm_min_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=max(_A0,_B0), r1=_A1  
extern __m128d _mm_max_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=max(_A0,_B0), r1=max(_A1,_B1)  
extern __m128d _mm_max_pd(__m128d _A, __m128d _B);

//Logical Operations(Floating Point SSE2 Intrinsics):and��or��xor�� andnot  
//����һ��__m128d�ļĴ�����r0=_A0 & _B0, r1=_A1 & _B1  
extern __m128d _mm_and_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(~_A0) & _B0, r1=(~_A1) & _B1  
extern __m128d _mm_andnot_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0 | _B0, r1=_A1 | _B1  
extern __m128d _mm_or_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0 ^ _B0, r1=_A1 ^ _B1  
extern __m128d _mm_xor_pd(__m128d _A, __m128d _B);

//Comparisions:==��<��<=��>��>=��!=  
//����һ��__m128d�ļĴ�����r0=(_A0 == _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpeq_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 == _B0) ? 0xffffffffffffffff : 0x0,   
//r1=(_A1 == _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpeq_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 < _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmplt_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 < _B0) ? 0xffffffffffffffff : 0x0,   
//r1=(_A1 < _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmplt_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 <= _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmple_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 <= _B0) ? 0xffffffffffffffff : 0x0,   
//r1=(_A1 <= _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmple_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 > _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpgt_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 > _B0) ? 0xffffffffffffffff : 0x0,   
//r1=(_A1 > _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpgt_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 >= _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpge_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 >= _B0) ? 0xffffffffffffffff : 0x0,   
//r1=(_A1 >= _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpge_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 != _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpneq_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 != _B0) ? 0xffffffffffffffff : 0x0,   
//r1=(_A1 != _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpneq_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=!(_A0 < _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpnlt_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=!(_A0 < _B0) ? 0xffffffffffffffff : 0x0,   
//r1=!(_A1 < _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpnlt_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=!(_A0 <= _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpnle_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=!(_A0 <= _B0) ? 0xffffffffffffffff : 0x0,   
//r1=!(_A1 <= _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpnle_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=!(_A0 > _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpngt_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=!(_A0 > _B0) ? 0xffffffffffffffff : 0x0,   
//r1=!(_A1 > _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpngt_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=!(_A0 >= _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpnge_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=!(_A0 >= _B0) ? 0xffffffffffffffff : 0x0,  
//r1=!(_A1 >= _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpnge_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 ord _B0) ? 0xffffffffffffffff : 0x0,   
//r1=(_A1 ord _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpord_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 ord _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpord_sd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 unord _B0) ? 0xffffffffffffffff : 0x0,   
//r1=(_A1 unord _B1) ? 0xffffffffffffffff : 0x0  
extern __m128d _mm_cmpunord_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(_A0 unord _B0) ? 0xffffffffffffffff : 0x0, r1=_A1  
extern __m128d _mm_cmpunord_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 != _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 1 is returned  
extern int _mm_comieq_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 < _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 1 is returned  
extern int _mm_comilt_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 <= _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 1 is returned  
extern int _mm_comile_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 > _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 0 is returned  
extern int _mm_comigt_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 >= _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 0 is returned  
extern int _mm_comige_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 != _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 0 is returned  
extern int _mm_comineq_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 == _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 1 is returned  
extern int _mm_ucomieq_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 < _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 1 is returned  
extern int _mm_ucomilt_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 <= _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 1 is returned  
extern int _mm_ucomile_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 > _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 0 is returned  
extern int _mm_ucomigt_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 >= _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 0 is returned  
extern int _mm_ucomige_sd(__m128d _A, __m128d _B);
//����һ��0��1��������r=(_A0 != _B0) ? 0x1 : 0x0, If _A and _B is a NaN, 0 is returned  
extern int _mm_ucomineq_sd(__m128d _A, __m128d _B);

//Conversion Operations  
//����һ��__m128d�ļĴ�����r0=(dobule)_A0, r1=(double)_A1  
extern __m128d _mm_cvtepi32_pd(__m128i _A);
//����һ��__m128i�ļĴ�����r0=(int)_A0, r1=(int)_A1, r2=0x0, r3=0x0  
extern __m128i _mm_cvtpd_epi32(__m128d _A);
//����һ��__m128i�ļĴ�����r0=(int)_A0, r1=(int)_A1, r2=0x0, r3=0x0,using truncate  
extern __m128i _mm_cvttpd_epi32(__m128d _A);
//����һ��__m128�ļĴ�����r0=(flaot)_A0, r1=(float)_A1, r2=(float)_A2, r3=(float)_A3  
extern __m128 _mm_cvtepi32_ps(__m128i _A);
//����һ��__m128i�ļĴ�����r0=(int)_A0, r1=(int)_A1, r2=(int)_A2, r3=(int)_A3  
extern __m128i _mm_cvtps_epi32(__m128 _A);
//����һ��__m128i�ļĴ�����r0=(int)_A0, r1=(int)_A1, r2=(int)_A2, r3=(int)_A3,using truncate  
extern __m128i _mm_cvttps_epi32(__m128 _A);
//����һ��__m128�ļĴ�����r0=(flaot)_A0, r1=(float)_A1, r2=0.0, r3=0.0  
extern __m128 _mm_cvtpd_ps(__m128d _A);
//����һ��__m128d�ļĴ�����r0=(dobule)_A0, r1=(double)_A1  
extern __m128d _mm_cvtps_pd(__m128 _A);
//����һ��__m128�ļĴ�����r0=(float)_B0, r1=_B1, r2=_B2, r3=_B3  
extern __m128 _mm_cvtsd_ss(__m128 _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=(double)_B0, r1=_A1  
extern __m128d _mm_cvtss_sd(__m128d _A, __m128 _B);
//����һ��32bit������r=(int)_A0  
extern int _mm_cvtsd_si32(__m128d _A);
//����һ��32bit������r=(int)_A0,using truncate  
extern int _mm_cvttsd_si32(__m128d _A);
//����һ��__m128d�ļĴ�����r0=(double)_B, r1=_A1  
extern __m128d _mm_cvtsi32_sd(__m128d _A, int _B);
//����һ��__m64�ļĴ�����r0=(int)_A0, r1=(int)_A1  
extern __m64 _mm_cvtpd_pi32(__m128d _A);
//����һ��__m64�ļĴ�����r0=(int)_A0, r1=(int)_A1,using truncate  
extern __m64 _mm_cvttpd_pi32(__m128d _A);
//����һ��__m128d�ļĴ�����r0=(dobule)_A0, r1=(double)_A1  
extern __m128d _mm_cvtpi32_pd(__m64 _A);

//Miscellaneous Operations(Floating-Point SSE2 Intrinsics)  
//����һ��__m128d�ļĴ�����r0=_A1, r1=_B1  
extern __m128d _mm_unpackhi_pd(__m128d _A, __m128d _B);
//����һ��__m128d�ļĴ�����r0=_A0, r1=_B0  
extern __m128d _mm_unpacklo_pd(__m128d _A, __m128d _B);
//����һ��2bit������r=sign(_A1) << 1 | sign(_A0)  
extern int _mm_movemask_pd(__m128d _A);
//����һ��__m128d�ļĴ�����Selects two specific double-precision,  
// floating-point values from _A and _B, based on the mask _I,  
//The mask must be an immediate  
extern __m128d _mm_shuffle_pd(__m128d _A, __m128d _B, int _I);

//Load Operations(Floating-Point SSE2 Intrinsics)  
//����һ��__m128d�ļĴ�����r0=_Dp[0], r1=_Dp[1], The address _Dp must be 16-byte aligned  
extern __m128d _mm_load_pd(double const*_Dp);
//����һ��__m128d�ļĴ�����r0=*_Dp, r1=*_Dp, The address _Dp does not need  
//to be 16-byte aligned  
extern __m128d _mm_load1_pd(double const*_Dp);
//����һ��__m128d�ļĴ�����r0=_Dp[1], r1=_Dp[0], The address _Dp must be 16-byte aligned  
extern __m128d _mm_loadr_pd(double const*_Dp);
//����һ��__m128d�ļĴ�����r0=_Dp[0], r1=_Dp[1], The address _Dp does not   
//need to be 16-byte aligned  
extern __m128d _mm_loadu_pd(double const*_Dp);
//����һ��__m128d�ļĴ�����r0=*_Dp, r1=0.0, The address _Dp does not   
//need to be 16-byte aligned  
extern __m128d _mm_load_sd(double const*_Dp);
//����һ��__m128d�ļĴ�����r0=_A0, r1=*_Dp, The address _Dp does not   
//need to be 16-byte aligned  
extern __m128d _mm_loadh_pd(__m128d _A, double const*_Dp);
//����һ��__m128d�ļĴ�����r0=*_Dp, r1=_A1, The address _Dp does not  
//need to be 16-byte aligned  
extern __m128d _mm_loadl_pd(__m128d _A, double const*_Dp);

//Set Operations(Floating-Point SSE2 Intrinsics)  
//����һ��__m128d�ļĴ�����r0=_W, r1=0.0  
extern __m128d _mm_set_sd(double _W);
//����һ��__m128d�ļĴ�����r0=_A, r1=_A  
extern __m128d _mm_set1_pd(double _A);
//����һ��__m128d�ļĴ�����r0=_Y, r1=_Z  
extern __m128d _mm_set_pd(double _Z, double _Y);
//����һ��__m128d�ļĴ�����r0=_Y, r1=_Z  
extern __m128d _mm_setr_pd(double _Y, double _Z);
//����һ��__m128d�ļĴ�����r0=0.0, r1=0.0  
extern __m128d _mm_setzero_pd(void);
//����һ��__m128d�ļĴ�����r0=_B0, r1=_A1  
extern __m128d _mm_move_sd(__m128d _A, __m128d _B);

//Store Operations(Floating-Point SSE2 Intrinsics)  
//����Ϊ�գ�*_Dp=_A0, The address _Dp does not need to be 16-byte aligned  
extern void _mm_store_sd(double *_Dp, __m128d _A);
//����Ϊ�գ�_Dp[0]=_A0, _Dp[1]=_A0, The address _Dp must be 16-byte aligned  
extern void _mm_store1_pd(double *_Dp, __m128d _A);
//����Ϊ�գ�_Dp[0]=_A0, _Dp[1]=_A1, The address _Dp must be 16-byte aligned  
extern void _mm_store_pd(double *_Dp, __m128d _A);
//����Ϊ�գ�_Dp[0]=_A0, _Dp[1]=_A1, The address _Dp does not need to be 16-byte aligned  
extern void _mm_storeu_pd(double *_Dp, __m128d _A);
//����Ϊ�գ�_Dp[0]=_A1, _Dp[1]=_A0, The address _Dp must be 16-byte aligned  
extern void _mm_storer_pd(double *_Dp, __m128d _A);
//����Ϊ�գ�*_Dp=_A1  
extern void _mm_storeh_pd(double *_Dp, __m128d _A);
//����Ϊ�գ�*_Dp=_A0  
extern void _mm_storel_pd(double *_Dp, __m128d _A);

//new convert to float  
//����һ��64bit double���ͣ�r=_A0, Extracts the lower order floating point value  
extern double _mm_cvtsd_f64(__m128d _A);

//Cache Support for Streaming SIMD Extensions 2 Floating-Point Operations  
//����Ϊ�գ�_Dp[0]=_A0, _Dp[1]=_A1, Stores the data in _A to the address _Dp without  
//polluting caches. The address _Dp must be 16-byte aligned. If the cache line   
//containing address _Dp is already in the cache, the cache will be updated  
extern void _mm_stream_pd(double *_Dp, __m128d _A);

/*------------Integer Intrinsics Using Streaming SIMD Extensions 2-------------*/
//Arithmetic Operations(Integer SSE2 Intrinsics):add��sub��mul��avg��min��max  
//����һ��__m128i�ļĴ�����r0=_A0+_B0, r1=_A1+_B1, ... r15=_A15+_B15  
extern __m128i _mm_add_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�������_A��_B�ж�Ӧλ�õ�16bit�з��Ż��޷��������ֱ���ӣ�  
//��ri=_Ai+_Bi(r0=_A0+_B0, r1=_A1+_B1, ... r7=_A7+_B7)  
extern __m128i _mm_add_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=_A0+_B0, r1=_A1+_B1, r2=_A2+_B2, r3=_A3+_B3  
extern __m128i _mm_add_epi32(__m128i _A, __m128i _B);
//����һ��__m64�ļĴ�����r=_A+_B  
extern __m64 _mm_add_si64(__m64 _A, __m64 _B);
//����һ��__m128i�ļĴ�����r0=_A0+_B0, r1=_A1+_B1  
extern __m128i _mm_add_epi64(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=SignedSaturate(_A0+_B0), r1=SignedSaturate(_A1+_B1), ...   
//r15=SignedSaturate(_A15+_B15), saturates  
extern __m128i _mm_adds_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�������_A��_B�ж�Ӧλ�õ�16bit�з��Ż��޷��������ֱ���ӣ�  
//r0=SignedSaturate(_A0+_B0), r1=SignedSaturate(_A1+_B1), ...   
//r7=SignedSaturate(_A7+_B7), �����������ʱ������Ϊ�߽�ֵ(saturates)  
extern __m128i _mm_adds_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=UnsignedSaturate(_A0+_B0), r1=UnsignedSaturate(_A1+_B1), ...   
//r15=UnsignedSaturate(_A15+_B15), saturates  
extern __m128i _mm_adds_epu8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=UnsignedSaturate(_A0+_B0), r1=UnsignedSaturate(_A1+_B1), ...   
//r7=UnsignedSaturate(_A7+_B7), saturates  
extern __m128i _mm_adds_epu16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=(_A0+_B0)/2, r1=(_A1+_B1)/2, ... r15=(_A15+_B15)/2, rounds  
extern __m128i _mm_avg_epu8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�������_A��_B�ж�Ӧλ�õ�16bit�޷�������ȡƽ����  
//��ri=(_Ai+_Bi)/2(r0=(_A0+_B0)/2, r1=(_A1+_B1)/2, ... r7=(_A7+_B7)/2), rounds  
extern __m128i _mm_avg_epu16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����������4���з��Ż��޷���32bit��������  
//�ֱ����㣺r0=(_A0*_B0)+(_A1*_B1), r1=(_A2*_B2)+(_A3*_B3),   
//r2=(_A4*_B4)+(_A5*_B5), r3=(_A6*_B6)+(_A7*_B7)  
extern __m128i _mm_madd_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����ȡ_A��_B�ж�Ӧλ�õ�16bit�з��Ż��޷������������ֵ��  
//��ri=max(_Ai,_Bi) (r0=max(_A0,_B1), r1=max(_A1,_B1), ... r7=max(_A7,_B7))  
extern __m128i _mm_max_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=max(_A0,_B1), r1=max(_A1,_B1), ... r15=max(_A15,_B15)  
extern __m128i _mm_max_epu8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����ȡ_A��_B�ж�Ӧλ�õ�16bit�з��Ż��޷�����������Сֵ��  
//��ri=min(_Ai, _Bi)(r0=min(_A0,_B1), r1=min(_A1,_B1), ... r7=min(_A7,_B7))  
extern __m128i _mm_min_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=min(_A0,_B1), r1=min(_A1,_B1), ... r15=min(_A15,_B15)  
extern __m128i _mm_min_epu8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ���������8���з��Ż��޷���16bit���������ֱ�Ϊ_A��_B��Ӧλ�õ�16bit  
//�з��Ż��޷���������˽���ĸ�16bit���ݣ���ri=(_Ai*_Bi)[31:16](r0=(_A0*_B0)[31:16],   
//r1=(_A1*_B1)[31:16] ... r7=(_A7*_B7)[31:16])  
extern __m128i _mm_mulhi_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=(_A0*_B0)[31:16], r1=(_A1*_B1)[31:16] ... r7=(_A7*_B7)[31:16]  
extern __m128i _mm_mulhi_epu16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ���������8���з��Ż��޷���16bit���������ֱ�Ϊ_A��_B��Ӧλ�õ�16bit  
//�з��Ż��޷���������˽���ĵ�16bit���ݣ���ri=(_Ai*_Bi)[15:0](r0=(_A0*_B0)[15:0],   
//r1=(_A1*_B1)[15:0] ... r7=(_A7*_B7)[15:0])  
extern __m128i _mm_mullo_epi16(__m128i _A, __m128i _B);
//����һ��__m64�ļĴ�����r=_A0*_B0  
extern __m64 _mm_mul_su32(__m64 _A, __m64 _B);
//����һ��__m128i�ļĴ�����r0=_A0*_B0, r1=_A2*_B2  
extern __m128i _mm_mul_epu32(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=abs(_A0-_B0) + abs(_A1-_B1) + ... + abs(_A7-_B7),   
//r1=0x0,r2=0x0, r3=0x0, r4=abs(_A8-_B8) + abs(_A9-_B9) + ... + abs(_A15-_B15),   
//r5=0x0, r6=0x0, r7=0x0  
extern __m128i _mm_sad_epu8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=_A0-_B0, r1=_A1-_B1, ... r15=_A15-_B15  
extern __m128i _mm_sub_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�������_A��_B�ж�Ӧλ�õ�16bit�з��Ż��޷��������ֱ������  
//��ri=_Ai-_Bi(r0=_A0-_B0, r1=_A1-_B1, ... r7=_A7-_B7)  
extern __m128i _mm_sub_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=_A0-_B0, r1=_A1-_B1, r2=_A2-_B2, r3=_A3-_B3  
extern __m128i _mm_sub_epi32(__m128i _A, __m128i _B);
//����һ��__m64�ļĴ�����r=_A-_B  
extern __m64 _mm_sub_si64(__m64 _A, __m64 _B);
//����һ��__m128i�ļĴ�����r0=_A0-_B0, r1=_A1-_B1  
extern __m128i _mm_sub_epi64(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=SignedSaturate(_A0-_B0), r1=SignedSaturate(_A1-_B1), ...   
//r15=SignedSaturate(_A15-_B15), saturate  
extern __m128i _mm_subs_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�������_A��_B�ж�Ӧλ�õ�16bit�з��Ż��޷��������ֱ������  
//�����������ʱ������Ϊ�߽�ֵ(saturate), r0=SignedSaturate(_A0-_B0),   
//r1=SignedSaturate(_A1-_B1), ... r7=SignedSaturate(_A7-_B7)  
extern __m128i _mm_subs_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=UnsignedSaturate(_A0-_B0), r1=UnsignedSaturate(_A1-_B1), ...  
//r15=UnsignedSaturate(_A15-_B15), saturate  
extern __m128i _mm_subs_epu8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=UnsignedSaturate(_A0-_B0), r1=UnsignedSaturate(_A1-_B1), ...   
//r15=UnsignedSaturate(_A7-_B7), saturate  
extern __m128i _mm_subs_epu16(__m128i _A, __m128i _B);

//Logical Operations(Integer SSE2 Intrinsics):and��or��xor��andnot  
//����һ��__m128i�ļĴ��������Ĵ���_A�ͼĴ���_B�Ķ�Ӧλ���а�λ������, r=_A & _B  
extern __m128i _mm_and_si128(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ��������Ĵ���_Aÿһλȡ�ǣ�Ȼ��ͼĴ���_B��ÿһλ���а�λ������,  
//r=(~_A) & _B  
extern __m128i _mm_andnot_si128(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ��������Ĵ���_A�ͼĴ���_B�Ķ�Ӧλ���а�λ������, r=_A | _B  
extern __m128i _mm_or_si128(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ��������Ĵ���_A�ͼĴ���_B�Ķ�Ӧλ���а�λ�������, r=_A ^ _B  
extern __m128i _mm_xor_si128(__m128i _A, __m128i _B);

//Shift Operations  
//����һ��__m128i�ļĴ�����r=_A << (_Imm * 8),  _Imm must be an immediate,    
//shifting in zeros  
extern __m128i _mm_slli_si128(__m128i _A, int _Imm);
//����һ��__m128i�ļĴ��������Ĵ���_A�е�8��16bit��������_Count������ͬ���߼�����,  
//r0=_A0 << _Count, r1=_A1 << _Count, ... r7=_A7 << count,  shifting in zeros  
extern __m128i _mm_slli_epi16(__m128i _A, int _Count);
//����һ��__m128i�ļĴ��������Ĵ���_A�е�8��16bit��������_Count�Ĵ����ж�Ӧλ�õ�����  
//�����߼�����, r0=_A0 << _Count, r1=_A1 << _Count, ... r7=_A7 << count,  shifting in zeros  
extern __m128i _mm_sll_epi16(__m128i _A, __m128i _Count);
//����һ��__m128i�ļĴ�����r0=_A0 << _Count, r1=_A1 << _Count, r2=_A2 << count,   
//r3=_A3 << count,  shifting in zeros  
extern __m128i _mm_slli_epi32(__m128i _A, int _Count);
//����һ��__m128i�ļĴ�����r0=_A0 << _Count, r1=_A1 << _Count, r2=_A2 << count,   
//r3=_A3 << count,  shifting in zeros  
extern __m128i _mm_sll_epi32(__m128i _A, __m128i _Count);
//����һ��__m128i�ļĴ�����r0=_A0 << _Count, r1=_A1 << _Count,  shifting in zeros  
extern __m128i _mm_slli_epi64(__m128i _A, int _Count);
//����һ��__m128i�ļĴ�����r0=_A0 << _Count, r1=_A1 << _Count,  shifting in zeros  
extern __m128i _mm_sll_epi64(__m128i _A, __m128i _Count);
//����һ��__m128i�ļĴ��������Ĵ���_A�е�8��16bit��������_Count������ͬ����������,  
//r0=_A0 >> _Count, r1=_A1 >> _Count, ... r7=_A7 >> count,  shifting in the sign bit  
extern __m128i _mm_srai_epi16(__m128i _A, int _Count);
//����һ��__m128i�ļĴ��������Ĵ���_A�е�8��16bit��������_Count�Ĵ����ж�Ӧλ�õ���������  
//��������,r0=_A0 >> _Count, r1=_A1 >> _Count, ... r7=_A7 >> count,  shifting in the sign bit  
extern __m128i _mm_sra_epi16(__m128i _A, __m128i _Count);
//����һ��__m128i�ļĴ�����r0=_A0 >> _Count, r1=_A1 >> _Count, r3=_A3 >> count,   
//r4=_A4 >> count,  shifting in the sign bit  
extern __m128i _mm_srai_epi32(__m128i _A, int _Count);
//����һ��__m128i�ļĴ�����r0=_A0 >> _Count, r1=_A1 >> _Count, r3=_A3 >> count,  
//r4=_A4 >> count,  shifting in the sign bit  
extern __m128i _mm_sra_epi32(__m128i _A, __m128i _Count);
//����һ��__m128i�ļĴ�����r=srl(_A, _Imm * 8),   _Imm must be an immediate,    
//shifting in zeros  
extern __m128i _mm_srli_si128(__m128i _A, int _Imm);
//����һ��__m128i�ļĴ��������Ĵ���_A�е�8��16bit��������_Count������ͬ���߼����ƣ�  
//��λ���ֵΪ0,r0=srl(_A0, _Count), r1=srl(_A1, _Count), ... r7=srl(_A7, _Count),   
//shifting in zeros  
extern __m128i _mm_srli_epi16(__m128i _A, int _Count);
//����һ��__m128i�ļĴ��������Ĵ���_A�е�8��16bit��������_Count�Ĵ����ж�Ӧλ�õ�����  
//�����߼����ƣ���λ���ֵΪ0, r0=srl(_A0, _Count), r1=srl(_A1, _Count), ...   
//r7=srl(_A7, _Count),  shifting in zeros  
extern __m128i _mm_srl_epi16(__m128i _A, __m128i _Count);
//����һ��__m128i�ļĴ�����r0=srl(_A0, _Count), r1=srl(_A1, _Count), r2=srl(_A2, _Count),  
//r3=srl(_A3, _Count),  shifting in zeros  
extern __m128i _mm_srli_epi32(__m128i _A, int _Count);
//����һ��__m128i�ļĴ�����r0=srl(_A0, _Count), r1=srl(_A1, _Count), r2=srl(_A2, _Count),  
//r3=srl(_A3, _Count),  shifting in zeros  
extern __m128i _mm_srl_epi32(__m128i _A, __m128i _Count);
//����һ��__m128i�ļĴ�����r0=srl(_A0, _Count), r1=srl(_A1, _Count), shifting in zeros  
extern __m128i _mm_srli_epi64(__m128i _A, int _Count);
//����һ��__m128i�ļĴ�����r0=srl(_A0, _Count), r1=srl(_A1, _Count), shifting in zeros  
extern __m128i _mm_srl_epi64(__m128i _A, __m128i _Count);

//Comparison Intrinsics(SSE2):==��>��<  
//����һ��__m128i�ļĴ�����r0=(_A0 == _B0) ? 0xff : 0x00,   
//r1=(_A1 == _B1) ? 0xff : 0x0, ... r15=(_A15 == _B15) ? 0xff : 0x0  
extern __m128i _mm_cmpeq_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ������ֱ�ȽϼĴ���_A�ͼĴ���_B��Ӧλ��16bit�����Ƿ���ȣ�����ȣ�  
//��λ�÷���0xffff�����򷵻�0x0����ri=(_Ai==_Bi)?0xffff:0x0(r0=(_A0 == _B0) ? 0xffff : 0x00,   
//r1=(_A1 == _B1) ? 0xffff : 0x0, ... r7=(_A7 == _B7) ? 0xffff : 0x0)  
extern __m128i _mm_cmpeq_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=(_A0 == _B0) ? 0xffffffff : 0x00,   
//r1=(_A1 == _B1) ? 0xffffffff : 0x0,  
//r2=(_A2 == _B2) ? 0xffffffff : 0x0, r3=(_A3 == _B3) ? 0xffffffff : 0x0  
extern __m128i _mm_cmpeq_epi32(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=(_A0 > _B0) ? 0xff : 0x00, r1=(_A1 > _B1) ? 0xff : 0x0, ...  
//r15=(_A15 > _B15) ? 0xff : 0x0  
extern __m128i _mm_cmpgt_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ������ֱ�ȽϼĴ���_A��ÿ��16bit�����Ƿ���ڼĴ���_B��Ӧλ��16bit��������  
//�����ڣ���λ�÷���0xffff�����򷵻�0x0��  
//��ri=(_Ai>_Bi)?0xffff:0x0(r0=(_A0 > _B0) ? 0xffff : 0x00,   
//r1=(_A1 > _B1) ? 0xffff : 0x0, ... r7=(_A7 > _B7) ? 0xffff : 0x0)  
extern __m128i _mm_cmpgt_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=(_A0 > _B0) ? 0xffffffff : 0x00,  
//r1=(_A1 > _B1) ? 0xffffffff : 0x0,  
//r2=(_A2 > _B2) ? 0xffffffff : 0x0, r3=(_A3 > _B3) ? 0xffffffff : 0x0  
extern __m128i _mm_cmpgt_epi32(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=(_A0 < _B0) ? 0xff : 0x00, r1=(_A1 < _B1) ? 0xff : 0x0, ...   
//r15=(_A15 < _B15) ? 0xff : 0x0  
extern __m128i _mm_cmplt_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ������ֱ�ȽϼĴ���_A��ÿ��16bit�����Ƿ�С�ڼĴ���_B��Ӧλ��16bit������  
//��С�ڣ���λ�÷���0xffff�����򷵻�0x0��  
//��ri=(_Ai<_Bi)?0xffff:0x0(r0=(_A0 < _B0) ? 0xffff : 0x00,   
//r1=(_A1 < _B1) ? 0xffff : 0x0, ... r7=(_A7 < _B7) ? 0xffff : 0x0)  
extern __m128i _mm_cmplt_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=(_A0 < _B0) ? 0xffffffff : 0x00,  
//r1=(_A1 < _B1) ? 0xffffffff : 0x0,   
//r2=(_A2 < _B2) ? 0xffffffff : 0x0, r3=(_A3 < _B3) ? 0xffffffff : 0x0  
extern __m128i _mm_cmplt_epi32(__m128i _A, __m128i _B);

//Conversion Intrinsics: int <-----> __m128i  
//����һ��__m128i�ļĴ�����r0=_A, r1=0x0, r2=0x0, r3=0x0  
extern __m128i _mm_cvtsi32_si128(int _A);
//����һ��32bit������r=_A0  
extern int _mm_cvtsi128_si32(__m128i _A);

//Miscellaneous Operations(Integer SSE2 Intrinsics)  
//����һ��__m128i�ļĴ�����r0=SignedSaturate(_A0), r1=SignedSaturate(_A1), ...   
//r7=SignedSaturate(_A7), r8=SignedSaturate(_B0), r9=SignedSaturate(_B1), ...   
//r15=SignedSaturate(_B7),  saturate  
extern __m128i _mm_packs_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=SignedSaturate(_A0), r1=SignedSaturate(_A1),   
//r2=SignedSaturate(_A2),r3=SignedSaturate(_A3), r4=SignedSaturate(_B0),   
//r5=SignedSaturate(_B1), r6=SignedSaturate(_B2), r7=SignedSaturate(_B3),  saturate  
extern __m128i _mm_packs_epi32(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=UnsignedSaturate(_A0), r1=UnsignedSaturate(_A1), ...   
//r7=UnsignedSaturate(_A7),r8=UnsignedSaturate(_B0), r9=UnsignedSaturate(_B1), ...   
//r15=UnsignedSaturate(_B7),  saturate  
extern __m128i _mm_packus_epi16(__m128i _A, __m128i _B);
//����һ��16bit����������_Imm��_A��8��16bit����ѡȡ��Ӧ��ŵ���,  
//r=(_Imm == 0) ? _A0 : ((_Imm == 1) ? _A1 : ... (_Imm == 7) ? _A7),   
//_Imm must be an immediate, zero extends  
extern int _mm_extract_epi16(__m128i _A, int _Imm);
//����һ��__m128i�ļĴ���������_Imm��_A��8��16bit���ж�Ӧ��ŵ����滻Ϊ_B,  
//r0=(_Imm == 0) ? _B : _A0; r1=(_Imm == 1) : _B : _A1, ... r7=(_Imm == 7) ? _B : _A7  
extern __m128i _mm_insert_epi16(__m128i _A, int _B, int _Imm);
//����һ��16bit������r=(_A15[7] << 15) | (_A14[7] << 14) ... (_A1[7] << 1) | _A0[7],   
//zero extends the upper bits  
extern int _mm_movemask_epi8(__m128i _A);
//����һ��__m128i�ļĴ��������ǽ�_A��128bit������32bitΪ��λ�������еõ��ģ�_ImmΪ��  
//һ����Ԫ�飬��ʾ�������е�˳�򡣵�_A��ԭ���洢������Ϊ16bitʱ������ָ�������һ��  
//�������С����磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7), _Imm=(2,3,0,1),����_AiΪ16bit������  
//_A0Ϊ��λ�����ؽ��Ϊ(_A2,_A3,_A0,_A1,_A6,_A7,_A4,_A5),  _Imm must be an immediate  
extern __m128i _mm_shuffle_epi32(__m128i _A, int _Imm);
//����һ��__m128i�ļĴ��������ǽ�_A�и�64bit������16bitΪ��λ�������еõ��ģ�_ImmΪһ����Ԫ�飬  
//��ʾ�������е�˳��_A�е�64bit����˳�򲻱䡣���磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7),   
//_Imm=(2,3,0,1),����_AiΪ16bit������_A0Ϊ��λ�����ؽ��Ϊ(_A0,_A1,_A2,_A3,_A5,_A4,_A7,_A6),   
//_Imm must be an immediate   
extern __m128i _mm_shufflehi_epi16(__m128i _A, int _Imm);
//����һ��__m128i�ļĴ��������ǽ�_A�е�64bit������16bitΪ��λ�������еõ��ģ�_ImmΪһ����Ԫ�飬  
//��ʾ�������е�˳��_A�и�64bit����˳�򲻱䡣���磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7),  
//_Imm=(2,3,0,1),����_AiΪ16bit������_A0Ϊ��λ�����ؽ��Ϊ(_A1,_A0,_A3,_A2,_A5,_A4,_A7,_A6),     
//_Imm must be an immediate  
extern __m128i _mm_shufflelo_epi16(__m128i _A, int _Imm);
//����һ��__m128i�ļĴ�����r0=_A8, r1=_B8, r2=_A9, r3=_B9, ... r14=_A15, r15=_B15  
extern __m128i _mm_unpackhi_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ����������Ĵ���_A�ͼĴ���_B�ĸ�64bit����16bitΪ��λ��֯��һ�顣  
//���磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7),_B=(_B0,_B1,_B2,_B3,_B4,_B5,_B6,_B7),  
//����_Ai,_BiΪ16bit������_A0,_B0Ϊ��λ�����ؽ��Ϊ(_A4,_B4,_A5,_B5,_A6,_B6,_A7,_B7),  
//r0=_A4, r1=_B4, r2=_A5, r3=_B5, r4=_A6, r5=_B6, r6=_A7, r7=_B7  
extern __m128i _mm_unpackhi_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ����������Ĵ���_A�ͼĴ���_B�ĸ�64bit����32bitΪ��λ��֯��һ�顣  
//���磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7),_B=(_B0,_B1,_B2,_B3,_B4,_B5,_B6,_B7),  
//����_Ai,_BiΪ16bit������_A0,_B0Ϊ��λ�����ؽ��Ϊ(_A4,_A5,_B4,_B5,_A6,_A7,_B6,_B7),  
//r0=_A2, r1=_B2, r2=_A3, r3=_B3  
extern __m128i _mm_unpackhi_epi32(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ����������Ĵ���_A�ͼĴ���_B�ĸ�64bit����64bitΪ��λ��֯��һ�顣  
//���磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7),_B=(_B0,_B1,_B2,_B3,_B4,_B5,_B6,_B7),  
//����_Ai,_BiΪ16bit������_A0,_B0Ϊ��λ��  
//���ؽ��Ϊ(_A4,_A5,_A6,_A7,_B4,_B5,_B6,_B7), r0=_A1, r1=_B1  
extern __m128i _mm_unpackhi_epi64(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ�����r0=_A0, r1=_B0, r2=_A1, r3=_B1, ... r14=_A7, r15=_B7  
extern __m128i _mm_unpacklo_epi8(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ����������Ĵ���_A�ͼĴ���_B�ĵ�64bit����16bitΪ��λ��֯��һ�顣  
//���磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7),_B=(_B0,_B1,_B2,_B3,_B4,_B5,_B6,_B7),  
//����_Ai,_BiΪ16bit������_A0,_B0Ϊ��λ�����ؽ��Ϊ(_A0,_B0,_A1,_B1,_A2,_B2,_A3,_B3),  
//r0=_A0, r1=_B0, r2=_A1, r3=_B1, r4=_A2, r5=_B2, r6=_A3, r7=_B3  
extern __m128i _mm_unpacklo_epi16(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ����������Ĵ���_A�ͼĴ���_B�ĵ�64bit����32bitΪ��λ��֯��һ�顣  
//���磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7),_B=(_B0,_B1,_B2,_B3,_B4,_B5,_B6,_B7),  
//����_Ai,_BiΪ16bit������_A0,_B0Ϊ��λ�����ؽ��Ϊ(_A0,_A1,_B0,_B1,_A2,_A3,_B2,_B3),  
//r0=_A0, r1=_B0, r2=_A1, r3=_B1  
extern __m128i _mm_unpacklo_epi32(__m128i _A, __m128i _B);
//����һ��__m128i�ļĴ����������Ĵ���_A�ͼĴ���_B�ĵ�64bit����32bitΪ��λ��֯��һ�顣  
//���磬_A=(_A0,_A1,_A2,_A3,_A4,_A5,_A6,_A7),_B=(_B0,_B1,_B2,_B3,_B4,_B5,_B6,_B7),  
//����_Ai,_BiΪ16bit������_A0,_B0Ϊ��λ�����ؽ��Ϊ(_A0,_A1,_A2,_A3,_B0,_B1,_B2,_B3),   
//ro=_A0, r1=_B0  
extern __m128i _mm_unpacklo_epi64(__m128i _A, __m128i _B);

//Load Operations(Integer SSE2 Intrinsics)  
//����Ϊһ��__m128i�ļĴ���������_Pָ������ݶ���ָ���Ĵ����У�ʵ��ʹ��ʱ��  
//_Pһ����ͨ������ת���õ���, Address _P must be 16-byte aligned  
extern __m128i _mm_load_si128(__m128i const*_P);
//����һ��__m128i�ļĴ�����Loads 128-bit value, Address _P does not need be 16-byte aligned  
extern __m128i _mm_loadu_si128(__m128i const*_P);
//����һ��__m128i�ļĴ�����r0=*p[63:0], r1=0x0, zeroing the upper 64 bits of the result  
extern __m128i _mm_loadl_epi64(__m128i const*_P);

//Set Operations(Integer SSE2 Intrinsics)  
//����һ��__m128i�ļĴ�����r0=_Q0, r1=_Q1  
extern __m128i _mm_set_epi64(__m64 _Q1, __m64 _Q0);
//����һ��__m128i�ļĴ�����r0=_I0, r1=_I1, r2=_I2, r3=_I3  
extern __m128i _mm_set_epi32(int _I3, int _I2, int _I1, int _I0);
//����һ��__m128i�ļĴ�����ʹ��8�������short�����������üĴ����������,  
//r0=_W0, r1=_W1, ... r7=_W7  
extern __m128i _mm_set_epi16(short _W7, short _W6, short _W5, short _W4,
	short _W3, short _W2, short _W1, short _W0);
//����һ��__m128i�ļĴ�����r0=_B0, r1=_B1, ... r15=_B15  
extern __m128i _mm_set_epi8(char _B15, char _B14, char _B13, char _B12, char _B11,
	char _B10, char _B9, char _B8, char _B7, char _B6, char _B5, char _B4,
	char _B3, char _B2, char _B1, char _B0);
//����һ��__m128i�ļĴ�����r0=_Q, r1=_Q  
extern __m128i _mm_set1_epi64(__m64 _Q);
//����һ��__m128i�ļĴ�����r0=_I, r1=_I, r2=_I, r3=_I  
extern __m128i _mm_set1_epi32(int _I);
//����һ��__m128i�ļĴ�����r0=_W, r1=_W, ... r7=_W  
extern __m128i _mm_set1_epi16(short _W);
//����һ��__m128i�ļĴ�����r0=_B, r1=_B, ... r15=_B  
extern __m128i _mm_set1_epi8(char _B);
//����һ��__m128i�ļĴ�����r=_Q  
extern __m128i _mm_setl_epi64(__m128i _Q);
//����һ��__m128i�ļĴ�����r0=_Q0, r1=_Q1  
extern __m128i _mm_setr_epi64(__m64 _Q0, __m64 _Q1);
//����һ��__m128i�ļĴ�����r0=_I0, r1=_I1, r2=_I2, r3=_I3  
extern __m128i _mm_setr_epi32(int _I0, int _I1, int _I2, int _I3);
//����һ��__m128i�ļĴ�����r0=_W0, r1=_W1, ... r7=_W7  
extern __m128i _mm_setr_epi16(short _W0, short _W1, short _W2, short _W3,
	short _W4, short _W5, short _W6, short _W7);
//����һ��__m128i�ļĴ�����r0=_B15, r1=_B14, ... r15=_B0  
extern __m128i _mm_setr_epi8(char _B15, char _B14, char _B13, char _B12, char _B11,
	char _B10, char _B9, char _B8, char _B7, char _B6, char _B5, char _B4,
	char _B3, char _B2, char _B1, char _B0);
//����һ��__m128i�ļĴ�����r=0x0  
extern __m128i _mm_setzero_si128(void);

//Store Operations(Integer SSE2 Intrinsics)  
//����Ϊ�գ������Ĵ���_B�е����ݴ洢��_Pָ��ĵ�ַ�У�ʵ��ʹ��ʱ��  
//_Pһ����ͨ������ת���õ���, *_P = _B, Address _P must be 16-byte aligned  
extern void _mm_store_si128(__m128i *_P, __m128i _B);
//����Ϊ�գ�*_P=_B, Address _P does not need to be 16-byte aligned  
extern void _mm_storeu_si128(__m128i *_P, __m128i _B);
//����Ϊ�գ�*_P[63:0] =_Q0, lower 64 bits  
extern void _mm_storel_epi64(__m128i *_P, __m128i _Q);
//����Ϊ�գ�if(_N0[7]) _P[0]=_D0, if(_N1[7]) _P[1]=_D1, ... if(_N15[7]) _P[15]=_D15,   
//The high bit of each byte in the selector _N determines whether the corresponding byte   
//in _D will be stored. Address _P does not need to be 16-byte aligned  
extern void _mm_maskmoveu_si128(__m128i _D, __m128i _N, char *_P);

//Integer, moves  
//����һ��__m128i�ļĴ�����r0=_Q0, r1=0x0, zeroing the upper bits  
extern __m128i _mm_move_epi64(__m128i _Q);
//����һ��__m128i�ļĴ�����r0=_Q, r1=0x0, zeroing the upper bits  
extern __m128i _mm_movpi64_epi64(__m64 _Q);
//����һ��__m64�ļĴ�����r=_Q0  
extern __m64 _mm_movepi64_pi64(__m128i _Q);

//Cache Support for Steaming SIMD Extensions 2 Integer Operations  
//����Ϊ�գ�*_P=_A, Stores the data in _A to the address _P without polluting the caches.  
//If the cache line containing address _P is already in the cache, the cache will be updated.   
//Address _P must be 16-byte aligned  
extern void _mm_stream_si128(__m128i *_P, __m128i _A);
//����Ϊ�գ�Cache line containing _P is flushed and invalidated from   
//all caches in the coherency domain  
extern void _mm_clflush(void const*_P);
//����Ϊ�գ�Guarantees that every load instruction that precedes, in program order, the load   
//fence instruction is globally visible before any load instruction   
//that follows the fence in program order  
extern void _mm_lfence(void);
//����Ϊ�գ�Guarantees that every memory access that precedes, in program order,   
//the memory fence instruction is globally visible before any memory instruction   
//that follows the fence in program order  
extern void _mm_mfence(void);
//����Ϊ�գ�*_P=_I, Stores the data in _I to the address _P without polluting the caches.   
//If the cache line containing address _P is already in the cache, the cache will be updated  
extern void _mm_stream_si32(int *_P, int _I);
//����Ϊ�գ�The execution of the next instruction is delayed an implementation specific   
//amount of time. The instruction does not modify the architectural state. This intrinsic  
//provides especially significant performance gain  
extern void _mm_pause(void);

/*---Support for casting between various SP, DP, INT vector types. Note that these do no
conversion of values, they just change the type----*/
//����һ��__m128�ļĴ�����Applies a type cast to reinterpret two 64-bit floating   
//point values passed in as a 128-bit parameter as packed 32-bit floating point values  
extern __m128  _mm_castpd_ps(__m128d);
//����һ��__m128i�ļĴ�����Applies a type cast to reinterpret two 64-bit  
//floating point values passed in as a 128-bit parameter as packed 32-bit integers  
extern __m128i _mm_castpd_si128(__m128d);
//����һ��__m128d�ļĴ�����Applies a type cast to reinterpret four 32-bit floating   
//point values passed in as a 128-bit parameter as packed 64-bit floating point values  
extern __m128d _mm_castps_pd(__m128);
//����һ��__m128i�ļĴ�����Applies a type cast to reinterpret four 32-bit floating   
//point values passed in as a 128-bit parameter as packed 32-bit integers  
extern __m128i _mm_castps_si128(__m128);
//����һ��__m128�ļĴ�����Applies a type cast to reinterpret four 32-bit integers   
//passed in as a 128-bit parameter as packed 32-bit floating point values  
extern __m128  _mm_castsi128_ps(__m128i);
//����һ��__m128d�ļĴ�����Applies a type cast to reinterpret four 32-bit   
//integers passed in as a 128-bit parameter as packed 64-bit floating point values  
extern __m128d _mm_castsi128_pd(__m128i);
