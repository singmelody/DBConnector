#pragma once
class NoCopyable
{
protected:
	NoCopyable() {}
	virtual ~NoCopyable(){}

private:
	NoCopyable(const NoCopyable&);
	const NoCopyable& operator= (const NoCopyable&);

};

