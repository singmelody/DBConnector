#pragma once

#include <list>
#include "BaseType.h"

template <class T> class MyList;

template <class T>
class MyListNode 
{
	friend class MyList< MyListNode<T> >;
public:
	MyListNode() 
	: _next(NULL)
	, _prev(NULL)
	, m_pOwner(NULL)
	, pList(NULL)
	{}

	virtual ~MyListNode() { Remove(); }

	MyListNode* prev() { return _prev; }
	void prev( MyListNode* _p) { _prev = _p; }

	MyListNode* next() { return _next; }
	void next( MyListNode* _n) { _next = _n; }

	T* Get() { return m_pOwner; }
	void Set(T* p) { m_pOwner = p;}

	bool InList() { return pList != NULL; }

	MyListNode<T>* Remove()
	{
		if (pList)
		{
			MyListNode<T>* pNext = pList->Remove(this);
			pList = NULL;
			return pNext;
		}

		return NULL;
	}
protected:
	T* m_pOwner;
	MyListNode* _prev;
	MyListNode* _next;
	MyList< MyListNode<T>>* pList;
};

template <class T>
class MyList
{
public:
	MyList(): _head(NULL), _back(NULL), m_nCnt(0){}

	virtual ~MyList(){}

	MyList& operator = (MyList& rh)
	{
		m_nCnt = rh.m_nCnt;
		_head = rh._head;
		_back = rh._back;

		T* tmp = GetNext(NULL);
		while(tmp)
		{
			tmp->pList = this;
			tmp = GetNext(tmp);
		}

		return *this;
	}

	// add to head
	void Add(T* ptr)
	{
		if (!ptr || NULL != ptr->pList )
			return;

		ptr->next(_head);
		if(_head)
		{
			_head->prev(ptr);
			_head = ptr;
		}
		else
		{
			_head = _back = ptr;
		}

		ptr->prev(NULL);
		ptr->pList = this;
		m_nCnt++;
	}

	// return next
	T* Remove(T* ptr)
	{
		if(!ptr || (ptr->pList != this))
			return NULL;

		if( _head == _back && _head == ptr)
		{
			_head = _back = NULL;
			m_nCnt = 0;
			ptr->prev(NULL);
			ptr->next(NULL);
			ptr->pList = NULL;
			return NULL;
		}

		T* res = ptr->next();
		T* pr = ptr->prev();
		T* nx = ptr->next();

		if( _head == ptr )
		{
			_head = nx;
			if(nx)
				nx->prev(NULL);
		}
		else
		{
			if (nx)
				nx->prev(pr);
			if(pr)
				pr->next(nx);
		}

		if(_back == ptr)
		{
			_back = pr;
			if(pr)
				pr->next(NULL);
		}

		if(_head)
			_head->prev(NULL);

		if(_back)
			_back->next(NULL);

		ptr->next(NULL);
		ptr->prev(NULL);
		ptr->pList = NULL;
		m_nCnt--;
		return res;
	}

	void Push_head(T* ptr)
	{
		Add(ptr);
	}

	void Push_back(T* ptr)
	{
		if(!ptr || NULL != ptr->pList )
			return;

		ptr->prev(_back);
		if(_back)
		{
			_back->next(ptr);
			_back = ptr;
		}
		else
		{
			_head = _back = ptr;
		}

		ptr->next(NULL);
		ptr->pList = this;
		m_nCnt++;
	}

	T* Pop_head()
	{
		T* pRes = _head;
		Remove(_head);
		return res;
	}

	T* Pop_back()
	{
		T* res = _back;
		Remove(_back);
		return res;
	}

	T* GetNext(T* ptr) const
	{
		if(!ptr)
			return _head;

		if( this != ptr->pList )
			return NULL;

		return ptr->next();
	}

	T* GetPrev(T* ptr) const
	{
		if(!ptr)
			return _back;

		if( this != ptr->pList )
			return NULL;

		return ptr->prev();
	}

	void InsertAfter( T* p, T* c)
	{
		if( !p || this != p->pList || !c || NULL != c->pList)
			return;

		if(!p->next())
		{
			Push_back(c);
			return;
		}

		c->next(p->next());
		c->prev(p);
		p->next()->prev(c);
		p->next(c);
		p->pList = this;
		m_nCnt++;
	}

	void InsertBefore(T* p, T* c)
	{
		if( !p || this != p->pList || !c || NULL != c->pList)
			return;

		if(!p->prev())
		{
			Push_head(c);
			return;
		}

		c->prev(p->prev());
	}

	int32 GetCount() const { return m_nCnt; }
protected:
	int32	m_nCnt;
	T*		_head;
	T*		_back;
};
