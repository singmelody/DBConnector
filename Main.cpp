// DBConnector.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SqliteInterface.h"
#include "DBDataType.h"

int _tmain(int argc, _TCHAR* argv[])
{
	DBInterface* pInterface = new SqliteInterface();
	pInterface->Open("..\\test");

	DBTable table;
	int Col_Content = table.GetColumnIdx("Content");
	if (Col_Content == -1)
	{
		printf("Invalid Column Name\n");
		return false;
	}
	
	pInterface->ExecuteSql("select * from test", table);

// 	for (;;)
// 	{
// 		
// 	}

	printf("I am here, waiting for u\n");
	return 0;
}
