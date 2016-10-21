#pragma once
#include <string>
#include "DBInterface.h"
#include <windows.h>
#include "sql.h"
#include "sqlext.h"
#include "BaseType.h"


class ODBCInterface : public DBInterface
{
	static const int MAXCOL = 500;
	static const int MAXROW = 50000;
	static const int MAX_COLUMN_BUFFER = 2049;
	static const int MAX_COLUMN_NAMELEN = 30;
	static const int DEFAULT_ROW_BUFFER_SIZE = 512;
public:
	ODBCInterface();
	~ODBCInterface();

	virtual bool Open(const char* pConnectStr, const char* pUsername, const char* pPassword);
	virtual bool Close();

	bool GetResult(DBTable* pTable, bool* arrColumnSignedFlag = 0);
	bool GetResult(DBTable* pTable, int32 nMaxRow, bool* arrFlag );

	virtual bool ExecuteSql(const char* sSql, DBTable& pTable);
	virtual bool ExecuteSqlInternal( const char* pSql, DBTable* pTable);

	virtual bool IsConnect() { return m_connected; }

	void DiagState();
protected:
	void Clear();

	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLRETURN	m_result;

	SQLLEN		m_rowCount;
	SQLHSTMT	m_hStmt;
	SQLSMALLINT m_colCount;
	bool		m_connected;

	std::string m_connectStr;
	std::string m_username;
	std::string m_password;

	char		m_databuffer[MAX_COLUMN_BUFFER];
	SQLINTEGER	m_dataLength;
	SQLINTEGER	m_errorCode;

	//DB_DS		m_db_static;
};

/*
class ODBCConnectionManager : public DBConnectionManager
{
public:
	ODBCConnectionManager(int32 n);
	virtual ~ODBCConnectionManager();

	virtual bool Init( const char* dbName, const char* user, const char* pwd, const char* codepageSql);

	virtual void Close();
	virtual DBConnection* GetDBInterface();

};
*/