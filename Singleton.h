#pragma once

template <class T>
class Singleton
{
public:
	static inline T& Instance()
	{
		return *InstancePtr();
	}
	static inline T* InstancePtr()
	{
		if(!m_ptr)
		{
			m_ptr = (T*)malloc(sizeof(T));
			if(m_ptr)
				new(m_ptr)T();
		}
		return m_ptr;
	}

protected:
	Singleton() {}
	virtual ~Singleton(){
	}
	Singleton(const Singleton<T>&);
	Singleton<T>& operator=(const Singleton<T>&);

private:
	static T* m_ptr;
};

template <class T>
T* Singleton<T>::m_ptr = NULL;
