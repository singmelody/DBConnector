#include "stdafx.h"
#include "SqliteInterface.h"
#include "sqlite3lib/sqlite3.h"

SqliteInterface::SqliteInterface()
	: DBInterface()
{
	m_db = NULL;
	m_stmt = NULL;
}

SqliteInterface::~SqliteInterface()
{
	Close();
}


bool SqliteInterface::Open(const char* dbFilePath)
{
	if (sqlite3_open(dbFilePath, &m_db) != SQLITE_OK)
	{
		ReportError();
		return false;
	}

	return true;
}


void SqliteInterface::Close()
{
	if (m_db)
		sqlite3_close(m_db);
}

bool SqliteInterface::ExecuteSql(const char* sqlStr, DBTable& table)
{
	if (sqlite3_prepare(m_db, sqlStr, strlen(sqlStr), &m_stmt, NULL) != SQLITE_OK)
	{
		ReportError();
		return false;
	}
	return true;
}


void SqliteInterface::ReportError()
{
	const char* error = sqlite3_errmsg(m_db);
	printf("%s\n",error);
}

