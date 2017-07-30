#include "SystemInfoTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
SystemInfoTable::loadFromDbc(const String& fileName)
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
SystemInfoTable::reloadFromDbc(const String& fileName)
{
	SystemInfoTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload SystemInfo table.");
		return false;
	}
	return true;
}

bool 
SystemInfoTable::saveToDbc(const String& filePath)
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
SystemInfoTable::loadFromCsv(const String& filePath)
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
		SystemInfoTemplate systemInfoTemplate;

		if (!csvReader.bind("systemID", systemInfoTemplate.systemID))
		{
			LOG_WARN("Failed to load SystemInfo.csv for [systemID]");
			return false;
		}
		if (!csvReader.bind("funId", systemInfoTemplate.funId))
		{
			LOG_WARN("Failed to load SystemInfo.csv for [funId]");
			return false;
		}
		if (!csvReader.bind("openConditionType", systemInfoTemplate.openConditionType))
		{
			LOG_WARN("Failed to load SystemInfo.csv for [openConditionType]");
			return false;
		}
		if (!csvReader.bind("openConditionValue", systemInfoTemplate.openConditionValue))
		{
			LOG_WARN("Failed to load SystemInfo.csv for [openConditionValue]");
			return false;
		}
		if (!csvReader.bind("haveGuide", systemInfoTemplate.haveGuide))
		{
			LOG_WARN("Failed to load SystemInfo.csv for [haveGuide]");
			return false;
		}
		if (!csvReader.bind("preNoticeLevel", systemInfoTemplate.preNoticeLevel))
		{
			LOG_WARN("Failed to load SystemInfo.csv for [preNoticeLevel]");
			return false;
		}
	

		mMap.insert(systemInfoTemplate.systemID, systemInfoTemplate);
	}
	fileStream.close();
	return true;
}

