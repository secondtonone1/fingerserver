#include "GemTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
GemTable::loadFromDbc(const String& fileName)
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
GemTable::reloadFromDbc(const String& fileName)
{
	GemTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload FashionTemplate table.");
		return false;
	}
	return true;
}

bool 
GemTable::saveToDbc(const String& filePath)
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
GemTable::loadFromCsv(const String& filePath)
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
		GemTemplate gemTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("gemId", gemTemplate.mGemId))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [gemId]");
			return false;
		}

		if (!csvReader.bind("level", gemTemplate.mLevel))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [level]");
			return false;
		}

		if (!csvReader.bind("power", gemTemplate.mPower))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [power]");
			return false;
		}

		if (!csvReader.bind("attrType1", gemTemplate.mAttrType1))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [attrType1]");
			return false;
		}

		if (!csvReader.bind("attrValue1", gemTemplate.mAttrValue1))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [attrValue1]");
			return false;
		}

		if (!csvReader.bind("attrType2", gemTemplate.mAttrType2))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [attrType2]");
			return false;
		}

		if (!csvReader.bind("attrValue2", gemTemplate.mAttrValue2))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [attrValue2]");
			return false;
		}

		if (!csvReader.bind("attrType3", gemTemplate.mAttrType3))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [attrType3]");
			return false;
		}

		if (!csvReader.bind("attrValue3", gemTemplate.mAttrValue3))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [gemId]");
			return false;
		}

		if (!csvReader.bind("attrType4", gemTemplate.mAttrType4))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [attrType4]");
			return false;
		}

		if (!csvReader.bind("attrValue4", gemTemplate.mAttrValue4))
		{
			LOG_WARN("Failed to load gemTemplate.csv for [attrValue4]");
			return false;
		}

		mMap.insert(gemTemplate.mGemId, gemTemplate);
	}
	fileStream.close();
	return true;
}

