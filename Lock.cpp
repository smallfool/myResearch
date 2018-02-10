#include "stdafx.h"
#include "..\include\Lock.h"


CLock gLock1;
//CLock gLock2;
//CLock gLock3;
//CLock gStrListLock;
CLock::CLock()
{
	::InitializeCriticalSection(&m_cs);
}


CLock::~CLock()
{
	::DeleteCriticalSection(&m_cs);
}
