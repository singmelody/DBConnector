#pragma once

#include "Singleton.h"

#define LOG_DOMAIN_DEFAILT (0x1001)

enum LogFileNameID
{
	eLogFile_All,
	eLogFile_Count
};

enum LogLevel
{
	eLL_ERROR,
	eLL_Count
};

enum LogDomain
{
	eLD_,
	eLD_Count
};

enum LogType
{
	eLogType_Message,
	eLogType_Warning,
	eLogType_Error,
	eLogType_Count,
};

class Console
{
public:
	Console(){}
	~Console() { Reset(); }

	static void SetColor( unsigned short attr);

	static void Red();
	static void Green();
	static void Blue();
	static void Yellow();
	static void Pink();
	static void CYan();
	static void White();
	static void Reset();

	static void SetBufferSize( short x, short y);
};

#define CONSOLE_RED Console temp; temp.Red();
#define CONSOLE_GREEN Console temp; temp.Green();
#define CONSOLE_BLUE Console temp; temp.Blue();
#define CONSOLE_YELLOW Console temp; temp.Yellow();
#define CONSOLE_CYAN Console temp; temp.CYan();

class MyLog : public Singleton<MyLog>
{
public:
	static void Create(const char* pFileName);

	static void message( const char* str, ...);
	static void warning( const char* str, ...);
	static void error( const char* str, ...);

	static void FillConfig();
protected:
	MyLog();
	~MyLog();

	friend class Singleton<MyLog>;

	static bool m_logSwitch[eLogType_Count];
	static bool m_logPrintSwitch[eLogType_Count];
};

