#pragma once

#include <string>

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

