#include "StdAfx.h"
#include "ConfigManager.h"
#include <algorithm>
// #include "MyFile.h"
// #include "MyLog.h"
#include "GameUtil.h"
//#include "thirdparty/tinyxml/tinyxml.h"

ConfigItem* ConfigSection::AddConfigItem( const char* valueName)
{
	std::string tmpName = valueName;
	std::transform( tmpName.begin(), tmpName.end(), tmpName.begin(), ::tolower);

	ConfigItem* pItem = new ConfigItem();
	m_Items[tmpName] = pItem;

	return pItem;
}

ConfigItem* ConfigSection::GetConfigItem( const char* valueName, bool bCreate)
{
	std::string tmpName = valueName;
	std::transform( tmpName.begin(), tmpName.end(), tmpName.begin(), ::tolower);

	ConfigItemMap::iterator itr = m_Items.find(tmpName);
	if( itr != m_Items.end() )
		return itr->second;

	if(!bCreate)
		return NULL;

	return AddConfigItem( valueName );
}


bool ConfigSection::GetConfigValue( const char* valueName, std::string& value)
{
	ConfigItem* pItem = GetConfigItem( valueName, false);
	if(!pItem)
		return false;

	value = pItem->Value;
	return true;
}


bool ConfigSection::GetConfigValue(const char* valueName, int8& nValue)
{
	ConfigItem* pItem = GetConfigItem( valueName, false);
	if(!pItem)
		return false;

	nValue = (int8)atoi(pItem->Value.c_str());
	return true;
}

bool ConfigSection::GetConfigValue(const char* valueName, int16& nValue)
{
	ConfigItem* pItem = GetConfigItem( valueName, false);
	if(!pItem)
		return false;

	nValue = (int16)atoi(pItem->Value.c_str());
	return true;
}

bool ConfigSection::GetConfigValue(const char* valueName, int32& nValue)
{
	ConfigItem* pItem = GetConfigItem( valueName, false);
	if(!pItem)
		return false;

	nValue = (int32)atoi(pItem->Value.c_str());
	return true;
}


bool ConfigSection::GetConfigValue(const char* valueName, double& nValue)
{
	ConfigItem* pItem = GetConfigItem( valueName, false);
	if(!pItem)
		return false;

	nValue = atof(pItem->Value.c_str());
	return true;
}

std::string ConfigManager::m_strMainPath;
bool ConfigManager::m_bCustomSectionValid = false;
std::string ConfigManager::m_strCustomSection;

ConfigManager::ConfigManager(void)
{
}


ConfigManager::~ConfigManager(void)
{
}

void ConfigManager::LoadStartConfig(int32 nArgc, char* argv[])
{
	for (int32 i = 0; i < nArgc; ++i)
	{
		char* strKey = argv[i];
		if ( strKey[0] == '-' && (i+1) < nArgc)
		{
			char* name = strKey + 1;
			if (strlen(name) > 0)
			{
				std::string sectionName = "CommonConfig";
				std::string valueName = name;

				int32 nPos = (int32)valueName.find('/');
				if ( nPos > 0 )
				{
					sectionName = valueName.substr( 0, nPos);
					valueName = valueName.substr( nPos+1, valueName.length() );
				}

				ConfigSection* pSection = GetConfigSection( sectionName.c_str(), false);
				if( pSection )
				{
					ConfigItem* pItem = pSection->GetConfigItem( valueName.c_str(), false);
					if( pItem != NULL )
						pItem->Value = argv[i+1];
				}
			}
		}
	}
}

bool ConfigManager::LoadConfig(const std::string& strFilePath, bool bReplaceOlder, MyFile* pFile /*= NULL*/)
{
	std::string strFormat = strFilePath.substr( strFilePath.find_last_of('.'));

	std::transform( strFormat.begin(), strFormat.end(), strFormat.begin(), ::tolower);

	if( strFormat == ".ini")
		return LoadConfigFromIni( strFilePath, bReplaceOlder, pFile);

	return LoadConfigFromXML( strFilePath, bReplaceOlder, pFile);
}

bool ConfigManager::LoadConfigFromIni(const std::string& strFilePath, bool bReplaceOlder, MyFile* pMyFile /*= NULL*/)
{
	FILE* pFile = NULL;
	pFile = fopen( strFilePath.c_str(), "rb");	

	if (!pFile)
	{
		MyLog::message("LoadConfigFromIni ReadFile Failed [%s]", strFilePath.c_str());
		return false;
	}

	std::string strCurrentSection;

	const int32 MAX_LINE_LENGTH = 8 * 1024;
	char * pBuff = new char[MAX_LINE_LENGTH];
	if(!pBuff)
	{
		MyLog::error("ConfigManager::LoadConfigFromIni Alloc Memory Failed ! file=[%s]", strFilePath.c_str() );
		return false;
	}

	bool bEof = false;
	bool bFailed = false;
	while( !bEof && !bFailed )
	{
		// 1.getline
		for (int32 i = 0; i < MAX_LINE_LENGTH; ++i)
		{
			int32 nRet = fgetc(pFile);
			char ch(nRet);
			if ( nRet == EOF)
			{
				ch = '\n';
				bEof = true;
			}
			if ( ch == '\n' )
			{
				if( i > 0 && pBuff[i-1] == '\r')
					--i;

				pBuff[i] = '\0';
				break;
			}
			else
				pBuff[i] = ch;
		}

		// 2.pass by the comment
		std::string strLine(pBuff);
		strLine = strLine.substr( 0, strLine.find_last_of('#'));
		if( strLine.size() < 2)
			continue;

		// 3.handle it
		if ( strLine[0] == '[' && strLine[strLine.size()-1] == ']')
		{
			strCurrentSection = strLine.substr( 1, strLine.size() - 2);
			continue;
		}

		if(strCurrentSection.empty())
		{
			MyLog::error("ConfigManager::LoadConfigFromIni Failed to load file [%s]!No Section", strFilePath.c_str());
			bFailed = true;
			continue;
		}

		std::string::size_type pos = strLine.find('=');
		if ( pos == std::string::npos )
			continue;

		std::string strName = GameUtil::TrimStr(strLine.substr( 0, pos), "\t ");
		std::string strValue = GameUtil::TrimStr(strLine.substr(pos+1), "\t \"");

		if( strValue.empty() || strValue.empty() )
			continue;

		if ( strCurrentSection == "Config")
		{
			std::string strPath = MainPath();
			strPath += strValue;

			bFailed = !LoadConfigFromIni( strPath.c_str(), bReplaceOlder);
			continue;
		}

		ConfigSection* pCurrentSection = GetConfigSection( strCurrentSection.c_str(), true);
		if (!pCurrentSection)
		{
			MyLog::error("ConfigManager::LoadConfigFromIni Failed to load file [%s]!Can not open section[%s]", strFilePath.c_str(), strCurrentSection.c_str());
			bFailed = true;
			continue;
		}

		ConfigItem* pItem = pCurrentSection->GetConfigItem( strName.c_str(), true);
		if(pItem)
		{
			if(!bReplaceOlder)
				continue;
		}
		else
		{
			pItem = pCurrentSection->AddConfigItem( strName.c_str() );
		}

		pItem->Value = strValue;
	}

	fclose(pFile);
	SAFE_DELETE_ARRAY(pBuff);

	return !bFailed;
}

bool ConfigManager::LoadConfigFromXML(const std::string& strFilePath, bool bReplaceOlder, MyFile* pFile /*= NULL*/)
{
	XmlDocument doc;
	doc.LoadFile( strFilePath.c_str());

	if (doc.Error())
	{
		MyLog::error("Load Config File [%s] Fail!", strFilePath.c_str());
		return false;
	}

	XmlElement* pRoot = doc.RootElement();
	if( pRoot == NULL)
		return false;

	std::string tmpsz = std::string("");
	LoadNodesFromXML( pRoot, tmpsz, bReplaceOlder);
	return true;
}

void ConfigManager::LoadNodesFromXML(XmlNode* pRoot, std::string& sectionName, bool bReplaceOlder)
{
	ConfigSection* pSection = GetConfigSection( sectionName.c_str(), true);

	for ( XmlElement* pNode = pRoot->FirstChildElement(); pNode != NULL; pNode = pNode->NextSiblingElement())
	{
		std::string strParam = pNode->Value();
		if ( strParam == "ConfigFile" )
		{
			std::string strPath = MainPath();
			strPath += GetXmlElementAttribute( pNode, "value");
			LoadConfigFromXML( strPath.c_str(), bReplaceOlder);
		}
		else if( strParam == "param" )
		{
			std::string strName = GetXmlElementAttribute( pNode, "name");
			ConfigItem* pItem = pSection->GetConfigItem( strName.c_str(), false);

			if( pItem != NULL )
			{
				if(!bReplaceOlder)
					continue;
			}
			else
			{
				pItem = pSection->AddConfigItem( strName.c_str() );
			}

			pItem->Value = GetXmlElementAttribute( pNode, "value");
		}
		else
		{
			std::string subsection;
			if( sectionName.length() > 0 )
				subsection = sectionName + "/" + strParam;
			else
				subsection = strParam;

			LoadNodesFromXML( pNode, subsection, bReplaceOlder);
		}
	}
}

void ConfigManager::SetMainPath(std::string& strMainPath)
{
	m_strMainPath = strMainPath;
}

void ConfigManager::SetCustomSection(std::string& strCustomSection)
{
	m_bCustomSectionValid = true;
	m_strCustomSection = strCustomSection;
}

ConfigSection* ConfigManager::GetConfigSection(const char* pSectionName, bool bCreate)
{
	std::string sectionName = pSectionName;

	std::transform( sectionName.begin(), sectionName.end(), sectionName.begin(), ::tolower);

	ConfigSectionMap::iterator itr = m_values.find( sectionName );
	if( itr != m_values.end() )
		return itr->second;

	if(!bCreate)
		return NULL;

	ConfigSection* pSection = new ConfigSection();
	m_values[sectionName] = pSection;
	return pSection;
}

std::string ConfigManager::GetXmlElementAttribute(XmlElement* pNode, char* pName)
{
	const char* pValue = pNode->Attribute(pName);
	if(pValue != NULL)
		return pValue;

	return "";
}
