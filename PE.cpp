#include "stdafx.h"
#include "..\include\PE.h"
#include "../include/Res.h"
#include "../include/FileObj.h"
#include "../include/Protection.h"
#include "../include/Alloc.h"

static TCHAR* gstrPESectionNames[] =
{
	TEXT("DOSͷ"),
	TEXT("PEͷ"),
	TEXT("���"),
	TEXT("�����"),
	TEXT("�����"),
	TEXT("��Դ"),
	TEXT("�ض�λ��"),
	TEXT("��������"),
	TEXT("TLS"),
	TEXT("������Ϣ"),
	TEXT("��Ȩ��Ϣ"),
	TEXT("�쳣"),
	TEXT("�ӳ�����"),
	TEXT("ȫ�ֱ���"),
	TEXT("COM"),
};

static TCHAR* gstrResTypeNames[] =
{
	TEXT("Cursor"),
	TEXT("Bitmap"),
	TEXT("Icon"),
	TEXT("Menu"),
	TEXT("Dialog"),

	TEXT("String"),
	TEXT("Font Directory"),
	TEXT("Font"),
	TEXT("Accelerators"),
	TEXT("RCData"),

	TEXT("MessageTables"),
	TEXT("Group Cursor"),
	TEXT("Group Icon"),
	TEXT("UserRes"),//13
	TEXT("UserRes"),//14


	TEXT("UserRes"),//15
	TEXT("Version Information"),//16
	TEXT("DlgInclude"),
	TEXT("UserRes"),//18
	TEXT("Plugplay"),

	TEXT("VXD"),
	TEXT("ANICURSOR"),
	TEXT("ANIICON"),
	TEXT("HTML")

};

CPE::CPE()
{
	m_pBuffer = NULL;
}


CPE::~CPE()
{
	TAlloc<CHeapAlloc> alloc;
	alloc.Free(m_pBuffer);
	alloc.Free(m_pImageBase);
}

// ��ָ�����ļ������м���
BOOL CPE::LoadFromFile(LPCTSTR lpszFileName)
{
	DWORD dwRet = 0L;
	BOOL ret = FALSE;
	CFileObj File;
	if (!File.Open(lpszFileName))
		return	FALSE;

	DWORD dwSize = File.GetSize();
	TAlloc<CHeapAlloc> alloc;
	

	void* pBuffer = alloc.Allocate(dwSize);
	if (!File.Read(pBuffer, dwSize))
	{
		alloc.Free(pBuffer);
		return FALSE;
	}
	this->m_pBuffer = pBuffer;
	return LoadFromMemory(m_pBuffer);
}


DWORD RVA2Offset(PIMAGE_SECTION_HEADER pSection, DWORD dwRVA);
DWORD RVA2Offset(PCHAR pImageBase, DWORD dwRVA);

PIMAGE_SECTION_HEADER ImageRVA2Section(PIMAGE_NT_HEADERS pNtHeaders, DWORD dwRVA)
{
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)((PCHAR(pNtHeaders)) + sizeof(IMAGE_NT_HEADERS));
	for (DWORD i = 0;i<pNtHeaders->FileHeader.NumberOfSections;i++)
	{
		if ((pSectionHeader->VirtualAddress) 
			&& (dwRVA <= (pSectionHeader->VirtualAddress + pSectionHeader->SizeOfRawData)))
		{
			return ((PIMAGE_SECTION_HEADER)pSectionHeader);
		}
		pSectionHeader++;
	}
	return(NULL);
}

BOOL CPE::LoadFromMemory(void* pBuffer)
{
	_IMAGE_DOS_HEADER* pDosHeaderInFile = (_IMAGE_DOS_HEADER*)pBuffer;
	_IMAGE_NT_HEADERS* pNtHeadersInFile = (_IMAGE_NT_HEADERS*)((UCHAR*)pBuffer + pDosHeaderInFile->e_lfanew);

	DWORD dwBaseAddress = pNtHeadersInFile->OptionalHeader.ImageBase;
	DWORD dwSize = pNtHeadersInFile->OptionalHeader.SizeOfImage;

	if (!dwSize)
	{
		//	dwSize = CalcImageSizeNeeded();
	}
	TAlloc<CVirtualAlloc> alloc;
	void* pBaseAddress = alloc.Allocate((void*)dwBaseAddress,dwSize);


	m_pImageBase = pBaseAddress;
	DWORD dwHeadSize = pNtHeadersInFile->OptionalHeader.SizeOfHeaders;
	if (dwHeadSize == 0)
	{
		dwHeadSize = sizeof(_IMAGE_DOS_HEADER) +pDosHeaderInFile->e_lfanew +
			pNtHeadersInFile->FileHeader.SizeOfOptionalHeader;
	}
	//����ͷ������
	CopyMemory(m_pImageBase, pBuffer, dwHeadSize);

	_IMAGE_DATA_DIRECTORY* pResDataDirInFile = &pNtHeadersInFile->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];
	_IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(pNtHeadersInFile);
	_IMAGE_SECTION_HEADER* pResSection = ImageRVA2Section(pNtHeadersInFile, pResDataDirInFile->VirtualAddress);

	///��ʼ���ڴ��е�ͷ���ṹ
	m_pDosHeader = (_IMAGE_DOS_HEADER*)m_pImageBase;
	m_pNtHeaders = (_IMAGE_NT_HEADERS*)((UCHAR*)m_pImageBase + m_pDosHeader->e_lfanew);
	m_pNtHeaders->OptionalHeader.ImageBase = (DWORD)m_pImageBase;

	//��ģ�����DllMain/WinMain;
	m_pfnOEP = (FARPROC)(pNtHeadersInFile->OptionalHeader.AddressOfEntryPoint + (PUCHAR)m_pImageBase);
	void* pResBaseInFile = (void*)((PUCHAR)pBuffer + RVA2Offset(pResSection,pResDataDirInFile->VirtualAddress));
	void* pResBaseInMemory=(void*)((PUCHAR)m_pImageBase+ RVA2Offset(pResSection, pResDataDirInFile->VirtualAddress));

	//������Դ�ļ�
	this->HandleResData(pResBaseInFile,pResBaseInMemory);


	return	TRUE;
}

DWORD RVA2Offset(PIMAGE_SECTION_HEADER pSectionHeader, DWORD dwRVA)
{
	return(dwRVA + pSectionHeader->PointerToRawData - pSectionHeader->VirtualAddress);
}

DWORD RVA2Offset(PCHAR pImageBase, DWORD dwRVA)
{
	DWORD _offset;
	PIMAGE_SECTION_HEADER section;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeaders;
	pDosHeader = PIMAGE_DOS_HEADER(pImageBase);
	pNtHeaders = (PIMAGE_NT_HEADERS)(pImageBase + pDosHeader->e_lfanew);
	section = ImageRVA2Section(pNtHeaders, dwRVA);
	if (section == NULL)
	{
		return(0);
	}
	_offset = dwRVA + section->PointerToRawData - section->VirtualAddress;
	return(_offset);
}



// ������Դ�ں���Դ����
BOOL CPE::HandleResData(void* pResBaseInFile, void* pResBaseInMem)
{
	_IMAGE_RESOURCE_DIRECTORY* pResDirInFile = (_IMAGE_RESOURCE_DIRECTORY*)pResBaseInFile;
	_IMAGE_RESOURCE_DIRECTORY_ENTRY* pResTypeEntryInFile = (_IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pResDirInFile + 1);
	ATLTRACE("NamedEntriesNumber: %x, IdEntriesNumber: %x\r\n", pResDirInFile->NumberOfNamedEntries, pResDirInFile->NumberOfIdEntries);

	for (WORD i = 0;i < pResDirInFile->NumberOfNamedEntries + pResDirInFile->NumberOfIdEntries;i++)
	{
		//	��һ��������
		if (pResTypeEntryInFile->NameIsString)
		{
			//��ȡ����
			/*ATLTRACE("NameOffset: %x\r\n", pResTypeEntryInFile->NameOffset);*/
			_IMAGE_RESOURCE_DIR_STRING_U* pString = (_IMAGE_RESOURCE_DIR_STRING_U*)((PUCHAR)pResDirInFile + pResTypeEntryInFile->NameOffset);
			ATL::CString str;
			TCHAR* pBuffer = str.GetBuffer(pString->Length + 1);
			CopyMemory(pBuffer, pString->NameString, pString->Length + pString->Length);
			pBuffer[pString->Length] = 0;
			str.ReleaseBuffer();
			RT_ANICURSOR;
			ATLTRACE("ResType: len: %d. Type: %ls\r\n", pString->Length, str);
		}
		else
		{
			if (pResTypeEntryInFile->Id >= 1 &&
				pResTypeEntryInFile->Id <= sizeof(gstrResTypeNames) / sizeof(*gstrResTypeNames))
			{
				if ((pResTypeEntryInFile->Id >= 13 && pResTypeEntryInFile->Id <= 15
					|| pResTypeEntryInFile->Id == 18))
				{
					ATLTRACE("ResType  Id: %x, Type: UserRes_%x\r\n", pResTypeEntryInFile->Id, pResTypeEntryInFile->Id);
				}
				else
				{
					ATLTRACE("ResType  Id: %x, Type: %ls\r\n", pResTypeEntryInFile->Id, gstrResTypeNames[pResTypeEntryInFile->Id - 1]);
				}
			}
			else
			{
				ATLTRACE("ResType  Id: %x, Type: UserRes_%x\r\n", pResTypeEntryInFile->Id, pResTypeEntryInFile->Id);
			}
		}

		//	��������
		if (pResTypeEntryInFile->DataIsDirectory)
		{
			//��һ������ԴID������
			PIMAGE_RESOURCE_DIRECTORY pResNameDir = (PIMAGE_RESOURCE_DIRECTORY)((PUCHAR)pResDirInFile + pResTypeEntryInFile->OffsetToDirectory);
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResNameDirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResNameDir + 1);
			for (WORD j = 0;j < pResNameDir->NumberOfIdEntries + pResNameDir->NumberOfNamedEntries;j++)
			{
				//type 
				if (pResNameDirEntry->NameIsString)
				{
					_IMAGE_RESOURCE_DIR_STRING_U* pString = (_IMAGE_RESOURCE_DIR_STRING_U*)((PUCHAR)pResNameDir + pResNameDirEntry->NameOffset);
					ATL::CString str;
					TCHAR* pBuffer = str.GetBuffer(pString->Length + 1);
					CopyMemory(pBuffer, pString->NameString, pString->Length + pString->Length);
					pBuffer[pString->Length] = 0;
					str.ReleaseBuffer();
					ATLTRACE("ResName: len: %d. Name: %ls\r\n", pString->Length, str);
				}
				else
				{
					ATLTRACE("ResId: %x\r\n", pResNameDirEntry->Id);
				}
				if (pResNameDirEntry->DataIsDirectory)
				{
					///������Ϊ�������Դ����
					PIMAGE_RESOURCE_DIRECTORY pResDataDir = (PIMAGE_RESOURCE_DIRECTORY)((PUCHAR)pResDirInFile + pResNameDirEntry->OffsetToDirectory);
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pResDataDirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDataDir + 1);
					for (WORD k = 0;k < pResDataDir->NumberOfIdEntries + pResDataDir->NumberOfNamedEntries;k++)
					{
						//Name ID 
						if (pResDataDirEntry->NameIsString)
						{
							_IMAGE_RESOURCE_DIR_STRING_U* pString = (_IMAGE_RESOURCE_DIR_STRING_U*)((PUCHAR)pResDataDir + pResDataDirEntry->NameOffset);
							ATL::CString str;
							TCHAR* pBuffer = str.GetBuffer(pString->Length + 1);
							CopyMemory(pBuffer, pString->NameString, pString->Length + pString->Length);
							pBuffer[pString->Length] = 0;
							str.ReleaseBuffer();
							//ATLTRACE("ResData: len: %d. Name: %ls\r\n", pString->Length, str);
						}
						else
						{
							//ATLTRACE("ResID Id: %x\r\n", pResDataDirEntry->Id);
						}
						if (!pResDataDirEntry->DataIsDirectory)
						{
							//res data
							//							PIMAGE_RESOURCE_DATA_ENTRY pDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)((PUCHAR)pResDataDir + pResDataDirEntry->OffsetToData);
							//							ATLTRACE("RESDATA: CP:%x, Offset: %x, Size: %x.\r\n", pDataEntry->CodePage, ((PUCHAR)pResDataDir + pDataEntry->OffsetToData), pDataEntry->Size);
						}
						pResDataDirEntry++;
					}
				}
				pResNameDirEntry++;
			}
		}
		pResTypeEntryInFile++;
	}

	return TRUE;
}
