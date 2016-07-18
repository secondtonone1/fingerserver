#include "AttrPowerTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
AttrPowerTable::loadFromDbc(const String& fileName)
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
AttrPowerTable::reloadFromDbc(const String& fileName)
{
	AttrPowerTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		
		LOG_WARN("Failed to reload AttrPower table.");
		return false;
	}
	return true;
}

bool 
AttrPowerTable::saveToDbc(const String& filePath)
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
AttrPowerTable::loadFromCsv(const String& filePath)
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
		AttrPowerTemplate attrPowerTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("key", attrPowerTemp.mAttrType))
		{
			LOG_WARN("Failed to load attrPowerTemp.csv for [key]");
			return false;
		}

		if (!csvReader.bindFloat("rate", attrPowerTemp.mAttrValue))
		{
			LOG_WARN("Failed to load attrPowerTemp.csv for [key]");
			return false;
		}

		mMap.insert(attrPowerTemp.mAttrType, attrPowerTemp);
	

	}
	fileStream.close();
	return true;
}