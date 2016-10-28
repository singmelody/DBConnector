// Game.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "SqliteInterface.h"
#include "DBDataType.h"
#include <iostream>
#include <sstream>

int _tmain(int argc, _TCHAR* argv[])
{
	FILE* p;
	freopen_s( &p, "1.txt", "w", stdout);
	std::cout << argc << "" << *argv << std::endl;
	return 0;
}

