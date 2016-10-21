#include "stdafx.h"
#include "SqliteInterface.h"
#include "sqlite3lib/sqlite3.h"
#include "DBDataType.h"

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


bool SqliteInterface::Open(const char* dbFilePath, const char* pPassword)
{
	if (sqlite3_open(dbFilePath, &m_db) != SQLITE_OK)
	{
		ReportError();
		return false;
	}

	size_t nKeyLen = strlen(pPassword);
	if (nKeyLen > 0)
	{
		if (sqlite3_key(m_db, pPassword, int(nKeyLen)) != 0)
		{
			sqlite3_close(m_db);
			printf("Password Is Not Right!\n");
			return false;
		}
	}

	return true;
}


bool SqliteInterface::Close()
{
	if (m_db)
		sqlite3_close(m_db);

	return true;
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
		const char* pName = sqlite3_column_name( m_stmt, i);
		const char* sType = sqlite3_column_decltype( m_stmt, i);

		int32 nDBType = eDB_UnKonw;

		if (const_cast<char*>(strstr(sType, "INT")))
		{
			if (strstr(sType, "(8)"))
				nDBType = eDB_INT64;
			else if (strstr(sType, "(4)"))
				nDBType = eDB_INT;
			else if (strstr(sType, "(2)"))
				nDBType = eDB_SMALLINT;
			else if (strstr(sType, "(1)"))
				nDBType = eDB_TINYINT;
		}
		else if (strstr(sType, "integer"))
		{
			nDBType = eDB_INT;
		}
		else if (strstr(sType, "REAL"))
		{
			if (strstr( sType, "(8)"))
				nDBType = eDB_DOUBLE;
			else if (strstr(sType, "(4)"))
				nDBType = eDB_FLOAT;
		}
		else if (strstr(sType, "DOUBLE"))
		{
			nDBType = eDB_DOUBLE;
		}
		else if (strstr(sType, "VARCHAR"))
		{
			nDBType = eDB_TEXT;
		}
		else if (strstr(sType, "blob"))
		{
			nDBType = eDB_TEXT;
		}

		table.AddColumn(pName, i, nDBType);
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

		pRow->SetDBTable(&table);
		pRow->Reserve(columnCount);

		for (int i = 0; i < columnCount; ++i)
		{
			int32 nDataType = sqlite3_column_type( m_stmt, i);
			if (nDataType == SQLITE_NULL)
			{
				pRow->AddColumn();
				continue;
			}

			int32 nType = table.GetColumnType(i);
			if (nType == eDB_TEXT || nType == eDB_BLOB)
			{
				int32 nLen = sqlite3_column_bytes( m_stmt, i);
				const unsigned char* text = sqlite3_column_text( m_stmt, i);

				bool bIsStr = ((nType == eDB_TEXT) ? true : false);

				pRow->AddColumn( (char*)(const_cast<unsigned char*>(text)), nLen, bIsStr);
			}
			else if (nType == eDB_INT64)
			{
				pRow->AddColumn(sqlite3_column_int64( m_stmt, i));
			}
			else if (nType == eDB_INT || nType == eDB_SMALLINT || nType == eDB_TINYINT)
			{
				pRow->AddColumn(sqlite3_column_int( m_stmt, i));
			}
			else if (nType == eDB_FLOAT || nType == eDB_DOUBLE)
			{
				pRow->AddColumn(sqlite3_column_double(m_stmt, i));
			}
		}

		table.m_rowList.push_back(pRow);
	}

	sqlite3_finalize(m_stmt);
	m_stmt = NULL;

	return true;
}
