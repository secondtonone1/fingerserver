#include "GemCombineOnceTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
GemCombineOnceTable::loadFromDbc(const String& fileName)
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
GemCombineOnceTable::reloadFromDbc(const String& fileName)
{
	GemCombineOnceTemplateMap tmpMap = mMap;
	GemCombineOnceTypeLvMap typeLvMap = mTypeLvMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mTypeLvMap = typeLvMap;
		LOG_WARN("Failed to reload GemCombineOnce table.");
		return false;
	}
	return true;
}

bool 
GemCombineOnceTable::saveToDbc(const String& filePath)
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
GemCombineOnceTable::loadFromCsv(const String& filePath)
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
		GemCombineOnceTemplate gemCombineOnceTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", gemCombineOnceTemplate.mID))
		{
			LOG_WARN("Failed to load gemcombineonce.csv for [id]");
			return false;
		}

		if (!csvReader.bind("type", gemCombineOnceTemplate.mType))
		{
			LOG_WARN("Failed to load gemcombineonce.csv for [type]");
			return false;
		}

		if (!csvReader.bind("level", gemCombineOnceTemplate.mLevel))
		{
			LOG_WARN("Failed to load gemcombineonce.csv for [level]");
			return false;
		}

		if (!csvReader.bind("count", gemCombineOnceTemplate.mCount))
		{
			LOG_WARN("Failed to load gemcombineonce.csv for [count]");
			return false;
		}

		
	
		mMap.insert(gemCombineOnceTemplate.mID, gemCombineOnceTemplate);

		GemCombineOnceTypeLvMap::Iter * typeLvMapIter = mTypeLvMap.find(gemCombineOnceTemplate.mType);
		if(typeLvMapIter)
		{
			GemIdAndCount gemIdAndCount;
			gemIdAndCount.mCount = gemCombineOnceTemplate.mCount;
			gemIdAndCount.mID = gemCombineOnceTemplate.mID;
			
			typeLvMapIter->mValue.insert(gemCombineOnceTemplate.mLevel, gemIdAndCount);
		}
		else
		{
			GemIdAndCount gemIdAndCount;
			gemIdAndCount.mCount = gemCombineOnceTemplate.mCount;
			gemIdAndCount.mID = gemCombineOnceTemplate.mID;

			Map<UInt32,GemIdAndCount> mapIdAndCount;
			mapIdAndCount.insert(gemCombineOnceTemplate.mLevel,gemIdAndCount);

			mTypeLvMap.insert(gemCombineOnceTemplate.mType,mapIdAndCount);
		}
	}
	fileStream.close();
	return true;
}

