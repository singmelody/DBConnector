#include "stdafx.h"
#include "SqliteInterface.h"
#include "sqlite3lib/sqlite3.h"
#include "DBTable.h"

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
	// get valid stmt
	if (sqlite3_prepare(m_db, sqlStr, strlen(sqlStr), &m_stmt, NULL) != SQLITE_OK)
	{
		ReportError();
		return false;
	}

	bool b = false;
	// get table brief
	GetTableBrief(table);

	// get result
	b = GetResult(table);

	return b;
}


void SqliteInterface::ReportError()
{
	const char* error = sqlite3_errmsg(m_db);
	printf("%s\n",error);
}

void SqliteInterface::GetTableBrief(DBTable& table)
{
	int columnCount = sqlite3_column_count(m_stmt);

	for (int i = 0; i < columnCount; ++i)
	{
		DBColumn* pColumn = new DBColumn();
		if (!pColumn)
			return;

		const char* columnName = sqlite3_column_decltype( m_stmt, i);

		printf("%s\n", columnName);
	}
}

bool SqliteInterface::GetResult(DBTable& table)
{
	while (sqlite3_step(m_stmt) != SQLITE_DONE)
	{
		int columnCount = sqlite3_column_count(m_stmt);
		if (columnCount <= 0)
		{
			printf("empty table\n");
			return false;
		}

		DBRow* pRow = new DBRow();
		if (!pRow)
		{
			printf("not enough memory\n");
			return false;
		}

		for (int i = 0; i < columnCount; ++i)
		{
			DBColumn* pColumn = new DBColumn();
			if (!pColumn)
			{
				printf("not enough memory\n");
				return false;
			}

			int nType = sqlite3_column_type(m_stmt, i);
		}

		table.m_rowList.push_back(pRow);
	}

	sqlite3_finalize(m_stmt);
	m_stmt = NULL;

	return true;
}
