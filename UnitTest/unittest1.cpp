#include "stdafx.h"
#include "CppUnitTest.h"
#include "SqliteInterface.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO:  �ڴ�������Դ���
			DBInterface* pInterface = new SqliteInterface();
			pInterface->Open("..\\test");
			delete pInterface;
		}

	};
}