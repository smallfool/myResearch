#pragma once


class CProtection
{
public:
	CProtection();
	virtual ~CProtection();
	BOOL AddProtection(void* Address, DWORD Size, DWORD dwProtection);
	BOOL RemoveProtection(DWORD dwProtection);
private:
	DWORD m_OldProtection;
};

