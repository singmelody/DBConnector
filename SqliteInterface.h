#pragma once
#include "DBInterface.h"
#include "sqlite3.h"
class SqliteInterface : public DBInterface
{
public:
	SqliteInterface();
	virtual ~SqliteInterface();

	virtual bool Open(const char* dbFilePath);
	virtual void Close();

protected:
	sqlite3*		m_db;
	sqlite3_stmt*	m_stmt;
};

