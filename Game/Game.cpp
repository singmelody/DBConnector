// Game.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "SqliteInterface.h"
#include "DBTable.h"

#include <memory>

int _tmain(int argc, _TCHAR* argv[])
{
	std::auto_ptr<DBInterface> pInterface(new SqliteInterface());



	return 0;
}

