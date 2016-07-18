#include "CourageContentTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
InlineActivityTable::loadFromDbc(const String& fileName)
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
InlineActivityTable::reloadFromDbc(const String& fileName)
{
	InlineActivityTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload InlineActivityTemplate table.");
		return false;
	}
	return true;
}

bool 
InlineActivityTable::saveToDbc(const String& filePath)
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
InlineActivityTable::loadFromCsv(const String& filePath)
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
		InlineActivityTemplate inlineActivityTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", inlineActivityTemplate.id))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [id]");
			return false;
		}

		if (!csvReader.bind("type", inlineActivityTemplate.type))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [type]");
			return false;
		}

		if (!csvReader.bind("activitytype", inlineActivityTemplate.activitytype))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [activitytype]");
			return false;
		}
	

		if (!csvReader.bind("neednum", inlineActivityTemplate.neednum))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [neednum]");
			return false;
		}

		if (!csvReader.bind("name", inlineActivityTemplate.name))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [name]");
			return false;
		}

		if (!csvReader.bind("award1", inlineActivityTemplate.award1))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [award1]");
			return false;
		}
		if (!csvReader.bind("award2", inlineActivityTemplate.award2))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [award2]");
			return false;
		}
		if (!csvReader.bind("award3", inlineActivityTemplate.award3))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [award3]");
			return false;
		}
		if (!csvReader.bind("begintime", inlineActivityTemplate.begintime))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [begintime]");
			return false;
		}
		if (!csvReader.bind("endtime", inlineActivityTemplate.endtime))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [endtime]");
			return false;
		}
		if (!csvReader.bind("daystart", inlineActivityTemplate.daystart))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [daystart]");
			return false;
		}
		if (!csvReader.bind("dayend", inlineActivityTemplate.dayend))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [dayend]");
			return false;
		}
		if (!csvReader.bind("lv", inlineActivityTemplate.lv))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [lv]");
			return false;
		}
		if (!csvReader.bind("gold", inlineActivityTemplate.gold))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [gold]");
			return false;
		}
		if (!csvReader.bind("month", inlineActivityTemplate.month))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [month]");
			return false;
		}
		if (!csvReader.bind("viplv", inlineActivityTemplate.viplv))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [viplv]");
			return false;
		}
		if (!csvReader.bind("time", inlineActivityTemplate.time))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [time]");
			return false;
		}
		if (!csvReader.bind("taskid1", inlineActivityTemplate.taskid1))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [taskid1]");
			return false;
		}
		if (!csvReader.bind("taskid2", inlineActivityTemplate.taskid2))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [taskid2]");
			return false;
		}
		if (!csvReader.bind("taskid3", inlineActivityTemplate.taskid3))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [taskid3]");
			return false;
		}
		if (!csvReader.bind("taskid4", inlineActivityTemplate.taskid4))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [taskid4]");
			return false;
		}
		if (!csvReader.bind("cost", inlineActivityTemplate.cost))
		{
			LOG_WARN("Failed to load InlineActivity.csv for [cost]");
			return false;
		}


		mMap.insert(inlineActivityTemplate.id, inlineActivityTemplate);
	}
	fileStream.close();
	return true;
}


