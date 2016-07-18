#include "GemLvTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
GemLvTable::loadFromDbc(const String& fileName)
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
GemLvTable::reloadFromDbc(const String& fileName)
{
	GemLvTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload GemLvTable table.");
		return false;
	}
	return true;
}

bool 
GemLvTable::saveToDbc(const String& filePath)
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
GemLvTable::loadFromCsv(const String& filePath)
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
		GemLvTemplate gemLvTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("gemLv", gemLvTemplate.mGemLv))
		{
			LOG_WARN("Failed to load gemLvTemplate.csv for [gemLv]");
			return false;
		}

		if (!csvReader.bind("playerLv", gemLvTemplate.mPlayerLv))
		{
			LOG_WARN("Failed to load gemLvTemplate.csv for [playerLv]");
			return false;
		}

		if (!csvReader.bind("lv1Count", gemLvTemplate.mLv1Count))
		{
			LOG_WARN("Failed to load gemLvTemplate.csv for [lv1Count]");
			return false;
		}

		
		mMap.insert(gemLvTemplate.mGemLv, gemLvTemplate);
	}
	fileStream.close();
	return true;
}

