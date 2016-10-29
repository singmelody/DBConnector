#include "StdAfx.h"
#include "MyLog.h"
#include "Log.h"
#include "ConfigManager.h"

#include <Windows.h>
#include <assert.h>

#define LOG_DOMAIN( logid , domain) \
	va_list args;	\
	va_start( args, fmt); \
	CacheLog( logid, domain, fmt, args); \
	va_end( args ); \

void Console::SetColor(unsigned short attr)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, attr);
}

void Console::Red()
{
	SetColor( FOREGROUND_RED | FOREGROUND_INTENSITY );
}

void Console::Green()
{
	SetColor( FOREGROUND_GREEN | FOREGROUND_INTENSITY );
}

void Console::Blue()
{
	SetColor( FOREGROUND_BLUE | FOREGROUND_INTENSITY );
}

void Console::Yellow()
{
	SetColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
}

void Console::Pink()
{
	SetColor( FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
}

void Console::CYan()
{
	SetColor( FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void Console::White()
{
	SetColor(0x0f);
}

void Console::Reset()
{
	White();
}


void Console::SetBufferSize(short x, short y)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coord;
	coord.X = x;
	coord.Y = y;

	if (!SetConsoleScreenBufferSize( hCon, coord))
	{
		CONSOLE_RED;
		MyLog::error("SetConsoleScreenBufferSize Error %d", GetLastError());
	}
}

bool MyLog::m_logSwitch[eLogType_Count];
bool MyLog::m_logPrintSwitch[eLogType_Count];

MyLog::MyLog(void)
{
	memset( m_logSwitch, 0, sizeof(m_logSwitch));
	memset( m_logPrintSwitch, 0, sizeof(m_logPrintSwitch));
}


MyLog::~MyLog(void)
{
}

void MyLog::Create(const char* pFileName)
{
	Instance();

	if(!g_pLog)
		assert(false);

	char szFile[128];
	char filePath[128];
	GetModuleFileName( NULL, filePath, sizeof(filePath));
	_snprintf_s( szFile, 127, "%s\\%s",  filePath, pFileName);

	bool ret = g_pLog->Init( DEFAULT_LOG_CACHE_SIZE, szFile);
	assert(ret);
}

void MyLog::message(const char* fmt, ...)
{
	LOG_DOMAIN( eLogFile_All, LOG_DOMAIN_DEFAILT);
}

void MyLog::warning(const char* fmt, ...)
{
	CONSOLE_YELLOW;
	LOG_DOMAIN( eLogFile_All, LOG_DOMAIN_DEFAILT);
}

void MyLog::error(const char* fmt, ...)
{
	CONSOLE_RED;
	LOG_DOMAIN( eLogFile_All, LOG_DOMAIN_DEFAILT);
}

void MyLog::FillConfig()
{
	int32 iConfig = 0;
	ConfigManager::GetConfigValue( "LogConfig", "LogActive", iConfig, true);
	
	char buff[32];
	memset( buff, 0, 32);

	for (int32 i = 0; i < eLogType_Count; ++i)
	{
		int32 nConfig;

		// file switch
		sprintf( buff, "logSwitch%d", i);
		ConfigManager::GetConfigValue( "LogConfig", buff, nConfig, true);
		m_logSwitch[i] = nConfig > 0;

		// print switch
		sprintf( buff, "logPrintSwitch%d", i);
		ConfigManager::GetConfigValue( "LogConfig", buff, nConfig, true);
		m_logPrintSwitch[i] = nConfig > 0;
	}
}
