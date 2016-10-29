#include "stdafx.h"
#include "CppUnitTest.h"
#include "SqliteInterface.h"
#include "ODBCInterface.h"
#include "DBDataType.h"
#include "GameUtil.h"
#include "MyListNode.h"
#include "ConfigManager.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest_Sqlite)
	{
	public:
		
		TEST_METHOD(SqliteQuery)
		{
			// TODO:  在此输入测试代码
			DBInterface* pInterface = new SqliteInterface();
			
			Assert::IsTrue(pInterface->Open("..\\test"));

			DBTable table;
			Assert::IsTrue(pInterface->ExecuteSql("select * from test", table));

			SAFE_DELETE(pInterface);
		}

		TEST_METHOD(EncryptSqliteQuery)
		{
			DBInterface* pInterface = new SqliteInterface();
			pInterface->Open("..\\Client.pxk", "", SQLITE_PASS);

			DBTable table;
			Assert::IsTrue(pInterface->ExecuteSql("select * from AchieveGrp", table));

			DBRowList& rows = table.m_rowList;

			for (auto itr = rows.begin(); itr != rows.end(); ++itr)
			{
				DBRow* row = *itr;
				if (!row)
					continue;

				static int Col_Grp = row->GetColumnIdx("id");
				static int Col_GrpDic = row->GetColumnIdx("dic");
				static int Col_SrvCheckGrp = row->GetColumnIdx("srvCheckGroup");
				static int Col_CltCheckGrp = row->GetColumnIdx("cltCheckGroup");
				static int Col_Des = row->GetColumnIdx("des");

				int32 nGrp = -1, nGrpDic = -1, nSrvCheckGrp = -1, nCltCheckGrp = -1;
				std::string strDes;
				row->Fill( nGrp, Col_Grp, -1);
				row->Fill( nGrpDic, Col_GrpDic, -1);
				row->Fill( nSrvCheckGrp, Col_SrvCheckGrp, -1);
				row->Fill( nCltCheckGrp, Col_CltCheckGrp, -1);
				row->Fill(strDes, Col_Des, "");

				std::stringstream ss;
				GameUtil::ConvertUtf8ToGBK(strDes);
				ss << nGrp << "\t" << nGrpDic << "\t" << nSrvCheckGrp << "\t" << nCltCheckGrp << "\t" <<strDes<<std::endl;
				Logger::WriteMessage( ss.str().c_str() );
			}

			DBTable table2;
			Assert::IsTrue(pInterface->ExecuteSql("select * from Dictionary Limit 0,1000", table2));

			DBRowList& rows2 = table2.m_rowList;

			for (auto itr = rows2.begin(); itr != rows2.end(); ++itr)
			{
				DBRow* row = *itr;
				if (!row)
					continue;

				static int Col_Idx = row->GetColumnIdx("idx");
				static int Col_Content = row->GetColumnIdx("Content");

				int32 nIdx = -1;
				std::string content = "";
				row->Fill(nIdx, Col_Idx, -1);
				row->Fill(content, Col_Content, "");

				std::stringstream ss;
				GameUtil::ConvertUtf8ToGBK(content);
				ss << nIdx << "\t" << content << std::endl;
				Logger::WriteMessage(ss.str().c_str());
			}

			pInterface->Close();
		}

		TEST_METHOD(TestMethod3)
		{

		}
	};

	TEST_CLASS(UnitTest_ODBC)
	{
		TEST_METHOD(ODBCQuery)
		{
			// TODO:  在此输入测试代码
			DBInterface* pInterface = new ODBCInterface();

			Assert::IsTrue(pInterface->Open("testdb", "root", "123456"));

			DBTable table;
			Assert::IsTrue(pInterface->ExecuteSql("select * from Dictionary", table));

			DBRowList& rows = table.m_rowList;

			for (auto itr = rows.begin(); itr != rows.end(); ++itr)
			{
				DBRow* row = *itr;
				if (!row)
					continue;

				static const int32 Col_Idx = row->GetColumnIdx("idx");
				static const int32 Col_ContentID = row->GetColumnIdx("Content");

				int32 nIdx = -1;
				std::string strContent = "";

				row->Fill( nIdx, Col_Idx, -1);
				row->Fill(strContent, Col_ContentID, "");

				std::stringstream ss;
				GameUtil::ConvertUtf8ToGBK(strContent);
				ss << nIdx << "\t" << strContent << std::endl;
				Logger::WriteMessage(ss.str().c_str());
			}

			SAFE_DELETE(pInterface);
		}
	};

	TEST_CLASS(UnitTest_GameUtil)
	{
		TEST_METHOD(MyListAdd)
		{
			// add
			MyList<MyListNode<int32> > pList;
			for (int32 i = 0; i < 100000; ++i)
			{
				MyListNode<int32>* pNode = new MyListNode<int32>();
				if (!pNode)
					continue;

				pList.Add(pNode);
			}

			Assert::IsTrue(pList.GetCount() == 100000);

			auto headPtr = pList.GetNext(NULL);
			while (headPtr)
			{
				auto tmpPtr = headPtr;
				headPtr = pList.Remove(headPtr);
				SAFE_DELETE(tmpPtr);
			}

			Assert::IsTrue(pList.GetCount() == 0);
		}

		TEST_METHOD(CommandOption)
		{
			int32 nArgc = 7;
			char* argv[] = { 
				"F:\\DBConnector\\Debug\\Game.exe", 
				"-Path", "../../", 
				"-Config", "GroupConfig0.ini", 
				"-Section", "Node0" };
			
			std::string strValue;
			bool b = GameUtil::GetCommandOpt("-Section", strValue, nArgc, argv);
			Assert::IsTrue(b);
			Assert::IsTrue("Node0" == strValue);
		}

		TEST_METHOD(Test_ConfigManager)
		{
			std::string worldIp;
			ConfigManager::GetConfigValue("CommonConfig", "WorldIP", worldIp);

			
		}
	};
}