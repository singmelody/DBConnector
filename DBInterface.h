#pragma once

class DBTable;

class DBInterface
{
public:
	DBInterface();
	virtual ~DBInterface();

	virtual bool Open(const char* dbFile) { return false; }
	virtual void Close() {}

	virtual bool ExecuteSql(const char* sqlStr, DBTable& table) { return false;  }
};