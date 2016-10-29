#include "StdAfx.h"
#include "Log.h"
#include <assert.h>
#include "MyLog.h"

#define MAX_LOG_NAME_LEN	32
#define LOG_BUFF_LENGTH		8192

LogBase* g_pLog = NULL;

void LogSystem_Flush()
{
	if(!g_pLog)
		return;

	g_pLog->FlushLog_All();
}

void CacheLog(int32 nLogID, int32 nDomainID, const char* msg, va_list arg)
{
	if(!g_pLog)
		return;

	assert( nLogID >= 0 && nLogID < LOG_COUNT);
	if( nLogID < 0 || nLogID >= LOG_COUNT )
		return;

	char szLogBuffer[LOG_BUFF_LENGTH];
	if(sFormatLog( szLogBuffer, sizeof(szLogBuffer), msg, arg, "[LogInfo]") > 0)
		g_pLog->CacheLog( nLogID, szLogBuffer);
}

int sFormatLog( char* szBuffer, size_t bufferLen, 
	const char* szFmt, va_list argPtr, const char* szPrefix)
{
	assert( szBuffer && bufferLen > 10 && szFmt && szPrefix);

	int32 nWriteLen = 0;
	size_t nCurrPos = 0;

	nWriteLen = _snprintf( szBuffer, bufferLen-1, "%s", szPrefix);

	if(nWriteLen < 0)
	{
		printf("********error log***********\n");
	}

	// buffer is full
	if(nWriteLen >= (int32)bufferLen - 1)
	{
		szBuffer[bufferLen-2] = '\n';
		szBuffer[bufferLen-1] = '\0';
	}

	nCurrPos = nWriteLen;

	// Ð´ÏûÏ¢ÄÚÈÝ
	nWriteLen = _vsnprintf( szBuffer + nCurrPos, bufferLen - nCurrPos, szFmt, argPtr);
	if(nWriteLen < 0)
	{
		printf("*****************error write log <msg>*****************");
		nWriteLen = (int32)(bufferLen - nCurrPos);
	}

	nCurrPos += nWriteLen;

	// add \n
	if( nCurrPos >= bufferLen -1)
		nCurrPos = bufferLen - 2;

	szBuffer[nCurrPos++] = '\n';
	szBuffer[nCurrPos] = '\0';
	assert( nCurrPos < bufferLen);

	return (int32)nCurrPos;
}

void myInvalidParameterHandler(
	const wchar_t* express, const wchar_t* func,  const wchar_t* file,
	unsigned int line, uintptr_t pReserved)
{
	wprintf(L"Invalid parameter detected in function:%s fiel:%s line:%d\n", func, file, line);
	wprintf(L"Expression:%s\n", express);
}

Log::Log(void)
{
// 	LARGE_INTEGER q;
// 	::QueryPerformanceFrequency(&q);

	_set_invalid_parameter_handler(myInvalidParameterHandler);

	for (int32 i = 0; i < LOG_COUNT; ++i)
	{
		m_logCache[i] = NULL;
		m_logPos[i] = 0;
		m_logDiskSize[i] = 0;
		m_logDiskHold[i] = 0;
	}

	m_pLogSaveDisk = NULL;
	m_cacheSize = 0;
	m_curDay = 0;
}


Log::~Log(void)
{
	for (int32 i = 0; i < LOG_COUNT; ++i)
	{
		if(!m_logCache[i])
			continue;

		SAFE_DELETE(m_logCache[i]);
	}

	if(m_pLogSaveDisk)
		SAFE_DELETE_ARRAY(m_pLogSaveDisk);
	
	m_cacheSize = 0;
}

bool Log::Init(int32 nCacheSize, const char* szName)
{
	if( 0 < nCacheSize )
		m_cacheSize = nCacheSize;
	else
		m_cacheSize = DEFAULT_LOG_CACHE_SIZE;

	for (int32 i = 0; i < LOG_COUNT; ++i)
	{
		m_logCache[i] = new char[m_cacheSize];
		if(!m_logCache[i])
			return false;

		memset( m_logCache[i], 0, m_cacheSize);
		m_logPos[i] = 0;
		m_logDiskSize[i] = 0;
		m_logDiskHold[i] = 0;
	}

	m_pLogSaveDisk = new char[m_cacheSize];
	if(!m_pLogSaveDisk)
		return false;
	memset(	m_pLogSaveDisk, 0, m_cacheSize);
	
	if(!szName)
		return false;

	m_logName = szName;
	return true;
}

void Log::CacheLog(int32 logID, const char* msg)
{
	if( logID < 0 || logID >= LOG_COUNT)
		return;

	int32 nLen = strlen(msg);
	if(nLen <= 0)
		return;

	m_logLock[logID].Lock();
	if(!m_logCache[logID])
	{
		m_logCache[logID] = new char[m_cacheSize];
		if(!m_logCache[logID])
			return m_logLock[logID].Unlock();

		memset( m_logCache[logID], 0, m_cacheSize);
		m_logPos[logID] = NULL;
		m_logDiskSize[logID] = NULL;
		m_logDiskHold[logID] = NULL;
	}

	if( m_logPos[logID] > m_cacheSize - nLen )
	{
		m_logLock->Unlock();
		return;
	}

	if(!m_logCache[logID])
	{
		m_logLock[logID].Unlock();
		return;
	}

	memcpy( m_logCache[logID] + m_logPos[logID], msg, nLen);
	m_logPos[logID] += nLen;

	m_logDiskSize[logID] += nLen;
	if(m_logDiskSize[logID] > DEFAULT_LOG_CACHE_SIZE)
	{
		m_logDiskSize[logID] = nLen;
		m_logDiskHold[logID]++;
	}

	m_logLock[logID].Unlock();

	DisplayLog(msg);
}

void Log::FlushLog(int32 logid)
{

}

void Log::FlushLog_All()
{

}

void LogBase::DisplayLog(const char* szMsg)
{
	printf("%s", szMsg);
}
