
#include "Table.h"
#include "MonsterAward.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
MonsterAwardTable::loadFromDbc(const String& fileName)
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
MonsterAwardTable::reloadFromDbc(const String& fileName)
{
	MonsterAwardTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload MonsterAwardTable table.");
		return false;
	}
	return true;
}

bool 
MonsterAwardTable::saveToDbc(const String& filePath)
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
MonsterAwardTable::loadFromCsv(const String& filePath)
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
		MonsterAwardTemplate monsterAwardTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", monsterAwardTemplate.id))
		{
			LOG_WARN("Failed to load monsterAwardTemplate.csv for [id]");
			return false;
		}

		if (!csvReader.bind("stageid", monsterAwardTemplate.stageid))
		{
			LOG_WARN("Failed to load monsterAwardTemplate.csv for [stageid]");
			return false;
		}

		if (!csvReader.bind("autoawardid", monsterAwardTemplate.autoawardid))
		{
			LOG_WARN("Failed to load monsterAwardTemplate.csv for [autoawardid]");
			return false;
		}

		if (!csvReader.bind("killawardid", monsterAwardTemplate.killawardid))
		{
			LOG_WARN("Failed to load monsterAwardTemplate.csv for [killawardid]");
			return false;
		}

	
		char str1[32]={};
		char str2[32]={};
		for (int i=1;i<50;i++)
		{
			KeyValue keyValue;
			sprintf(str1, "%s%d","time",i);
			sprintf(str2, "%s%d","awardid",i);

			if (!csvReader.bind(str1, keyValue.key))
			{
				LOG_WARN("Failed to load monsterAwardTemplate.csv for [key]");
				break;
			}
			if (keyValue.key == 0)
			{
				break;
			}

			if (!csvReader.bind(str2, keyValue.value))
			{
				LOG_WARN("Failed to load monsterAwardTemplate.csv for [value]");
				break;
			}
		
			monsterAwardTemplate.timesAward.insertTail(keyValue);

		}	

		
		mMap.insert(monsterAwardTemplate.id, monsterAwardTemplate);
	}
	fileStream.close();
	return true;
}

