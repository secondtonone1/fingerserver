#include "BuffTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
BuffTable::loadFromDbc(const String& fileName)
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
BuffTable::reloadFromDbc(const String& fileName)
{
	BuffTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload BuffTemplate table.");
		return false;
	}
	return true;
}

bool 
BuffTable::saveToDbc(const String& filePath)
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
BuffTable::loadFromCsv(const String& filePath)
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
		BuffTemplate buffTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", buffTemplate.mId))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [Id]");
			return false;
		}

		if (!csvReader.bind("name", buffTemplate.mName))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [name]");
			return false;
		}

		if (!csvReader.bind("desc", buffTemplate.mDesc))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [desc]");
			return false;
		}

		if (!csvReader.bind("type", buffTemplate.mType))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [type]");
			return false;
		}

		if (!csvReader.bind("subType", buffTemplate.mSubType))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [subType]");
			return false;
		}

		if (!csvReader.bind("keepTime", buffTemplate.mKeepTime))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [keepTime]");
			return false;
		}

		if (!csvReader.bind("resPrefix", buffTemplate.mResPrefix))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [resPrefix]");
			return false;
		}

		if (!csvReader.bind("param1", buffTemplate.mParam1))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [param1]");
			return false;
		}

		if (!csvReader.bind("param2", buffTemplate.mParam2))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [param2]");
			return false;
		}

		if (!csvReader.bind("param3", buffTemplate.mParam3))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [param3]");
			return false;
		}

		if (!csvReader.bind("param4", buffTemplate.mParam4))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [param4]");
			return false;
		}

		if (!csvReader.bind("param5", buffTemplate.mParam5))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [param5]");
			return false;
		}

		if (!csvReader.bind("param6", buffTemplate.mParam6))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [param6]");
			return false;
		}

		if (!csvReader.bind("param7", buffTemplate.mParam7))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [param7]");
			return false;
		}

		if (!csvReader.bind("param8", buffTemplate.mParam8))
		{
			LOG_WARN("Failed to load BuffTemplate.csv for [param8]");
			return false;
		}

		mMap.insert(buffTemplate.mId, buffTemplate);
	}
	fileStream.close();
	return true;
}

