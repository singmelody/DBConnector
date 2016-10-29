#pragma once

#include <string>
#include <set>

class MyPath
{
public:
	MyPath()
	{
		m_MainPath = "./";
	}

	~MyPath(){}

	static const char* MainPath() { return m_MainPath.c_str(); }
	static void SetMainPath(const char* path);
	static void GetDirFiles(const std::string& path, std::set<std::string>& files);
protected:
	static std::string m_MainPath;
};

class GameUtil
{
public:
	GameUtil();
	~GameUtil();

	static bool GetCommandOpt(const std::string& strOptName, std::string& strOptValue, int32 nArgc, char* argv[]);
	static bool GetCommandOpt(const std::string& strOptName, int32 nArgc, char* argv[]);

	static std::string TrimStr(const std::string& str, const std::string strTrim);

	static void ConvertUtf8ToGBK(std::string &strUtf8);
};

