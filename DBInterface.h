#pragma once

#define USE_SQLITE_DB		0	// sqlite may not be thread safe, please use single thread
#define USE_ODBC_DB			1
#define USE_CONNECTION		4	// may be less the num of thread , depends on the test result
#define MAX_USE_CONNECTION	16

class DBTable;

class DBInterface
{
public:
	DBInterface();
	virtual ~DBInterface();

	virtual bool Open(const char* dbFile, const char* pUserName = "", const char* pPassword = "") { return false; }
	virtual bool Close() { return false; }

	virtual bool ExecuteSql(const char* sqlStr, DBTable& table) { return false;  }
};

