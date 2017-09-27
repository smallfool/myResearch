int __thiscall CCredenceInput::DoModal(CWnd *this)
{
	signed int v1; // eax@2
	unsigned int tm; // eax@4
	signed int index; // esi@4
	int v4; // ecx@5
	int v5; // edx@5
	signed int idx; // ecx@6
	unsigned int *v7; // eax@6
	char v8; // dl@7
	unsigned int nDate; // eax@9
	int sqlresult; // esi@13
	int v11; // eax@13
	signed int i; // edi@14
	int offset; // esi@15
	int v14; // et2@15
	unsigned __int16 mon; // di@18
	int yr; // esi@22
	signed int day; // eax@27
	char v19; // [sp-14h] [bp-AE4h]@1
	int v20; // [sp-10h] [bp-AE0h]@1
	char *v21; // [sp-Ch] [bp-ADCh]@1
	const char *v22; // [sp-8h] [bp-AD8h]@1
	char *v23; // [sp-4h] [bp-AD4h]@1
	int strAccId; // [sp+0h] [bp-AD0h]@11
	char v25; // [sp+Ch] [bp-AC4h]@32
	int v26; // [sp+6Ch] [bp-A64h]@32
	char Dest; // [sp+2F4h] [bp-7DCh]@11
	char buf[4]; // [sp+3F8h] [bp-6D8h]@30
	char v29; // [sp+3FCh] [bp-6D4h]@30
	char v30; // [sp+500h] [bp-5D0h]@8
	unsigned int v31; // [sp+8CCh] [bp-204h]@8
	char v32; // [sp+8D4h] [bp-1FCh]@8
	char v33[128]; // [sp+8F4h] [bp-1DCh]@8
	char v34; // [sp+974h] [bp-15Ch]@8
	char Date; // [sp+994h] [bp-13Ch]@8
	char v36[36]; // [sp+998h] [bp-138h]@4
	char v37; // [sp+9BCh] [bp-114h]@8
	unsigned __int16 year; // [sp+9C8h] [bp-108h]@16
	unsigned __int16 month; // [sp+9CAh] [bp-106h]@18
	unsigned __int16 date; // [sp+9CEh] [bp-102h]@20
	unsigned int v41[40]; // [sp+9DCh] [bp-F4h]@4
	char v42; // [sp+A7Ch] [bp-54h]@4
	string str; // [sp+A80h] [bp-50h]@1
	int strText[16]; // [sp+A94h] [bp-3Ch]@1

	strText[11] = (int)this;
	string::string(&str);
	v23 = (char *)1;
	strText[8] = (int)&v19;
	strText[14] = 0;
	std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string<char, std::char_traits<char>, std::allocator<char>>(
		&v19,
		"0101020101",
		(char *)&strText[9] + 3);
	if (CheckRoleRight(v19, v20, v21, v22, v23))
	{
		strText[0] = *(_DWORD *)"CC;B;;BCC;B;;BCC";
		strText[1] = *(_DWORD *)";;BCC;B;;BCC";
		strText[2] = *(_DWORD *)"C;B;;BCC";
		strText[3] = *(_DWORD *)";BCC";
		LOBYTE(strText[4]) = aCcBBccBBcc[16];
		LOBYTE(strText[4]) = 0;
		v1 = 0;
		do
			*((_BYTE *)strText + v1++) -= 10;
		while (v1 < 16);
		memset(&v41[32], 0, 32u);
		v42 = 0;
		memset(v36, 0, 32u);
		v36[32] = 0;
		tm = time(0);
		srand(tm);
		index = 0;
		do
		{
			v4 = index * rand() % 94;
			v5 = index++ % 32;
			*((_BYTE *)&v41[32] + v5) = v4 + 33;
		} while (index < 1000);
		v41[1] = 0x78;
		v41[2] = 0xFFFFFFB7;
		v41[0] = 0xFFFFFFE0;
		v41[4] = -11;
		v41[3] = -20;
		v41[14] = -20;
		v41[6] = -95;
		v41[10] = -95;
		v41[12] = 107;
		v41[28] = 107;
		v41[5] = -26;
		v41[7] = -49;
		v41[8] = 63;
		v41[9] = -121;
		v41[11] = -66;
		v41[13] = -16;
		v41[15] = 15;
		v41[16] = 99;
		v41[17] = -118;
		v41[18] = -71;
		v41[19] = -69;
		v41[20] = -18;
		v41[21] = 19;
		v41[22] = 98;
		v41[23] = 67;
		v41[24] = 36;
		v41[25] = 0xFFFFFFE0;
		v41[26] = 68;
		v41[27] = 0;
		v41[29] = -13;
		v41[30] = 91;
		v41[31] = -128;
		idx = 0;
		v7 = v41;
		do
		{
			v8 = *(_BYTE *)v7;
			++v7;
			*((_BYTE *)&v41[32] + idx++) = v8;
		} while (idx < 32);
		sub_5B3B25(&v30);
		LOBYTE(strText[14]) = 1;
		sub_5B3B55(strText, dword_60F414, 16, 16);
		sub_5B4C6D(&v41[32], v36, 32, 0);
		qmemcpy(v33, &v32, v31);
		sub_5B3B55(&byte_60F240, dword_60F414, 16, 16);
		sub_5B4C6D(&byte_60F360, &v41[32], 32, 0);
		Date = 0;
		qmemcpy(&v34, &v41[32], 0x20u);
		qmemcpy(v33, &v32, v31);
		sub_5B3B55(strText, dword_60F414, 16, 16);
		sub_5B4C6D(&v34, &v41[32], 32, 0);
		qmemcpy(&v37, &v41[32], 0x20u);
		if ((v41[32] & 1) == 1)
			nDate = v41[34];
		else
			nDate = 3;
		v23 = `std::basic_string<char, std::char_traits<char>, std::allocator<char>>::_Nullstr'::`2'::_C;
		v22 = (const char *)(31 * nDate);
		v21 = v36;
		sprintf(
			&Dest,
			"SELECT IFNULL(%s,0),%d FROM a_credence WHERE AccID='%s'",
			v36,
			31 * nDate,
			`std::basic_string<char, std::char_traits<char>, std::allocator<char>>::_Nullstr'::`2'::_C,
			strAccId);
		if (mysql_real_query(0, &Dest, strlen(&Dest)))
		{
			v23 = (char *)16;
			v22 = dword_4189DC;
			v21 = (char *)mysql_error(0);
		__reporterror:
			CWnd::MessageBoxA((CWnd *)strText[11], v21, v22, (unsigned int)v23);
		LABEL_33:
			LOBYTE(strText[14]) = 0;
			sub_5B3B4E(&v30);
			goto __quit;
		}
		strText[10] = mysql_store_result(0);
		sqlresult = mysql_fetch_row(strText[10]);
		strText[8] = sqlresult;
		v11 = atoi(*(const char **)(sqlresult + 4));
		if (atoi(*(const char **)sqlresult) <= v11)
		{
			qmemcpy(v33, &v32, v31);
			sub_5B3B55(strText, dword_60F414, 16, 16);
			v23 = (char *)atoi(*(const char **)strText[8]);
			sprintf((char *)&v41[32], "1|%d", v23);
			sub_5B4AE8(&v41[32], v36, 32, 0);
			*(_DWORD *)buf = 2;
			memset(&v29, 0, 0x104u);
			qmemcpy(&v29, v36, 0x20u);
			if (send(0, buf, 264, 0) == -1)
			{
				mysql_free_result(strText[10]);
				PostQuitMessage(0);
				goto LABEL_33;
			}
		}
		else
		{
			i = 0;
			do
			{
				offset = i % 32;
				*((_BYTE *)&v41[32] + offset) = i * rand() % 94 + 33;
				v14 = i++ * rand() % 94;
				v36[offset] = v14 + 33;
			} while (i < 1000);
			mysql_free_result(strText[10]);
			if (year <= 1900u
				|| year >= 3000u
				|| (mon = month) == 0
				|| month >= 13u
				|| !date
				|| date >= 32u
				|| (COleDateTime::GetTickCount(&strText[5]), yr = year, yr < COleDateTime::GetYear((#51 * )&strText[5]))
				|| yr == COleDateTime::GetYear((#51 * )&strText[5]) && mon < COleDateTime::GetMonth((#51 * )&strText[5])
				|| yr == COleDateTime::GetYear((#51 * )&strText[5])
				&& mon == COleDateTime::GetMonth((#51 * )&strText[5])
				&& (day = COleDateTime::GetDay((#51 * )&strText[5]), date < day))
			{
				v23 = (char *)16;
				v22 = "提醒";
				v21 = "此账套试用期已到！请将软件注册后再进行业务！";
				goto __reporterror;
			}
		}
		mysql_free_result(strText[10]);
		sub_4C0E67(0);
		v26 = 0;
		LOBYTE(strText[14]) = 2;
		CDialog::DoModal((CDialog *)&v25);
		LOBYTE(strText[14]) = 1;
		sub_4A3F3F(&v25);
		goto LABEL_33;
	}
__quit:
	strText[14] = -1;
	return sub_484E6A(&str);
}