#include "stdafx.h"
#include "ODBCInterface.h"
#include "DBDataType.h"

ODBCInterface::ODBCInterface()
{
	m_hEnv = NULL;
	m_hDbc = NULL;
	m_result = SQL_SUCCESS;

	m_rowCount = NULL;
	m_hStmt = NULL;
	m_colCount = NULL;

	m_connected = false;
}


ODBCInterface::~ODBCInterface()
{
	if (m_hStmt)
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);

	if (m_hDbc)
		SQLDisconnect(m_hDbc);

	if (m_hDbc)
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);

	if (m_hEnv)
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
}

bool ODBCInterface::Open(const char* pConnectStr, const char* pUsername, const char* pPassword)
{
	Close();

	m_connectStr = pConnectStr;
	m_username = pUsername;
	m_password = pPassword;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc);

	m_result = SQLConnect( m_hDbc, (SQLCHAR*)pConnectStr, SQL_NTS, (SQLCHAR*)pUsername, SQL_NTS, (SQLCHAR*)pPassword, SQL_NTS);

	if( m_result != SQL_SUCCESS && m_result != SQL_SUCCESS_WITH_INFO )
	{
		printf("ErrorSql:ConnectDBError %s@%s, %s", pConnectStr, pUsername, pPassword);
		DiagState();

		return false;
	}

	m_result = SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt);

	if( m_result != SQL_SUCCESS && m_result != SQL_SUCCESS_WITH_INFO )
	{
		m_hStmt = NULL;
		return false;
	}

	m_connected = true;

	return true;
}

bool ODBCInterface::ExecuteSqlInternal(const char* pSql, DBTable* pTable)
{
	if (!pTable || !pTable)
		return false;

	try
	{
		Clear();

		m_result = SQLExecDirect( m_hStmt, (SQLCHAR*)pSql, SQL_NTS);
		
		if (m_result != SQL_SUCCESS && m_result != SQL_SUCCESS_WITH_INFO && m_result != SQL_NO_DATA)
		{
			DiagState();
			printf("this sql is error:%s", pSql);
			return false;
		}

		m_rowCount = m_colCount = 0;
		if (SQL_SUCCESS != SQLRowCount(m_hStmt, &m_rowCount))
		{
			printf("sql row count failed");
			return false;
		}

		if (SQL_SUCCESS != SQLNumResultCols(m_hStmt, &m_colCount))
		{
			printf("sql col count failed");
			return false;
		}

		if (m_colCount > MAXCOL)
			return false;

		if (m_colCount <= 0 && m_rowCount <= 0)
		{
			Clear();
			return true;
		}

		SQLCHAR rColName[MAX_COLUMN_NAMELEN];
		SQLSMALLINT Type;

		int32 nDBType = eDB_UnKonw;

		bool arrUnsignedFlag[MAXCOL + 1];
		for (int32 i = 0; i < m_colCount; ++i)
		{
			m_result = SQLDescribeCol(m_hStmt, i + 1, rColName, MAX_COLUMN_NAMELEN, NULL, &Type, NULL, NULL, NULL);

			if ( ( m_result != SQL_SUCCESS) && ( m_result != SQL_SUCCESS_WITH_INFO) && ( m_result != SQL_NO_DATA ))
				continue;

			arrUnsignedFlag[i] = false;

			// 对可能出现unsinged的数据列进行标记,方便dbtable统一处理
			switch (Type)
			{
			case SQL_INTEGER:
			case SQL_BIGINT:
				{
					SQLLEN len = 0;
					SQLRETURN ret = SQLColAttribute( m_hStmt, i+1, SQL_DESC_UNSIGNED, 0, 0, 0, &len);
					if (ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)
						arrUnsignedFlag[i] = ( len == SQL_TRUE);
				}
				break;
			default:
				break;
			}

			switch (Type)
			{
			case SQL_INTEGER:
			case SQL_NUMERIC:
				nDBType = eDB_INT;
				break;
			case SQL_BIGINT:
				nDBType = eDB_INT64;
				break;
			case SQL_BINARY:
			case SQL_VARBINARY:
			case SQL_LONGVARBINARY:
				nDBType = eDB_BINARY;
				break;
			case SQL_CHAR:
			case SQL_VARCHAR:
			case SQL_LONGVARCHAR:
				nDBType = eDB_TEXT;
				break;
			case SQL_SMALLINT:
				nDBType = eDB_SMALLINT;
				break;
			case SQL_TINYINT:
				nDBType = eDB_TINYINT;
				break;
			case SQL_REAL:
			case SQL_FLOAT:
				nDBType = eDB_FLOAT;
				break;
			case SQL_DOUBLE:
				nDBType = eDB_DOUBLE;
			default:
				nDBType = eDB_UnKonw;
				break;
			}

			pTable->AddColumn( (char*)rColName, i, nDBType);
		}

		return GetResult( pTable, arrUnsignedFlag);
	}
	catch (...)
	{
		printf("something bad happen");
	}

	return true;
}

void ODBCInterface::DiagState()
{
	int i = 1;
	SQLINTEGER navError;
	SQLCHAR SqlState[6];
	SQLSMALLINT MsgLen;
	SQLCHAR errorMsg[255];
	memset( errorMsg, 0, sizeof(errorMsg));
	while (SQLGetDiagRec( SQL_HANDLE_DBC, m_hDbc, i, SqlState, &navError, errorMsg, sizeof(errorMsg), &MsgLen) != SQL_NO_DATA)
	{
		i++;
		if(!m_hDbc)
		{
			printf("hdc is null");
			break;
		}
	}

	errorMsg[254] = '\0';

	if (strlen((const char*)errorMsg) == 0)
		m_result = SQLError( m_hEnv, m_hDbc, m_hStmt, SqlState, &navError, errorMsg, sizeof(errorMsg), &MsgLen);

	m_errorCode = navError;

	printf("ErrorSql:ErrorCode = %d\nErrorMsg = %s", m_errorCode, errorMsg);

	printf("Mysql Error %s", SqlState);
}

void ODBCInterface::Clear()
{
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_UNBIND);
}

bool ODBCInterface::Close()
{
	if (m_hStmt)
	{
		try
		{
			SQLCloseCursor(m_hStmt);
			SQLFreeStmt(m_hStmt, SQL_UNBIND);
			SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
			m_hStmt = NULL;
			printf("DBA Close Database Connection");
		}
		catch (...)
		{
			m_hStmt = NULL;
		}
	}

	if (m_hDbc)
	{
		try
		{
			SQLDisconnect(m_hDbc);
			SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
			m_hDbc = NULL;
		}
		catch (...)
		{
			m_hDbc = NULL;
		}
	}

	if (m_hEnv)
	{
		try
		{
			SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
			m_hEnv = NULL;
		}
		catch (...)
		{
			m_hEnv = NULL;
		}
	}

	m_connected = false;

	return true;
}


bool ODBCInterface::GetResult(DBTable* pTable, bool* arrColumnSignedFlag /*= 0*/)
{
	return GetResult( pTable, MAXROW, arrColumnSignedFlag);
}

bool ODBCInterface::GetResult(DBTable* pTable, int32 nMaxRow, bool* arrFlag)
{
	if(!m_connected || !pTable)
		return false;

	if( m_rowCount == 0 || m_colCount == 0)
		return false;

	nMaxRow = (m_rowCount > nMaxRow) ? nMaxRow : m_rowCount;
	for(int32 i = 0; i < nMaxRow; ++i)
	{
		m_result = SQLFetch(m_hStmt);

		if (m_result != SQL_SUCCESS_WITH_INFO && m_result != SQL_SUCCESS && m_result != SQL_NO_DATA)
			break;

		if (m_result == SQL_NO_DATA)
			break;

		DBRow* pRow = new DBRow;
		if(!pRow)
			assert(false);

		pRow->SetDBTable(pTable);

		for (int32 i = 0; i < m_colCount; ++i)
		{
			int32 totalGet = 0;
			volatile SQLLEN	lenData = 0;
			SQLSMALLINT type = (SQLSMALLINT)pTable->GetColumnType(i);

			bool bUnSignedInt = (arrFlag == 0)? false: arrFlag[i];

			switch(type)
			{
			case eDB_INT:
				{
					SQLINTEGER val = 0;
					SQLRETURN ret = SQLGetData( m_hStmt, i+1, bUnSignedInt ? SQL_C_ULONG:SQL_C_SLONG, &val, sizeof(val),  (SQLLEN*)(&lenData) );
					lenData = (SQLLEN)(*(SQLINTEGER*)((char*)(&lenData)));
					if ( ( ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) && lenData != SQL_NULL_DATA)
						pRow->AddColumn((int)val);
					else
						pRow->AddColumn();
				}
				break;
			case eDB_INT64:
				{
					SQLBIGINT val = 0;
					SQLRETURN ret = SQLGetData( m_hStmt, i+1, bUnSignedInt ? SQL_C_UBIGINT:SQL_C_SBIGINT, &val, sizeof(val), (SQLLEN*)(&lenData ));
					lenData = (SQLLEN)(*(SQLINTEGER*)((char*)(&lenData)));
					if ( ( ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) && lenData != SQL_NULL_DATA)
						pRow->AddColumn((int64)val);
					else
						pRow->AddColumn();
				}
				break;
			case eDB_FLOAT:
			case eDB_DOUBLE:
				{
					SQLDOUBLE val = 0;
					SQLRETURN ret = SQLGetData( m_hStmt, i+1, SQL_C_DOUBLE, &val, sizeof(val) , (SQLLEN*)(&lenData));
					lenData = (SQLLEN)(*(SQLINTEGER*)((char*)(&lenData)));
					if ( ( ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) && lenData != SQL_NULL_DATA)
						pRow->AddColumn((int64)val);
					else
						pRow->AddColumn();
				}
				break;
			case eDB_BINARY:
				{
					SQLRETURN ret = SQLGetData( m_hStmt, i+1, SQL_C_BINARY, m_databuffer, MAX_COLUMN_BUFFER, (SQLLEN*)(&lenData));
					lenData = (SQLLEN)(*(SQLINTEGER*)((char*)(&lenData)));
					if ( ( ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) && lenData != SQL_NULL_DATA)
						pRow->AddColumn( (char*)m_databuffer, lenData, true);
					else
						pRow->AddColumn();
				}
				break;
			case eDB_TEXT:
				{
					SQLRETURN ret = SQLGetData( m_hStmt, i+1, SQL_C_CHAR, m_databuffer, MAX_COLUMN_BUFFER, (SQLLEN*)(&lenData));
					lenData = (SQLLEN)(*(SQLINTEGER*)((char*)(&lenData)));
					if ( ( ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) && lenData != SQL_NULL_DATA)
						pRow->AddColumn( (char*)m_databuffer, lenData, true);
					else
						pRow->AddColumn();
				}
				break;
			case eDB_SMALLINT:
			case eDB_TINYINT:
				{
					SQLINTEGER val = 0;
					SQLRETURN ret = SQLGetData( m_hStmt, i+1, SQL_C_LONG, &val, sizeof(val), (SQLLEN*)(&lenData));
					lenData = (SQLLEN)(*(SQLINTEGER*)((char*)(&lenData)));
					if ( ( ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) && lenData != SQL_NULL_DATA)
						pRow->AddColumn((int32)val);
					else
						pRow->AddColumn();
				}
				break;
			default:
				{
					pRow->AddColumn( );
				}
				break;
			}
		}
		pTable->m_rowList.push_back(pRow);
	}

	return true;
}

bool ODBCInterface::ExecuteSql(const Char* sSql, DBTable& pTable)
{
	if ( !ExecuteSqlInternal( sSql, &pTable) )
		return false;

	return true;
}