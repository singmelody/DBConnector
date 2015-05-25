#pragma once
class DBInterface
{
public:
	DBInterface();
	virtual ~DBInterface();

	virtual bool Open() { return false; }
	virtual void Close() {}
};