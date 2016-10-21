#include "stdafx.h"
#include "CppUnitTest.h"
#include "SqliteInterface.h"
#include "ODBCInterface.h"
#include "DBDataType.h"
#include "GameUtil.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(SqliteQuery)
		{
			// TODO:  �ڴ�������Դ���
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

	TEST_CLASS(UnitTest2)
	{
		TEST_METHOD(ODBCQuery)
		{
			// TODO:  �ڴ�������Դ���
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
}