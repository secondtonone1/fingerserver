#include "GemCombineTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
GemCombineTable::loadFromDbc(const String& fileName)
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
GemCombineTable::reloadFromDbc(const String& fileName)
{
	GemCombineTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload GemCombineTemplate table.");
		return false;
	}
	return true;
}

bool 
GemCombineTable::saveToDbc(const String& filePath)
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
GemCombineTable::loadFromCsv(const String& filePath)
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
		GemCombineTemplate gemCombineTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", gemCombineTemplate.mID))
		{
			LOG_WARN("Failed to load gemcombine.csv for [id]");
			return false;
		}
		
		if (!csvReader.bind("nextAmount", gemCombineTemplate.mAmount))
		{
			LOG_WARN("Failed to load gemcombine.csv for [nextAmount]");
			return false;
		}
		
		if (!csvReader.bind("preGem", gemCombineTemplate.mPreGem))
		{
			LOG_WARN("Failed to load gemcombine.csv for [preGem]");
			return false;
		}

		if (!csvReader.bind("nextGem", gemCombineTemplate.mNextGem))
		{
			LOG_WARN("Failed to load gemcombine.csv for [nextGem]");
			return false;
		}

		if (!csvReader.bind("preAmount", gemCombineTemplate.mPreAmount))
		{
			LOG_WARN("Failed to load gemcombine.csv for [preAmount]");
			return false;
		}

		mMap.insert(gemCombineTemplate.mID, gemCombineTemplate);
	}
	fileStream.close();
	return true;
}

