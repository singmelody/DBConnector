#pragma once

#include <string>

class GameUtil
{
public:
	GameUtil();
	~GameUtil();

	static void ConvertUtf8ToGBK(std::string &strUtf8);
};

