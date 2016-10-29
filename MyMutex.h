#pragma once

#include "NoCopyable.h"
#include <Windows.h>
#define LOCK(pLock) AUTOLOCK(*pLock)

#define AUTOLOCK(mutex) \
	AutoLock __autolock(&mutex);

// »¥³âËø
class Mutex : private NoCopyable
{
public:
	Mutex();
	~Mutex();

	void Lock();
	bool TryLock();
	void Unlock();

protected:
	::CRITICAL_SECTION m_cs;
};

// ×÷ÓÃÓòËø
class AutoLock
{
public:
	AutoLock(Mutex* pMutex) : m_pLock(NULL)
	{
		if (!pMutex)
			return;

		m_pLock = pMutex;
		m_pLock->Lock();
	}

	~AutoLock()
	{
		if (!m_pLock)
			return;

		m_pLock->Unlock();
	}

protected:
	Mutex*	m_pLock;
};