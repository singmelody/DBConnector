#pragma once
#include <vector>

class DBRowData
{
public:

};

class DBRow
{
public:
	DBRowData* m_rowData;
};

class DBColumn
{
public:

};

class DBTable
{
public:
	DBTable();
	~DBTable();

	std::vector<DBRow*> m_rowList;
protected:

};

