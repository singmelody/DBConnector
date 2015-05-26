#include "stdafx.h"
#include "CppUnitTest.h"
#include "SqliteInterface.h"
#include "DBTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

	};
}