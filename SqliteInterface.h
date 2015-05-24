#pragma once
#include "DBInterface.h"

struct sqlite3;
struct sqlite3_stmt;

class SqliteInterface : public DBInterface
{
public:
	SqliteInterface();
	virtual ~SqliteInterface();

	virtual bool Open(const char* dbFilePath);
	virtual void Close();

	virtual bool ExecuteSql(const char* sqlStr, DBTable& table);
protected:
	void ReportError();
	void GetTableBrief(DBTable& table);
	bool GetResult(DBTable& table);

	sqlite3*		m_db;
	sqlite3_stmt*	m_stmt;
};

