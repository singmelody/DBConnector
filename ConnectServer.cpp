#include "stdafx.h"
#include "ConnectServer.h"
#include "MyLog.h"
#include <sstream>
#include "GameUtil.h"
#include "ConfigManager.h"

ConnectServer::ConnectServer()
{
}


ConnectServer::~ConnectServer()
{
}

bool ConnectServer::Init(int32 nArgc, char* argv[])
{

	std::stringstream strArgs;
	std::string strConfig;

	for (int32 i = 0; i < nArgc; ++i)
	{
		char* sArg = argv[i];
		strArgs << " " << sArg;
		if (strcmp(sArg, "-Path") == 0)
		{
			MyPath::SetMainPath(argv[i + 1]);
			MyLog::message("Path:%s", argv[i + 1]);
		}
		else if ((strcmp(sArg, "-StartConfig") == 0) || (strcmp(sArg, "-Config") == 0))
		{
			strConfig = argv[i + 1];
		}
		else if (strcmp(sArg, "-Section") == 0)
		{
			std::string strSection = argv[i + 1];
			ConfigManager::SetCustomSection(strSection);
		}
	}

	MyLog::message("Start Args[%s]", strArgs.str().c_str());

	std::string strPath = MyPath::MainPath();
	strPath += "Data/Config/";
	ConfigManager::SetMainPath(strPath);


	if (strConfig.length() > 0)
	{
		std::string strPath = ConfigManager::MainPath() + strConfig;
		ConfigManager::LoadConfig(strPath.c_str(), true);
	}

	// load argv argument
	ConfigManager::LoadStartConfig(nArgc, argv);

	// load srv config
	//ServerConfig::FillConfig();

	return true;
}
