#include "PlayerExpTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
PlayerExpTable::loadFromDbc(const String& fileName)
{
	StreamBuffer streamBuffer(8192 - sizeof(StreamBuffer::Node));
	Int32 length = RESOURCE_GROUP_MANAGER().loadFileToStream(fileName, 
		streamBuffer, "Data");
	if (length <= 0)
	{
		LOG_WARN("Failed to load file %s", fileName.c_str());
		return false;
	}
	if (!unserialize(*this, streamBuffer, 0))
	{
		LOG_WARN("Failed to unserialize file %s", fileName.c_str());
		return false;
	}
	return true;
}

bool 
PlayerExpTable::reloadFromDbc(const String& fileName)
{
	PlayerExpTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload PlayerExpTemplate table.");
		return false;
	}
	return true;
}

bool 
PlayerExpTable::saveToDbc(const String& filePath)
{
	StreamBuffer streamBuffer(8192 - sizeof(StreamBuffer::Node));
	serialize(*this, streamBuffer, 0);
	if (!FileUtil::saveFile(filePath, streamBuffer))
	{
		LOG_WARN("Failed to save file %s.", filePath.c_str());
		return false;
	}
	return true;
}

bool 
PlayerExpTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	std::ifstream fileStream(filePath.c_str());
	if (fileStream.fail())
	{
		LOG_WARN("Failed to open %s file.", filePath.c_str());
		return false;
	}
	CsvReader csvReader(fileStream, ",");
	csvReader.initTitle();
	while (csvReader.readLine())
	{
		PlayerExpTemplate playerExpTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("level", playerExpTemplate.mLevel))
		{
			LOG_WARN("Failed to load playerExp.csv for [level]");
			return false;
		}

		if (!csvReader.bind("exp", playerExpTemplate.mExp))
		{
			LOG_WARN("Failed to load playerExp.csv for [exp]");
			return false;
		}

		if(!csvReader.bind("strength", playerExpTemplate.mStrength))
		{
			LOG_WARN("Failed to load playerExp.csv for[strength]");
			return false;
		}

		if(!csvReader.bind("strengthAdd", playerExpTemplate.mStrengthAdd))
		{
			LOG_WARN("Failed to load playerExp.csv for[strengthAdd]");
			return false;
		}

		mMap.insert(playerExpTemplate.mLevel, playerExpTemplate);
	}
	fileStream.close();
	return true;
}

