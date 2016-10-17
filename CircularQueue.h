#pragma once

#include "BaseType.h"

template <class T>
class Helper
{
public:
	enum { eNeedInit = 0 };

	Helper()
	{
		int32 i = 1;
	}
};

template<class T>
class Helper<T*>
{
public:
	enum { eNeedInit = 1 };

	Helper<T*>(){
		int32 i = 0;
	}
};

template <class T, int32 ElEMENTCOUNT>
class CircularQueue
{
	T		m_elements[ElEMENTCOUNT];
	int32	_push;
	int32	_pop;
	int32	_count;
public:
	CircularQueue(void)
	{
		_push = _pop = _count = 0;

		if (Helper<T>::eNeedInit)
		{
		}
		else
		{
		}
	}
	~CircularQueue(void){}
};

