#include "stdafx.h"
#include "CppUnitTest.h"
#include "SqliteInterface.h"
#include "DBTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#define SQLITE_PASS ""
namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO:  在此输入测试代码
			DBInterface* pInterface = new SqliteInterface();
			
			Assert::IsTrue(pInterface->Open("..\\test"));

			DBTable table;
			Assert::IsTrue(pInterface->ExecuteSql("select * from test", table));

			delete pInterface;
		}

		TEST_METHOD(TestMethod2)
		{
			DBInterface* pInterface = new SqliteInterface();
			pInterface->Open("..\\Client.pxk", SQLITE_PASS);

			DBTable table;
			Assert::IsTrue(pInterface->ExecuteSql("select * from AchieveGrp", table));

// 			std::vector<DBRow*>& rows = table.m_rowList;
// 
// 			for (auto itr = rows.begin(); itr != rows.end(); ++itr)
// 			{
// 				if (!*itr)
// 					continue;
// 
// 				static int Col_Grp = rows.getColumnIndex("id");
// 				static int 
// 			}
		}

	};
}