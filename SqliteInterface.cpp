#include "stdafx.h"
#include "SqliteInterface.h"
#include "sqlite3lib/sqlite3.h"

SqliteInterface::SqliteInterface()
{
}


SqliteInterface::~SqliteInterface()
{
}

bool SqliteInterface::Open(const char* dbFilePath)
{
	sqlite3_open(, &m_db);
	return true;
}


void SqliteInterface::Close()
{

}