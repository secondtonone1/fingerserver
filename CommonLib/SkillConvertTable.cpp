#include "SkillConvertTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
SkillConvertTable::loadFromDbc(const String& fileName)
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
SkillConvertTable::reloadFromDbc(const String& fileName)
{
	SkillListMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload skillcompare table.");
		return false;
	}
	return true;
}

bool 
SkillConvertTable::saveToDbc(const String& filePath)
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
SkillConvertTable::loadFromCsv(const String& filePath)
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
		SkillListTemplate skillListTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", skillListTemp.key))
		{
			LOG_WARN("Failed to load skillbase.csv for [skillId]");
			return false;
		}

		skillListTemp.skillList.insertTail(skillListTemp.key);

		if (!csvReader.bind("id1", skillListTemp.skill1))
		{
			LOG_WARN("Failed to load skillbase.csv for [skillId]");
			return false;
		}

		skillListTemp.skillList.insertTail(skillListTemp.skill1);

		if (!csvReader.bind("id2", skillListTemp.skill2))
		{
			LOG_WARN("Failed to load skillbase.csv for [skillId]");
			return false;
		}

		skillListTemp.skillList.insertTail(skillListTemp.skill2);

		if (!csvReader.bind("id3", skillListTemp.skill3))
		{
			LOG_WARN("Failed to load skillbase.csv for [skillId]");
			return false;
		}

		skillListTemp.skillList.insertTail(skillListTemp.skill3);

		if (!csvReader.bind("id4", skillListTemp.skill4))
		{
			LOG_WARN("Failed to load skillbase.csv for [skillId]");
			return false;
		}

		skillListTemp.skillList.insertTail(skillListTemp.skill4);

		mMap.insert(skillListTemp.key, skillListTemp);
	}
	fileStream.close();
	return true;
}

