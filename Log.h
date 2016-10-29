#pragma once
#include <string>
#include "BaseType.h"
#include "MyMutex.h"

#define DEFAULT_LOG_CACHE_SIZE 1024*1024*4

void CacheLog(int32 nLogID, int32 nDomainID, const char* msg, va_list arg);
int sFormatLog( char* szBuffer, size_t bufferLen, 
	const char* szFmt, va_list argPtr, const char* szPrefix);
void LogSystem_Flush();


enum LOG_FILE_NAME_ID
{
	LOG_FILE_ALL,
	LOG_COUNT
};


class LogBase
{
public:
	LogBase(){}
	virtual ~LogBase(){}

	virtual bool Init( int32 nCacheSize, const char* szName) { return false; }
	virtual void CacheLog( int32 logID, const char* msg){}
	virtual void FlushLog( int32 logid){}
	virtual void FlushLog_All(){}


protected:
	// 显示日志
	void DisplayLog(const char* szMsg);
	// 获取当前的日志文件名
	int32			m_curDay;
	std::string		m_logName;
	int32			m_logDiskSize[LOG_COUNT];
	int32			m_logDiskHold[LOG_COUNT];
};

class Log : public LogBase
{
public:
	Log();
	virtual ~Log();

	virtual bool Init( int32 nCacheSize, const char* szName);
	virtual void CacheLog( int32 logID, const char* msg);
	virtual void FlushLog( int32 logid);
	virtual void FlushLog_All();

	void DiskLog(int32 nlogid, const char* msg);

private:
	char*	m_logCache[LOG_COUNT];
	int32	m_logPos[LOG_COUNT];
	Mutex	m_logLock[LOG_COUNT];

	char*   m_pLogSaveDisk;
	int32	m_cacheSize;
};

extern LogBase* g_pLog;