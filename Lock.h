#pragma once

class CLock
{
public:
	CLock();
	~CLock();
	void Lock()
	{
		::EnterCriticalSection(&m_cs);
	}
	void Unlock()
	{
		::LeaveCriticalSection(&m_cs);
	}
private:
	CRITICAL_SECTION m_cs;
};

template <typename TLock = CLock>
class TAutoLock : public TLock
{
public:
	TAutoLock(TLock& l)
		:m_Lock(l)
	{
		m_Lock.Lock();
	}
	~TAutoLock()
	{
		m_Lock.Unlock();
	}
private:
	TLock& m_Lock;
};

extern CLock gLock1;
//extern CLock gLock2;
//extern CLock gLock3;
//extern CLock gStrListLock;
