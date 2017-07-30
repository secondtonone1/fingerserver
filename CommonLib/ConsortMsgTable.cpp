#include "ConsortMsgTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
ConsortMsgTable::loadFromDbc(const String& fileName)
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
ConsortMsgTable::reloadFromDbc(const String& fileName)
{
	ConsortMsgTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
	  
		LOG_WARN("Failed to reload ConsortMsg table.");
		return false;
	}
	return true;
}

bool 
ConsortMsgTable::saveToDbc(const String& filePath)
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
ConsortMsgTable::loadFromCsv(const String& filePath)
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
		ConsortMsgTemplate consortMsgTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", consortMsgTemp.mId))
		{
			LOG_WARN("Failed to load consortmsg.csv for [id]");
			return false;
		}

		if (!csvReader.bind("visible", consortMsgTemp.mVisible))
		{
			LOG_WARN("Failed to load consortmsg.csv for [visible]");
			return false;
		}

		mMap.insert(consortMsgTemp.mId, consortMsgTemp);

	}

	fileStream.close();
	return true;
}