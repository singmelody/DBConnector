#pragma once

#include <string>
#include <map>
#include "BaseType.h"

class MyFile;

class ConfigItem
{
public:
	std::string Value;
	std::string Description;
};

typedef std::map<std::string, ConfigItem*> ConfigItemMap;

class ConfigSection
{
public:
	ConfigItem* AddConfigItem( const char* valueName);
	ConfigItem* GetConfigItem( const char* valueName, bool bCreate);

	bool GetConfigValue( const char* valueName, std::string& value);
	bool GetConfigValue( const char* valueName, int8& nValue);
	bool GetConfigValue( const char* valueName, int16& nValue);
	bool GetConfigValue( const char* valueName, int32& nValue);
	bool GetConfigValue( const char* valueName, double& nValue);

	ConfigItemMap m_Items;
};

typedef std::map<std::string, ConfigSection*> ConfigSectionMap;

class ConfigManager
{
public:
	ConfigManager(void);
	~ConfigManager(void);

	static void LoadStartConfig( int32 nArgc, char* argv[]);

	static bool LoadConfig( const std::string& strFilePath, bool bReplaceOlder, MyFile* pFile = NULL);
	static bool LoadConfigFromIni( const std::string& strFilePath, bool bReplaceOlder, MyFile* pFile = NULL);
	static bool LoadConfigFromXML( const std::string& strFilePath, bool bReplaceOlder, MyFile* pFile = NULL);
	static void LoadNodesFromXML( XmlNode* pRoot, std::string& sectionName, bool bReplaceOlder);

	template <typename T>
	static bool GetConfigValue( const char* sectionName, const std::string& valueName, T& value, bool bUseCustomSection = false)
	{
		bool bGet = false;
		if( bUseCustomSection && m_bCustomSectionValid )
		{
			std::string str = std::string(sectionName) + "/" + m_strCustomSection;
			bGet = GetConfigValue2<T>( str, valueName, value);
		}

		return bGet ? true : GetConfigValue2<T>( sectionName, valueName, value);
	}


	static const std::string& MainPath();
	static void SetMainPath(std::string& strMainPath);
	static void SetCustomSection(std::string& strCustomSection);

	static ConfigSection* GetConfigSection( const char* pSectionName, bool bCreate = false);
	static std::string GetXmlElementAttribute( XmlElement* pNode, char* pName);

	static ConfigSectionMap m_values;

	static std::string m_strMainPath;
	static bool	m_bCustomSectionValid;
	static std::string m_strCustomSection;

protected:
	template <typename T>
	static bool GetConfigValue2( const std::string& sectionName, const std::string& valueName, T& value)
	{
		ConfigSection* pSection = GetConfigSection( sectionName.c_str(), false);
		if( pSection == NULL )
			return false;

		return pSection->GetConfigValue( valueName.c_str(), value);
	}
};

